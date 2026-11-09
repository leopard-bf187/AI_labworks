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
#include "rhi_IRHIDescriptor.h"


namespace krystallic
{
    namespace RenderHardwareInterface
    {
        struct IRHISampler : IRHIDescriptor
        {
            virtual ~IRHISampler() = default;

            virtual ERRCODE GetDesc(SRHISamplerDesc* outDesc) = 0;

            inline static SGuid GUID() 
            {
                return {0x574a936b, 0xe82, 0x4786, {0x91, 0x5, 0x84, 0x50, 0xb5, 0xaf, 0x36, 0xae}};
            }
        };
    }
} // namespace krystallic