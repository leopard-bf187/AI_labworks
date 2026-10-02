/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 11.07.2026
*/


#include "linux_classes.h"

#include <errno.h>
#include <poll.h>
#include <string.h>


static const wl_registry_listener g_registryListener =
{
    CPlatformManager::RegistryGlobal,
    CPlatformManager::RegistryGlobalRemove
};


static const xdg_wm_base_listener g_xdgWmBaseListener =
{
    CPlatformManager::XdgWmBasePing
};


CPlatformManager::CPlatformManager(IAllocator* allocator, const SNativeHandle* sysHandle) : 
    Inherit(allocator),
    m_display(nullptr),
    m_registry(nullptr),
    m_compositor(nullptr),
    m_shm(nullptr),
    m_decorationManager(nullptr),
    m_xdgWmBase(nullptr),
    m_hInstHandle({nullptr, -1, 0}),
    m_exitCode(0),
    m_ownsDisplay(false),
    m_running(false)
{
    m_memoryManager.Attach(CPlatformMemoryManager::_Create(allocator));
    m_threadManager.Attach(CPlatformThreadManager::_Create(allocator));
    m_processManager.Attach(CPlatformProcessManager::_Create(allocator));
    m_timeManager.Attach(CPlatformTimeManager::_Create(allocator));
    m_inputManager.Attach(CPlatformInputManager::_Create(allocator));
    m_systemInfo.Attach(CPlatformSystemInfo::_Create(allocator));

    m_hInstHandle = {nullptr, -1, 0};

    if (sysHandle && sysHandle->ptr)
    {
        m_display = static_cast<wl_display*>(sysHandle->ptr);
        m_hInstHandle = *sysHandle;
        m_ownsDisplay = false;
        m_running = InitializeWayland();
    }
    else
    {
        m_display = wl_display_connect(nullptr);

        if (m_display)
        {
            m_hInstHandle = {m_display, -1, 0};
            m_ownsDisplay = true;
            m_running = InitializeWayland();
        }
    }
}


CPlatformManager::~CPlatformManager()
{
    ShutdownWayland();
}


uint32 CPlatformManager::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CPlatformManager::_Destroy(this);
        return 0;
    }

    return ref;
};


uint32 CPlatformManager::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return this->IncRef();
    }

    if (guid == IPlatformManager::GUID())
    {
        *IFace = static_cast<IPlatformManager*>(this);
        return this->IncRef();
    }

    return 0;
}


ERRCODE CPlatformManager::QueryMemoryManager(IPlatformMemoryManager** outManager)
{
    if (!outManager)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!m_memoryManager.Get())
        return PLATFORM_ERR_NOT_SUPPORTED;

    *outManager = static_cast<IPlatformMemoryManager*>(m_memoryManager.Get());
    m_memoryManager->IncRef();

    return PLATFORM_ERR_OK;
};


ERRCODE CPlatformManager::QueryThreadManager(IPlatformThreadManager** outMgr)
{
    if (!outMgr)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!m_threadManager.Get())
        return PLATFORM_ERR_NOT_SUPPORTED;

    *outMgr = static_cast<IPlatformThreadManager*>(m_threadManager.Get());
    m_threadManager->IncRef();

    return PLATFORM_ERR_OK;
};


ERRCODE CPlatformManager::QueryProcessManager(IPlatformProcessManager** outMgr)
{
    if (!outMgr)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!m_processManager.Get())
        return PLATFORM_ERR_NOT_SUPPORTED;

    *outMgr = static_cast<IPlatformProcessManager*>(m_processManager.Get());
    m_processManager->IncRef();

    return PLATFORM_ERR_OK;
};


ERRCODE CPlatformManager::QueryTimeManager(IPlatformTimeManager** outTimer)
{
    if (!outTimer)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!m_timeManager.Get())
        return PLATFORM_ERR_NOT_SUPPORTED;

    *outTimer = static_cast<IPlatformTimeManager*>(m_timeManager.Get());
    m_timeManager->IncRef();

    return PLATFORM_ERR_OK;
};


ERRCODE CPlatformManager::QueryInputManager(IPlatformInputManager** outMgr)
{
    if (!outMgr)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!m_inputManager.Get())
        return PLATFORM_ERR_NOT_SUPPORTED;

    *outMgr = static_cast<IPlatformInputManager*>(m_inputManager.Get());
    m_inputManager->IncRef();

    return PLATFORM_ERR_OK;
};


ERRCODE CPlatformManager::QuerySystemInfo(IPlatformSystemInfo** outSysInfo)
{
    if (!outSysInfo)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!m_systemInfo.Get())
        return PLATFORM_ERR_NOT_SUPPORTED;

    *outSysInfo = static_cast<IPlatformSystemInfo*>(m_systemInfo.Get());
    m_systemInfo->IncRef();

    return PLATFORM_ERR_OK;
};


ERRCODE CPlatformManager::CreateNativeWindow(const Stdlib::String& title, const SPlatformWindowDesc* desc, IPlatformWindowCallback* optionalWndCallback, IPlatformWindow* parentWindow, IPlatformWindow** outWindow)
{
    if (!desc || !outWindow)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    *outWindow = nullptr;

    if (!m_running || !m_display || !m_compositor || !m_xdgWmBase)
        return PLATFORM_ERR_NOT_SUPPORTED;

    CPlatformWindow* parent = parentWindow ? static_cast<CPlatformWindow*>(parentWindow) : nullptr;

    RefCounted<CPlatformWindow> window;
    window.Attach(CPlatformWindow::_Create(m_allocator.Get()));

    if (!window.Get())
        return PLATFORM_ERR_NOT_SUPPORTED;

    ERRCODE result = window->Initialize(this, title, desc, optionalWndCallback, parent);

    if (result != PLATFORM_ERR_OK)
    {
        window->Delete();
        return result;
    }

    *outWindow = static_cast<IPlatformWindow*>(window.Get());
    window->IncRef();

    return PLATFORM_ERR_OK;
}


