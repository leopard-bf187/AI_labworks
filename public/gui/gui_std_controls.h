/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @Ra192192
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

			typedef enum _GUI_REG_STD_CONTROL_SYSTEM_ID : qword
            {
                GUI_REG_STD_CONTROL_SYS_FRAME = (1 << 0),
                GUI_REG_STD_CONTROL_SYS_BUTTON = (1 << 1),
                GUI_REG_STD_CONTROL_SYS_IMAGE = (1 << 2),
            } GUI_STD_CONTROL_SYSTEM_ID;


			typedef enum _GUI_STD_CONTROL_TYPE : dword
            {
                GUI_STD_CONTROL_FRAME = 0,
                GUI_STD_CONTROL_BUTTON,
                GUI_STD_CONTROL_IMAGE,

                GUI_STD_CONTROL_SCREEN,
                GUI_STD_CONTROL_OVERLAY,
                GUI_STD_CONTROL_POPUP,
                GUI_STD_CONTROL_WINDOW,
                GUI_STD_CONTROL_LABEL,
                GUI_STD_CONTROL_CHECKBOX,
                GUI_STD_CONTROL_RADIOBUTTON,
                GUI_STD_CONTROL_SLIDER,
                GUI_STD_CONTROL_GROUPBOX,
                GUI_STD_CONTROL_COMBOBOX,
                GUI_STD_CONTROL_EDITBOX,
                GUI_STD_CONTROL_LISTBOX,
                GUI_STD_CONTROL_PROGRESSBAR,
                GUI_STD_CONTROL_UNKNOWN = 0xffffffff,
            } GUI_STD_CONTROL_TYPE;


			typedef enum _GUI_STD_FRAME_STYLE : dword
            {
                // style list
            } GUI_STD_FRAME_STYLE;


			typedef enum _GUI_STD_BUTTON_STYLE : dword
            {
                // style list
            } GUI_STD_BUTTON_STYLE;


			typedef struct _GUI_STD_FRAME_DESC
            {
                int      controlID;
                dword    m_frameStyle;
                GUI_RECT m_frameWidth;
            } GUI_STD_FRAME_DESC;


			typedef struct _GUI_STD_BUTTON_DESC
            {
                int          controlID;
                OnCallbackFn m_onEnable;
                OnCallbackFn m_onHover;
                OnCallbackFn m_onMouseClick;
                OnCallbackFn m_onMouseDblClick;
                OnCallbackFn m_onMouseHold;
            } GUI_STD_BUTTON_DESC;

        } // namespace STDUI
    } // namespace GUI
} // namespace krystallic