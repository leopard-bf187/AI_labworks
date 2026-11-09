/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 
*
*  Description: 
*
*  Date: 13.06.2026
*/


#pragma once


#include "rhi_dll_types.h"


namespace krystallic
{
    namespace RenderHardwareInterface
    {
        struct IRHICommandList : IBase
        {
            virtual ~IRHICommandList() = default;

            // indev

            inline static SGuid GUID() 
            { 
                return {0xf5238928, 0xefc0, 0x466a, {0xa1, 0x63, 0xa2, 0x24, 0x80, 0xfd, 0x56, 0x42}}; 
            }
        };
    }
} // namespace krystallic