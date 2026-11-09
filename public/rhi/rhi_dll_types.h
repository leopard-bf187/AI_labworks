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
#include "../base.h"
#include "../stdlib_dll.h"
#include "../common.h"
#include "../platform.h"


#include "rhi_dll_enums.h"


#define RHI_COMMON_VERSION_MAJOR    0x0000
#define RHI_COMMON_VERSION_MINOR    0x0009
#define RHI_COMMON_VERSION_PATCH    0x0000

#define RHI_D3D11_VERSION_MAJOR     0x0000
#define RHI_D3D11_VERSION_MINOR     0x0007
#define RHI_D3D11_VERSION_PATCH     0x0000

#define RHI_GL4_VERSION_MAJOR       0x0000
#define RHI_GL4_VERSION_MINOR       0x000C
#define RHI_GL4_VERSION_PATCH       0x0000

#define RHI_GLES3_VERSION_MAJOR     0x0000
#define RHI_GLES3_VERSION_MINOR     0x0007
#define RHI_GLES3_VERSION_PATCH     0x0000


#if defined(KRYSTALLIC_OS_WINNT)
#if defined(RHI_API_EXPORT)
#define RHI_API __declspec(dllexport)
#else
#define RHI_API __declspec(dllimport)
#endif
#elif defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)
#define RHI_API __attribute__((visibility("default")))
#endif



