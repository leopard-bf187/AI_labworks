/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 19.07.2026
*/


#include "android_classes.h"



CPlatformWindow::CPlatformWindow(IAllocator* allocator) : 
    Inherit(allocator), m_desc{}, m_window{}, m_handle(nullptr), m_callback(g_DefaultWindowCallback), m_visible(false), m_active(false)
{

}


CPlatformWindow::~CPlatformWindow()
{
    DetachNativeWindow();
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


ERRCODE CPlatformWindow::Show()
{
    return PLATFORM_ERR_NOT_SUPPORTED;
}


ERRCODE CPlatformWindow::Hide()
{
    return PLATFORM_ERR_NOT_SUPPORTED;
}


ERRCODE CPlatformWindow::Minimize()
{
    return PLATFORM_ERR_NOT_SUPPORTED;
}


ERRCODE CPlatformWindow::Maximize()
{
    return PLATFORM_ERR_NOT_SUPPORTED;
}


ERRCODE CPlatformWindow::Restore()
{
    return PLATFORM_ERR_NOT_SUPPORTED;
}


ERRCODE CPlatformWindow::SetTitle(const Stdlib::String&)
{
    return PLATFORM_ERR_NOT_SUPPORTED;
}


ERRCODE CPlatformWindow::SetSize(uint32, uint32)
{
    return PLATFORM_ERR_NOT_SUPPORTED;
}


ERRCODE CPlatformWindow::SetPosition(int32, int32)
{
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
    if (m_handle)
    {
        const int32 width = ANativeWindow_getWidth(m_handle);
        if (width > 0)
            return static_cast<uint32>(width);
    }

    return m_desc.width;
}


uint32 CPlatformWindow::GetHeight() const
{
    if (m_handle)
    {
        const int32 height = ANativeWindow_getHeight(m_handle);
        if (height > 0)
            return static_cast<uint32>(height);
    }

    return m_desc.height;
}


bool CPlatformWindow::IsVisible() const
{
    return m_visible && m_handle;
}


bool CPlatformWindow::IsActive() const
{
    return m_active && m_handle;
}


ERRCODE CPlatformWindow::GetNativeHandle(SNativeHandle * outHandle) const
{
    if (!outHandle)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    *outHandle = m_window;

    return PLATFORM_ERR_OK;
}


void CPlatformWindow::Initialize(const SPlatformWindowDesc* desc, IPlatformWindowCallback* callback)
{
    if (desc)
        m_desc = *desc;

    SetCallback(callback);
}


void CPlatformWindow::AttachNativeWindow(ANativeWindow* window)
{
    if (m_handle == window)
    {
        RefreshSize();
        return;
    }

    if (m_handle)
        DetachNativeWindow();

    if (!window)
        return;

    ANativeWindow_acquire(window);
    m_handle = window;
    m_window = {};
    m_window.ptr = m_handle;
    m_visible = true;

    RefreshSize();

    if (m_callback)
    {
        m_callback->OnCreate(&m_window);
        m_callback->OnShow(&m_window);
    }
}


void CPlatformWindow::DetachNativeWindow()
{
    if (!m_handle)
        return;

    if (m_callback)
    {
        if (m_active)
            m_callback->OnActivate(&m_window, false);

        if (m_visible)
            m_callback->OnHide(&m_window);

        m_callback->OnDestroy(&m_window);
    }

    m_active = false;
    m_visible = false;

    ANativeWindow_release(m_handle);
    m_handle = nullptr;
    m_window = {};
}


void CPlatformWindow::RefreshSize()
{
    if (!m_handle)
        return;

    const int32 width = ANativeWindow_getWidth(m_handle);
    const int32 height = ANativeWindow_getHeight(m_handle);

    if (width > 0)
        m_desc.width = static_cast<uint32>(width);

    if (height > 0)
        m_desc.height = static_cast<uint32>(height);
}


void CPlatformWindow::SetActiveState(bool active)
{
    if (m_active == active)
        return;

    m_active = active;

    if (m_callback && m_handle)
        m_callback->OnActivate(&m_window, active);
}


