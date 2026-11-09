/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Description: GUI render strategy on Krystallic RHI / OpenGL 4.6.
*/

#include "GUIRHI.h"
#include <cstddef>
#include <cstdio>
#include <cstring>

IGUIRenderStrategy* g_guiRenderer = nullptr;

static const char* g_guiVertexGLSL = R"GLSL(
#version 460 core
layout(location=0) in vec2 a_position;
layout(location=1) in vec2 a_uv;
layout(location=2) in vec4 a_color;
layout(location=3) in uint a_textureSlot;
layout(location=4) in uint a_textureIndex;

layout(std140, binding=0) uniform UIProjection
{
    mat4 projection;
};
out vec2 v_uv;
out vec4 v_color;
flat out uint v_slot;
flat out uint v_index;
flat out uint v_font;
void main()
{
    gl_Position = projection * vec4(a_position, 0.0, 1.0);
    v_uv = a_uv;
    v_color = a_color;
    v_slot = a_textureSlot & 15u;
    v_index = a_textureIndex;
    v_font = (a_textureSlot & 128u) != 0u ? 1u : 0u;
}
)GLSL";

static const char* g_guiPixelGLSL = R"GLSL(
#version 460 core
layout(location=0) out vec4 o_color;
layout(binding=0) uniform sampler2D font0;
layout(binding=1) uniform sampler2D font1;
in vec2 v_uv;
in vec4 v_color;
flat in uint v_slot;
flat in uint v_index;
flat in uint v_font;
void main()
{
    if (v_font == 0u)
    {
        o_color = v_color;
        return;
    }
    vec4 samples = v_slot == 0u ? texture(font0, v_uv) : texture(font1, v_uv);
    float opacity = samples[min(v_index, 3u)];
    o_color = vec4(v_color.rgb, v_color.a * opacity);
}
)GLSL";


void CGUIRHIStrategy::Initialize(IRHIDevice* device, IRHIDeviceContext* context, IRHIShaderCompiler* compiler, ERHIBackend backend)
{
    m_device = device;
    m_context = context;
    m_compiler = compiler;
    m_backend = backend;
}


bool CGUIRHIStrategy::CompileShader(ERHIShaderStage stage, const char* source, const char* entryPoint, RefCounted<IRHIShader>& shader, Common::IBuffer** byteCode)
{
    if (!m_compiler || !m_device)
        return false;

    SRHIShaderCompileDesc desc{};
    desc.language = RHI_GLSL;
    desc.stage = stage;
    desc.entryPoint = entryPoint;
    desc.target.glsl = SRHIShaderTarget::GL_460;

    RefCounted<Common::IBuffer> result;
    if (m_compiler->Compile(source, static_cast<uint>(std::strlen(source)), &desc, result.ReleaseAndGetAddressOf()) != RHI_ERR_OK)
        return false;

    if (m_device->CreateShader(stage, result.Get(), shader.ReleaseAndGetAddressOf(), entryPoint) != RHI_ERR_OK)
        return false;

    if (byteCode)
    {
        result->IncRef();
        *byteCode = result.Get();
    }

    return true;
}


