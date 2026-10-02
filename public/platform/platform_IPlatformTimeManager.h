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
        struct IPlatformTimeManager : IBase
        {
            virtual Tick GetSystemTime() = 0;
            virtual Tick GetUTCTime() = 0;

            virtual Tick GetMonotonicTime() = 0;

            virtual Tick GetTicks() = 0;

            virtual Tick GetPerformanceCounter() = 0;
            virtual Tick GetPerformanceFrequency() = 0;

            virtual ERRCODE CreateTimer(ISystemTimer** outTimer) = 0;

            inline static SGuid GUID()
            {
                return { 0x1b9a75fb, 0x7b5d, 0x4fee, {0xb6, 0x1d, 0xd, 0x8b, 0x1e, 0xd8, 0xb0, 0x83} };
            }
        };

        struct ISystemTimer : IBase
        {
            virtual void Start() = 0;
            virtual void Stop() = 0;
            virtual void Reset() = 0;

            virtual double GetSeconds() = 0;
            virtual double GetMilliseconds() = 0;
            virtual double GetMicroseconds() = 0;

            inline static SGuid GUID()
            {
                return { 0x87a31b91, 0x7fb2, 0x4be9, { 0xa9, 0x7a, 0x39, 0x6b, 0xf3, 0xb5, 0x15, 0xe8 } };
            }
        };
    } 
}