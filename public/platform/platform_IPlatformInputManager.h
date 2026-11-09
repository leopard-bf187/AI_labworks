/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @Ra192192
*
*  Description: platform input manager interface (spec 30.6.2)
*
*  Date: 02.06.2026
*/


#pragma once


#include "platform_dll_types.h"
#include "platform_IKeyboard.h"
#include "platform_IMouse.h"
#include "platform_IGamepad.h"
#include "platform_ITouchScreen.h"


namespace krystallic
{
    namespace Platform
    {
        struct IPlatformInputManager : IBase
        {
            virtual ~IPlatformInputManager() = default;

            /// Initializes the backend, prepares raw input structures and enumerates devices.
            /// Returns PLATFORM_ERR_ALREADY_EXISTS when the backend is already initialized.
            virtual ERRCODE Initialize() = 0;

            /// Advances the input update model: previous state <- current state, then reads the
            /// backend (evdev events or injected simulation state) and refreshes pressed/released
            /// flags and deltas. Must not allocate memory. Returns PLATFORM_ERR_NOT_SUPPORTED
            /// before Initialize().
            virtual ERRCODE Update() = 0;

            /// Releases backend resources, device handles and internal state.
            /// Idempotent: shutting down an uninitialized backend returns PLATFORM_ERR_OK.
            virtual ERRCODE Shutdown() = 0;

            /// Enumeration is refreshed by Initialize(); devices lost at runtime are dropped by
            /// Update(). Hotplug policy: call Shutdown() + Initialize() to re-scan the system.
            virtual uint32 GetAvailableKeyboardCount() = 0;
            virtual uint32 GetAvailableMouseCount() = 0;
            virtual uint32 GetAvailableGamepadCount() = 0;
            virtual uint32 GetAvailableTouchScreenCount() = 0;

            /// Creates a ref-counted device view over the backend device state.
            /// Returns PLATFORM_ERR_INVALID_ARGUMENT for a nullptr out pointer,
            /// PLATFORM_ERR_NOT_SUPPORTED before Initialize(),
            /// PLATFORM_ERR_OUT_OF_RANGE for an index without a connected device.
            virtual ERRCODE CreateKeyboard(uint32 index, IKeyboard** outKb) = 0;
            virtual ERRCODE CreateMouse(uint32 index, IMouse** outMs) = 0;
            virtual ERRCODE CreateGamepad(uint32 index, IGamepad** outGp) = 0;
            virtual ERRCODE CreateTouchScreen(uint32 index, ITouchScreen** outTs) = 0;

            /// The input manager itself has no single native handle:
            /// PLATFORM_ERR_INVALID_ARGUMENT for nullptr, PLATFORM_ERR_NOT_SUPPORTED otherwise.
            virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const = 0;

            inline static SGuid GUID() { return {0x38316cae, 0x3ff0, 0x445c, {0xa1, 0x4, 0x2d, 0xc1, 0xb9, 0x2e, 0xf6, 0xf7}}; }
        };
    } // namespace Platform
} // namespace krystallic