ERRCODE CGUIRHIStrategy::OnResourceCreate(GUI_RENDERABLE_RESOURCE_TYPE type, void* creationStructurePtr, void** outHandle)
{
    if (!outHandle || !m_device)
        return GUI_ERR_INVALID_ARG;

    *outHandle = nullptr;

    if (type == GUI_RENDERABLE_RESOURCE_INPUT_LAYOUT)
    {
        SRHIInputElementDesc elements[5]{};
        const char* names[] = { "POSITION", "TEXCOORD", "COLOR", "TEXTURE_SLOT", "TEXTURE_INDEX" };
        const ERHIFormat formats[] = { RHI_FORMAT_R32G32_FLOAT, RHI_FORMAT_R32G32_FLOAT, RHI_FORMAT_R32G32B32A32_FLOAT, RHI_FORMAT_R32_UINT, RHI_FORMAT_R32_UINT };
        const uint offsets[] = { offsetof(XYUV1C32DW2, pos), offsetof(XYUV1C32DW2, uv), offsetof(XYUV1C32DW2, color), offsetof(XYUV1C32DW2, textureSlot), offsetof(XYUV1C32DW2, textureArrayIndex) };

        for (uint i = 0; i < 5; ++i)
        {
            elements[i].semanticName = names[i];
            elements[i].format = formats[i];
            elements[i].alignedByteOffset = offsets[i];
            elements[i].inputSlotClass = RHI_INPUT_PER_VERTEX_DATA;
        }

        IRHIInputLayout* layout = nullptr;
        if (m_device->CreateInputLayout(5, elements, m_vertexByteCode.Get(), &layout) != RHI_ERR_OK)
            return GUI_ERR_INVALID_RENDER_STRATEGY;

        *outHandle = layout;
        return GUI_ERR_OK;
    }

    if (type == GUI_RENDERABLE_RESOURCE_VERTEX_BUFFER || type == GUI_RENDERABLE_RESOURCE_INDEX_BUFFER)
    {
        GUI_CREATE_BUFF* guiDesc = static_cast<GUI_CREATE_BUFF*>(creationStructurePtr);
        if (!guiDesc)
            return GUI_ERR_INVALID_ARG;

        SRHIResourceDesc desc{};
        desc.type = RHI_RESOURCE_BUFFER;
        desc.width = guiDesc->m_bufferSize;
        desc.format = type == GUI_RENDERABLE_RESOURCE_INDEX_BUFFER ? RHI_FORMAT_R32_UINT : RHI_FORMAT_UNKNOWN;
        desc.usage = RHI_USAGE_DYNAMIC;
        desc.cpuAccess = RHI_CPU_WRITE;
        desc.bindFlags = type == GUI_RENDERABLE_RESOURCE_VERTEX_BUFFER ? RHI_BIND_VERTEX_BUFFER : RHI_BIND_INDEX_BUFFER;

        IRHIResource* resource = nullptr;
        if (m_device->CreateResource(&desc, nullptr, &resource) != RHI_ERR_OK)
            return GUI_ERR_INVALID_RENDER_STRATEGY;

        *outHandle = resource;
        return GUI_ERR_OK;
    }

    if (type == GUI_RENDERABLE_RESOURCE_TEXTURE_2D)
    {
        GUI_CREATE_TEXTURE* guiDesc = static_cast<GUI_CREATE_TEXTURE*>(creationStructurePtr);
        if (!guiDesc || guiDesc->m_format != GUI_RGBA8_UNORM)
            return GUI_ERR_INVALID_ARG;

        SGUIRHITexture* texture = new SGUIRHITexture;
        SRHIResourceDesc desc{};
        desc.type = RHI_RESOURCE_TEXTURE2D;
        desc.width = guiDesc->m_width;
        desc.height = guiDesc->m_height;
        desc.mipLevels = guiDesc->m_mips ? guiDesc->m_mips : 1;
        desc.arraySize = 1;
        desc.sampleDesc.count = 1;
        desc.format = RHI_FORMAT_R8G8B8A8_UNORM;
        desc.usage = RHI_USAGE_DEFAULT;
        desc.cpuAccess = RHI_CPU_DEFAULT;
        desc.bindFlags = RHI_BIND_SHADER_RESOURCE;

        SRHISubresourceData data{};
        data.sysMem = guiDesc->m_initialData;
        data.sysMemPitch = guiDesc->m_rowPitch;

        if (m_device->CreateResource(&desc, data.sysMem ? &data : nullptr, texture->resource.ReleaseAndGetAddressOf()) != RHI_ERR_OK ||
            m_device->CreateShaderResourceView(texture->resource.Get(), nullptr, texture->view.ReleaseAndGetAddressOf(), nullptr) != RHI_ERR_OK)
        {
            delete texture;
            return GUI_ERR_INVALID_RENDER_STRATEGY;
        }

        *outHandle = texture;
        return GUI_ERR_OK;
    }

    return GUI_ERR_NOT_IMPLEMENTED;
}


