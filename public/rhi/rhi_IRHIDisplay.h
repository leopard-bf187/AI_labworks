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
        struct IRHIDisplay : IBase
        {
            virtual ~IRHIDisplay() = default;

            virtual ERRCODE GetDesc(SRHIDisplayDesc* outDesc) = 0;
            virtual uint    GetDisplayModeCount(ERHIFormat colorFormat) = 0;
            virtual ERRCODE CreateDisplayModeList(const SRHIDisplayModeFilter* filter, IRHIDisplayModeList** outModeList) = 0;
            virtual ERRCODE FindClosestMatchingMode(const SRHIDisplayMode* modeToMatch, SRHIDisplayMode* outMode) const = 0;

            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;

            inline static SGuid GUID() 
            { 
                return {0x893468cf, 0xfc9b, 0x4a1a, {0x8d, 0x6a, 0x5f, 0x68, 0xc1, 0x5, 0x5d, 0x23}}; 
            }
        };
    }
} // namespace krystallic