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


#include "win32_classes.h"


extern LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);


CPlatformManager::CPlatformManager(IAllocator* allocator, const SNativeHandle* optSysHandle) : 
    Inherit(allocator), m_running(true), m_hInst(nullptr)
{
    m_memoryManager.Attach(CPlatformMemoryManager::_Create(allocator));
    m_threadManager.Attach(CPlatformThreadManager::_Create(allocator));
    m_processManager.Attach(CPlatformProcessManager::_Create(allocator));
    m_timeManager.Attach(CPlatformTimeManager::_Create(allocator));
    m_inputManager.Attach(CPlatformInputManager::_Create(allocator));
    m_systemInfo.Attach(CPlatformSystemInfo::_Create(allocator));

    if (!optSysHandle)
    {
        m_hInstHandle.ptr = GetModuleHandle(0);
        m_hInstHandle.fd = -1;
        m_hInstHandle.type = NATIVE_HANDLE_APP_INSTANCE;
    }
    else
    {
        m_hInstHandle = *optSysHandle;
    }

    m_hInst = (HINSTANCE)m_hInstHandle.ptr;
}


CPlatformManager::~CPlatformManager()
{
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

    Stdlib::StringU16 windowName(title);
    Stdlib::StringU16 className(windowName);
    className.Append("WndClass");

    LPWSTR windowNameStr = reinterpret_cast<LPWSTR>(windowName.Data());
    LPWSTR classNameStr = reinterpret_cast<LPWSTR>(className.Data());

    //byte a = (byte)((desc->backgroundColor.mask & 0xff000000) >> 24);
    //byte r = (byte)((desc->backgroundColor.mask & 0x00ff0000) >> 16);
    //byte g = (byte)((desc->backgroundColor.mask & 0x0000ff00) >> 8);
    //byte b = (byte)((desc->backgroundColor.mask & 0x000000ff));
    //COLORREF bkColor = RGB(r, g, b);

    COLORREF bkColor = RGB(desc->backgroundColor.g, desc->backgroundColor.r, desc->backgroundColor.a);

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = sizeof(void*);
    wc.cbWndExtra = sizeof(void*);
    wc.hInstance = m_hInst;
    wc.hIcon   = desc->icon.ptr ? (HICON)desc->icon.ptr : LoadIconW(m_hInst, MAKEINTRESOURCEW(32512));
    wc.hIconSm = desc->iconSmall.ptr ? (HICON)desc->iconSmall.ptr : LoadIconW(m_hInst, MAKEINTRESOURCEW(32512));
    wc.hCursor = desc->cursor.ptr ? (HCURSOR)desc->cursor.ptr : LoadCursorW(m_hInst, MAKEINTRESOURCEW(32512));
    wc.hbrBackground = CreateSolidBrush(bkColor);
    wc.lpszClassName = classNameStr;
    wc.lpszMenuName = 0;

    if (FAILED(RegisterClassExW(&wc)))
        return PLATFORM_ERR_BACKEND_FAILED_TO_CREATE;

    DWORD style = 0;

    if (desc->style.caption) style |= WS_CAPTION;
    if (desc->style.resizable) style |= WS_THICKFRAME;
    if (desc->style.sysmenu) style |= WS_SYSMENU;
    if (desc->style.maximizeButton) style |= WS_MAXIMIZEBOX;
    if (desc->style.minimizeButton) style |= WS_MINIMIZEBOX;

    HWND parent = 0;

    if (parentWindow)
    {
        style |= WS_CHILD;
        SNativeHandle handle;
        parentWindow->GetNativeHandle(&handle);
        parent = (HWND)handle.ptr;
    }

    RefCounted<CPlatformWindow> window;
    window.Attach(CPlatformWindow::_Create(m_allocator.Get()));

    if (!window.Get())
    {
        UnregisterClassW(classNameStr, m_hInst);
        return PLATFORM_ERR_OUT_OF_MEMORY;
    }

    HWND hwnd = CreateWindowExW(WS_EX_APPWINDOW, classNameStr, windowNameStr, style, desc->x, desc->y, desc->width, desc->height, parent, 0, m_hInst, window.Get());

    if (!hwnd)
        return PLATFORM_ERR_BACKEND_FAILED_TO_CREATE;

    window->m_desc = *desc;
    window->m_callback = optionalWndCallback ? optionalWndCallback : g_DefaultWindowCallback;
    window->m_windowName = std::move(windowName);
    window->m_className = std::move(className);
    window->m_window.ptr = hwnd;
    window->m_hInst = m_hInst;

    SetLastError(ERROR_SUCCESS);

    LONG_PTR swlResult = SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window.Get()));
    if (swlResult == 0 && GetLastError() != ERROR_SUCCESS)
    {
        DestroyWindow(hwnd);
        UnregisterClassW(classNameStr, m_hInst);
        return PLATFORM_ERR_BACKEND_FAILED_TO_CREATE;
    }

    //RECT rcClient, rcWind;
    //POINT ptDiff;
    //GetClientRect(hwnd, &rcClient);
    //GetWindowRect(hwnd, &rcWind);
    //ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
    //ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
    //int px = (GetSystemMetrics(SM_CXSCREEN) - rcWind.right - rcWind.left) / 2;
    //int py = (GetSystemMetrics(SM_CYSCREEN) - rcWind.bottom - rcWind.top) / 2;

    //MoveWindow(hwnd, desc->x + rcWind.left, desc->y + rcWind.top, desc->width + ptDiff.x, desc->height + ptDiff.y, TRUE);

    if (*outWindow)
        (*outWindow)->Delete();
    
    *outWindow = window.Cast<IPlatformWindow*>();
    window->IncRef();

    return PLATFORM_ERR_OK;
}


bool CPlatformManager::PollEvents()
{
    MSG msg{};

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            m_running = false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return m_running;
}


bool CPlatformManager::IsRunning() const
{
    return m_running;
}


void CPlatformManager::RequestQuit(int exitCode)
{
    m_running = false;
    PostQuitMessage(exitCode);
}


ERRCODE CPlatformManager::GetNativeHandle(SNativeHandle* appInstanceHandle) const
{
    if (!appInstanceHandle)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    *appInstanceHandle = m_hInstHandle;

    return PLATFORM_ERR_OK;
}


