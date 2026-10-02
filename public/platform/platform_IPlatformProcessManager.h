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
        struct IPlatformProcessManager : IBase
        {
            virtual ~IPlatformProcessManager() = default;

            inline static SGuid GUID()
            {
                return {0xcaa43ca1, 0x9ecf, 0x48d7, {0x80, 0x6a, 0x22, 0x2, 0x2b, 0xc8, 0xb0, 0x2d}};
            }
        };

        struct IProcess
        {
            virtual ERRCODE GetMemoryStatus(SProcessMemoryStatus* outStatus) const = 0;
        };
    } 
} 
