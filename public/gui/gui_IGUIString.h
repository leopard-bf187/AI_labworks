/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @Ra192192
*
*  Description:
*
*  Date: 02.04.2026
*/


#pragma once


#include "gui_dll_types.h"


namespace krystallic
{
    namespace GUI
    {
        struct IGUIString : IBase
        {
            virtual ~IGUIString() = default;

            virtual ERRCODE Clear() = 0;

            virtual ERRCODE SetText(const char* utf8Text, GUI_TEXT_STYLE* style) = 0;
            virtual ERRCODE AppendText(const char* utf8Text, GUI_TEXT_STYLE* style) = 0;
            virtual ERRCODE AppendToRun(int runIndex, const char* utf8Text) = 0;

            virtual const char* GetTextUTF8() const = 0;
            virtual uint        GetTextLengthBytes() const = 0;
            virtual uint        GetTextLengthCodepoints() const = 0;

            virtual uint                GetRunCount() const = 0;
            virtual const GUI_TEXT_RUN* GetRuns() const = 0;
            virtual const GUI_TEXT_RUN* GetRunByCursor(int cursor) = 0;
            virtual int                 GetRunIndexByCursor(int cursor) = 0;

            inline static SGuid GUID()
            {
                return {0x27b7cb84, 0xf402, 0x49fd, {0xb5, 0x4b, 0xb4, 0xcf, 0x31, 0x9d, 0x8f, 0xbe}};
            }
        };
    } // namespace GUI
} // namespace krystallic
