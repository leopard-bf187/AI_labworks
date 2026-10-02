/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
* 
*  Authors: 
*
*  Description:
*
*  Date: 
*/


#pragma once


#include "pch.h"


namespace krystallic
{
    namespace GUI
    {
        enum GUI_ERRCODE : ERRCODE
        {
            GUI_ERR_OK = 0,
            GUI_ERR_INVALID_ARG,
            GUI_ERR_NOT_IMPLEMENTED,
            GUI_ERR_INVALID_PARENT_INDEX,
            GUI_ERR_INVALID_CONTROL_ID,
            GUI_ERR_CONTROL_NOT_FOUND,
            GUI_ERR_INVALID_CONTROL_INDEX,
            GUI_ERR_INVALID_RENDER_STRATEGY,
            GUI_ERR_FAILED_TO_CREATE,
            GUI_ERR_OUT_OF_MEMORY,
            //GUI_ERR_,
            //GUI_ERR_,
            //GUI_ERR_,
            //GUI_ERR_,
            //GUI_ERR_,
            //GUI_ERR_,
            //GUI_ERR_,
            //GUI_ERR_,
            //GUI_ERR_,
            //GUI_ERR_,
        };


        enum GUI_CONTROL_FLAGS : dword
        {
            GUI_FLAG_IS_ROOT_CONTROL = (1 << 0),
            GUI_FLAG_SUPPORTS_INPUT = (1 << 1),
            GUI_FLAG_HAS_CHILDREN = (1 << 2),

            GUI_FLAG_ANCHOR_LEFT = (1 << 8),
            GUI_FLAG_ANCHOR_TOP = (1 << 9),
            GUI_FLAG_ANCHOR_RIGHT = (1 << 10),
            GUI_FLAG_ANCHOR_BOTTOM = (1 << 11),
            GUI_FLAG_STRETCH_LEFT = (1 << 12),
            GUI_FLAG_STRETCH_TOP = (1 << 13),
            GUI_FLAG_STRETCH_RIGHT = (1 << 14),
            GUI_FLAG_STRETCH_BOTTOM = (1 << 15),
        };


        enum GUI_EVENT_TYPE : dword
        {
            GUI_EVENT_HOVER_ENTER,
            GUI_EVENT_HOVER_LEAVE,
            GUI_EVENT_MOUSE_DOWN,
            GUI_EVENT_MOUSE_UP,
            GUI_EVENT_CLICK,
            GUI_EVENT_DOUBLE_CLICK,
            GUI_EVENT_FOCUS_GAINED,
            GUI_EVENT_FOCUS_LOST,
        };


        enum GUI_CONTROL_STYLES : dword
        {
            // common control
            GUI_STYLE_BORDER = (1 << 0),
            GUI_STYLE_FRAME = (1 << 1),
            GUI_STYLE_TEXT = (1 << 2),
            GUI_STYLE_TEXTURED = (1 << 3),
            GUI_STYLE_OPACITY = (1 << 4),
            GUI_STYLE_TRANSPARENT = (1 << 5),

            // button
            GUI_STYLE_PUSH_BUTTON = (1 << 6),

            // combobox, edit box
            GUI_STYLE_FRAME_EX_LIST = (1 << 7),

            // editbox
            GUI_STYLE_MULTILINE = (1 << 8),

            // checkbox
            GUI_STYLE_THREE_STATE = (1 << 9),

            // radio button, slider, progressbar
            GUI_STYLE_VERTICAL = (1 << 10),

            // groupbox
            GUI_STYLE_ROUNDED = (1 << 11),
            GUI_STYLE_ALIGNED = (1 << 12),
            GUI_STYLE_BOTTOM_HEADER = (1 << 13),

            // listbox
            GUI_STYLE_SORTED = (1 << 14),
            GUI_STYLE_MULTISEL = (1 << 15),

            // slider
            GUI_STYLE_GROOVE = (1 << 16),
            GUI_STYLE_TICKS = (1 << 17),

