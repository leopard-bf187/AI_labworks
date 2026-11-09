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
        struct IRHIRenderTargetView : IRHIResourceView
        {
            virtual ~IRHIRenderTargetView() = default;

            virtual ERRCODE GetDesc(SRHIRenderTargetViewDesc* outDesc) = 0;

            inline static SGuid GUID() 
            {
                return {0x5e5a3137, 0x191c, 0x467d, {0xa8, 0x23, 0x30, 0x55, 0xa1, 0xb6, 0xde, 0x90}};
            }
        };
    }
} // namespace krystallic