bool CPlatformManager::PollEvents()
{
    if (!m_running || !m_display)
        return false;

    if (wl_display_dispatch_pending(m_display) < 0)
    {
        m_running = false;
        return false;
    }

    while (wl_display_prepare_read(m_display) != 0)
    {
        if (wl_display_dispatch_pending(m_display) < 0)
        {
            m_running = false;
            return false;
        }
    }

    if (wl_display_flush(m_display) < 0 && errno != EAGAIN)
    {
        wl_display_cancel_read(m_display);
        m_running = false;
        return false;
    }

    pollfd displayFd{};
    displayFd.fd = wl_display_get_fd(m_display);
    displayFd.events = POLLIN;

    int pollResult = poll(&displayFd, 1, 0);

    if (pollResult < 0)
    {
        wl_display_cancel_read(m_display);

        if (errno == EINTR)
            return m_running;

        m_running = false;
        return false;
    }

    if (pollResult == 0 || !(displayFd.revents & POLLIN))
    {
        wl_display_cancel_read(m_display);
        return m_running;
    }

    if (wl_display_read_events(m_display) < 0)
    {
        m_running = false;
        return false;
    }

    if (wl_display_dispatch_pending(m_display) < 0)
    {
        m_running = false;
        return false;
    }

    return m_running;
}


bool CPlatformManager::IsRunning() const
{
    return m_running;
}


void CPlatformManager::RequestQuit(int exitCode)
{
    m_exitCode = exitCode;
    m_running = false;
}


ERRCODE CPlatformManager::GetNativeHandle(SNativeHandle* appInstanceHandle) const
{
    if (!appInstanceHandle)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    *appInstanceHandle = m_hInstHandle;

    return PLATFORM_ERR_OK;
}




bool CPlatformManager::InitializeWayland()
{
    if (!m_display)
        return false;

    m_registry = wl_display_get_registry(m_display);

    if (!m_registry)
        return false;

    wl_registry_add_listener(m_registry, &g_registryListener, this);

    if (wl_display_roundtrip(m_display) < 0)
        return false;

    if (!m_compositor || !m_shm || !m_xdgWmBase)
        return false;

    xdg_wm_base_add_listener(m_xdgWmBase, &g_xdgWmBaseListener, this);
    return true;
}


void CPlatformManager::ShutdownWayland()
{
    m_running = false;

    if (m_xdgWmBase)
    {
        xdg_wm_base_destroy(m_xdgWmBase);
        m_xdgWmBase = nullptr;
    }

    if (m_shm)
    {
        wl_shm_destroy(m_shm);
        m_shm = nullptr;
    }

    if (m_compositor)
    {
        wl_compositor_destroy(m_compositor);
        m_compositor = nullptr;
    }

    if (m_decorationManager)
    {
        zxdg_decoration_manager_v1_destroy(m_decorationManager);
        m_decorationManager = nullptr;
    }

    if (m_registry)
    {
        wl_registry_destroy(m_registry);
        m_registry = nullptr;
    }

    if (m_display)
    {
        if (m_ownsDisplay)
            wl_display_disconnect(m_display);

        m_display = nullptr;
    }

    m_hInstHandle = {nullptr, -1, 0};
}


void CPlatformManager::RegistryGlobal(void* data, wl_registry* registry, uint32 name, const char* interface, uint32 version)
{
    CPlatformManager* manager = static_cast<CPlatformManager*>(data);

    if (!manager || !interface)
        return;

    if (Stdlib::StrCmp(interface, wl_compositor_interface.name) == 0)
    {
        uint32 bindVersion = version < 1u ? version : 1u;
        if (bindVersion > 4) bindVersion = 4;
        manager->m_compositor = static_cast<wl_compositor*>(wl_registry_bind(registry, name, &wl_compositor_interface, bindVersion));
        return;
    }

    if (strcmp(interface, wl_shm_interface.name) == 0)
    {
        manager->m_shm = static_cast<wl_shm*>(wl_registry_bind(registry, name, &wl_shm_interface, 1));
        return;
    }

    if (Stdlib::StrCmp(interface, xdg_wm_base_interface.name) == 0)
    {
        uint32 bindVersion = version < 1u ? version : 1u;

        if (bindVersion > 1) bindVersion = 1;

        manager->m_xdgWmBase = static_cast<xdg_wm_base*>(wl_registry_bind(registry, name, &xdg_wm_base_interface, bindVersion));

        if (manager->m_xdgWmBase)
            xdg_wm_base_add_listener(manager->m_xdgWmBase, &g_xdgWmBaseListener, manager);

        return;
    }

    if (Stdlib::StrCmp(interface, zxdg_decoration_manager_v1_interface.name) == 0)
    {
        uint32_t bindVersion = version;
        if (bindVersion > 1) bindVersion = 1;
        manager->m_decorationManager = static_cast<zxdg_decoration_manager_v1*>(wl_registry_bind(registry, name, &zxdg_decoration_manager_v1_interface, bindVersion));
        return;
    }
}


void CPlatformManager::RegistryGlobalRemove(void* data, wl_registry* registry, uint32 name)
{
    (void)data;
    (void)registry;
    (void)name;
}


void CPlatformManager::XdgWmBasePing(void* data, xdg_wm_base* wmBase, uint32 serial)
{
    (void)data;
    xdg_wm_base_pong(wmBase, serial);
}