            // progressbar
            GUI_STYLE_MARQUEE = (1 << 18),
        };



        enum GUI_CONTROL_STATES : dword
        {
            // common control
            GUI_STATE_ENABLED = (1 << 0),
            GUI_STATE_FOCUSED = (1 << 1),
            GUI_STATE_HOVERED = (1 << 2),
            GUI_STATE_VISIBLE = (1 << 3),

            //Button
            GUI_STATE_PUSHED = (1 << 4),

            //ComboBox
            GUI_STATE_OPENED = (1 << 5),

            //EditBox
            GUI_STATE_FORCED_FOCUS = (1 << 6),

            //Checkbox
            GUI_STATE_CHECKED = (1 << 7),

            //RadioButton, ComboBox, ListBox
            GUI_STATE_CHANGED = (1 << 8),
        };


        enum GUI_CONTROL_EVENT_ID : dword
        {
            GUI_EVT_ON_ENABLE = (1 << 0),         // OnEnable
            GUI_EVT_ON_FOCUS = (1 << 1),          // OnFocus
            GUI_EVT_ON_HOVER = (1 << 2),          // OnHover
            GUI_EVT_ON_MOUSE_CLICK = (1 << 3),    // OnMouseClick
            GUI_EVT_ON_MOUSE_DBLCLICK = (1 << 4), // OnMouseDblClick
            GUI_EVT_ON_MOUSE_HOLD = (1 << 5),     // OnMouseHold
            GUI_EVT_ON_VALUE_CHANGE = (1 << 6),   // OnValueChange
            GUI_EVT_ON_SELECT = (1 << 7),         // OnSelect
            GUI_EVT_ON_THUMB_MOVE = (1 << 8),     // OnThumbMove
            GUI_EVT_ON_SET_CURSOR = (1 << 9),     // OnSetCursor
            GUI_EVT_ON_CHAR = (1 << 10),          // OnChar
            GUI_EVT_ON_INPUT = (1 << 11),         // OnInput
            GUI_EVT_ON_KEY_COMBO = (1 << 12),     // OnKeyCombo
        };



        enum GUI_RESOURCE_TYPE
        {
            GUI_RESOURCE_CURSOR,
            GUI_RESOURCE_FONT,
            GUI_RESOURCE_TEXTURE,
        };


        enum GUI_RENDERABLE_RESOURCE_TYPE
        {
            GUI_RENDERABLE_RESOURCE_INPUT_LAYOUT,
            GUI_RENDERABLE_RESOURCE_VERTEX_BUFFER,
            GUI_RENDERABLE_RESOURCE_INDEX_BUFFER,
            GUI_RENDERABLE_RESOURCE_TEXTURE_2D,
            GUI_RENDERABLE_RESOURCE_SHADER_BLOB,
            GUI_RENDERABLE_RESOURCE_VERTEX_SHADER,
            GUI_RENDERABLE_RESOURCE_PIXEL_SHADER,
        };


        enum GUI_RESOURCE_MAP_TYPE
        {
            GUI_RESOURCE_MAP_READ,
            GUI_RESOURCE_MAP_WRITE,
            GUI_RESOURCE_MAP_READ_WRITE,
            GUI_RESOURCE_MAP_WRITE_DISCARD,
            GUI_RESOURCE_MAP_WRITE_NO_OVERWRITE,
        };


        enum GUI_RESOURCE_USAGE_TYPE
        {
            GUI_RESOURCE_USAGE_DEFAULT = 0,
            GUI_RESOURCE_USAGE_IMMUTABLE = 1,
            GUI_RESOURCE_USAGE_DYNAMIC = 2,
            GUI_RESOURCE_USAGE_STAGING = 3
        };


        enum GUI_TEXTURE_FORMAT : uint
        {
            GUI_R8_TYPELESS,
            GUI_R8_UNORM,
            GUI_R8_UINT,
            GUI_R8_SNORM,
            GUI_R8_SINT,

