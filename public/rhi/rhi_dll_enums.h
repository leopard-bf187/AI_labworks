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


#include "../pch.h"


namespace krystallic
{
    namespace RenderHardwareInterface
    {
        enum ERHIError : ERRCODE
        {
            RHI_ERR_OK = 0x00,
            RHI_ERR_INVALID_ARGUMENT,
            RHI_ERR_NOT_IMPLEMENTED,
            RHI_ERR_NOT_SUPPORTED,
            RHI_ERR_FAILED,
            RHI_ERR_FAILED_TO_CREATE,
            RHI_ERR_BACKEND_ERROR,
            RHI_ERR_BACKEND_NOT_INITIALIZED,
            RHI_ERR_OUT_OF_MEMORY,
            RHI_ERR_INVALID_SHADER_PROGRAM,
            RHI_ERR_SHADER_INVALID,
            RHI_ERR_SHADER_COMPILE_FAILED,
            RHI_ERR_SHADER_INCLUDE_NOT_SUPPORTED,
            RHI_ERR_SHADER_LINK_FAILED,
            RHI_ERR_ALREADY_BINDED_IN_PIPELINE,
            RHI_ERR_LAST_VALUE,
        };


        enum ERHIBackend
        {
            RHI_BACKEND_DIRECT3D11,
            RHI_BACKEND_OPENGL,
            RHI_BACKEND_OPENGLES,
            RHI_BACKEND_LAST_VALUE,
        };


        enum ERHIBackendVersion
        {
            RHI_BACKEND_D3D110,
            RHI_BACKEND_D3D111,
            RHI_BACKEND_D3D112,
            RHI_BACKEND_OGL43,
            RHI_BACKEND_OGL44,
            RHI_BACKEND_OGL45,
            RHI_BACKEND_OGL46,
            RHI_BACKEND_OGLES30,
            RHI_BACKEND_OGLES31,
            RHI_BACKEND_OGLES32,
            RHI_BACKEND_VERSION_LAST_VALUE,
        };


        enum ERHIDisplayModeFilterFlags : dword
        {
            RHI_DISPLAY_MODE_FILTER_NONE = 0,
            RHI_DISPLAY_MODE_FILTER_FORMAT       = (1 << 0),
            RHI_DISPLAY_MODE_FILTER_RESOLUTION   = (1 << 1),
            RHI_DISPLAY_MODE_FILTER_REFRESH_RATE = (1 << 2),
            RHI_DISPLAY_MODE_FILTER_SCALING      = (1 << 3),
        };


        enum ERHIScanlineOrder
        {
            RHI_SCANLINE_UNSPECIFIED,
            RHI_SCANLINE_PROGRESSIVE,
            RHI_SCANLINE_UPPER_FIELD_FIRST,
            RHI_SCANLINE_LOWER_FIELD_FIRST,
            RHI_SCANLINE_INTERLACED,
            RHI_SCANLINE_ORDER_LAST_VALUE,
        };


        enum ERHIDisplayScaling
        {
            RHI_DISPLAY_SCALING_UNSPECIFIED,
            RHI_DISPLAY_SCALING_CENTERED,
            RHI_DISPLAY_SCALING_STRETCHED,
            RHI_DISPLAY_SCALING_ASPECT_RATIO_STRETCHED,
            RHI_DISPLAY_SCALING_LAST_VALUE,
        };


	enum ERHIModeRotation
        {
            RHI_MODE_ROT_UNSPEC,
            RHI_MODE_ROT_IDENTITY,
            RHI_MODE_ROT_90,
            RHI_MODE_ROT_180,
            RHI_MODE_ROT_270,
            RHI_MODE_ROTATION_LAST_VALUE,
        };


        enum ERHIDriverType
        {
            RHI_DRIVER_TYPE_UNKNOWN,
            RHI_DRIVER_TYPE_HARDWARE,
            RHI_DRIVER_TYPE_WARP,
            RHI_DRIVER_TYPE_REFERENCE,
            RHI_DRIVER_TYPE_LAST_VALUE,
        };


