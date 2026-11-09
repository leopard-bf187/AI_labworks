/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @Ra192192
*
*  Description:
*
*  Date: 02.06.2026
*/


#pragma once


#include "platform_dll_types.h"


namespace krystallic
{
    namespace Platform
    {

        struct IPlatformManager : IBase
        {
            virtual ~IPlatformManager() = default;

            virtual EPlatformType GetPlatformType() const = 0;

            virtual ERRCODE QueryMemoryManager(IPlatformMemoryManager** outManager) = 0;
            virtual ERRCODE QueryThreadManager(IPlatformThreadManager** outMgr) = 0;
            virtual ERRCODE QueryProcessManager(IPlatformProcessManager** outMgr) = 0;
            virtual ERRCODE QueryTimeManager(IPlatformTimeManager** outTimer) = 0;
            virtual ERRCODE QueryInputManager(IPlatformInputManager** outMgr) = 0;
            virtual ERRCODE QuerySystemInfo(IPlatformSystemInfo** outSysInfo) = 0;

            virtual ERRCODE CreateNativeWindow(const Stdlib::String& title, const SPlatformWindowDesc* desc, IPlatformWindowCallback* optionalWndCallback, IPlatformWindow* parentWindow, IPlatformWindow** outWindow) = 0;

            virtual bool PollEvents() = 0;
            virtual bool IsRunning() const = 0;
            virtual void RequestQuit(int exitCode) = 0;

            virtual ERRCODE GetNativeHandle(SNativeHandle* appInstanceHandle) const = 0;

            inline static SGuid GUID()
            {
                return {0x38316cae, 0x3ff0, 0x445c, {0xa1, 0x4, 0x2d, 0xc1, 0xb9, 0x2e, 0xf6, 0xf7}};
            }
        };
    } 
}