            GUI_R16_TYPELESS,
            GUI_R16_FLOAT,
            GUI_R16_UNORM,
            GUI_R16_UINT,
            GUI_R16_SNORM,
            GUI_R16_SINT,

            GUI_R32_TYPELESS,
            GUI_R32_FLOAT,
            GUI_R32_UINT,
            GUI_R32_SINT,

            GUI_RG8_TYPELESS,
            GUI_RG8_UNORM,
            GUI_RG8_UINT,
            GUI_RG8_SNORM,
            GUI_RG8_SINT,

            GUI_RG16_TYPELESS,
            GUI_RG16_FLOAT,
            GUI_RG16_UNORM,
            GUI_RG16_UINT,
            GUI_RG16_SNORM,
            GUI_RG16_SINT,

            GUI_RG32_TYPELESS,
            GUI_RG32_FLOAT,
            GUI_RG32_UINT,
            GUI_RG32_SINT,

            GUI_RGB32_TYPELESS,
            GUI_RGB32_FLOAT,
            GUI_RGB32_UINT,
            GUI_RGB32_SINT,

            GUI_RGBA8_TYPELESS,
            GUI_RGBA8_UNORM,
            GUI_RGBA8_UNORM_SRGB,
            GUI_RGBA8_UINT,
            GUI_RGBA8_SNORM,
            GUI_RGBA8_SINT,

            GUI_RGBA16_TYPELESS,
            GUI_RGBA16_FLOAT,
            GUI_RGBA16_UNORM,
            GUI_RGBA16_UINT,
            GUI_RGBA16_SNORM,
            GUI_RGBA16_SINT,

            GUI_RGBA32_TYPELESS,
            GUI_RGBA32_FLOAT,
            GUI_RGBA32_UINT,
            GUI_RGBA32_SINT,
        };


        enum GUI_FONT_STYLE
        {
            GUI_FONT_REGULAR = 0,
            GUI_FONT_BOLD = (1 << 0),
            GUI_FONT_ITALIC = (1 << 1),
        };


        enum GUI_FONT_DECORATION
        {
            GUI_FONT_DECOR_NONE = 0,
            GUI_FONT_DECOR_STRIKE = (1 << 0),
            GUI_FONT_DECOR_UNDERLINE = (1 << 1),
        };


        enum GUI_FONT_MODE
        {
            GUI_FONT_MODE_STATIC_ATLAS,
            GUI_FONT_MODE_DYNAMIC_ATLAS,
        };


        enum GUI_GLYPH_RANGE_MASK : dword
        {
            GUI_GLYPH_RANGE_DIGITS = (1 << 0),
            GUI_GLYPH_RANGE_PUNCTUATION = (1 << 1),
            GUI_GLYPH_RANGE_GUI_SYMBOLS = (1 << 2),
            GUI_GLYPH_RANGE_LATIN_BASIC = (1 << 3),
            GUI_GLYPH_RANGE_LATIN_EXTENDED = (1 << 4),
            GUI_GLYPH_RANGE_CYRILLIC = (1 << 5),
            GUI_GLYPH_RANGE_CUSTOM = (1ul << 31),
        };


        enum GUI_STRING_DRAW_FLAGS : dword
        {
            GUI_STRING_DRAW_OVERRIDE_STYLE_COLOR,
            GUI_STRING_DRAW_VERTICAL_ALIGN_TOP,
            GUI_STRING_DRAW_VERTICAL_ALIGN_CENTER,
            GUI_STRING_DRAW_VERTICAL_ALIGN_BOTTOM,
            GUI_STRING_DRAW_HORIZONTAL_ALIGN_LEFT,
            GUI_STRING_DRAW_HORIZONTAL_ALIGN_MIDDLE,
            GUI_STRING_DRAW_HORIZONTAL_ALIGN_RIGHT,
        };
    } // namespace GUI
} // namespace krystallic