#ifdef __cplusplus
extern "C" {
#endif


namespace krystallic
{
    namespace RenderHardwareInterface
    {
        struct IRHIFactory;
        struct IRHIFrameDebug;
        struct IRHIDebug;
        struct IRHIQuery;
        struct IRHIAdapter;
        struct IRHIDisplay;
        struct IRHIDisplayModeList;
        struct IRHISwapChain;
        struct IRHIDevice;
        struct IRHIDeviceContext;
        struct IRHIFrameBuffer;
        struct IRHIDeviceFeatures;
        struct IRHICommandList;
        struct IRHIInputLayout;
        struct IRHIResource;
        struct IRHIVertexBufferSet;
        struct IRHIDescriptorSet;
        struct IRHIDescriptor;
        struct IRHISampler;
        struct IRHIResourceView;
        struct IRHIConstantBufferView;
        struct IRHIShaderResourceView;
        struct IRHIRenderTargetView;
        struct IRHIDepthStencilView;
        struct IRHIUnorderedAccessView;
        struct IRHIDepthStencilState;
        struct IRHIBlendState;
        struct IRHIRasterizerState;
        struct IRHIShaderCompiler;
        struct IRHIShaderReflection;
        struct IRHIShader;
        struct IRHIShaderProgram;


        struct SRHIAdapterDesc
        {
            Stdlib::String deviceName;
            uint           vendorId;
            uint           deviceId;
            uint           subsysId;
            uint           revision;
            uint64         dedicatedVideoMemory;
            uint64         dedicatedSystemMemory;
            uint64         sharedSystemMemory;
            bool           softwareAdapter;
        };


        struct SRHIDisplayDesc
        {
            Stdlib::String deviceName;
            Stdlib::String displayName;
            Platform::SNativeHandle monitor;
            ERHIModeRotation rotation;
            uint64           displayId;
            uint32           adapterIndex;
            int              desktopX;
            int              desktopY;
            int              desktopWidth;
            int              desktopHeight;
            bool             primary;
        };


        struct SRHISampleDesc
        {
            uint count;
            uint quality;
        };


        struct SRHIRational
        {
            uint numerator;
            uint denomenator;
        };


        struct SRHIDisplayMode
        {
            uint               width;
            uint               height;
            ERHIFormat         format;
            SRHIRational       refreshRate;
            ERHIDisplayScaling scaling;
            ERHIScanlineOrder  scanlineOrder;
        };


        struct SRHIDisplayModeFilter
        {
            dword              flags;
            ERHIFormat         format;
            uint               minWidth;
            uint               minHeight;
            uint               maxWidth;
            uint               maxHeight;
            SRHIRational       minRefreshRate;
            SRHIRational       maxRefreshRate;
            ERHIDisplayScaling scaling;
            ERHIScanlineOrder  scanlineOrder;
        };


        struct SRHIResolution
        {
            uint width;
            uint height;
        };


        struct SRHIDisplayModeKey
        {
            uint resolutionIndex;
            uint formatIndex;
            uint refreshRateIndex;
            uint scalingIndex;
            uint scanlineOrderIndex;
        };


        struct SRHISwapChainDesc
        {
            // The window belongs to the Platform module. The swap chain retains
            // a reference while its native rendering surface exists and obtains
            // the backend-specific handle through GetNativeHandle().
            Platform::IPlatformWindow* platformWindow;

            // Zero size requests the current platform window dimensions.
            uint width;
            uint height;

            // A zero bufferCount requests the backend default.
            // Exact buffer counts and swap effects are backend-dependent.
            uint bufferCount;
            ERHIFormat colorFormat;

            // The depth buffer is optional and separately owned by the swap chain.
            bool createDepthBuffer;
            ERHIFormat depthStencilFormat;

            // MSAA applies to rendering attachments, not necessarily to the
            // native presentation buffer. Unsupported combinations must fail.
            SRHISampleDesc sampleDesc;

            bool fullscreen;
            SRHIRational refreshRate;
            ERHIDisplayScaling scaling;
            ERHIScanlineOrder scanlineOrder;

            ERHISwapChainSwapEffect swapEffect;
            ERHISwapChainAlphaMode alphaMode;

            // Preferred initial presentation policy. Present() may override it.
            ERHIVSyncMode vsyncMode;
            dword flags;
        };


        struct SRHIDeviceInitialDesc
        {
            dword creationFlags;
            // Platform manager's native handle (wl_display* on Wayland).
            // The pointed-to handle is borrowed for device initialization.
            Platform::SNativeHandle* platformNativeHandle;
        };


        typedef struct SRHIRect
        {
            int left;
            int top;
            int right;
            int bottom;
        } SRHIScissorRect;


        struct SRHIBox
        {
            uint left;
            uint top;
            uint front;
            uint right;
            uint bottom;
            uint back;
        };


        struct SRHIQueryDesc
        {
            ERHIQueryType type;
        };


        struct SRHIQueryDataTimestampDisjoint
        {
            uint64 frequency;
            bool   disjoint;
        };


        struct SRHIQueryDataPipelineStatistics
        {
            uint64 iaVertices;
            uint64 iaPrimitives;
            uint64 vsInvocations;
            uint64 gsInvocations;
            uint64 gsPrimitives;
            uint64 rasterizerInvocations;
            uint64 rasterizerPrimitives;
            uint64 psInvocations;
            uint64 hsInvocations;
            uint64 dsInvocations;
            uint64 csInvocations;
        };


        struct SRHIFrameBufferDesc
        {
            dword m_usageMask;
            byte m_rtvCapacity;
            byte m_uavCapacity;
        };


        struct SRHIDeviceLimits
        {
            uint maxTexture1DSize;
            uint maxTexture2DSize;
            uint maxTexture3DSize;
            uint maxTextureCubeSize;
            uint maxTextureArrayLayers;
            uint maxRenderTargets;
            uint maxViewports;
            uint maxVertexBuffers;
            uint maxVertexAttributes;
            uint maxConstantBuffersPerStage;
            uint maxShaderResourcesPerStage;
            uint maxSamplersPerStage;
            uint maxUAVs;
            uint maxAnisotropy;
            uint maxMSAASamples;
        };


        struct SRHIInputElementDesc
        {
            const char*             semanticName;
            uint                    semanticIndex;
            ERHIFormat              format;
            uint                    bufferSlot;
            uint                    alignedByteOffset;
            ERHIInputClassification inputSlotClass;
            uint                    instanceDataStepRate;
        };


        struct SRHIStreamOutputDeclarationEntry
        {
            uint        stream;
            const char* semanticName;
            uint        semanticIndex;
            uint        startComponent;
            uint        componentCount;
            uint        outputSlot;
        };


        struct SRHIStreamOutputDesc
        {
            uint                                    entryCount;
            const SRHIStreamOutputDeclarationEntry* entries;
            const uint*                             bufferStrides;
            uint                                    bufferStrideCount;
            uint                                    rasterizedStream;
        };


        struct SRHIResourceDesc
        {
            ERHIResourceType  type;
            uint              width;
            uint              height;
            uint              depth;
            uint              mipLevels;
            uint              arraySize;
            ERHIFormat        format;
            SRHISampleDesc    sampleDesc;
            ERHIUsage         usage;
            dword             bindFlags;
            ERHICPUAccessFlags cpuAccess;
            dword             resourceMiscFlags;
            uint              structureByteStride;
        };


        struct SRHISubresourceData
        {
            void* sysMem;
            uint  sysMemPitch;
            uint  sysMemSlicePitch;
        };


        typedef struct _buffer_rtv_srv
        {
            uint ElementOffset;
            uint ElementWidth;
        } SRHIBufferRTV, SRHIBufferSRV;

        typedef struct _buffer_uav {
            uint  FirstElement;
            uint  NumElements;
            dword Flags;
        } SRHIBufferUAV;

        typedef struct _texarr_dsv_rtv_uav
        {
            uint MipSlice;
            uint FirstArraySlice;
            uint ArraySize;
        } SRHITex1DArrayDSV, SRHITex1DArrayRTV, SRHITex1DArrayUAV, SRHITex2DArrayDSV, SRHITex2DArrayRTV,
            SRHITex2DArrayUAV;

        typedef struct _texarr_srv
        {
            uint MostDetailedMip;
            uint MipLevels;
            uint FirstArraySlice;
            uint ArraySize;
        } SRHITex1DArraySRV, SRHITex2DArraySRV;

        typedef struct _tex_dsv_rtv_uav
        {
            uint MipSlice;
        } SRHITex1DDSV, SRHITex1DRTV, SRHITex1DUAV, SRHITex2DDSV, SRHITex2DRTV, SRHITex2DUAV;

        typedef struct _tex_srv
        {
            uint MostDetailedMip;
            uint MipLevels;
        } SRHITex1DSRV, SRHITex2DSRV, SRHITex3DSRV, SRHITexCubeSRV;

        typedef struct _tex2darrms_dsv_rtv_srv
        {
            uint FirstArraySlice;
            uint ArraySize;
        } SRHITex2DMSArrayDSV, SRHITex2DMSArrayRTV, SRHITex2DMSArraySRV;

        typedef struct _tex3d_rtv_uav
        {
            uint MipSlice;
            uint FirstWSlice;
            uint WSize;
        } SRHITex3DRTV, SRHITex3DUAV;

        struct SRHITexCubeArraySRV
        {
            uint MostDetailedMip;
            uint MipLevels;
            uint First2DArrayFace;
            uint NumCubes;
        };


        struct SRHIShaderResourceViewDesc
        {
            ERHIFormat                Format;
            ERHIResourceViewDimension ViewDimension;
            union
            {
                SRHIBufferSRV       Buffer;
                SRHITex1DSRV        Texture1D;
                SRHITex1DArraySRV   Texture1DArray;
                SRHITex2DSRV        Texture2D;
                SRHITex2DArraySRV   Texture2DArray;
                SRHITex2DMSArraySRV Texture2DMSArray;
                SRHITex3DSRV        Texture3D;
                SRHITexCubeSRV      TextureCube;
                SRHITexCubeArraySRV TextureCubeArray;
            };
        };


        struct SRHIRenderTargetViewDesc
        {
            ERHIFormat                Format;
            ERHIResourceViewDimension ViewDimension;
            union
            {
                SRHIBufferRTV       Buffer;
                SRHITex1DRTV        Texture1D;
                SRHITex1DArrayRTV   Texture1DArray;
                SRHITex2DRTV        Texture2D;
                SRHITex2DArrayRTV   Texture2DArray;
                SRHITex2DMSArrayRTV Texture2DMSArray;
                SRHITex3DRTV        Texture3D;
            };
        };


        struct SRHIDepthStencilViewDesc
        {
            ERHIFormat                Format;
            ERHIResourceViewDimension ViewDimension;
            dword                     Flags;
            union
            {
                SRHITex1DDSV        Texture1D;
                SRHITex1DArrayDSV   Texture1DArray;
                SRHITex2DDSV        Texture2D;
                SRHITex2DArrayDSV   Texture2DArray;
                SRHITex2DMSArrayDSV Texture2DMSArray;
            };
        };


        struct SRHIUnorderedAccessViewDesc
        {
            ERHIFormat                Format;
            ERHIResourceViewDimension ViewDimension;
            union
            {
                SRHIBufferUAV     Buffer;
                SRHITex1DUAV      Texture1D;
                SRHITex1DArrayUAV Texture1DArray;
                SRHITex2DUAV      Texture2D;
                SRHITex2DArrayUAV Texture2DArray;
                SRHITex3DUAV      Texture3D;
            };
        };


        struct SRHIViewport
        {
            float X;
            float Y;
            float Width;
            float Height;
            float MinZ;
            float MaxZ;
        };


        struct SRHISamplerDesc
        {
            ERHIFilter         Filter;
            ERHITextureAddress AddressU;
            ERHITextureAddress AddressV;
            ERHITextureAddress AddressW;
            float              MipLODBias;
            uint               MaxAnisotropy;
            ERHIComparisonFunc ComparisonFunc;
            float              BorderColor[4];
            float              MinLOD;
            float              MaxLOD;
        };


        struct SRHIRasterizerStateDesc
        {
            ERHIFillMode FillMode;
            ERHICullMode CullMode;
            bool         FrontCounterClockwise;
            int          DepthBias;
            float        DepthBiasClamp;
            float        SlopeScaledDepthBias;
            bool         DepthClipEnable;
            bool         ScissorEnable;
            bool         MultisampleEnable;
            bool         AntialiasedLineEnable;
        };


        struct SRHIRenderTargetBlendDesc
        {
            bool               BlendEnable;
            ERHIBlend          SrcBlend;
            ERHIBlend          DestBlend;
            ERHIBlendOperation BlendOp;
            ERHIBlend          SrcBlendAlpha;
            ERHIBlend          DestBlendAlpha;
            ERHIBlendOperation BlendOpAlpha;
            uint8              RenderTargetWriteMask;
        };


        struct SRHIBlendStateDesc
        {
            bool                      AlphaToCoverageEnable;
            bool                      IndependentBlendEnable;
            SRHIRenderTargetBlendDesc RenderTarget[8];
        };


        struct SRHIDepthStencilOpDesc
        {
            ERHIStencilOperation StencilFailOp;
            ERHIStencilOperation StencilDepthFailOp;
            ERHIStencilOperation StencilPassOp;
            ERHIComparisonFunc   StencilFunc;
        };


        struct SRHIDepthStencilStateDesc
        {
            bool                   DepthEnable;
            ERHIDepthWriteMask     DepthWriteMask;
            ERHIComparisonFunc     DepthFunc;
            bool                   StencilEnable;
            uint8                  StencilReadMask;
            uint8                  StencilWriteMask;
            SRHIDepthStencilOpDesc FrontFace;
            SRHIDepthStencilOpDesc BackFace;
        };


        struct SRHIMappedSubresource
        {
            void* pData;
            uint  RowPitch;
            uint  DepthPitch;
        };


        struct SRHIShaderMacro
        {
            Stdlib::StringView MacroName;
            Stdlib::StringView MacroValue;
        };


        struct SRHIShaderCompileDesc
        {
            ERHIShaderLanguage language;
            ERHIShaderStage    stage;
            Stdlib::StringView entryPoint;
            union
            {
                SRHIShaderTarget::HLSL   hlsl;
                SRHIShaderTarget::GLSL   glsl;
                SRHIShaderTarget::GLSLES glsles;
            } target;
            const SRHIShaderMacro* macros;
            uint32                 macroCount;
            dword                  compileFlags = RHI_SHADER_COMPILE_NONE;
        };


        struct SRHIShaderRuntimeDesc
        {
            ERHIShaderLanguage     language;
            ERHIShaderStage        stage;
            ERHIRenderPipelineType pipeline;
            union
            {
                SRHIShaderTarget::HLSL   hlsl;
                SRHIShaderTarget::GLSL   glsl;
                SRHIShaderTarget::GLSLES glsles;
            } target;
        };


        struct SRHIShaderProgramDesc
        {
            ERHIRenderPipelineType pipeline;

            IRHIShader* vertexShader;
            IRHIShader* pixelShader;
            IRHIShader* geometryShader;
            IRHIShader* hullShader;
            IRHIShader* domainShader;
            IRHIShader* computeShader;

            const SRHIStreamOutputDesc* streamOutput;
            const char* debugName = nullptr;
        };


        typedef ERRCODE(*CreateRHIFactoryFn)(Common::IAllocator* objectAllocator, IRHIFactory** outFactory);
        extern "C" RHI_API ERRCODE CreateRHIFactory(Common::IAllocator* objectAllocator, IRHIFactory** outFactory);
    }
} // namespace krystallic


#ifdef __cplusplus
}
#endif


