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
        struct IRHIDepthStencilView : IRHIResourceView
        {
            virtual ~IRHIDepthStencilView() = default;

            virtual ERRCODE GetDesc(SRHIDepthStencilViewDesc* outDesc) = 0;

            inline static SGuid GUID() 
            { 
                return {0x4eee8a9f, 0x7118, 0x45ee, {0x92, 0x8c, 0x27, 0xe1, 0x8f, 0x14, 0xe3, 0x98}}; 
            }
        };
    }
} // namespace krystallic