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
        struct IRHIDevice : IBase
        {
            virtual ~IRHIDevice() = default;

            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;

            virtual ERRCODE GetFrameDebug(IRHIFrameDebug** outFrameDebug) = 0;
            virtual ERRCODE GetDebugInterface(IRHIDebug** outDebug) = 0;
            virtual ERRCODE GetDeviceFeatures(IRHIDeviceFeatures** outFeatures) = 0;

            virtual ERRCODE GetImmediateContext(IRHIDeviceContext** outContext) = 0;
            virtual ERRCODE GetShaderCompiler(IRHIShaderCompiler** outCompiler) = 0;

            virtual ERRCODE CreateDeferredContext(IRHIDeviceContext** outDeferredContext) = 0;
            virtual ERRCODE CreateInputLayout(uint numElements, const SRHIInputElementDesc* elements, Common::IBuffer* vertexShaderByteCode, IRHIInputLayout** outInputLayout) = 0;
            virtual ERRCODE CreateFrameBuffer(const SRHIFrameBufferDesc* desc, IRHIFrameBuffer** outFrameBuffer) = 0;
            virtual ERRCODE CreateResource(SRHIResourceDesc* desc, SRHISubresourceData* initialData, IRHIResource** outResource) = 0;
            virtual ERRCODE CreateVertexBufferSet(uint capacity, IRHIVertexBufferSet** vbSet) = 0;
            virtual ERRCODE CreateDescriptorSet(ERHIDescriptorType type, uint capacity, IRHIDescriptorSet** outDescriptorSet) = 0;
            virtual ERRCODE CreateConstantBufferView(IRHIResource* constantBuffer, IRHIConstantBufferView** outView, IRHIDescriptorSet* setToStore) = 0;
            virtual ERRCODE CreateShaderResourceView(IRHIResource* resource, const SRHIShaderResourceViewDesc* desc, IRHIShaderResourceView** outView, IRHIDescriptorSet* SRVSetToStore) = 0;
            virtual ERRCODE CreateRenderTargetView(IRHIResource* resource, const SRHIRenderTargetViewDesc* desc, IRHIRenderTargetView** outView, IRHIDescriptorSet* RTVSetToStore) = 0;
            virtual ERRCODE CreateDepthStencilView(IRHIResource* resource, const SRHIDepthStencilViewDesc* desc, IRHIDepthStencilView** outView, IRHIDescriptorSet* DSVSetToStore) = 0;
            virtual ERRCODE CreateUnorderedAccessView(IRHIResource* resource, const SRHIUnorderedAccessViewDesc* desc, IRHIUnorderedAccessView** outView, IRHIDescriptorSet* UAVSetToStore) = 0;
            virtual ERRCODE CreateSampler(const SRHISamplerDesc* desc, IRHISampler** outSampler, IRHIDescriptorSet* samplerSetToStore) = 0;
            virtual ERRCODE CreateRasterizerState(const SRHIRasterizerStateDesc* desc, IRHIRasterizerState** outState) = 0;
            virtual ERRCODE CreateBlendState(const SRHIBlendStateDesc* desc, IRHIBlendState** outState) = 0;
            virtual ERRCODE CreateDepthStencilState(const SRHIDepthStencilStateDesc* desc, IRHIDepthStencilState** outState) = 0;
            virtual ERRCODE CreateShader(ERHIShaderStage shaderStage, Common::IBuffer* byteCode, IRHIShader** outShader, const char* debugName = nullptr) = 0;
            virtual ERRCODE CreateShaderProgram(const SRHIShaderProgramDesc* desc, IRHIShaderProgram** outShaderProgram) = 0;

            // indev
            virtual ERRCODE CreateQuery(const SRHIQueryDesc* desc, IRHIQuery** outQuery) = 0;

            inline static SGuid GUID() 
            { 
                return {0x44afa32f, 0xfa44, 0x47b4, {0xaf, 0xb2, 0x88, 0xeb, 0xf7, 0x15, 0xa7, 0x3c}}; 
            }
        };
    }
} // namespace krystallic