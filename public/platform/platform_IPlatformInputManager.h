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
        struct IPlatformInputManager : IBase
        {
            virtual ~IPlatformInputManager() = default;

            inline static SGuid GUID()
            {
                return {0x38316cae, 0x3ff0, 0x445c, {0xa1, 0x4, 0x2d, 0xc1, 0xb9, 0x2e, 0xf6, 0xf7}};
            }
        };
    } 
}