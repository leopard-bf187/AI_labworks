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
#include "rhi_IRHIResourceView.h"


namespace krystallic
{
    namespace RenderHardwareInterface
    {
        struct IRHIConstantBufferView : IRHIResourceView
        {
            virtual ~IRHIConstantBufferView() = default;

            inline static SGuid GUID() 
            { 
                return {0x192bdd04, 0xb31c, 0x4326, {0x89, 0x51, 0xd0, 0x87, 0xdc, 0xbd, 0xee, 0x76}}; 
            }
        };
    }
} // namespace krystallic