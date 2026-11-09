/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @Ra192192
*
*  Description: gamepad input device interface
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
        struct IGamepad : IBase
        {
            virtual ~IGamepad() = default;

            /// Returns false while the backend reports the device as disconnected.
            virtual bool IsConnected() = 0;

            /// Bitmask of EGamepadButton values.
            virtual dword GetButtons() = 0;

            /// Triggers normalized to [0, 1].
            virtual float32 GetLeftTrigger() = 0;
            virtual float32 GetRightTrigger() = 0;

            /// Sticks normalized to [-1, 1].
            virtual float32 GetLeftStickX() = 0;
            virtual float32 GetLeftStickY() = 0;
            virtual float32 GetRightStickX() = 0;
            virtual float32 GetRightStickY() = 0;

            /// Requests rumble motors in [0, 1]. Returns PLATFORM_ERR_NOT_SUPPORTED when the
            /// backend cannot rumble the device (Linux stub: read-only evdev handle).
            virtual ERRCODE SetVibration(float32 leftMotor, float32 rightMotor) = 0;

            /// Fills static gamepad properties. Returns PLATFORM_ERR_INVALID_ARGUMENT for nullptr,
            /// PLATFORM_ERR_NOT_FOUND when the device is gone (shutdown or disconnected backend).
            virtual ERRCODE GetGamepadInfo(SGamepadInfo* outInfo) = 0;

            /// Copies the raw gamepad snapshot cached by the last Update(). Never allocates.
            virtual ERRCODE GetRawState(SGamepadState* state) = 0;

            /// Returns the backend device handle when the backend exposes one
            /// (Linux: evdev file descriptor), PLATFORM_ERR_NOT_SUPPORTED otherwise.
            virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const = 0;

            inline static SGuid GUID() { return {0x61d08fa9, 0x55c4, 0x4e2b, {0xa0, 0x38, 0x74, 0xcb, 0x2f, 0x96, 0x51, 0x08}}; }
        };
    } // namespace Platform
} // namespace krystallic
