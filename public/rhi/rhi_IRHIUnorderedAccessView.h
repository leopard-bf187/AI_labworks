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
        struct IRHIUnorderedAccessView : IRHIResourceView
        {
            virtual ~IRHIUnorderedAccessView() = default;

            virtual ERRCODE GetDesc(SRHIUnorderedAccessViewDesc* outDesc) = 0;

            inline static SGuid GUID() 
            {
                return {0x603352cd, 0x4a5b, 0x4e68, {0xbc, 0x9e, 0xd0, 0x5e, 0x2, 0x5d, 0x1a, 0xc1}};
            }
        };
    }
} // namespace krystallic