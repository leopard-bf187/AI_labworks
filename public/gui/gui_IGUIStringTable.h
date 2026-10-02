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
        struct IGUIStringTable : IBase
        {
        protected:
            dword       m_hash;
            const char* m_id;

        public:
            virtual ~IGUIStringTable() = default;

            inline const char* GetID() const { return m_id; };
            inline dword       GetHashID() const { return m_hash; };

            virtual ERRCODE AddString(const char* stringID, IGUIString* str, GUI_TEXT_REF* outRef = 0) = 0;
            virtual ERRCODE AddPlainString(const char*     stringID,
                                           const char*     utf8Text,
                                           GUI_TEXT_STYLE* style,
                                           GUI_TEXT_REF*   outRef = 0) = 0;

            virtual ERRCODE RemoveStringByID(const char* stringID) = 0;
            virtual ERRCODE RemoveStringByRef(const GUI_TEXT_REF& ref) = 0;

            virtual ERRCODE GetStringByID(const char* stringID, IGUIString** outString) = 0;
            virtual ERRCODE GetStringByRef(const GUI_TEXT_REF& ref, IGUIString** outString) = 0;

            virtual GUI_TEXT_REF GetStringRefByID(const char* stringID) const = 0;

            virtual uint GetStringCount() const = 0;

            inline static SGuid GUID()
            {
                return {0xd3ff005d, 0x2d1d, 0x42fb, {0xbc, 0x97, 0x95, 0xf0, 0xb9, 0xec, 0x5f, 0xe7}};
            }
        };
    } // namespace GUI
} // namespace krystallic
