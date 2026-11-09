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
        struct IRHIShaderResourceView : IRHIResourceView
        {
            virtual ~IRHIShaderResourceView() = default;

            virtual ERRCODE GetDesc(SRHIShaderResourceViewDesc* outDesc) = 0;

            inline static SGuid GUID() 
            {
                return {0xa5f3e841, 0x5445, 0x436c, {0x90, 0x7a, 0x6a, 0x68, 0x22, 0x8d, 0x6, 0xed}};
            }
        };
    }
} // namespace krystallic