        enum ERHIDebugMessageSeverity
        {
            RHI_DEBUG_SEVERITY_INFO,
            RHI_DEBUG_SEVERITY_WARNING,
            RHI_DEBUG_SEVERITY_ERROR,
            RHI_DEBUG_SEVERITY_LAST_VALUE,
        };


        enum ERHIQueryType
        {
            RHI_QUERY_EVENT,
            RHI_QUERY_OCCLUSION,
            RHI_QUERY_TIMESTAMP,
            RHI_QUERY_TIMESTAMP_DISJOINT,
            RHI_QUERY_PIPELINE_STATISTICS,
            RHI_QUERY_OCCLUSION_PREDICATE,
            RHI_QUERY_TYPE_LAST_VALUE,
        };


        enum ERHIDeviceContextMode
        {
            RHI_DEVICE_CONTEXT_IMMEDIATE,
            RHI_DEVICE_CONTEXT_DEFERRED,
            RHI_DEVICE_CONTEXT_MODE_LAST_VALUE,
        };


        enum ERHIResourceType
        {
            RHI_RESOURCE_BUFFER,
            RHI_RESOURCE_TEXTURE1D,
            RHI_RESOURCE_TEXTURE2D,
            RHI_RESOURCE_TEXTURE3D,
            RHI_RESOURCE_TYPE_LAST_VALUE,
        };


        enum ERHIResourceMiscFlags
        {
            RHI_RESOURCE_MISC_GENERATE_MIPS          = (1 << 0),
            RHI_RESOURCE_MISC_SHARED                 = (1 << 1),
            RHI_RESOURCE_MISC_TEXTURECUBE            = (1 << 2),
            RHI_RESOURCE_MISC_DRAWINDIRECT_ARGS      = (1 << 3),
            RHI_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS = (1 << 4),
            RHI_RESOURCE_MISC_BUFFER_STRUCTURED      = (1 << 5),
            RHI_RESOURCE_MISC_RESOURCE_CLAMP         = (1 << 6),
            RHI_RESOURCE_MISC_SHARED_KEYEDMUTEX      = (1 << 7),
            RHI_RESOURCE_MISC_GDI_COMPATIBLE         = (1 << 8),
        };


        enum ERHIBufferUAVFlags
        {
            RHI_BUFFER_UAV_FLAG_RAW     = (1 << 0),
            RHI_BUFFER_UAV_FLAG_APPEND  = (1 << 1),
            RHI_BUFFER_UAV_FLAG_COUNTER = (1 << 2),
        };


        enum ERHIDepthStencilViewFlags
        {
            RHI_DSV_READ_ONLY_DEPTH   = (1 << 0),
            RHI_DSV_READ_ONLY_STENCIL = (1 << 1),
        };


        enum ERHIResourceViewDimension
        {
            RHI_RESOURCE_VIEW_UNKNOWN,
            RHI_RESOURCE_VIEW_BUFFER,
            RHI_RESOURCE_VIEW_TEX1D,
            RHI_RESOURCE_VIEW_TEX1D_ARRAY,
            RHI_RESOURCE_VIEW_TEX2D,
            RHI_RESOURCE_VIEW_TEX2D_ARRAY,
            RHI_RESOURCE_VIEW_TEX2DMS,
            RHI_RESOURCE_VIEW_TEX2DMS_ARRAY,
            RHI_RESOURCE_VIEW_TEX3D,
            RHI_RESOURCE_VIEW_TEXCUBE,
            RHI_RESOURCE_VIEW_TEXCUBE_ARRAY,
            RHI_RESOURCE_VIEW_DIMENSION_LAST_VALUE,
        };


        enum ERHIDescriptorType
        {
            RHI_DESCRIPTOR_SAMPLER,
            RHI_DESCRIPTOR_CONSTANT_BUFFER_VIEW,
            RHI_DESCRIPTOR_SHADER_RESOURCE_VIEW,
            RHI_DESCRIPTOR_RENDER_TARGET_VIEW,
            RHI_DESCRIPTOR_DEPTH_STENCIL_VIEW,
            RHI_DESCRIPTOR_UNORDERED_ACCESS_VIEW,
            RHI_DESCRIPTOR_TYPE_LAST_VALUE,
        };


