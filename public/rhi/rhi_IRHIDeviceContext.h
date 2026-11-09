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
        struct IRHIDeviceContext : IBase
        {
            virtual ~IRHIDeviceContext() = default;

            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;

            virtual void ClearState() = 0;

            virtual void    BeginQuery(IRHIQuery* query) = 0;
            virtual void    EndQuery(IRHIQuery* query) = 0;
            virtual ERRCODE GetQueryData(IRHIQuery* query, void* outData, uint dataSize, dword flags) = 0;

            virtual ERRCODE SetPredication(IRHIQuery* query, bool value) = 0;
            virtual ERRCODE GetPredication(IRHIQuery** query, bool* value) = 0;

            virtual ERRCODE Map(IRHIResource* resource, uint subresource, ERHIMap mapType, dword mapFlags, SRHIMappedSubresource* mappedResource) = 0;
            virtual void    Unmap(IRHIResource* resource, uint subresource) = 0;

            virtual void CopyResource(IRHIResource* dstResource, IRHIResource* srcResource) = 0;
            virtual void CopyStructureCount(IRHIResource* dstBuffer, uint dstAlignedByteOffset, IRHIUnorderedAccessView* srcView) = 0;
            virtual void CopySubresourceRegion(IRHIResource* dstResource, uint dstSubresource, uint dstX, uint dstY, uint dstZ, IRHIResource* srcResource, uint srcSubresource, const SRHIBox* srcBox) = 0;
            virtual void UpdateSubresource(IRHIResource* dstResource, uint dstSubresource, const SRHIBox* dstBox, const void* pSrcData, uint srcRowPitch, uint srcDepthPitch) = 0;
            virtual void ResolveSubresource(IRHIResource* dstResource, uint dstSubresource, IRHIResource* srcResource, uint srcSubresource, ERHIFormat format) = 0;

            virtual void ClearRenderTarget(uint renderTargetSlot, const float colorRGBA[4]) = 0;
            virtual void ClearDepthStencil(dword clearFlags, float depth, uint8 stencil) = 0;
            virtual void ClearUnorderedAccessViewUint(IRHIUnorderedAccessView* unorderedAccessView, const uint Values[4]) = 0;
            virtual void ClearUnorderedAccessViewFloat(IRHIUnorderedAccessView* unorderedAccessView, const float Values[4]) = 0;

            virtual void IA_SetInputLayout(IRHIInputLayout* format) = 0;
            virtual void IA_GetInputLayout(IRHIInputLayout** format) = 0;
            virtual void IA_SetVertexBuffers(uint startSlot, uint count, IRHIResource* const* buffers, const uint* strides, const uint* offsets) = 0;
            virtual void IA_GetVertexBuffers(uint startSlot, uint count, IRHIResource** buffers, uint* strides, uint* offsets) = 0;
            virtual void IA_SetVertexBuffers(IRHIVertexBufferSet* vbSet) = 0;
            virtual void IA_GetVertexBuffers(IRHIVertexBufferSet* vbSetToStore) = 0;
            virtual void IA_SetIndexBuffer(uint offset, IRHIResource* buffer) = 0;
            virtual void IA_GetIndexBuffer(uint* offset, IRHIResource** buffer) = 0;
            virtual void IA_SetPrimitiveTopology(ERHIPrimitiveTopology primitiveTopology) = 0;
            virtual void IA_GetPrimitiveTopology(ERHIPrimitiveTopology* primitiveTopology) = 0;

            virtual void SetShader(ERHIShaderStage shaderStage, IRHIShader* shader) = 0;
            virtual void GetShader(ERHIShaderStage shaderStage, IRHIShader** outShader) = 0;
            virtual void SetShaderProgram(IRHIShaderProgram* program) = 0;
            virtual void GetShaderProgram(IRHIShaderProgram** outProgram) = 0;
            virtual void SetConstantBuffers(dword shaderStageMask, uint startSlot, uint numBuffers, IRHIConstantBufferView* const* buffers) = 0;
            virtual void SetConstantBuffers(dword shaderStageMask, const IRHIDescriptorSet* constantBufferSet) = 0;
            virtual void SetSamplers(dword shaderStageMask, uint startSlot, uint numSamplers, IRHISampler* const* states) = 0;
            virtual void SetSamplers(dword shaderStageMask, const IRHIDescriptorSet* samplerSet) = 0;
            virtual void SetResources(dword shaderStageMask, uint startSlot, uint numResources, IRHIShaderResourceView* const* resources) = 0;
            virtual void SetResources(dword shaderStageMask, const IRHIDescriptorSet* shaderResourceViewSet) = 0;
            virtual void GetConstantBuffers(ERHIShaderStage shaderStage, uint startSlot, uint numBuffers, IRHIConstantBufferView** outBuffers) = 0;
            virtual void GetConstantBuffers(ERHIShaderStage shaderStage, IRHIDescriptorSet* constantBufferSetToStore) = 0;
            virtual void GetSamplers(ERHIShaderStage shaderStage, uint startSlot, uint numSamplers, IRHISampler** states) = 0;
            virtual void GetSamplers(ERHIShaderStage shaderStage, IRHIDescriptorSet* samplerSetToStore) = 0;
            virtual void GetResources(ERHIShaderStage shaderStage, uint startSlot, uint numResources, IRHIShaderResourceView** resources) = 0;
            virtual void GetResources(ERHIShaderStage shaderStage, IRHIDescriptorSet* shaderResourceViewSetToStore) = 0;

            inline void VS_SetShader(IRHIShader* shader) { return SetShader(RHI_SHADER_STAGE_VERTEX, shader); }
            inline void VS_GetShader(IRHIShader** outShader) { return GetShader(RHI_SHADER_STAGE_VERTEX, outShader); }
            inline void VS_SetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView* const* buffers) { return SetConstantBuffers(RHI_SHADER_STAGE_VERTEX, startSlot, numBuffers, buffers); }
            inline void VS_SetConstantBuffers(const IRHIDescriptorSet* constantBufferSet) { return SetConstantBuffers(RHI_SHADER_STAGE_VERTEX, constantBufferSet); }
            inline void VS_SetSamplers(uint startSlot, uint numSamplers, IRHISampler* const* states) { return SetSamplers(RHI_SHADER_STAGE_VERTEX, startSlot, numSamplers, states); }
            inline void VS_SetSamplers(const IRHIDescriptorSet* samplerSet) { return SetSamplers(RHI_SHADER_STAGE_VERTEX, samplerSet); }
            inline void VS_SetResources(uint startSlot, uint numResources, IRHIShaderResourceView* const* resources) { return SetResources(RHI_SHADER_STAGE_VERTEX, startSlot, numResources, resources); }
            inline void VS_SetResources(const IRHIDescriptorSet* shaderResourceViewSet) { return SetResources(RHI_SHADER_STAGE_VERTEX, shaderResourceViewSet); }
            inline void VS_GetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView** outBuffers) { return GetConstantBuffers(RHI_SHADER_STAGE_VERTEX, startSlot, numBuffers, outBuffers); }
            inline void VS_GetConstantBuffers(IRHIDescriptorSet* constantBufferSetToStore) { return GetConstantBuffers(RHI_SHADER_STAGE_VERTEX, constantBufferSetToStore); }
            inline void VS_GetSamplers(uint startSlot, uint numSamplers, IRHISampler** states) { return GetSamplers(RHI_SHADER_STAGE_VERTEX, startSlot, numSamplers, states); }
            inline void VS_GetSamplers(IRHIDescriptorSet* samplerSetToStore) { return GetSamplers(RHI_SHADER_STAGE_VERTEX, samplerSetToStore); }
            inline void VS_GetResources(uint startSlot, uint numResources, IRHIShaderResourceView** resources) { return GetResources(RHI_SHADER_STAGE_VERTEX, startSlot, numResources, resources); }
            inline void VS_GetResources(IRHIDescriptorSet* shaderResourceViewSetToStore) { return GetResources(RHI_SHADER_STAGE_VERTEX, shaderResourceViewSetToStore); }

            inline void PS_SetShader(IRHIShader* shader) { return SetShader(RHI_SHADER_STAGE_PIXEL, shader); }
            inline void PS_GetShader(IRHIShader** outShader) { return GetShader(RHI_SHADER_STAGE_PIXEL, outShader); }
            inline void PS_SetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView* const* buffers) { return SetConstantBuffers(RHI_SHADER_STAGE_PIXEL, startSlot, numBuffers, buffers); }
            inline void PS_SetConstantBuffers(const IRHIDescriptorSet* constantBufferSet) { return SetConstantBuffers(RHI_SHADER_STAGE_PIXEL, constantBufferSet); }
            inline void PS_SetSamplers(uint startSlot, uint numSamplers, IRHISampler* const* states) { return SetSamplers(RHI_SHADER_STAGE_PIXEL, startSlot, numSamplers, states); }
            inline void PS_SetSamplers(const IRHIDescriptorSet* samplerSet) { return SetSamplers(RHI_SHADER_STAGE_PIXEL, samplerSet); }
            inline void PS_SetResources(uint startSlot, uint numResources, IRHIShaderResourceView* const* resources) { return SetResources(RHI_SHADER_STAGE_PIXEL, startSlot, numResources, resources); }
            inline void PS_SetResources(const IRHIDescriptorSet* shaderResourceViewSet) { return SetResources(RHI_SHADER_STAGE_PIXEL, shaderResourceViewSet); }
            inline void PS_GetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView** outBuffers) { return GetConstantBuffers(RHI_SHADER_STAGE_PIXEL, startSlot, numBuffers, outBuffers); }
            inline void PS_GetConstantBuffers(IRHIDescriptorSet* constantBufferSetToStore) { return GetConstantBuffers(RHI_SHADER_STAGE_PIXEL, constantBufferSetToStore); }
            inline void PS_GetSamplers(uint startSlot, uint numSamplers, IRHISampler** states) { return GetSamplers(RHI_SHADER_STAGE_PIXEL, startSlot, numSamplers, states); }
            inline void PS_GetSamplers(IRHIDescriptorSet* samplerSetToStore) { return GetSamplers(RHI_SHADER_STAGE_PIXEL, samplerSetToStore); }
            inline void PS_GetResources(uint startSlot, uint numResources, IRHIShaderResourceView** resources) { return GetResources(RHI_SHADER_STAGE_PIXEL, startSlot, numResources, resources); }
            inline void PS_GetResources(IRHIDescriptorSet* shaderResourceViewSetToStore) { return GetResources(RHI_SHADER_STAGE_PIXEL, shaderResourceViewSetToStore); }

            inline void GS_SetShader(IRHIShader* shader) { return SetShader(RHI_SHADER_STAGE_GEOMETRY, shader); }
            inline void GS_GetShader(IRHIShader** outShader) { return GetShader(RHI_SHADER_STAGE_GEOMETRY, outShader); }
            inline void GS_SetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView* const* buffers) { return SetConstantBuffers(RHI_SHADER_STAGE_GEOMETRY, startSlot, numBuffers, buffers); }
            inline void GS_SetConstantBuffers(const IRHIDescriptorSet* constantBufferSet) { return SetConstantBuffers(RHI_SHADER_STAGE_GEOMETRY, constantBufferSet); }
            inline void GS_SetSamplers(uint startSlot, uint numSamplers, IRHISampler* const* states) { return SetSamplers(RHI_SHADER_STAGE_GEOMETRY, startSlot, numSamplers, states); }
            inline void GS_SetSamplers(const IRHIDescriptorSet* samplerSet) { return SetSamplers(RHI_SHADER_STAGE_GEOMETRY, samplerSet); }
            inline void GS_SetResources(uint startSlot, uint numResources, IRHIShaderResourceView* const* resources) { return SetResources(RHI_SHADER_STAGE_GEOMETRY, startSlot, numResources, resources); }
            inline void GS_SetResources(const IRHIDescriptorSet* shaderResourceViewSet) { return SetResources(RHI_SHADER_STAGE_GEOMETRY, shaderResourceViewSet); }
            inline void GS_GetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView** outBuffers) { return GetConstantBuffers(RHI_SHADER_STAGE_GEOMETRY, startSlot, numBuffers, outBuffers); }
            inline void GS_GetConstantBuffers(IRHIDescriptorSet* constantBufferSetToStore) { return GetConstantBuffers(RHI_SHADER_STAGE_GEOMETRY, constantBufferSetToStore); }
            inline void GS_GetSamplers(uint startSlot, uint numSamplers, IRHISampler** states) { return GetSamplers(RHI_SHADER_STAGE_GEOMETRY, startSlot, numSamplers, states); }
            inline void GS_GetSamplers(IRHIDescriptorSet* samplerSetToStore) { return GetSamplers(RHI_SHADER_STAGE_GEOMETRY, samplerSetToStore); }
            inline void GS_GetResources(uint startSlot, uint numResources, IRHIShaderResourceView** resources) { return GetResources(RHI_SHADER_STAGE_GEOMETRY, startSlot, numResources, resources); }
            inline void GS_GetResources(IRHIDescriptorSet* shaderResourceViewSetToStore) { return GetResources(RHI_SHADER_STAGE_GEOMETRY, shaderResourceViewSetToStore); }

            inline void HS_SetShader(IRHIShader* shader) { return SetShader(RHI_SHADER_STAGE_HULL, shader); }
            inline void HS_GetShader(IRHIShader** outShader) { return GetShader(RHI_SHADER_STAGE_HULL, outShader); }
            inline void HS_SetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView* const* buffers) { return SetConstantBuffers(RHI_SHADER_STAGE_HULL, startSlot, numBuffers, buffers); }
            inline void HS_SetConstantBuffers(const IRHIDescriptorSet* constantBufferSet) { return SetConstantBuffers(RHI_SHADER_STAGE_HULL, constantBufferSet); }
            inline void HS_SetSamplers(uint startSlot, uint numSamplers, IRHISampler* const* states) { return SetSamplers(RHI_SHADER_STAGE_HULL, startSlot, numSamplers, states); }
            inline void HS_SetSamplers(const IRHIDescriptorSet* samplerSet) { return SetSamplers(RHI_SHADER_STAGE_HULL, samplerSet); }
            inline void HS_SetResources(uint startSlot, uint numResources, IRHIShaderResourceView* const* resources) { return SetResources(RHI_SHADER_STAGE_HULL, startSlot, numResources, resources); }
            inline void HS_SetResources(const IRHIDescriptorSet* shaderResourceViewSet) { return SetResources(RHI_SHADER_STAGE_HULL, shaderResourceViewSet); }
            inline void HS_GetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView** outBuffers) { return GetConstantBuffers(RHI_SHADER_STAGE_HULL, startSlot, numBuffers, outBuffers); }
            inline void HS_GetConstantBuffers(IRHIDescriptorSet* constantBufferSetToStore) { return GetConstantBuffers(RHI_SHADER_STAGE_HULL, constantBufferSetToStore); }
            inline void HS_GetSamplers(uint startSlot, uint numSamplers, IRHISampler** states) { return GetSamplers(RHI_SHADER_STAGE_HULL, startSlot, numSamplers, states); }
            inline void HS_GetSamplers(IRHIDescriptorSet* samplerSetToStore) { return GetSamplers(RHI_SHADER_STAGE_HULL, samplerSetToStore); }
            inline void HS_GetResources(uint startSlot, uint numResources, IRHIShaderResourceView** resources) { return GetResources(RHI_SHADER_STAGE_HULL, startSlot, numResources, resources); }
            inline void HS_GetResources(IRHIDescriptorSet* shaderResourceViewSetToStore) { return GetResources(RHI_SHADER_STAGE_HULL, shaderResourceViewSetToStore); }

            inline void DS_SetShader(IRHIShader* shader) { return SetShader(RHI_SHADER_STAGE_DOMAIN, shader); }
            inline void DS_GetShader(IRHIShader** outShader) { return GetShader(RHI_SHADER_STAGE_DOMAIN, outShader); }
            inline void DS_SetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView* const* buffers) { return SetConstantBuffers(RHI_SHADER_STAGE_DOMAIN, startSlot, numBuffers, buffers); }
            inline void DS_SetConstantBuffers(const IRHIDescriptorSet* constantBufferSet) { return SetConstantBuffers(RHI_SHADER_STAGE_DOMAIN, constantBufferSet); }
            inline void DS_SetSamplers(uint startSlot, uint numSamplers, IRHISampler* const* states) { return SetSamplers(RHI_SHADER_STAGE_DOMAIN, startSlot, numSamplers, states); }
            inline void DS_SetSamplers(const IRHIDescriptorSet* samplerSet) { return SetSamplers(RHI_SHADER_STAGE_DOMAIN, samplerSet); }
            inline void DS_SetResources(uint startSlot, uint numResources, IRHIShaderResourceView* const* resources) { return SetResources(RHI_SHADER_STAGE_DOMAIN, startSlot, numResources, resources); }
            inline void DS_SetResources(const IRHIDescriptorSet* shaderResourceViewSet) { return SetResources(RHI_SHADER_STAGE_DOMAIN, shaderResourceViewSet); }
            inline void DS_GetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView** outBuffers) { return GetConstantBuffers(RHI_SHADER_STAGE_DOMAIN, startSlot, numBuffers, outBuffers); }
            inline void DS_GetConstantBuffers(IRHIDescriptorSet* constantBufferSetToStore) { return GetConstantBuffers(RHI_SHADER_STAGE_DOMAIN, constantBufferSetToStore); }
            inline void DS_GetSamplers(uint startSlot, uint numSamplers, IRHISampler** states) { return GetSamplers(RHI_SHADER_STAGE_DOMAIN, startSlot, numSamplers, states); }
            inline void DS_GetSamplers(IRHIDescriptorSet* samplerSetToStore) { return GetSamplers(RHI_SHADER_STAGE_DOMAIN, samplerSetToStore); }
            inline void DS_GetResources(uint startSlot, uint numResources, IRHIShaderResourceView** resources) { return GetResources(RHI_SHADER_STAGE_DOMAIN, startSlot, numResources, resources); }
            inline void DS_GetResources(IRHIDescriptorSet* shaderResourceViewSetToStore) { return GetResources(RHI_SHADER_STAGE_DOMAIN, shaderResourceViewSetToStore); }

            inline void CS_SetShader(IRHIShader* shader) { return SetShader(RHI_SHADER_STAGE_COMPUTE, shader); }
            inline void CS_GetShader(IRHIShader** outShader) { return GetShader(RHI_SHADER_STAGE_COMPUTE, outShader); }
            inline void CS_SetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView* const* buffers) { return SetConstantBuffers(RHI_SHADER_STAGE_COMPUTE, startSlot, numBuffers, buffers); }
            inline void CS_SetConstantBuffers(const IRHIDescriptorSet* constantBufferSet) { return SetConstantBuffers(RHI_SHADER_STAGE_COMPUTE, constantBufferSet); }
            inline void CS_SetSamplers(uint startSlot, uint numSamplers, IRHISampler* const* states) { return SetSamplers(RHI_SHADER_STAGE_COMPUTE, startSlot, numSamplers, states); }
            inline void CS_SetSamplers(const IRHIDescriptorSet* samplerSet) { return SetSamplers(RHI_SHADER_STAGE_COMPUTE, samplerSet); }
            inline void CS_SetResources(uint startSlot, uint numResources, IRHIShaderResourceView* const* resources) { return SetResources(RHI_SHADER_STAGE_COMPUTE, startSlot, numResources, resources); }
            inline void CS_SetResources(const IRHIDescriptorSet* shaderResourceViewSet) { return SetResources(RHI_SHADER_STAGE_COMPUTE, shaderResourceViewSet); }
            inline void CS_GetConstantBuffers(uint startSlot, uint numBuffers, IRHIConstantBufferView** outBuffers) { return GetConstantBuffers(RHI_SHADER_STAGE_COMPUTE, startSlot, numBuffers, outBuffers); }
            inline void CS_GetConstantBuffers(IRHIDescriptorSet* constantBufferSetToStore) { return GetConstantBuffers(RHI_SHADER_STAGE_COMPUTE, constantBufferSetToStore); }
            inline void CS_GetSamplers(uint startSlot, uint numSamplers, IRHISampler** states) { return GetSamplers(RHI_SHADER_STAGE_COMPUTE, startSlot, numSamplers, states); }
            inline void CS_GetSamplers(IRHIDescriptorSet* samplerSetToStore) { return GetSamplers(RHI_SHADER_STAGE_COMPUTE, samplerSetToStore); }
            inline void CS_GetResources(uint startSlot, uint numResources, IRHIShaderResourceView** resources) { return GetResources(RHI_SHADER_STAGE_COMPUTE, startSlot, numResources, resources); }
            inline void CS_GetResources(IRHIDescriptorSet* shaderResourceViewSetToStore) { return GetResources(RHI_SHADER_STAGE_COMPUTE, shaderResourceViewSetToStore); }

            virtual void SO_SetTargets(uint numBuffers, IRHIResource* const* soTargets, const uint* offsets) = 0;
            virtual void SO_GetTargets(uint numBuffers, IRHIResource** soTargets) = 0;

            virtual void RS_SetState(IRHIRasterizerState* state) = 0;
            virtual void RS_GetState(IRHIRasterizerState** state) = 0;
            virtual void RS_SetViewports(uint numViewports, SRHIViewport* viewports) = 0;
            virtual void RS_GetViewports(uint* numViewports, SRHIViewport* viewports) = 0;
            virtual void RS_SetScissorRects(uint numRects, SRHIScissorRect* scissorRects) = 0;
            virtual void RS_GetScissorRects(uint* numRects, SRHIScissorRect* scissorRects) = 0;

            virtual void OM_UnbindAllTargets() = 0;

            virtual void OM_SetBlendState(IRHIBlendState* state, const float blendFactor[4], uint sampleMask) = 0;
            virtual void OM_GetBlendState(IRHIBlendState** state, float blendFactor[4], uint* sampleMask) = 0;
            virtual void OM_SetDepthStencilState(IRHIDepthStencilState* state, uint stencilRef) = 0;
            virtual void OM_GetDepthStencilState(IRHIDepthStencilState** state, uint* stencilRef) = 0;
            virtual void OM_SetFrameBuffer(IRHIFrameBuffer* FBO) = 0;
            virtual void OM_GetFrameBuffer(IRHIFrameBuffer** outFBO) = 0;

            virtual ERRCODE ValidateShaderPrograms() = 0;

            virtual void DrawAuto() = 0;
            virtual void Draw(uint vertexCount, uint startVertex) = 0;
            virtual void DrawInstanced(uint vertexCountPerInstance, uint instanceCount, uint startVertexLocation, uint startInstanceLocation) = 0;
            virtual void DrawIndexed(uint indexCount, uint startIndexLocation, int baseVertexLocation) = 0;
            virtual void DrawIndexedInstanced(uint indexCountPerInstance, uint instanceCount, uint startIndexLocation, int baseVertexLocation, uint startInstanceLocation) = 0;
            virtual void DrawInstancedIndirect(IRHIResource* bufferForArgs, uint alignedByteOffsetForArgs) = 0;
            virtual void DrawIndexedInstancedIndirect(IRHIResource* bufferForArgs, uint alignedByteOffsetForArgs) = 0;

            virtual void CS_SetUnorderedAccessViews(uint startSlot, uint numUAVs, IRHIUnorderedAccessView* const* unorderedAccessViews, const uint* UAVInitialCounts) = 0;
            virtual void CS_SetUnorderedAccessViews(const IRHIDescriptorSet* UAVSet, const uint* UAVInitialCounts) = 0;
            virtual void CS_GetUnorderedAccessViews(uint startSlot, uint numUAVs, IRHIUnorderedAccessView** unorderedAccessViews) = 0;
            virtual void CS_GetUnorderedAccessViews(IRHIDescriptorSet* UAVSetToStore) = 0;

            virtual void Dispatch(uint threadGroupX, uint threadGroupY, uint threadGroupZ) = 0;
            virtual void DispatchIndirect(IRHIResource* bufferForArgs, uint alignedByteOffsetForArgs) = 0;

            virtual ERRCODE FinishCommandList(bool restoreDeferredContextState, IRHICommandList** commandList) = 0;
            virtual ERRCODE ExecuteCommandList(IRHICommandList* commandList, bool restoreImmediateContextState) = 0;

            inline static SGuid GUID() 
            { 
                return {0xd348c6e2, 0xa082, 0x49f6, {0x96, 0x8a, 0x4c, 0xb2, 0x98, 0x41, 0x18, 0x6d}};
            }
        };
    }
} // namespace krystallic