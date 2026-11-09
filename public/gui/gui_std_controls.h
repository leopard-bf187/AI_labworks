/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD & @Ra192192
*
*  Description:
*
*  Date: 15.10.2025
*/


#pragma once


#include "gui_dll_types.h"


namespace krystallic
{
	namespace GUI
	{
		namespace STDUI
		{

			enum GUI_REG_STD_CONTROL_SYSTEM_ID : qword
			{
				GUI_REG_STD_CONTROL_SYS_FRAME = (1 << 0),
				GUI_REG_STD_CONTROL_SYS_BUTTON = (1 << 1),
				GUI_REG_STD_CONTROL_SYS_LABEL = (1 << 2),
				GUI_REG_STD_CONTROL_SYS_IMAGE = (1ull << 3),
				GUI_REG_STD_CONTROL_SYS_OVERLAY = (1ull << 4),
				GUI_REG_STD_CONTROL_SYS_GROUPBOX = (1ull << 5),
				GUI_REG_STD_CONTROL_SYS_PROGRESSBAR = (1ull << 6),
				GUI_REG_STD_CONTROL_SYS_RADIOBUTTON = (1ull << 7),
				GUI_REG_STD_CONTROL_SYS_CHECKBOX = (1ull << 8),
				GUI_REG_STD_CONTROL_SYS_SLIDER = (1ull << 9),
			};

			enum GUI_STD_CONTROL_TYPE : dword
			{
				GUI_STD_CONTROL_FRAME = 0,
				GUI_STD_CONTROL_BUTTON,
				GUI_STD_CONTROL_LABEL,

				GUI_STD_CONTROL_IMAGE,
				GUI_STD_CONTROL_SCREEN,
				GUI_STD_CONTROL_OVERLAY,
				GUI_STD_CONTROL_POPUP,
				GUI_STD_CONTROL_WINDOW,
				GUI_STD_CONTROL_CHECKBOX,
				GUI_STD_CONTROL_RADIOBUTTON,
				GUI_STD_CONTROL_SLIDER,
				GUI_STD_CONTROL_GROUPBOX,
				GUI_STD_CONTROL_COMBOBOX,
				GUI_STD_CONTROL_EDITBOX,
				GUI_STD_CONTROL_LISTBOX,
				GUI_STD_CONTROL_PROGRESSBAR,
				GUI_STD_CONTROL_UNKNOWN = 0xffffffff,
			};

			enum GUI_STD_COLOR_THEME
			{
				GUI_STD_COLOR_THEME_LIGHT,
				GUI_STD_COLOR_THEME_DARK,
			};

			enum GUI_STD_FRAME_STYLE : dword
			{
				//ďĺđĺ÷ĺíü ńňčëĺé
			};

			enum GUI_STD_BUTTON_STYLE : dword
			{
				// ďĺđĺ÷ĺíü ńňčëĺé
			};


			struct GUI_STD_FRAME_DESC
			{
				int controlID;
				dword m_frameStyle;
				GUI_RECT m_frameWidth;
			};


			struct GUI_STD_BUTTON_DESC
			{
				int controlID;
				OnCallbackFn m_onEnable;
				OnCallbackFn m_onHover;
				OnCallbackFn m_onMouseClick;
				OnCallbackFn m_onMouseDblClick;
				OnCallbackFn m_onMouseHold;
				GUI_TEXT_REF m_text;
				float m_pixelSize;
			};

			// Label-specific rendering flags. The default matches the legacy Label.
			enum GUI_STD_LABEL_FLAGS : dword
			{
				GUI_STD_LABEL_DRAW_BACKGROUND = (1u << 0),
				GUI_STD_LABEL_OVERRIDE_TEXT_COLOR = (1u << 1),
				GUI_STD_LABEL_ALIGN_LEFT = (1u << 2),
				GUI_STD_LABEL_ALIGN_CENTER = (1u << 3),
				GUI_STD_LABEL_ALIGN_RIGHT = (1u << 4),
				GUI_STD_LABEL_ALIGN_TOP = (1u << 5),
				GUI_STD_LABEL_ALIGN_MIDDLE = (1u << 6),
				GUI_STD_LABEL_ALIGN_BOTTOM = (1u << 7),
			};