        enum ERHIDataType
        {
            RHI_BYTE1,
            RHI_BYTE2,
            RHI_BYTE3,
            RHI_BYTE4,
            RHI_UBYTE1,
            RHI_UBYTE2,
            RHI_UBYTE3,
            RHI_UBYTE4,
            RHI_SHORT1,
            RHI_SHORT2,
            RHI_SHORT3,
            RHI_SHORT4,
            RHI_USHORT1,
            RHI_USHORT2,
            RHI_USHORT3,
            RHI_USHORT4,
            RHI_INT1,
            RHI_INT2,
            RHI_INT3,
            RHI_INT4,
            RHI_UINT1,
            RHI_UINT2,
            RHI_UINT3,
            RHI_UINT4,
            RHI_FLOAT1,
            RHI_FLOAT2,
            RHI_FLOAT3,
            RHI_FLOAT4,
            RHI_DATA_TYPE_LAST_VALUE,
        };


        enum ERHIShaderStage : dword
        {
            RHI_SHADER_STAGE_VERTEX   = (1 << 0),
            RHI_SHADER_STAGE_PIXEL    = (1 << 1),
            RHI_SHADER_STAGE_GEOMETRY = (1 << 2),
            RHI_SHADER_STAGE_HULL     = (1 << 3),
            RHI_SHADER_STAGE_DOMAIN   = (1 << 4),
            RHI_SHADER_STAGE_COMPUTE  = (1 << 5),
            RHI_SHADER_STAGE_GRAPHICS = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
        };


        enum ERHIRenderPipelineType
        {
            RHI_RENDER_PIPELINE_UNKNOWN,
            RHI_RENDER_PIPELINE_GRAPHICS,
            RHI_RENDER_PIPELINE_COMPUTE,
            RHI_RENDER_PIPELINE_TYPE_LAST_VALUE,
        };


        enum ERHICPUAccessFlags
        {
            RHI_CPU_DEFAULT,
            RHI_CPU_WRITE,
            RHI_CPU_READ,
            RHI_CPU_FULL_ACCESS,
        };


        enum ERHIInputClassification
        {
            RHI_INPUT_PER_VERTEX_DATA,
            RHI_INPUT_PER_INSTANCE_DATA,
            RHI_INPUT_CLASSIFICATION_LAST_VALUE,
        };


        enum ERHIUsage
        {
            RHI_USAGE_DEFAULT,
            RHI_USAGE_IMMUTABLE,
            RHI_USAGE_DYNAMIC,
            RHI_USAGE_STAGING,
            RHI_USAGE_LAST_VALUE,
        };


        enum ERHIUsageType
        {
            RHI_USAGE_BACK_BUFFER,
            RHI_USAGE_READ_ONLY,
            RHI_USAGE_RENDER_TARGET_OUTPUT,
            RHI_USAGE_SHADER_INPUT,
            RHI_USAGE_SHARED,
            RHI_USAGE_UNORDERED_ACCESS,
            RHI_USAGE_TYPE_LAST_VALUE,
        };


        enum ERHIFillMode
        {
            RHI_FILL_WIREFRAME,
            RHI_FILL_SOLID,
            RHI_FILL_MODE_LAST_VALUE,
        };


        enum ERHICullMode
        {
            RHI_CULL_NONE,
            RHI_CULL_FRONT,
            RHI_CULL_BACK,
            RHI_CULL_MODE_LAST_VALUE,
        };


        enum ERHIComparisonFunc
        {
            RHI_CMP_NEVER,
            RHI_CMP_LESS,
            RHI_CMP_EQUAL,
            RHI_CMP_LESSEQUAL,
            RHI_CMP_GREATER,
            RHI_CMP_NOTEQUAL,
            RHI_CMP_GREATEREQUAL,
            RHI_CMP_ALWAYS,
            RHI_CMP_FUNC_LAST_VALUE,
        };


        enum ERHIDepthWriteMask
        {
            RHI_DWM_ZERO = 0,
            RHI_DWM_ALL  = 1,
        };


