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
        struct IRHISwapChain : IBase
        {
            virtual ~IRHISwapChain() = default;

            virtual ERRCODE GetDesc(SRHISwapChainDesc* outDesc) = 0;
            virtual ERRCODE GetBackBufferTexture(IRHIResource** outBackBufferTex2D) = 0;
            virtual ERRCODE GetDepthBufferTexture(IRHIResource** outDepthBufferTex2D) = 0;
            virtual ERRCODE GetRenderTargetView(IRHIRenderTargetView** outRTV) = 0;
            virtual ERRCODE GetDepthStencilView(IRHIDepthStencilView** outDSV) = 0;

            virtual ERRCODE GetContainingDisplay(IRHIDisplay** outDisplay) = 0;

            virtual ERRCODE SetFullscreenState(bool fullscreen, IRHIDisplay* targetDisplay) = 0;
            virtual ERRCODE GetFullscreenState(bool* fullscreen, IRHIDisplay** outTargetDisplay) = 0;

            virtual ERRCODE ResizeBuffers(uint numBuffers, uint width, uint height, ERHIFormat format, dword swapChainFlags) = 0;
            virtual ERRCODE ResizeTargets(const SRHIDisplayMode* newDisplayMode) = 0;

            virtual ERRCODE Present(ERHIVSyncMode vsync, dword flags) = 0;

            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;

            inline static SGuid GUID() 
            {
                return {0x879697e6, 0xf3af, 0x4e8a, {0xaa, 0x7f, 0xb7, 0xde, 0x2e, 0x8, 0xf, 0xa3}};
            }
        };
    }
} // namespace krystallic