ERRCODE CGUIRHIStrategy::OnResourceDestroy(GUI_RENDERABLE_RESOURCE_TYPE type, void* handle)
{
    if (!handle)
        return GUI_ERR_OK;

    if (type == GUI_RENDERABLE_RESOURCE_TEXTURE_2D)
        delete static_cast<SGUIRHITexture*>(handle);
    else if (type == GUI_RENDERABLE_RESOURCE_INPUT_LAYOUT)
        static_cast<IRHIInputLayout*>(handle)->Delete();
    else if (type == GUI_RENDERABLE_RESOURCE_VERTEX_BUFFER || type == GUI_RENDERABLE_RESOURCE_INDEX_BUFFER)
        static_cast<IRHIResource*>(handle)->Delete();
    else
        return GUI_ERR_NOT_IMPLEMENTED;

    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnResourceMap(GUI_RENDERABLE_RESOURCE_TYPE type, void* resourceHandle, uint subresource, GUI_MAPPED_RESOURCE* outMappedResource)
{
    if (!resourceHandle || !outMappedResource || (type != GUI_RENDERABLE_RESOURCE_VERTEX_BUFFER && type != GUI_RENDERABLE_RESOURCE_INDEX_BUFFER))
        return GUI_ERR_INVALID_ARG;

    SRHIMappedSubresource mapped{};
    if (m_context->Map(static_cast<IRHIResource*>(resourceHandle), subresource, RHI_MAP_WRITE_DISCARD, 0, &mapped) != RHI_ERR_OK)
        return GUI_ERR_INVALID_RENDER_STRATEGY;

    outMappedResource->m_data = mapped.pData;
    outMappedResource->m_rowPitch = mapped.RowPitch;
    outMappedResource->m_depthPitch = mapped.DepthPitch;
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnResourceUnmap(GUI_RENDERABLE_RESOURCE_TYPE type, void* resourceHandle, uint subresource)
{
    if (type != GUI_RENDERABLE_RESOURCE_VERTEX_BUFFER && type != GUI_RENDERABLE_RESOURCE_INDEX_BUFFER)
        return GUI_ERR_INVALID_ARG;

    m_context->Unmap(static_cast<IRHIResource*>(resourceHandle), subresource);
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnResourceUpdateRegion(GUI_RENDERABLE_RESOURCE_TYPE type, void* resourceHandle, const GUI_RECT& dstRectPixels, const void* srcData, uint srcRowPitch)
{
    if (type != GUI_RENDERABLE_RESOURCE_TEXTURE_2D || !resourceHandle || !srcData)
        return GUI_ERR_INVALID_ARG;

    SGUIRHITexture* texture = static_cast<SGUIRHITexture*>(resourceHandle);
    SRHIBox box{};
    box.left = static_cast<uint>(dstRectPixels.m_left);
    box.top = static_cast<uint>(dstRectPixels.m_top);
    box.right = static_cast<uint>(dstRectPixels.m_right);
    box.bottom = static_cast<uint>(dstRectPixels.m_bottom);
    box.back = 1;
    m_context->UpdateSubresource(texture->resource.Get(), 0, &box, srcData, srcRowPitch, 0);
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnRenderCreate()
{
    if (m_backend != RHI_BACKEND_OPENGL || !CompileShader(RHI_SHADER_STAGE_VERTEX, g_guiVertexGLSL, "main", m_vertexShader, m_vertexByteCode.ReleaseAndGetAddressOf()) ||
        !CompileShader(RHI_SHADER_STAGE_PIXEL, g_guiPixelGLSL, "main", m_pixelShader))
        return GUI_ERR_INVALID_RENDER_STRATEGY;

    SRHIShaderProgramDesc program{};
    program.pipeline = RHI_RENDER_PIPELINE_GRAPHICS;
    program.vertexShader = m_vertexShader.Get();
    program.pixelShader = m_pixelShader.Get();
    program.debugName = "GUI GL4";
    if (m_device->CreateShaderProgram(&program, m_program.ReleaseAndGetAddressOf()) != RHI_ERR_OK)
        return GUI_ERR_INVALID_RENDER_STRATEGY;

    SRHIResourceDesc cb{};
    cb.type = RHI_RESOURCE_BUFFER;
    cb.width = 64;
    cb.usage = RHI_USAGE_DEFAULT;
    cb.cpuAccess = RHI_CPU_DEFAULT;
    cb.bindFlags = RHI_BIND_CONSTANT_BUFFER;
    if (m_device->CreateResource(&cb, nullptr, m_projectionBuffer.ReleaseAndGetAddressOf()) != RHI_ERR_OK ||
        m_device->CreateConstantBufferView(m_projectionBuffer.Get(), m_projectionView.ReleaseAndGetAddressOf(), nullptr) != RHI_ERR_OK)
        return GUI_ERR_INVALID_RENDER_STRATEGY;

    SRHISamplerDesc sampler{};
    sampler.Filter = RHI_FILTER_MIN_MAG_MIP_POINT;
    sampler.AddressU = sampler.AddressV = sampler.AddressW = RHI_TEXTURE_ADDRESS_CLAMP;
    sampler.MaxAnisotropy = 1;
    sampler.MaxLOD = 1.0f;
    if (m_device->CreateSampler(&sampler, m_pointSampler.ReleaseAndGetAddressOf(), nullptr) != RHI_ERR_OK)
        return GUI_ERR_INVALID_RENDER_STRATEGY;

    SRHIRasterizerStateDesc raster{};
    raster.FillMode = RHI_FILL_SOLID;
    raster.CullMode = RHI_CULL_NONE;
    raster.DepthClipEnable = true;
    raster.ScissorEnable = true;
    if (m_device->CreateRasterizerState(&raster, m_rasterizer.ReleaseAndGetAddressOf()) != RHI_ERR_OK)
        return GUI_ERR_INVALID_RENDER_STRATEGY;

    SRHIBlendStateDesc blend{};
    blend.RenderTarget[0].BlendEnable = true;
    blend.RenderTarget[0].SrcBlend = RHI_BLEND_SRC_ALPHA;
    blend.RenderTarget[0].DestBlend = RHI_BLEND_INV_SRC_ALPHA;
    blend.RenderTarget[0].BlendOp = RHI_BLEND_OP_ADD;
    blend.RenderTarget[0].SrcBlendAlpha = RHI_BLEND_ONE;
    blend.RenderTarget[0].DestBlendAlpha = RHI_BLEND_INV_SRC_ALPHA;
    blend.RenderTarget[0].BlendOpAlpha = RHI_BLEND_OP_ADD;
    blend.RenderTarget[0].RenderTargetWriteMask = RHI_COLOR_WRITE_MASK_ALL;
    if (m_device->CreateBlendState(&blend, m_blend.ReleaseAndGetAddressOf()) != RHI_ERR_OK)
        return GUI_ERR_INVALID_RENDER_STRATEGY;

    SRHIDepthStencilStateDesc depth{};
    depth.DepthEnable = false;
    depth.DepthWriteMask = RHI_DWM_ZERO;
    depth.DepthFunc = RHI_CMP_ALWAYS;
    if (m_device->CreateDepthStencilState(&depth, m_depth.ReleaseAndGetAddressOf()) != RHI_ERR_OK)
        return GUI_ERR_INVALID_RENDER_STRATEGY;

    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnRenderDestroy()
{
    m_vertexByteCode.Reset();
    m_vertexShader.Reset();
    m_pixelShader.Reset();
    m_program.Reset();
    m_projectionView.Reset();
    m_projectionBuffer.Reset();
    m_pointSampler.Reset();
    m_rasterizer.Reset();
    m_blend.Reset();
    m_depth.Reset();
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnRenderReset()
{
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnRenderUpdate()
{
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnRenderResize(GUI_VIEWPORT vp)
{
    m_viewport = vp;
    if (!vp.m_width || !vp.m_height)
        return GUI_ERR_INVALID_ARG;

    const float width = vp.m_width;
    const float height = vp.m_height;
    const float projection[16] =
    {
        2.0f / width, 0.0f, 0.0f, 0.0f,
        0.0f, -2.0f / height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f - 2.0f * vp.m_x / width, 1.0f + 2.0f * vp.m_y / height, 0.0f, 1.0f
    };
    m_context->UpdateSubresource(m_projectionBuffer.Get(), 0, nullptr, projection, 0, 0);
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnRenderBegin(InLayoutHandle inputLayout, VBuffHandle vbuff, IBuffHandle ibuff, const GUI_MATERIAL& material)
{
    m_context->IA_SetInputLayout(static_cast<IRHIInputLayout*>(inputLayout));

    IRHIResource* vertexBuffer = static_cast<IRHIResource*>(vbuff);
    const uint stride = sizeof(XYUV1C32DW2), offset = 0;
    m_context->IA_SetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
    m_context->IA_SetIndexBuffer(0, static_cast<IRHIResource*>(ibuff));
    m_context->IA_SetPrimitiveTopology(RHI_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_context->SetShaderProgram(m_program.Get());
    m_context->VS_SetConstantBuffers(0, 1, m_projectionView.GetAddressOf());

    IRHISampler* sampler = m_pointSampler.Get();
    m_context->PS_SetSamplers(0, 1, &sampler);

    IRHIShaderResourceView* textures[8]{};
    for (uint i = 0; i < 8; ++i)
    {
        SGUIRHITexture* texture = static_cast<SGUIRHITexture*>(material.m_textures2D[i]);
        textures[i] = texture ? texture->view.Get() : nullptr;
    }
    m_context->PS_SetResources(0, 8, textures);

    m_context->RS_SetState(m_rasterizer.Get());
    const float factor[4]{};
    m_context->OM_SetBlendState(m_blend.Get(), factor, 0xffffffff);
    m_context->OM_SetDepthStencilState(m_depth.Get(), 0);
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnRenderEnd()
{
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::PreRender(GUI_DRAW_COMMAND* command)
{
    SRHIScissorRect rect{};
    rect.left = static_cast<int>(command->m_clipRect.m_left);
    rect.top = static_cast<int>(command->m_clipRect.m_top);
    rect.right = static_cast<int>(command->m_clipRect.m_right);
    rect.bottom = static_cast<int>(command->m_clipRect.m_bottom);
    m_context->RS_SetScissorRects(1, &rect);
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::OnRender(GUI_DRAW_COMMAND* command)
{
    m_context->DrawIndexed(command->m_indexCount, command->m_indexOffset, 0);
    return GUI_ERR_OK;
}


ERRCODE CGUIRHIStrategy::PostRender()
{
    return GUI_ERR_OK;
}
