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
        struct IRHIShader : IBase
        {
            virtual ~IRHIShader() = default;

            virtual ERHIShaderLanguage GetLanguage() const = 0;
            virtual ERRCODE            GetRuntimeDesc(SRHIShaderRuntimeDesc* desc) const = 0;
            virtual ERRCODE            GetLastError(Common::IError** outErrorObject) = 0;

            inline static SGuid GUID() 
            { 
                return {0x286debd9, 0x7d7f, 0x4556, {0xa9, 0xfa, 0x80, 0xb0, 0x98, 0x8e, 0xe4, 0x8d}};
            }
        };
    }
} // namespace krystallic