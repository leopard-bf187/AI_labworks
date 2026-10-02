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


#include "android_classes.h"


static void AndroidAppCommandCallback(android_app* app, int32_t command)
{
    if (!app || !app->userData)
        return;

    CPlatformManager* manager = static_cast<CPlatformManager*>(app->userData);
    manager->ProcessAppCommand(command);
}


CPlatformManager::CPlatformManager(IAllocator* allocator, const SNativeHandle* androidAppObjPtr) : Inherit(allocator)
{
    m_memoryManager.Attach(CPlatformMemoryManager::_Create(allocator));
    m_threadManager.Attach(CPlatformThreadManager::_Create(allocator));
    m_processManager.Attach(CPlatformProcessManager::_Create(allocator));
    m_timeManager.Attach(CPlatformTimeManager::_Create(allocator));
    m_inputManager.Attach(CPlatformInputManager::_Create(allocator));
    m_systemInfo.Attach(CPlatformSystemInfo::_Create(allocator));

    if (androidAppObjPtr)
    {
        m_hInstHandle = *androidAppObjPtr;

        m_app = (android_app*)androidAppObjPtr->ptr;

        //assert(m_app != nullptr);

        if(m_app)
        {
            m_activity = m_app->activity;
            m_app->userData = this;
            m_app->onAppCmd = AndroidAppCommandCallback;
        }
    }
    else
    {
        m_hInstHandle = {nullptr, 0, 0};
        m_app = nullptr;
        m_activity = nullptr;
    }
    m_running = true;
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
    (void)title;

    if (!desc || !outWindow)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    *outWindow = nullptr;

    if (parentWindow)
        return PLATFORM_ERR_NOT_SUPPORTED;

    if (!m_app)
        return PLATFORM_ERR_NOT_SUPPORTED;

    if (!m_platformWindow)
    {
        m_platformWindow.Attach(CPlatformWindow::_Create(m_allocator.Get()));

        if (!m_platformWindow.Get())
            return PLATFORM_ERR_OUT_OF_MEMORY;

        m_platformWindow->Initialize(desc, optionalWndCallback);

        if (m_app->window)
            m_platformWindow->AttachNativeWindow(m_app->window);
    }
    else
    {
        m_platformWindow->SetCallback(optionalWndCallback);
    }

    *outWindow = m_platformWindow.Cast<IPlatformWindow*>();
    m_platformWindow->IncRef();

    return PLATFORM_ERR_OK;
}



bool CPlatformManager::PollEvents()
{
    int ident = 0;
    int events = 0;
    android_poll_source* source = nullptr;

    while ((ident = ALooper_pollOnce(0, nullptr, &events, reinterpret_cast<void**>(&source))) >= 0)
    {
        if (source)
            source->process(m_app, source);

        if (m_app->destroyRequested)
        {
            m_running = false;
            return false;
        }
    }

    return m_running;
}


bool CPlatformManager::IsRunning() const
{
    return m_running;
}


void CPlatformManager::RequestQuit(int exitCode)
{
    if (!m_running)
        return;

    m_running = false;

    if (m_app && m_app->activity)
        ANativeActivity_finish(m_app->activity);
}


ERRCODE CPlatformManager::GetNativeHandle(SNativeHandle* appInstanceHandle) const
{
    if (!appInstanceHandle)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    *appInstanceHandle = m_hInstHandle;

    return PLATFORM_ERR_OK;
}


void CPlatformManager::ProcessAppCommand(int32 command)
{
    if(!m_platformWindow.Get())
        return;

    switch (command)
    {
        case APP_CMD_INIT_WINDOW:
        {
            if (m_app->window)
                m_platformWindow->AttachNativeWindow(m_app->window);
            break;
        }

        case APP_CMD_TERM_WINDOW:
        {
            m_platformWindow->DetachNativeWindow();
            break;
        }

        case APP_CMD_WINDOW_RESIZED:
        case APP_CMD_CONTENT_RECT_CHANGED:
        {
            if (m_platformWindow->m_handle)
            {
                m_platformWindow->RefreshSize();

                if (m_platformWindow->m_callback)
                    m_platformWindow->m_callback->OnSize(&m_platformWindow->m_window, m_platformWindow->GetWidth(), m_platformWindow->GetHeight());
            }
            break;
        }

        case APP_CMD_WINDOW_REDRAW_NEEDED:
        {
            if (m_platformWindow->m_handle && m_platformWindow->m_callback)
                m_platformWindow->m_callback->OnPaint(&m_platformWindow->m_window);
            break;
        }

        case APP_CMD_GAINED_FOCUS:
        {
            m_platformWindow->SetActiveState(true);
            break;
        }

        case APP_CMD_LOST_FOCUS:
        {
            m_platformWindow->SetActiveState(false);
            break;
        }

        case APP_CMD_DESTROY:
        {
            m_platformWindow->DetachNativeWindow();
            m_running = false;
            break;
        }

        default:
            break;
    }
}

