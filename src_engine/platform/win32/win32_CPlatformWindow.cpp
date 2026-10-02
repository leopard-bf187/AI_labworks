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


#include "win32_classes.h"



CPlatformWindow::CPlatformWindow(IAllocator* allocator) : Inherit(allocator)
{
    m_window = {nullptr, -1, 0};
    m_callback = g_DefaultWindowCallback;
    m_desc = {};
    m_hInst = 0;
    m_visible = false;
    m_active = false;
}


CPlatformWindow::~CPlatformWindow()
{
    DestroyWindow((HWND)m_window.ptr);
    UnregisterClassW(reinterpret_cast<LPWSTR>(m_className.Data()), m_hInst);
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
    ShowWindow((HWND)m_window.ptr, SW_SHOW);
    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::Hide()
{
    ShowWindow((HWND)m_window.ptr, SW_HIDE);
    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::Minimize()
{
    ShowWindow((HWND)m_window.ptr, SW_MINIMIZE);
    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::Maximize()
{
    ShowWindow((HWND)m_window.ptr, SW_MAXIMIZE);
    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::Restore()
{
    return PLATFORM_ERR_NOT_IMPLEMENTED;
}


ERRCODE CPlatformWindow::SetTitle(const Stdlib::String& title)
{
    m_windowName = title;

    if(SetWindowTextW((HWND)m_window.ptr, reinterpret_cast<LPWSTR>(m_windowName.Data())) != 0)
        return PLATFORM_ERR_BACKEND_ERROR;

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformWindow::SetSize(uint32 width, uint32 height)
{
    return PLATFORM_ERR_NOT_IMPLEMENTED;
}


ERRCODE CPlatformWindow::SetPosition(int32 x, int32 y)
{
    return PLATFORM_ERR_NOT_IMPLEMENTED;
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

    return PLATFORM_ERR_NOT_IMPLEMENTED;
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

    *outHandle = m_window;

    return PLATFORM_ERR_OK;
}

