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
        struct IRHIDescriptor : IBase
        {
            virtual ~IRHIDescriptor() = default;

            virtual ERHIDescriptorType GetDescriptorType() const = 0;

            inline static SGuid GUID() 
            { 
                return {0x493cb1e1, 0x7fa4, 0x485a, {0xb2, 0x4a, 0xb6, 0xe, 0xa3, 0x5d, 0xba, 0x7}}; 
            }
        };
    }
} // namespace krystallic