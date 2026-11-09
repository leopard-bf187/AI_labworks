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
        struct IRHIRasterizerState : IBase
        {
            virtual ~IRHIRasterizerState() = default;

            virtual ERRCODE GetDesc(SRHIRasterizerStateDesc* outDesc) = 0;

            inline static SGuid GUID()
            { 
                return {0xad0b4a4f, 0xdb18, 0x4630, {0x86, 0xbb, 0x89, 0x99, 0xb, 0x55, 0xb8, 0x69}};
            }
        };
    }
} // namespace krystallic