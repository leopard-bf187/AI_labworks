/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: Wayland platform window implementation.
*
*  Date: 26.07.2026
*/

#include "linux_classes.h"
#include <stdlib.h>


static const xdg_surface_listener g_xdgSurfaceListener =
{
    CPlatformWindow::XdgSurfaceConfigure
};


static const xdg_toplevel_listener g_xdgToplevelListener =
{
    CPlatformWindow::XdgToplevelConfigure,
    CPlatformWindow::XdgToplevelClose
};


static const zxdg_toplevel_decoration_v1_listener g_toplevelDecorationListener =
{
    CPlatformWindow::XdgDecorationConfigure
};


CPlatformWindow::CPlatformWindow(IAllocator* allocator) :
    Inherit(allocator),
    m_surface(nullptr),
    m_xdgSurface(nullptr),
    m_xdgToplevel(nullptr),
    m_toplevelDecoration(nullptr),
    m_placeholderBuffer(nullptr),
    m_placeholderMemory(nullptr),
    m_placeholderMemorySize(0),
    m_placeholderFd(-1),
    m_placeholderWidth(0),
    m_placeholderHeight(0),
    m_waylandHandle{nullptr, nullptr},
    m_window{nullptr, -1, 0},
    m_callback(g_DefaultWindowCallback),
    m_visible(false),
    m_active(false),
    m_maximized(false),
    m_fullscreen(false),
    m_created(false),
    m_configured(false)
{
}


CPlatformWindow::~CPlatformWindow()
{
    DestroyNativeWindow(false);
}


uint32 CPlatformWindow::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CPlatformWindow::_Destroy(this);
        return 0;
    }

    return ref;
}


uint32 CPlatformWindow::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return this->IncRef();
    }

    if (guid == IPlatformWindow::GUID())
    {
        *IFace = static_cast<IPlatformWindow*>(this);
        return this->IncRef();
    }

    return 0;
}


// ERRCODE CPlatformWindow::Show()
// {
//     // if (!m_surface)
//     //     return PLATFORM_ERR_NOT_SUPPORTED;

//     // if (!m_visible)
//     // {
//     //     m_visible = true;
//     //     m_desc.style.visible = true;
//     //     wl_surface_commit(m_surface);
//     //     m_callback->OnShow(&m_window);
//     // }

//     // return PLATFORM_ERR_OK;

//     if (!m_surface)
//         return PLATFORM_ERR_INVALID_STATE;

//     if (m_visible)
//         return PLATFORM_ERR_OK;

//     m_visible = true;
//     m_desc.visible = true;

//     if (m_callback)
//         m_callback->OnShow(&m_window);

//     return PLATFORM_ERR_OK;
// }



// ERRCODE CPlatformWindow::Hide()
// {
//     // if (!m_surface)
//     //     return PLATFORM_ERR_NOT_SUPPORTED;

//     // if (m_visible)
//     // {
//     //     wl_surface_attach(m_surface, nullptr, 0, 0);
//     //     wl_surface_commit(m_surface);
//     //     m_visible = false;
//     //     m_desc.style.visible = 0;
//     //     m_callback->OnHide(&m_window);
//     // }

//     // return PLATFORM_ERR_OK;

//     if (!m_surface)
//         return PLATFORM_ERR_INVALID_STATE;

//     if (!m_visible)
//         return PLATFORM_ERR_OK;

//     wl_surface_attach(m_surface, nullptr, 0, 0);
//     wl_surface_commit(m_surface);
//     wl_display_flush(m_manager->m_display);

//     m_visible = false;
//     m_desc.visible = false;

//     if (m_callback)
//         m_callback->OnHide(&m_window);

//     return PLATFORM_ERR_OK;
// }


