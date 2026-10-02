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
        // прокси в виде объекта, который указывает на элемент управления в массиве элементов под капотом
        struct IGUIControl : IBase
        {
        protected:
            const char* m_id;
            int         m_index;
            dword*      m_type;
            GUI_RECT*   m_local;
            dword*      m_flags;
            dword*      m_state;
            dword*      m_style;
            int*        m_layer;
            float*      m_zOrder;
            void**      m_userData;

            IGUIControl()
                : m_id(0), m_index(-1), m_type(0), m_local(0), m_flags(0), m_state(0), m_style(0), m_layer(0),
                  m_zOrder(0), m_userData(0)
            {
            }

        public:
            virtual ~IGUIControl() = default;

            const char* GetID() const { return m_id; }

            const int GetIndex() const { return m_index; }

            const dword GetControlType() const { return *m_type; }

            Math::CVector2 GetPosition() const { return Math::CVector2(m_local->m_left, m_local->m_top); }
            Math::CVector2 GetSize() const { return Math::CVector2(m_local->GetWidth(), m_local->GetHeight()); }

            dword GetFlags() const { return *m_flags; }
            void  SetFlags(dword flags) { *m_flags = flags; }

            dword GetState() const { return *m_state; }
            void  SetState(dword state) { *m_state = state; }

            dword GetStyle() const { return *m_style; }
            void  SetStyle(dword style) { *m_style = style; }

            void* GetUserData() const { return *m_userData; }
            void  SetUserData(void* userData) { *m_userData = userData; }

            virtual IGUIContext* GetCurrentContext() = 0;
            virtual IGUIControl* GetParent() = 0;
            virtual IGUIControl* GetNextSibling() = 0;
            virtual IGUIControl* GetPrevSibling() = 0;
            virtual IGUIControl* GetFirstChild() = 0;
            virtual IGUIControl* GetLastChild() = 0;

            virtual ERRCODE SendMsg(dword msgCode, llong dataA, llong dataB) { return 0; };

            inline static SGuid GUID()
            {
                return {0x3f238c77, 0xbdb6, 0x48f1, {0x86, 0xb4, 0xcd, 0x24, 0x6b, 0x4a, 0x18, 0x4a}};
            }
        };
    } // namespace GUI
} // namespace krystallic
