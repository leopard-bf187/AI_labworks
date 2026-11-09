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
        struct IRHIFactory : IBase
        {
            virtual ~IRHIFactory() = default;

            virtual ERHIBackend GetBackend() const = 0;

            virtual uint GetAdapterCount() = 0;
            virtual ERRCODE GetAdapter(uint adapterIndex, IRHIAdapter** outAdapter) = 0;
            virtual ERRCODE CreateDevice(IRHIAdapter* adapter, SRHIDeviceInitialDesc* desc, IRHIDevice** outDevice, IRHIDeviceContext** outImmediateContext = nullptr, IRHIShaderCompiler** outShaderCompiler = nullptr) = 0;
            virtual ERRCODE CreateSwapChain(IRHIDevice* graphicsDevice, SRHISwapChainDesc* desc, IRHISwapChain** outSwapChain) = 0;
            
            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;

            inline static SGuid GUID()
            {
                return { 0xcf5b51e5, 0x5624, 0x4bd5, { 0x8f, 0x3c, 0xb6, 0xe8, 0xb2, 0xcd, 0xb5, 0x26 } };
            }
        };
    }
} // namespace krystallic