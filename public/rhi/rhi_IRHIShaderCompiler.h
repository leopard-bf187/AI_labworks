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
        struct IRHIShaderCompiler : IBase
        {
            virtual ~IRHIShaderCompiler() = default;

            virtual ERRCODE Compile(Common::IBuffer* source, const SRHIShaderCompileDesc* desc, Common::IBuffer** outShaderCompiledData) = 0;
            virtual ERRCODE Compile(const char* buff, const uint buffLen, const SRHIShaderCompileDesc* desc, Common::IBuffer** outShaderCompiledData) = 0;
            virtual ERRCODE Reflect(Common::IBuffer* shaderCompiledData, IRHIShaderReflection** shaderReflection) = 0;

            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;

            inline static SGuid GUID() 
            { 
                return {0x5bf60e72, 0x3ca5, 0x4df1, {0xa8, 0xb4, 0x5d, 0x39, 0x2a, 0xac, 0x2c, 0x5c}};
            }
        };
    }
} // namespace krystallic