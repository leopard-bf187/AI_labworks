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
        struct IRHIShaderReflection : IBase
        {
            virtual ~IRHIShaderReflection() = default;

            inline static SGuid GUID() 
            {
                return {0xcfba2100, 0xa56e, 0x4fcb, {0x9b, 0x92, 0x2c, 0xb5, 0x1b, 0x9, 0xb1, 0xc2}};
            }
        };
    }
} // namespace krystallic