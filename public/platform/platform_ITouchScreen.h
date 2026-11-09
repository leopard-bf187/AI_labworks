/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @Ra192192
*
*  Description: touchscreen input device interface
*
*  Date: 09.10.2026
*/


#pragma once


#include "platform_dll_types.h"
#include "platform_input_types.h"


namespace krystallic
{
    namespace Platform
    {
        struct ITouchScreen : IBase
        {
            virtual ~ITouchScreen() = default;

            /// Number of contacts reported by the last Update().
            virtual uint32 GetTouchCount() = 0;

            /// Copies one contact by index. Returns PLATFORM_ERR_INVALID_ARGUMENT for nullptr out,
            /// PLATFORM_ERR_OUT_OF_RANGE for an index that is not present in the current frame.
            virtual ERRCODE GetTouch(uint32 index, STouch* state) = 0;

            /// Fills static touchscreen properties. Returns PLATFORM_ERR_INVALID_ARGUMENT for
            /// nullptr, PLATFORM_ERR_NOT_FOUND when the device is gone.
            virtual ERRCODE GetTouchScreenInfo(STouchScreenInfo* outInfo) = 0;

            /// Copies the raw touchscreen snapshot cached by the last Update(). Never allocates.
            virtual ERRCODE GetRawState(STouchState* state) = 0;

            /// Returns the backend device handle when the backend exposes one
            /// (Linux: evdev file descriptor), PLATFORM_ERR_NOT_SUPPORTED otherwise.
            virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const = 0;

            inline static SGuid GUID() { return {0x2f85b7d1, 0x9e03, 0x4a17, {0xc5, 0x6b, 0x88, 0x24, 0x1f, 0xd3, 0xa9, 0x3c}}; }
        };
    } // namespace Platform
} // namespace krystallic