ERRCODE CPlatformWindow::Show()
{
    if (!m_surface)
        return PLATFORM_ERR_NOT_SUPPORTED;

    if (m_visible)
        return PLATFORM_ERR_OK;

    m_visible = true;
    m_desc.style.visible = true;

    if (m_configured)
    {
        ERRCODE result = CreatePlaceholderBuffer(m_desc.width, m_desc.height);

        if (result != PLATFORM_ERR_OK)
            return result;

        result = AttachPlaceholderBuffer();

        if (result != PLATFORM_ERR_OK)
            return result;
    }
    else
    {
        wl_surface_commit(m_surface);

        if (m_manager && m_manager->m_display)
            wl_display_flush(m_manager->m_display);
    }

    if (m_callback)
        m_callback->OnShow(&m_window);

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::Hide()
{
    if (!m_surface)
        return PLATFORM_ERR_NOT_SUPPORTED;

    if (!m_visible)
        return PLATFORM_ERR_OK;

    wl_surface_attach(m_surface, nullptr, 0, 0);
    wl_surface_commit(m_surface);

    if (m_manager && m_manager->m_display)
        wl_display_flush(m_manager->m_display);

    m_visible = false;
    m_desc.style.visible = false;

    if (m_callback)
        m_callback->OnHide(&m_window);

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::Minimize()
{
    if (!m_xdgToplevel)
        return PLATFORM_ERR_NOT_SUPPORTED;

    xdg_toplevel_set_minimized(m_xdgToplevel);
    m_callback->OnMinimize(&m_window);
    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::Maximize()
{
    if (!m_xdgToplevel)
        return PLATFORM_ERR_NOT_SUPPORTED;

    xdg_toplevel_set_maximized(m_xdgToplevel);
    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::Restore()
{
    if (!m_xdgToplevel)
        return PLATFORM_ERR_NOT_SUPPORTED;

    if (m_fullscreen)
        xdg_toplevel_unset_fullscreen(m_xdgToplevel);

    if (m_maximized)
        xdg_toplevel_unset_maximized(m_xdgToplevel);

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::SetTitle(const Stdlib::String& title)
{
    if (!m_xdgToplevel || !title.Data())
        return PLATFORM_ERR_INVALID_ARGUMENT;

    xdg_toplevel_set_title(m_xdgToplevel, title.Data());
    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::SetSize(uint32 width, uint32 height)
{
    if (!m_xdgToplevel || width == 0 || height == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    m_desc.width = width;
    m_desc.height = height;

    if (!m_desc.style.resizable)
    {
        xdg_toplevel_set_min_size(m_xdgToplevel, static_cast<int32>(width), static_cast<int32>(height));
        xdg_toplevel_set_max_size(m_xdgToplevel, static_cast<int32>(width), static_cast<int32>(height));
    }

    m_callback->OnSizing(&m_window, width, height);
    m_callback->OnSize(&m_window, width, height);
    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::SetPosition(int32 x, int32 y)
{
    (void)x;
    (void)y;
    return PLATFORM_ERR_NOT_SUPPORTED;
}


void CPlatformWindow::SetCallback(IPlatformWindowCallback* callback)
{
    m_callback = callback ? callback : g_DefaultWindowCallback;
}


IPlatformWindowCallback* CPlatformWindow::GetCallback()
{
    return m_callback;
}


ERRCODE CPlatformWindow::GetDesc(SPlatformWindowDesc* outDesc) const
{
    if (!outDesc)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    *outDesc = m_desc;
    return PLATFORM_ERR_OK;
}


uint32 CPlatformWindow::GetWidth() const
{
    return m_desc.width;
}


uint32 CPlatformWindow::GetHeight() const
{
    return m_desc.height;
}


bool CPlatformWindow::IsVisible() const
{
    return m_visible;
}


bool CPlatformWindow::IsActive() const
{
    return m_active;
}


ERRCODE CPlatformWindow::GetNativeHandle(SNativeHandle* outHandle) const
{
    if (!outHandle)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!m_surface)
        return PLATFORM_ERR_NOT_SUPPORTED;

    *outHandle = m_window;
    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::Initialize(CPlatformManager* manager, const Stdlib::String& title, const SPlatformWindowDesc* desc, IPlatformWindowCallback* callback, CPlatformWindow* parentWindow)
{
    if (!manager || !desc || !manager->m_display || !manager->m_compositor || !manager->m_xdgWmBase)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    m_manager = manager;

    m_desc = *desc;
    m_visible = desc->style.visible;
    m_fullscreen = desc->style.fullscreen;
    this->SetCallback(callback);

    m_surface = wl_compositor_create_surface(manager->m_compositor);

    if (!m_surface)
        return PLATFORM_ERR_NOT_SUPPORTED;

    m_xdgSurface = xdg_wm_base_get_xdg_surface(manager->m_xdgWmBase, m_surface);

    if (!m_xdgSurface)
        return PLATFORM_ERR_NOT_SUPPORTED;

    xdg_surface_add_listener(m_xdgSurface, &g_xdgSurfaceListener, this);

    m_xdgToplevel = xdg_surface_get_toplevel(m_xdgSurface);

    if (!m_xdgToplevel)
        return PLATFORM_ERR_NOT_SUPPORTED;

    if (m_manager->m_decorationManager)
    {
        m_toplevelDecoration = zxdg_decoration_manager_v1_get_toplevel_decoration(m_manager->m_decorationManager, m_xdgToplevel);

        if (m_toplevelDecoration)
        {
            zxdg_toplevel_decoration_v1_add_listener(m_toplevelDecoration, &g_toplevelDecorationListener, this);
            zxdg_toplevel_decoration_v1_set_mode(m_toplevelDecoration, ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE);
        }
    }

    xdg_toplevel_add_listener(m_xdgToplevel, &g_xdgToplevelListener, this);

    if (parentWindow && parentWindow->m_xdgToplevel)
        xdg_toplevel_set_parent(m_xdgToplevel, parentWindow->m_xdgToplevel);

    if (title.Data() && title.Data()[0] != '\0')
        xdg_toplevel_set_title(m_xdgToplevel, title.Data());

    xdg_toplevel_set_app_id(m_xdgToplevel, "krystallic-engine");

    if (!m_desc.style.resizable)
    {
        xdg_toplevel_set_min_size(m_xdgToplevel, static_cast<int32>(m_desc.width), static_cast<int32>(m_desc.height));
        xdg_toplevel_set_max_size(m_xdgToplevel, static_cast<int32>(m_desc.width), static_cast<int32>(m_desc.height));
    }

    if (m_desc.style.fullscreen)
        xdg_toplevel_set_fullscreen(m_xdgToplevel, nullptr);

    m_waylandHandle.display = manager->m_display;
    m_waylandHandle.surface = m_surface;
    m_window = {&m_waylandHandle, -1, 0};

    wl_surface_commit(m_surface);

    if (wl_display_roundtrip(manager->m_display) < 0)
        return PLATFORM_ERR_NOT_SUPPORTED;

    m_created = true;
    m_callback->OnCreate(&m_window);

    if (m_visible)
        m_callback->OnShow(&m_window);

    return PLATFORM_ERR_OK;
}


void CPlatformWindow::DestroyNativeWindow(bool nitifyCallback)
{
    if (!m_created)
        return;

    m_created = false;
    m_visible = false;
    m_active = false;
    m_configured = false;

    DestroyPlaceholderBuffer();

    if (m_toplevelDecoration)
    {
        zxdg_toplevel_decoration_v1* decoration = m_toplevelDecoration;
        m_toplevelDecoration = nullptr;
        zxdg_toplevel_decoration_v1_destroy(decoration);
    }

    if (m_xdgToplevel)
    {
        xdg_toplevel* toplevel = m_xdgToplevel;
        m_xdgToplevel = nullptr;
        xdg_toplevel_destroy(toplevel);
    }

    if (m_xdgSurface)
    {
        xdg_surface* xdgSurface = m_xdgSurface;
        m_xdgSurface = nullptr;
        xdg_surface_destroy(xdgSurface);
    }

    if (m_surface)
    {
        wl_surface* surface = m_surface;
        m_surface = nullptr;
        wl_surface_destroy(surface);
    }

    m_waylandHandle = {nullptr, nullptr};
    m_window = {nullptr, -1, 0};
    //m_manager = nullptr;
    
    if(nitifyCallback && m_callback)
        m_callback->OnDestroy(&m_window);
}


ERRCODE CPlatformWindow::CreatePlaceholderBuffer(uint32 width, uint32 height)
{
    if (!m_manager || !m_manager->m_shm || width == 0 || height == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (m_placeholderBuffer && m_placeholderWidth == width && m_placeholderHeight == height)
        return PLATFORM_ERR_OK;

    DestroyPlaceholderBuffer();

    const uint64 stride64 = static_cast<uint64>(width) * 4;
    const uint64 size64 = stride64 * static_cast<uint64>(height);

    if (stride64 > 0x7FFFFFFF || size64 > 0x7FFFFFFF)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    const int32 stride = static_cast<int32>(stride64);
    const int32 size = static_cast<int32>(size64);

    char fileName[] = "/tmp/krystallic-wayland-XXXXXX";

    int fd = mkstemp(fileName);

    if (fd < 0)
        return PLATFORM_ERR_NOT_SUPPORTED;

    unlink(fileName);

    if (ftruncate(fd, static_cast<off_t>(size)) != 0)
    {
        close(fd);
        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    void* memory = mmap(nullptr, static_cast<size_t>(size), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (memory == MAP_FAILED)
    {
        close(fd);
        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    dword* pixels = static_cast<dword*>(memory);
    const uint64 pixelCount = static_cast<uint64>(width) * height;

    dword color = m_desc.backgroundColor.mask;

    for (uint64 i = 0; i < pixelCount; ++i)
        pixels[i] = color;

    wl_shm_pool* pool = wl_shm_create_pool(m_manager->m_shm, fd, size);

    if (!pool)
    {
        munmap(memory, static_cast<size_t>(size));
        close(fd);
        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    wl_buffer* buffer = wl_shm_pool_create_buffer(pool, 0, static_cast<int32_t>(width), static_cast<int32_t>(height), stride, WL_SHM_FORMAT_XRGB8888);

    wl_shm_pool_destroy(pool);

    if (!buffer)
    {
        munmap(memory, static_cast<size_t>(size));
        close(fd);
        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    m_placeholderBuffer = buffer;
    m_placeholderMemory = memory;
    m_placeholderMemorySize = size64;
    m_placeholderFd = fd;
    m_placeholderWidth = width;
    m_placeholderHeight = height;

    return PLATFORM_ERR_OK;
}


void CPlatformWindow::DestroyPlaceholderBuffer()
{
    // if (m_placeholderBuffer)
    // {
    //     wl_buffer_destroy(m_placeholderBuffer);
    //     m_placeholderBuffer = nullptr;
    // }

    // if (m_placeholderMemory)
    // {
    //     munmap(m_placeholderMemory, static_cast<size_t>(m_placeholderMemorySize));
    //     m_placeholderMemory = nullptr;
    // }

    // if (m_placeholderFd >= 0)
    // {
    //     close(m_placeholderFd);
    //     m_placeholderFd = -1;
    // }

    // m_placeholderMemorySize = 0;
    // m_placeholderWidth = 0;
    // m_placeholderHeight = 0;

    wl_buffer* buffer = m_placeholderBuffer;
    m_placeholderBuffer = nullptr;

    void* mappedData = m_placeholderMemory;
    m_placeholderMemory = nullptr;

    size_t mappedSize = m_placeholderMemorySize;
    m_placeholderMemorySize = 0;

    int fd = m_placeholderFd;
    m_placeholderFd = -1;

    if (buffer)
        wl_buffer_destroy(buffer);

    if (mappedData && mappedSize)
        munmap(mappedData, mappedSize);

    if (fd >= 0)
        close(fd);
}


ERRCODE CPlatformWindow::AttachPlaceholderBuffer()
{
    if (!m_surface || !m_placeholderBuffer)
        return PLATFORM_ERR_NOT_SUPPORTED;

    wl_surface_attach(m_surface, m_placeholderBuffer, 0, 0);

    wl_surface_damage(m_surface, 0, 0, static_cast<int32_t>(m_placeholderWidth), static_cast<int32_t>(m_placeholderHeight));

    wl_surface_commit(m_surface);

    if (m_manager && m_manager->m_display)
        wl_display_flush(m_manager->m_display);

    return PLATFORM_ERR_OK;
}


void CPlatformWindow::XdgSurfaceConfigure(void* data, xdg_surface* surface, uint32 serial)
{
    // CPlatformWindow* window = static_cast<CPlatformWindow*>(data);
    // xdg_surface_ack_configure(surface, serial);

    // if (window->m_surface)
    //     wl_surface_commit(window->m_surface);

    // if (window->m_callback)
    //     window->m_callback->OnPaint(&window->m_window);

        CPlatformWindow* window = static_cast<CPlatformWindow*>(data);

    if (!window)
        return;

    xdg_surface_ack_configure(surface, serial);

    window->m_configured = true;

    if (window->m_visible)
    {
        ERRCODE result = window->CreatePlaceholderBuffer(window->m_desc.width, window->m_desc.height);

        if (result == PLATFORM_ERR_OK)
            window->AttachPlaceholderBuffer();
    }

    if (window->m_callback)
        window->m_callback->OnPaint(&window->m_window);
}


void CPlatformWindow::XdgToplevelConfigure(void* data, xdg_toplevel* toplevel, int32 width, int32 height, wl_array* states)
{
    (void)toplevel;

    CPlatformWindow* window = static_cast<CPlatformWindow*>(data);
    bool active = false;
    bool maximized = false;
    bool fullscreen = false;

    const dword* state = static_cast<const dword*>(states->data);
    const dword* stateEnd = state + states->size / sizeof(dword);

    for (; state < stateEnd; ++state)
    {
        switch (*state)
        {
            case XDG_TOPLEVEL_STATE_ACTIVATED:
                active = true;
                break;

            case XDG_TOPLEVEL_STATE_MAXIMIZED:
                maximized = true;
                break;

            case XDG_TOPLEVEL_STATE_FULLSCREEN:
                fullscreen = true;
                break;

            default:
                break;
        }
    }

    if (window->m_active != active)
    {
        window->m_active = active;
        window->m_callback->OnActivate(&window->m_window, active);

        if (active)
            window->m_callback->OnSetFocus(&window->m_window);
        else
            window->m_callback->OnKillFocus(&window->m_window);
    }

    if (!window->m_maximized && maximized)
        window->m_callback->OnMaximize(&window->m_window);

    window->m_maximized = maximized;
    window->m_fullscreen = fullscreen;

    if (width > 0 && height > 0)
    {
        // uint32 newWidth = static_cast<uint32>(width);
        // uint32 newHeight = static_cast<uint32>(height);

        // if (window->m_desc.width != newWidth || window->m_desc.height != newHeight)
        // {
        //     window->m_callback->OnSizing(&window->m_window, newWidth, newHeight);
        //     window->m_desc.width = newWidth;
        //     window->m_desc.height = newHeight;
        //     window->m_callback->OnSize(&window->m_window, newWidth, newHeight);
        // }

        uint32 newWidth = static_cast<uint32>(width);
        uint32 newHeight = static_cast<uint32>(height);

        if (window->m_desc.width != newWidth || window->m_desc.height != newHeight)
        {
            window->m_callback->OnSizing(&window->m_window, newWidth, newHeight);

            window->m_desc.width = newWidth;
            window->m_desc.height = newHeight;

            if (window->m_visible && window->m_configured)
            {
                if (window->CreatePlaceholderBuffer(newWidth, newHeight) == PLATFORM_ERR_OK)
                {
                    window->AttachPlaceholderBuffer();
                }
            }

            window->m_callback->OnSize(&window->m_window, newWidth, newHeight);
        }
    }
}


void CPlatformWindow::XdgToplevelClose(void* data, xdg_toplevel* toplevel)
{
    // (void)toplevel;

    // CPlatformWindow* window = static_cast<CPlatformWindow*>(data);

    // if (!window)
    //     return;

    // window->m_visible = false;

    // if (window->m_callback)
    //     window->m_callback->OnDestroy(&window->m_window);

    (void)toplevel;

    CPlatformWindow* window = static_cast<CPlatformWindow*>(data);

    if (!window)
        return;

    window->DestroyNativeWindow(true);
}


void CPlatformWindow::XdgDecorationConfigure(void* data, zxdg_toplevel_decoration_v1* decoration, uint32 mode)
{
    (void)decoration;

    CPlatformWindow* window = static_cast<CPlatformWindow*>(data);

    if (!window) return;

    switch (mode)
    {
        case ZXDG_TOPLEVEL_DECORATION_V1_MODE_SERVER_SIDE:
            // Системные декорации включены.
            break;

        case ZXDG_TOPLEVEL_DECORATION_V1_MODE_CLIENT_SIDE:
            // Compositor требует, чтобы клиент рисовал рамку сам.
            break;

        default:
            break;
    }
}

