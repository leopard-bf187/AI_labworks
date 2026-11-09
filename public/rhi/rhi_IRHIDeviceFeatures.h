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
        struct IRHIDeviceFeatures : IBase
        {
            virtual ~IRHIDeviceFeatures() = default;

            virtual ERHIBackend GetBackend() const = 0;
            virtual dword       GetSupportedPipelines() const = 0;

            virtual bool HasGeometryShader() const = 0;
            virtual bool HasStreamOutput() const = 0;
            virtual uint GetMaxStreamOutputBuffers() const = 0;
            virtual bool HasTessellationShader() const = 0;
            virtual bool HasComputeShader() const = 0;
            virtual bool HasUAV() const = 0;
            virtual bool HasCommandLists() const = 0;
            virtual bool HasIndirectDraw() const = 0;
            virtual bool HasInstancing() const = 0;
            virtual bool HasTexture1D() const = 0;
            virtual bool HasTexture3D() const = 0;
            virtual bool HasTextureCubeArray() const = 0;
            virtual bool HasTextureMSAA() const = 0;
            virtual bool HasDebugMarkers() const = 0;
            virtual bool HasPredication() const = 0;
            virtual bool HasBC1Compression() const = 0;
            virtual bool HasBC2Compression() const = 0;
            virtual bool HasBC3Compression() const = 0;
            virtual bool HasBC4Compression() const = 0;
            virtual bool HasBC5Compression() const = 0;
            virtual bool HasBC6Compression() const = 0;
            virtual bool HasBC7Compression() const = 0;
            virtual bool HasETC1Compression() const = 0;
            virtual bool HasETC2Compression() const = 0;
            virtual bool HasEACCompression() const = 0;
            virtual bool HasASTCCompression() const = 0;

            virtual ERRCODE GetLimits(SRHIDeviceLimits* outLimits) const = 0;
            virtual bool    IsFormatSupported(ERHIFormat format, dword bindFlags) const = 0;

            inline static SGuid GUID() 
            { 
                return {0x3a902663, 0xb4a, 0x4e1d, {0x82, 0x23, 0x70, 0x53, 0x1a, 0x59, 0xca, 0x9a}};
            }
        };
    }
} // namespace krystallic