        enum ERHIStencilOperation
        {
            RHI_STENCIL_OP_KEEP,
            RHI_STENCIL_OP_ZERO,
            RHI_STENCIL_OP_REPLACE,
            RHI_STENCIL_OP_INCR_SAT,
            RHI_STENCIL_OP_DECR_SAT,
            RHI_STENCIL_OP_INVERT,
            RHI_STENCIL_OP_INCR,
            RHI_STENCIL_OP_DECR,
            RHI_STENCIL_OP_LAST_VALUE,
        };


        enum ERHIBlend
        {
            RHI_BLEND_ZERO,
            RHI_BLEND_ONE,
            RHI_BLEND_SRC_COLOR,
            RHI_BLEND_INV_SCR_COLOR,
            RHI_BLEND_SRC_ALPHA,
            RHI_BLEND_INV_SRC_ALPHA,
            RHI_BLEND_DEST_ALPHA,
            RHI_BLEND_INV_DEST_ALPHA,
            RHI_BLEND_DEST_COLOR,
            RHI_BLEND_INV_DEST_COLOR,
            RHI_BLEND_SRC_ALPHA_SAT,
            RHI_BLEND_BLEND_FACTOR,
            RHI_BLEND_INV_BLEND_FACTOR,
            RHI_BLEND_SRC1_COLOR,
            RHI_BLEND_INV_SRC1_COLOR,
            RHI_BLEND_SRC1_AlPHA,
            RHI_BLEND_INV_SRC1_AlPHA,
            RHI_BLEND_LAST_VALUE,
        };


        enum ERHIBlendOperation
        {
            RHI_BLEND_OP_ADD,
            RHI_BLEND_OP_SUB,
            RHI_BLEND_OP_REV_SUB,
            RHI_BLEND_OP_MIN,
            RHI_BLEND_OP_MAX,
            RHI_BLEND_OP_LAST_VALUE,
        };


        enum ERHIColorWriteMask : dword
        {
            RHI_COLOR_WRITE_MASK_RED    = 1,
            RHI_COLOR_WRITE_MASK_GREEN  = 2,
            RHI_COLOR_WRITE_MASK_BLUE   = 4,
            RHI_COLOR_WRITE_MASK_ALPHA  = 8,
            RHI_COLOR_WRITE_MASK_ALL    = (1 | 2 | 4 | 8),
        };


        enum ERHIFilterType
        {
            RHI_FILTER_TYPE_POINT,
            RHI_FILTER_TYPE_LINEAR,
            RHI_FILTER_TYPE_LAST_VALUE,
        };


        enum ERHIFilter
        {
            RHI_FILTER_MIN_MAG_MIP_POINT,
            RHI_FILTER_MIN_MAG_POINT_MIP_LINEAR,
            RHI_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT,
            RHI_FILTER_MIN_POINT_MAG_MIP_LINEAR,
            RHI_FILTER_MIN_LINEAR_MAG_MIP_POINT,
            RHI_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
            RHI_FILTER_MIN_MAG_LINEAR_MIP_POINT,
            RHI_FILTER_MIN_MAG_MIP_LINEAR,
            RHI_FILTER_ANISOTROPIC,
            RHI_FILTER_COMPARISON_MIN_MAG_MIP_POINT,
            RHI_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR,
            RHI_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT,
            RHI_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR,
            RHI_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT,
            RHI_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
            RHI_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT,
            RHI_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR,
            RHI_FILTER_COMPARISON_ANISOTROPIC,
            RHI_FILTER_LAST_VALUE,
        };


        enum ERHITextureAddress
        {
            RHI_TEXTURE_ADDRESS_WRAP,
            RHI_TEXTURE_ADDRESS_MIRROR,
            RHI_TEXTURE_ADDRESS_CLAMP,
            RHI_TEXTURE_ADDRESS_BORDER,
            RHI_TEXTURE_ADDRESS_MIRRORONCE,
            RHI_TEXTURE_ADDRESS_LAST_VALUE,
        };


