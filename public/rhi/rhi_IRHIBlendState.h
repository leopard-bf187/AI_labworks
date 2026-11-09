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
        struct IRHIBlendState : IBase
        {
            virtual ~IRHIBlendState() = default;

            virtual ERRCODE GetDesc(SRHIBlendStateDesc* outDesc) = 0;

            inline static SGuid GUID() 
            { 
                return {0xd89648e1, 0x5189, 0x481c, {0x8f, 0x95, 0xe3, 0x4b, 0x50, 0xcd, 0x7f, 0x35}}; 
            }
        };
    }
} // namespace krystallic