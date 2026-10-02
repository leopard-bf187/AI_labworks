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
        struct IPlatformThreadManager : IBase
        {
            virtual ~IPlatformThreadManager() = default;

            inline static SGuid GUID()
            {
                return {0x1c7c3b80, 0x9c91, 0x4593, {0x8b, 0x82, 0x20, 0xf, 0x7b, 0x76, 0xce, 0xed}};
            }
        };
    } 
}