        enum ERHIResourceBindFlag : dword
        {
            RHI_BIND_VERTEX_BUFFER    = (1 << 0),
            RHI_BIND_INDEX_BUFFER     = (1 << 1),
            RHI_BIND_CONSTANT_BUFFER  = (1 << 2),
            RHI_BIND_SHADER_RESOURCE  = (1 << 3),
            RHI_BIND_STREAM_OUTPUT    = (1 << 4),
            RHI_BIND_RENDER_TARGET    = (1 << 5),
            RHI_BIND_DEPTH_STENCIL    = (1 << 6),
            RHI_BIND_UNORDERED_ACCESS = (1 << 7),
        };


        enum ERHIPrimitive
        {
            RHI_PRIMITIVE_UNDEFINED,
            RHI_PRIMITIVE_POINT,
            RHI_PRIMITIVE_LINE,
            RHI_PRIMITIVE_TRIANGLE,
            RHI_PRIMITIVE_LINE_ADJ,
            RHI_PRIMITIVE_TRIANGLE_ADJ,
            RHI_PRIMITIVE_1_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_2_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_3_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_4_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_5_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_6_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_7_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_8_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_9_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_10_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_11_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_12_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_13_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_14_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_15_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_16_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_17_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_18_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_19_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_20_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_21_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_22_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_23_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_24_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_25_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_26_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_27_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_28_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_29_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_30_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_31_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_32_CONTROL_POINT_PATCH,
            RHI_PRIMITIVE_LAST_VALUE,
        };


        enum ERHIPrimitiveTopology
        {
            RHI_PRIMITIVE_TOPOLOGY_UNDEFINED,
            RHI_PRIMITIVE_TOPOLOGY_POINTLIST,
            RHI_PRIMITIVE_TOPOLOGY_LINELIST,
            RHI_PRIMITIVE_TOPOLOGY_LINESTRIP,
            RHI_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
            RHI_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
            RHI_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
            RHI_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
            RHI_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
            RHI_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ,
            RHI_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST,
            RHI_PRIMITIVE_TOPOLOGY_LAST_VALUE,
        };


        enum ERHIClearFlags
        {
            RHI_CLEAR_DEPTH   = 1,
            RHI_CLEAR_STENCIL = 2,
        };


        // Backend-neutral swap-chain creation options. Unsupported options are
        // rejected by the backend, not silently approximated.
        enum ERHISwapChainFlags : dword
        {
            RHI_SWAP_CHAIN_FLAG_NONE                          = 0,
            RHI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH             = (1u << 0),
            RHI_SWAP_CHAIN_FLAG_ALLOW_TEARING                 = (1u << 1),
            RHI_SWAP_CHAIN_FLAG_BACK_BUFFER_SHADER_RESOURCE  = (1u << 2),
            RHI_SWAP_CHAIN_FLAG_DEPTH_BUFFER_SHADER_RESOURCE = (1u << 3),
            RHI_SWAP_CHAIN_FLAG_STEREO                        = (1u << 4),
        };


        enum ERHISwapChainSwapEffect
        {
            RHI_SWAP_EFFECT_DEFAULT,
            RHI_SWAP_EFFECT_DISCARD,
            RHI_SWAP_EFFECT_SEQUENTIAL,
            RHI_SWAP_EFFECT_FLIP_SEQUENTIAL,
            RHI_SWAP_EFFECT_FLIP_DISCARD,
            RHI_SWAP_EFFECT_LAST_VALUE,
        };


        enum ERHISwapChainAlphaMode
        {
            RHI_SWAP_ALPHA_MODE_UNSPECIFIED,
            RHI_SWAP_ALPHA_MODE_OPAQUE,
            RHI_SWAP_ALPHA_MODE_PREMULTIPLIED,
            RHI_SWAP_ALPHA_MODE_STRAIGHT,
            RHI_SWAP_ALPHA_MODE_LAST_VALUE,
        };


        enum ERHIVSyncMode
        {
            RHI_VSYNC_NONE,
            RHI_VSYNC_FULL,
            RHI_VSYNC_1DIV2,
            RHI_VSYNC_1DIV3,
            RHI_VSYNC_1DIV4,
            RHI_VSYNC_LAST_VALUE,
        };


