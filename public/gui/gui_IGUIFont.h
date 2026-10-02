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
        struct IGUIFont : IBase
        {
        protected:
            const char* m_id;
            dword       m_hash;
            dword       m_style;

            IGUIFont() : m_id(0), m_hash(0), m_style(0) {}

        public:
            virtual ~IGUIFont() = default;

            inline const char* GetID() const { return m_id; };
            inline dword       GetHashID() const { return m_hash; };
            inline bool        HasFaceStyle(dword style) { return (m_style & style) != 0ul; }

            virtual ERRCODE
            AddFaceData(Common::IBuffer* fontData,
                        GUI_FONT_STYLE   style) = 0; // добавляет данные шрифта в объект, преобразуя их в глифы и т.д.

            inline static SGuid GUID()
            {
                return {0x95742050, 0x3915, 0x40a7, {0xa1, 0xab, 0xc4, 0xa1, 0x84, 0x50, 0xb, 0xa}};
            }
        };
    } // namespace GUI
} // namespace krystallic
