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
        struct IRHIInputLayout : IBase
        {
            virtual ~IRHIInputLayout() = default;

            inline static SGuid GUID() 
            { 
                return {0x6baba57c, 0xb0f0, 0x46e3, {0xaf, 0x82, 0xc2, 0x7f, 0x38, 0x84, 0xae, 0x96}}; 
            }
        };
    }
} // namespace krystallic