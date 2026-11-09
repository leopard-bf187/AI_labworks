/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @Ra192192
*
*  Description: mouse input device interface
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
        struct IMouse : IBase
        {
            virtual ~IMouse() = default;

            /// Absolute cursor position in backend units accumulated since Initialize().
            virtual int32 GetX() = 0;
            virtual int32 GetY() = 0;

            /// Movement produced by the last Update() frame only.
            virtual int32 GetDeltaX() = 0;
            virtual int32 GetDeltaY() = 0;

            /// Wheel movement of the last Update() frame only; reset on every Update().
            virtual int32 GetWheelDelta() = 0;

            /// Returns true while the button is held. Unknown button indices return false.
            virtual bool IsButtonDown(uint32 button) = 0;

            /// Fills static mouse properties. Returns PLATFORM_ERR_INVALID_ARGUMENT for nullptr,
            /// PLATFORM_ERR_NOT_FOUND when the device is gone (shutdown or disconnected backend).
            virtual ERRCODE GetMouseInfo(SMouseInfo* outInfo) = 0;

            /// Copies the raw mouse snapshot cached by the last Update(). Never allocates.
            virtual ERRCODE GetRawState(SMouseState* state) = 0;

            /// Returns the backend device handle when the backend exposes one
            /// (Linux: evdev file descriptor), PLATFORM_ERR_NOT_SUPPORTED otherwise.
            virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const = 0;

            inline static SGuid GUID() { return {0x3c7b9e45, 0x12af, 0x40d6, {0xb8, 0x47, 0x62, 0x19, 0xde, 0x05, 0x9b, 0x71}}; }
        };
    } // namespace Platform
} // namespace krystallic
