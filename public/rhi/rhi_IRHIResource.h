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
        struct IRHIResource : IBase
        {
            virtual ~IRHIResource() = default;

            virtual ERHIResourceType GetType() const = 0;
            virtual ERRCODE          GetDesc(SRHIResourceDesc* desc) = 0;

            inline static SGuid GUID() 
            { 
                return {0x608fb8c4, 0x26d1, 0x4123, {0x83, 0x1f, 0x83, 0x29, 0x12, 0x6, 0xee, 0x89}};
            }
        };
    }
} // namespace krystallic