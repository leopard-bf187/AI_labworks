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
        struct IRHIDepthStencilState : IBase
        {
            virtual ~IRHIDepthStencilState() = default;

            virtual ERRCODE GetDesc(SRHIDepthStencilStateDesc* outDesc) = 0;

            inline static SGuid GUID() 
            { 
                return {0x46ae405a, 0xc838, 0x4231, {0xa2, 0x49, 0xb4, 0x1e, 0xf0, 0x2d, 0x17, 0x62}}; 
            }
        };
    }
} // namespace krystallic