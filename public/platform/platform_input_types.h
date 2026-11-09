/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @Ra192192
*
*  Description: backend-independent input types shared by all platform input backends
*
*  Date: 09.10.2026
*/


#pragma once


#include "../pch.h"


namespace krystallic
{
    namespace Platform
    {
        /// Input device classes exposed by IPlatformInputManager enumeration and creation.
        enum EInputDeviceType : uint32
        {
            INPUT_DEVICE_KEYBOARD,
            INPUT_DEVICE_MOUSE,
            INPUT_DEVICE_GAMEPAD,
            INPUT_DEVICE_TOUCHSCREEN,
        };


        /// Maximum number of simultaneous touch contacts reported by one touchscreen.
        constexpr uint32 INPUT_TOUCH_MAX_CONTACTS = 10;

        /// Capacity of the raw keyboard state bitmap. Key codes are bit indices.
        constexpr uint32 INPUT_KEY_STATE_DWORD_COUNT = 8;
        constexpr uint32 INPUT_KEY_STATE_CAPACITY = INPUT_KEY_STATE_DWORD_COUNT * 32;

        /// Fixed capacity of a device name string, including the terminator.
        constexpr uint32 INPUT_DEVICE_NAME_CAPACITY = 128;


        /// Backend-independent key codes.
        /// Codes are stable bit indices inside SKeyboardState; append new codes only.
        enum EKeyCode : uint32
        {
            KEYCODE_NONE = 0,

            KEYCODE_A = 1,
            KEYCODE_B,
            KEYCODE_C,
            KEYCODE_D,
            KEYCODE_E,
            KEYCODE_F,
            KEYCODE_G,
            KEYCODE_H,
            KEYCODE_I,
            KEYCODE_J,
            KEYCODE_K,
            KEYCODE_L,
            KEYCODE_M,
            KEYCODE_N,
            KEYCODE_O,
            KEYCODE_P,
            KEYCODE_Q,
            KEYCODE_R,
            KEYCODE_S,
            KEYCODE_T,
            KEYCODE_U,
            KEYCODE_V,
            KEYCODE_W,
            KEYCODE_X,
            KEYCODE_Y,
            KEYCODE_Z,

            KEYCODE_0,
            KEYCODE_1,
            KEYCODE_2,
            KEYCODE_3,
            KEYCODE_4,
            KEYCODE_5,
            KEYCODE_6,
            KEYCODE_7,
            KEYCODE_8,
            KEYCODE_9,

            KEYCODE_F1,
            KEYCODE_F2,
            KEYCODE_F3,
            KEYCODE_F4,
            KEYCODE_F5,
            KEYCODE_F6,
            KEYCODE_F7,
            KEYCODE_F8,
            KEYCODE_F9,
            KEYCODE_F10,
            KEYCODE_F11,
            KEYCODE_F12,

            KEYCODE_ESCAPE,
            KEYCODE_BACKSPACE,
            KEYCODE_TAB,
            KEYCODE_ENTER,
            KEYCODE_SPACE,
            KEYCODE_CAPSLOCK,
            KEYCODE_SCROLLLOCK,
            KEYCODE_NUMLOCK,
            KEYCODE_PAUSE,
            KEYCODE_PRINTSCREEN,

            KEYCODE_LSHIFT,
            KEYCODE_RSHIFT,
            KEYCODE_LCTRL,
            KEYCODE_RCTRL,
            KEYCODE_LALT,
            KEYCODE_RALT,
            KEYCODE_LGUI,
            KEYCODE_RGUI,
            KEYCODE_MENU,

            KEYCODE_INSERT,
            KEYCODE_DELETE,
            KEYCODE_HOME,
            KEYCODE_END,
            KEYCODE_PAGEUP,
            KEYCODE_PAGEDOWN,
            KEYCODE_UP,
            KEYCODE_DOWN,
            KEYCODE_LEFT,
            KEYCODE_RIGHT,

            KEYCODE_MINUS,
            KEYCODE_EQUALS,
            KEYCODE_LBRACKET,
            KEYCODE_RBRACKET,
            KEYCODE_BACKSLASH,
            KEYCODE_SEMICOLON,
            KEYCODE_APOSTROPHE,
            KEYCODE_GRAVE,
            KEYCODE_COMMA,
            KEYCODE_PERIOD,
            KEYCODE_SLASH,

            KEYCODE_KP_0,
            KEYCODE_KP_1,
            KEYCODE_KP_2,
            KEYCODE_KP_3,
            KEYCODE_KP_4,
            KEYCODE_KP_5,
            KEYCODE_KP_6,
            KEYCODE_KP_7,
            KEYCODE_KP_8,
            KEYCODE_KP_9,
            KEYCODE_KP_ENTER,
            KEYCODE_KP_PLUS,
            KEYCODE_KP_MINUS,
            KEYCODE_KP_MULTIPLY,
            KEYCODE_KP_DIVIDE,
            KEYCODE_KP_DECIMAL,

            KEYCODE_LIMIT,
        };

        static_assert(KEYCODE_LIMIT <= INPUT_KEY_STATE_CAPACITY, "key codes must fit into the raw keyboard state bitmap");