        enum ERHIMap
        {
            RHI_MAP_READ,
            RHI_MAP_WRITE,
            RHI_MAP_READ_WRITE,
            RHI_MAP_WRITE_DISCARD,
            RHI_MAP_WRITE_NO_OVERWRITE,
            RHI_MAP_LAST_VALUE,
        };


        enum ERHIMapFlags
        {
            RHI_MAP_FLAG_DO_NOT_WAIT = 0x100000L
        };


        enum ERHICubeTextureFaces
        {
            RHI_CUBEFACE_POS_X,
            RHI_CUBEFACE_NEG_X,
            RHI_CUBEFACE_POS_Y,
            RHI_CUBEFACE_NEG_Y,
            RHI_CUBEFACE_POS_Z,
            RHI_CUBEFACE_NEG_Z,
            RHI_CUBEFACE_LAST_VALUE,
        };


        enum ERHIFormat
        {
            RHI_FORMAT_UNKNOWN,
            RHI_FORMAT_R32G32B32A32_TYPELESS,
            RHI_FORMAT_R32G32B32A32_FLOAT,
            RHI_FORMAT_R32G32B32A32_UINT,
            RHI_FORMAT_R32G32B32A32_SINT,
            RHI_FORMAT_R32G32B32_TYPELESS,
            RHI_FORMAT_R32G32B32_FLOAT,
            RHI_FORMAT_R32G32B32_UINT,
            RHI_FORMAT_R32G32B32_SINT,
            RHI_FORMAT_R16G16B16A16_TYPELESS,
            RHI_FORMAT_R16G16B16A16_FLOAT,
            RHI_FORMAT_R16G16B16A16_UNORM,
            RHI_FORMAT_R16G16B16A16_UINT,
            RHI_FORMAT_R16G16B16A16_SNORM,
            RHI_FORMAT_R16G16B16A16_SINT,
            RHI_FORMAT_R32G32_TYPELESS,
            RHI_FORMAT_R32G32_FLOAT,
            RHI_FORMAT_R32G32_UINT,
            RHI_FORMAT_R32G32_SINT,
            RHI_FORMAT_R32G8X24_TYPELESS,
            RHI_FORMAT_D32_FLOAT_S8X24_UINT,
            RHI_FORMAT_R32_FLOAT_X8X24_TYPELESS,
            RHI_FORMAT_X32_TYPELESS_G8X24_UINT,
            RHI_FORMAT_R10G10B10A2_TYPELESS,
            RHI_FORMAT_R10G10B10A2_UNORM,
            RHI_FORMAT_R10G10B10A2_UINT,
            RHI_FORMAT_R11G11B10_FLOAT,
            RHI_FORMAT_R8G8B8A8_TYPELESS,
            RHI_FORMAT_R8G8B8A8_UNORM,
            RHI_FORMAT_R8G8B8A8_UNORM_SRGB,
            RHI_FORMAT_R8G8B8A8_UINT,
            RHI_FORMAT_R8G8B8A8_SNORM,
            RHI_FORMAT_R8G8B8A8_SINT,
            RHI_FORMAT_R16G16_TYPELESS,
            RHI_FORMAT_R16G16_FLOAT,
            RHI_FORMAT_R16G16_UNORM,
            RHI_FORMAT_R16G16_UINT,
            RHI_FORMAT_R16G16_SNORM,
            RHI_FORMAT_R16G16_SINT,
            RHI_FORMAT_R32_TYPELESS,
            RHI_FORMAT_D32_FLOAT,
            RHI_FORMAT_R32_FLOAT,
            RHI_FORMAT_R32_UINT,
            RHI_FORMAT_R32_SINT,
            RHI_FORMAT_R24G8_TYPELESS,
            RHI_FORMAT_D24_UNORM_S8_UINT,
            RHI_FORMAT_R24_UNORM_X8_TYPELESS,
            RHI_FORMAT_X24_TYPELESS_G8_UINT,
            RHI_FORMAT_R8G8_TYPELESS,
            RHI_FORMAT_R8G8_UNORM,
            RHI_FORMAT_R8G8_UINT,
            RHI_FORMAT_R8G8_SNORM,
            RHI_FORMAT_R8G8_SINT,
            RHI_FORMAT_R16_TYPELESS,
            RHI_FORMAT_R16_FLOAT,
            RHI_FORMAT_D16_UNORM,
            RHI_FORMAT_R16_UNORM,
            RHI_FORMAT_R16_UINT,
            RHI_FORMAT_R16_SNORM,
            RHI_FORMAT_R16_SINT,
            RHI_FORMAT_R8_TYPELESS,
            RHI_FORMAT_R8_UNORM,
            RHI_FORMAT_R8_UINT,
            RHI_FORMAT_R8_SNORM,
            RHI_FORMAT_R8_SINT,
            RHI_FORMAT_A8_UNORM,
            RHI_FORMAT_R1_UNORM,
            RHI_FORMAT_R9G9B9E5_SHAREDEXP,
            RHI_FORMAT_R8G8_B8G8_UNORM,
            RHI_FORMAT_G8R8_G8B8_UNORM,
            RHI_FORMAT_BC1_TYPELESS,
            RHI_FORMAT_BC1_UNORM,
            RHI_FORMAT_BC1_UNORM_SRGB,
            RHI_FORMAT_BC2_TYPELESS,
            RHI_FORMAT_BC2_UNORM,
            RHI_FORMAT_BC2_UNORM_SRGB,
            RHI_FORMAT_BC3_TYPELESS,
            RHI_FORMAT_BC3_UNORM,
            RHI_FORMAT_BC3_UNORM_SRGB,
            RHI_FORMAT_BC4_TYPELESS,
            RHI_FORMAT_BC4_UNORM,
            RHI_FORMAT_BC4_SNORM,
            RHI_FORMAT_BC5_TYPELESS,
            RHI_FORMAT_BC5_UNORM,
            RHI_FORMAT_BC5_SNORM,
            RHI_FORMAT_B5G6R5_UNORM,
            RHI_FORMAT_B5G5R5A1_UNORM,
            RHI_FORMAT_B8G8R8A8_UNORM,
            RHI_FORMAT_B8G8R8X8_UNORM,
            RHI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM,
            RHI_FORMAT_B8G8R8A8_TYPELESS,
            RHI_FORMAT_B8G8R8A8_UNORM_SRGB,
            RHI_FORMAT_B8G8R8X8_TYPELESS,
            RHI_FORMAT_B8G8R8X8_UNORM_SRGB,
            RHI_FORMAT_BC6H_TYPELESS,
            RHI_FORMAT_BC6H_UF16,
            RHI_FORMAT_BC6H_SF16,
            RHI_FORMAT_BC7_TYPELESS,
            RHI_FORMAT_BC7_UNORM,
            RHI_FORMAT_BC7_UNORM_SRGB,
            RHI_FORMAT_ETC1_UNORM,
            RHI_FORMAT_ETC2_R8G8B8_UNORM,
            RHI_FORMAT_ETC2_R8G8B8_UNORM_SRGB,
            RHI_FORMAT_ETC2_R8G8B8A1_UNORM,
            RHI_FORMAT_ETC2_R8G8B8A1_UNORM_SRGB,
            RHI_FORMAT_ETC2_R8G8B8A8_UNORM,
            RHI_FORMAT_ETC2_R8G8B8A8_UNORM_SRGB,
            RHI_FORMAT_EAC_R11_UNORM,
            RHI_FORMAT_EAC_R11_SNORM,
            RHI_FORMAT_EAC_R11G11_UNORM,
            RHI_FORMAT_EAC_R11G11_SNORM,
            RHI_FORMAT_ASTC_4X4_UNORM,
            RHI_FORMAT_ASTC_4X4_UNORM_SRGB,
            RHI_FORMAT_ASTC_5X4_UNORM,
            RHI_FORMAT_ASTC_5X4_UNORM_SRGB,
            RHI_FORMAT_ASTC_5X5_UNORM,
            RHI_FORMAT_ASTC_5X5_UNORM_SRGB,
            RHI_FORMAT_ASTC_6X5_UNORM,
            RHI_FORMAT_ASTC_6X5_UNORM_SRGB,
            RHI_FORMAT_ASTC_6X6_UNORM,
            RHI_FORMAT_ASTC_6X6_UNORM_SRGB,
            RHI_FORMAT_ASTC_8X5_UNORM,
            RHI_FORMAT_ASTC_8X5_UNORM_SRGB,
            RHI_FORMAT_ASTC_8X6_UNORM,
            RHI_FORMAT_ASTC_8X6_UNORM_SRGB,
            RHI_FORMAT_ASTC_8X8_UNORM,
            RHI_FORMAT_ASTC_8X8_UNORM_SRGB,
            RHI_FORMAT_ASTC_10X5_UNORM,
            RHI_FORMAT_ASTC_10X5_UNORM_SRGB,
            RHI_FORMAT_ASTC_10X6_UNORM,
            RHI_FORMAT_ASTC_10X6_UNORM_SRGB,
            RHI_FORMAT_ASTC_10X8_UNORM,
            RHI_FORMAT_ASTC_10X8_UNORM_SRGB,
            RHI_FORMAT_ASTC_10X10_UNORM,
            RHI_FORMAT_ASTC_10X10_UNORM_SRGB,
            RHI_FORMAT_ASTC_12X10_UNORM,
            RHI_FORMAT_ASTC_12X10_UNORM_SRGB,
            RHI_FORMAT_ASTC_12X12_UNORM,
            RHI_FORMAT_ASTC_12X12_UNORM_SRGB,
            RHI_FORMAT_LAST_VALUE,
        };

