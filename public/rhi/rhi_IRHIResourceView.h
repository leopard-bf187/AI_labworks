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
        struct IRHIResourceView : IRHIDescriptor
        {
            virtual ~IRHIResourceView() = default;

            virtual ERRCODE GetResource(IRHIResource** outResource) = 0;

            inline static SGuid GUID() 
            {
                return {0x42d44cfb, 0x6dc, 0x439a, {0xbe, 0xfa, 0x78, 0xde, 0x74, 0xbc, 0xbb, 0xd1}};
            }
        };
    }
} // namespace krystallic