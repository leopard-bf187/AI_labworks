/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @Ra192192
*
*  Description: keyboard input device interface
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
        struct IKeyboard : IBase
        {
            virtual ~IKeyboard() = default;

            /// Returns true while the key is held down. Unknown key codes return false.
            virtual bool IsKeyDown(uint32 key) = 0;

            /// Returns true only on the update frame where the key went down.
            virtual bool IsKeyPressed(uint32 key) = 0;

            /// Returns true only on the update frame where the key went up.
            virtual bool IsKeyReleased(uint32 key) = 0;

            /// Fills static keyboard properties. Returns PLATFORM_ERR_INVALID_ARGUMENT for nullptr,
            /// PLATFORM_ERR_NOT_FOUND when the device is gone (shutdown or disconnected backend).
            virtual ERRCODE GetKeyboardInfo(SKeyboardInfo* outInfo) = 0;

            /// Copies the raw key bitmap cached by the last Update(). Never allocates.
            virtual ERRCODE GetRawState(SKeyboardState* state) = 0;

            /// Returns the backend device handle when the backend exposes one
            /// (Linux: evdev file descriptor), PLATFORM_ERR_NOT_SUPPORTED otherwise.
            virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const = 0;

            inline static SGuid GUID() { return {0x9a4f1c02, 0x7d31, 0x4b8e, {0x91, 0x2c, 0x5f, 0xa7, 0x13, 0x6d, 0x84, 0xe5}}; }
        };
    } // namespace Platform
} // namespace krystallic
