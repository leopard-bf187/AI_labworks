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
        struct IRHIVertexBufferSet : IBase
        {
        protected:
            uint m_capacity;
            uint m_currentBufferCount;

            IRHIVertexBufferSet() = default;

        public:
            virtual ~IRHIVertexBufferSet() = default;

            virtual void SetVertexBuffer(uint slot, const IRHIResource* vertexBuffer, uint stride, uint offset) = 0;
            virtual void GetVertexBuffer(uint slot, IRHIResource** vertexBuffer, uint* stride, uint* offset) = 0;

            uint GetCapacity() const { return m_capacity; }

            uint GetCurrentBufferCount() const { return m_currentBufferCount; }

            inline static SGuid GUID() 
            { 
                return
                {0xde2930b0, 0x4ac5, 0x4621, {0xbf, 0x39, 0xc0, 0x46, 0x74, 0x7, 0xa9, 0x63}};
            }
        };
    }
} // namespace krystallic