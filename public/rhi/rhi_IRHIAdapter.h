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
        struct IRHIAdapter : IBase
        {
            virtual ~IRHIAdapter() = default;

            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;
            virtual ERRCODE GetDesc(SRHIAdapterDesc* outDesc) = 0;
            virtual uint    GetDisplayCount() = 0;
            virtual ERRCODE GetDisplay(uint index, IRHIDisplay** outDisplay) = 0;

            inline static SGuid GUID() 
            { 
                return {0x1c29e277, 0xd8c0, 0x4918, {0x89, 0x70, 0xd9, 0xb8, 0xe1, 0x2, 0xa2, 0xa3}}; 
            }
        };
    }
} // namespace krystallic