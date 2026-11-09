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
        struct IRHIDisplayModeList : IBase
        {
            virtual ~IRHIDisplayModeList() = default;

            virtual uint    GetResolutionCount() const = 0;
            virtual ERRCODE GetResolution(uint index, SRHIResolution* outResolution) const = 0;

            virtual uint    GetFormatCount() const = 0;
            virtual ERRCODE GetFormat(uint index, ERHIFormat* outFormat) const = 0;

            virtual uint    GetRefreshRateCount() const = 0;
            virtual ERRCODE GetRefreshRate(uint index, SRHIRational* outRefreshRate) const = 0;

            virtual uint    GetDisplayModeCount() const = 0;
            virtual ERRCODE GetDisplayMode(uint index, SRHIDisplayMode* outMode) const = 0;

            virtual ERRCODE GetModeKey(uint index, SRHIDisplayModeKey* outKey) const = 0;
            virtual ERRCODE GetDisplayModeByKey(const SRHIDisplayModeKey* key, SRHIDisplayMode* outMode) const = 0;

            virtual ERRCODE ValidateMode(const SRHIDisplayMode* mode) const = 0;

            inline static SGuid GUID() 
            { 
                return {0xb277204d, 0xd8a5, 0x4c67, {0x8f, 0x42, 0x79, 0x90, 0x96, 0xba, 0xca, 0x67}};
            }
        };
    }
} // namespace krystallic