        enum ERHIFrameBufferUsageFlags : dword
        {
            RHI_FRAME_BUFFER_USAGE_RTV = (1 << 0),
            RHI_FRAME_BUFFER_USAGE_DSV = (1 << 1),
            RHI_FRAME_BUFFER_USAGE_UAV = (1 << 2),
        };

        enum ERHIFrameBufferBindFlags : dword
        {
            RHI_FRAME_BIND_RTV_0 = (1 << 0),
            RHI_FRAME_BIND_RTV_1 = (1 << 1),
            RHI_FRAME_BIND_RTV_2 = (1 << 2),
            RHI_FRAME_BIND_RTV_3 = (1 << 3),
            RHI_FRAME_BIND_RTV_4 = (1 << 4),
            RHI_FRAME_BIND_RTV_5 = (1 << 5),
            RHI_FRAME_BIND_RTV_6 = (1 << 6),
            RHI_FRAME_BIND_RTV_7 = (1 << 7),
            RHI_FRAME_BIND_DSV = (1 << 8),
            RHI_FRAME_BIND_UAV_0 = (1 << 9),
            RHI_FRAME_BIND_UAV_1 = (1 << 10),
            RHI_FRAME_BIND_UAV_2 = (1 << 11),
            RHI_FRAME_BIND_UAV_3 = (1 << 12),
            RHI_FRAME_BIND_UAV_4 = (1 << 13),
            RHI_FRAME_BIND_UAV_5 = (1 << 14),
            RHI_FRAME_BIND_UAV_6 = (1 << 15),
            RHI_FRAME_BIND_UAV_7 = (1 << 16),
        };

        enum ERHIShaderCompileFlags : dword
        {
            RHI_SHADER_COMPILE_NONE = 0,
            RHI_SHADER_COMPILE_WARNINGS_AS_ERRORS = (1u << 0),
        };

        enum ERHIShaderLanguage
        {
            RHI_HLSL,
            RHI_GLSL,
            RHI_GLSL_ES,
        };

        struct SRHIShaderTarget
        {
            enum HLSL
            {
                SM_4_0,
                SM_4_1,
                SM_5_0,
            };

            enum GLSL
            {
                GL_330,
                GL_400,
                GL_410,
                GL_420,
                GL_430,
                GL_440,
                GL_450,
                GL_460,
            };

            enum GLSLES
            {
                ES_300,
                ES_310,
                ES_320
            };
        };

    }
} // namespace krystallic