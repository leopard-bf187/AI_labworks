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
        struct IRHIDescriptorSet : IBase
        {
        protected:
            uint m_capacity = 0;
            uint m_count = 0;
            ERHIDescriptorType m_type = RHI_DESCRIPTOR_TYPE_LAST_VALUE;

            IRHIDescriptorSet() = default;

        public:
            virtual ~IRHIDescriptorSet() = default;

            uint GetCapacity() const { return m_capacity; }
            uint GetCount() const { return m_count; }
            ERHIDescriptorType GetType() const { return m_type; }

            virtual ERRCODE GetDescriptor(uint index, IRHIDescriptor** outDescriptor) = 0;
            virtual ERRCODE SetDescriptor(uint index, IRHIDescriptor* outDescriptor) = 0;

            inline static SGuid GUID() 
            { 
                return {0x5d35e1b8, 0x26d1, 0x4f41, {0x8d, 0x7e, 0x3d, 0xe8, 0xf4, 0x9b, 0xe5, 0x16}}; 
            }
        };
    }
} // namespace krystallic