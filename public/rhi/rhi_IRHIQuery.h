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
        struct IRHIQuery : IBase
        {
            virtual ~IRHIQuery() = default;

            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;

            virtual ERHIQueryType GetType() const = 0;
            virtual ERRCODE       GetDesc(SRHIQueryDesc* outDesc) const = 0;

            inline static SGuid GUID()
            { 
                return {0xba8f3be8, 0xcf33, 0x44fb, {0x81, 0x4d, 0xc2, 0xee, 0x9a, 0x2a, 0x4b, 0xeb}};
            }
        };
    }
} // namespace krystallic