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
        struct IRHIShaderProgram : IBase
        {
        protected:
            dword                  m_shaderStageMask = 0ul;
            ERHIRenderPipelineType m_pipelineType = RHI_RENDER_PIPELINE_UNKNOWN;

            IRHIShaderProgram() = default;

        public:
            virtual ~IRHIShaderProgram() = default;

            bool HasShader(ERHIShaderStage stage) const { return (m_shaderStageMask & stage) != 0ul; };

            dword GetShaderStageMask() const { return m_shaderStageMask; }

            ERHIRenderPipelineType GetPipelineType() const { return m_pipelineType; }

            virtual bool    IsValid() const = 0;
            virtual ERRCODE GetShader(ERHIShaderStage stage, IRHIShader** shader) = 0;
            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;

            inline static SGuid GUID() 
            {
                return {0x5dd2a889, 0x1a7b, 0x426f, {0xa1, 0xa4, 0x4e, 0x1b, 0x4a, 0x72, 0xa1, 0x5b}};
            }
        };
    }
} // namespace krystallic