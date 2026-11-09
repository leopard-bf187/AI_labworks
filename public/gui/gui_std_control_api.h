/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Description: Inline typed accessors for standard GUI controls.
*/

#pragma once

#include "gui_IGUIContext.h"
#include "gui_std_controls.h"

namespace krystallic
{
	namespace GUI
	{
		namespace STDUI
		{
			inline ERRCODE GetCheckBoxState(IGUIContext* context, int controlIndex, dword* outState)
			{
				return context ? context->GetStdControlCheckState(controlIndex, GUI_STD_CONTROL_CHECKBOX, outState) : GUI_ERR_INVALID_ARG;
			}

			inline ERRCODE SetCheckBoxState(IGUIContext* context, int controlIndex, dword state)
			{
				return context ? context->SetStdControlCheckState(controlIndex, GUI_STD_CONTROL_CHECKBOX, state) : GUI_ERR_INVALID_ARG;
			}

			inline ERRCODE GetRadioButtonChecked(IGUIContext* context, int controlIndex, bool* outChecked)
			{
				if (!outChecked)
					return GUI_ERR_INVALID_ARG;

				*outChecked = false;
				dword state = 0;
				ERRCODE result = context ? context->GetStdControlCheckState(controlIndex, GUI_STD_CONTROL_RADIOBUTTON, &state) : GUI_ERR_INVALID_ARG;

				if (result == GUI_ERR_OK)
					*outChecked = (state & GUI_STATE_CHECKED) != 0;

				return result;
			}

			inline ERRCODE SetRadioButtonChecked(IGUIContext* context, int controlIndex, bool checked)
			{
				return context ? context->SetStdControlCheckState(controlIndex, GUI_STD_CONTROL_RADIOBUTTON, checked ? GUI_STATE_CHECKED : 0) : GUI_ERR_INVALID_ARG;
			}

			inline ERRCODE GetSliderValue(IGUIContext* context, int controlIndex, float* outValue)
			{
				return context ? context->GetStdControlFloatValue(controlIndex, GUI_STD_CONTROL_SLIDER, outValue) : GUI_ERR_INVALID_ARG;
			}

			inline ERRCODE SetSliderValue(IGUIContext* context, int controlIndex, float value)
			{
				return context ? context->SetStdControlFloatValue(controlIndex, GUI_STD_CONTROL_SLIDER, value) : GUI_ERR_INVALID_ARG;
			}

			inline ERRCODE GetProgressBarValue(IGUIContext* context, int controlIndex, float* outValue)
			{
				return context ? context->GetStdControlFloatValue(controlIndex, GUI_STD_CONTROL_PROGRESSBAR, outValue) : GUI_ERR_INVALID_ARG;
			}

			inline ERRCODE SetProgressBarValue(IGUIContext* context, int controlIndex, float value)
			{
				return context ? context->SetStdControlFloatValue(controlIndex, GUI_STD_CONTROL_PROGRESSBAR, value) : GUI_ERR_INVALID_ARG;
			}
		}
	}
}