			constexpr dword GUI_STD_LABEL_DEFAULT_FLAGS =
				GUI_STD_LABEL_DRAW_BACKGROUND | GUI_STD_LABEL_OVERRIDE_TEXT_COLOR |
				GUI_STD_LABEL_ALIGN_CENTER | GUI_STD_LABEL_ALIGN_MIDDLE;


			struct GUI_STD_LABEL_DESC
			{
				GUI_TEXT_REF m_text;
				float m_pixelSize = 0.0f;
				dword m_flags = GUI_STD_LABEL_DEFAULT_FLAGS;
			};


			enum GUI_STD_OVERLAY_FLAGS : dword
			{
				GUI_STD_OVERLAY_DRAW_BACKGROUND = (1u << 0),
			};


			struct GUI_STD_OVERLAY_DESC
			{
				dword m_flags = 0;
			};


			enum GUI_STD_GROUPBOX_FLAGS : dword
			{
				GUI_STD_GROUPBOX_DRAW_BACKGROUND = (1u << 0),
				GUI_STD_GROUPBOX_DRAW_BORDER = (1u << 1),
				GUI_STD_GROUPBOX_DRAW_HEADER = (1u << 2),
			};

			constexpr dword GUI_STD_GROUPBOX_DEFAULT_FLAGS =
				GUI_STD_GROUPBOX_DRAW_BACKGROUND | GUI_STD_GROUPBOX_DRAW_BORDER | GUI_STD_GROUPBOX_DRAW_HEADER;


			struct GUI_STD_GROUPBOX_DESC
			{
				GUI_TEXT_REF m_text;
				float m_pixelSize = 0.0f;
				float m_borderWidth = 1.0f;
				float m_headerWidth = 0.0f; // 0: automatic visual header width.
				GUI_RECT m_padding; // All zero: automatic insets (8 / header height / 8 / 8).
				dword m_flags = GUI_STD_GROUPBOX_DEFAULT_FLAGS;
			};


			enum GUI_STD_PROGRESSBAR_FLAGS : dword
			{
				GUI_STD_PROGRESSBAR_DRAW_BORDER = (1u << 0),
				GUI_STD_PROGRESSBAR_REVERSE = (1u << 1),
			};


			enum GUI_STD_CHECKBOX_FLAGS : dword
			{
				GUI_STD_CHECKBOX_INDICATOR_RIGHT = (1u << 0),
			};


			struct GUI_STD_CHECKBOX_DESC
			{
				GUI_TEXT_REF m_text;
				float m_pixelSize = 0.0f;
				dword m_flags = 0;
				OnCheckStateChangeFn m_onCheckChanged = nullptr;
			};


			struct GUI_STD_RADIOBUTTON_DESC
			{
				GUI_TEXT_REF m_text;
				float m_pixelSize = 0.0f;
				dword m_groupID = 0;
				OnCheckStateChangeFn m_onCheckChanged = nullptr;
			};


			enum GUI_STD_SLIDER_FLAGS : dword
			{
				GUI_STD_SLIDER_REVERSE = (1u << 0),
			};


			struct GUI_STD_SLIDER_DESC
			{
				float m_min = 0.0f;
				float m_max = 1.0f;
				float m_value = 0.0f;
				float m_step = 0.0f; // 0 = continuous; positive values enable snapping.
				dword m_flags = 0;
				OnCheckValueChangeFn m_onValueChanged = nullptr;
			};


			struct GUI_STD_PROGRESSBAR_DESC
			{
				float m_min = 0.0f;
				float m_max = 1.0f;
				float m_value = 0.0f;
				dword m_flags = GUI_STD_PROGRESSBAR_DRAW_BORDER;
			};
		}
	}
}