        /// Mouse buttons as bit indices inside SMouseState::buttons.
        enum EMouseButton : uint32
        {
            MOUSE_BUTTON_LEFT = 0,
            MOUSE_BUTTON_RIGHT,
            MOUSE_BUTTON_MIDDLE,
            MOUSE_BUTTON_X1,
            MOUSE_BUTTON_X2,

            MOUSE_BUTTON_COUNT_LIMIT,

            MOUSE_BUTTON_MASK_LEFT = (1u << MOUSE_BUTTON_LEFT),
            MOUSE_BUTTON_MASK_RIGHT = (1u << MOUSE_BUTTON_RIGHT),
            MOUSE_BUTTON_MASK_MIDDLE = (1u << MOUSE_BUTTON_MIDDLE),
            MOUSE_BUTTON_MASK_X1 = (1u << MOUSE_BUTTON_X1),
            MOUSE_BUTTON_MASK_X2 = (1u << MOUSE_BUTTON_X2),
        };


        /// Gamepad buttons as bit indices inside SGamepadState::buttons.
        enum EGamepadButton : uint32
        {
            GAMEPAD_BUTTON_A = 0,
            GAMEPAD_BUTTON_B,
            GAMEPAD_BUTTON_X,
            GAMEPAD_BUTTON_Y,
            GAMEPAD_BUTTON_LEFT_SHOULDER,
            GAMEPAD_BUTTON_RIGHT_SHOULDER,
            GAMEPAD_BUTTON_BACK,
            GAMEPAD_BUTTON_START,
            GAMEPAD_BUTTON_GUIDE,
            GAMEPAD_BUTTON_LEFT_STICK,
            GAMEPAD_BUTTON_RIGHT_STICK,
            GAMEPAD_BUTTON_DPAD_UP,
            GAMEPAD_BUTTON_DPAD_DOWN,
            GAMEPAD_BUTTON_DPAD_LEFT,
            GAMEPAD_BUTTON_DPAD_RIGHT,

            GAMEPAD_BUTTON_COUNT_LIMIT,
        };


        /// Per-contact lifecycle reported by ITouchScreen.
        enum ETouchState : uint32
        {
            TOUCH_STATE_NONE = 0,
            TOUCH_STATE_DOWN,
            TOUCH_STATE_MOVE,
            TOUCH_STATE_UP,
        };


        /// Full keyboard snapshot for one update frame.
        /// Raw state: every key known to the backend is stored as a bit, no per-frame filtering.
        struct SKeyboardState
        {
            dword keys[INPUT_KEY_STATE_DWORD_COUNT];

            inline void Clear()
            {
                for (uint32 i = 0; i < INPUT_KEY_STATE_DWORD_COUNT; ++i)
                {
                    keys[i] = 0;
                }
            }

            inline bool IsKeyDown(uint32 key) const
            {
                if (key >= INPUT_KEY_STATE_CAPACITY)
                {
                    return false;
                }

                return (keys[key >> 5] & (1u << (key & 31))) != 0;
            }

            inline void SetKey(uint32 key, bool down)
            {
                if (key >= INPUT_KEY_STATE_CAPACITY)
                {
                    return;
                }

                const dword mask = (1u << (key & 31));

                if (down)
                {
                    keys[key >> 5] |= mask;
                }
                else
                {
                    keys[key >> 5] &= ~mask;
                }
            }
        };


        struct SKeyboardInfo
        {
            char   name[INPUT_DEVICE_NAME_CAPACITY];
            uint32 index;
            uint32 keyCount;
            bool   connected;
        };


        /// Mouse snapshot for one update frame.
        /// Deltas and wheelDelta are valid only for the frame produced by the last Update().
        struct SMouseState
        {
            int32 x;
            int32 y;
            int32 deltaX;
            int32 deltaY;
            int32 wheelDelta;
            dword buttons;
            bool  connected;
        };


        struct SMouseInfo
        {
            char   name[INPUT_DEVICE_NAME_CAPACITY];
            uint32 index;
            uint32 buttonCount;
            bool   connected;
        };


        /// Gamepad snapshot. Sticks are normalized to [-1, 1], triggers to [0, 1].
        struct SGamepadState
        {
            bool    connected;
            dword   buttons;
            float32 leftTrigger;
            float32 rightTrigger;
            float32 leftStickX;
            float32 leftStickY;
            float32 rightStickX;
            float32 rightStickY;
        };


        struct SGamepadInfo
        {
            char   name[INPUT_DEVICE_NAME_CAPACITY];
            uint32 index;
            uint32 buttonCount;
            bool   connected;
        };


        /// One active touch contact.
        struct STouch
        {
            uint32      id;
            int32       x;
            int32       y;
            float32     pressure;
            ETouchState state;
        };


        /// Touchscreen snapshot for one update frame.
        struct STouchState
        {
            uint32 touchCount;
            STouch touches[INPUT_TOUCH_MAX_CONTACTS];
        };


        struct STouchScreenInfo
        {
            char   name[INPUT_DEVICE_NAME_CAPACITY];
            uint32 index;
            uint32 maxTouchCount;
            bool   connected;
        };
    } // namespace Platform
} // namespace krystallic
