/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @Ra192192
*
*  Description:
*
*  Date:
*/


#pragma once


#include "std_dll_types.h"
#include "std_StringView.h"
#include "std_String.h"


namespace krystallic
{
    namespace Stdlib
    {
        class STDLIB_API StringU16
        {
        private:
            char16_t* m_Data;
            uint      m_SizeUnits;
            uint      m_Capacity;

        private:
            void        AllocateEmpty();
            void        Reallocate(uint newCapacity);
            void        EnsureCapacity(uint requiredCapacity);
            bool        IsPointerInside(const char16_t* ptr) const;
            static uint GrowCapacity(uint requiredCapacity);

            // Raw unit paths for conversions: units must not alias this
            // string's buffer.
            void AppendUnits(const char16_t* units, uint count);
            void AssignUnits(const char16_t* units, uint count);

            friend class StringU32;

        public:
            StringU16();
            StringU16(const char16_t* str);
            StringU16(const String& strUtf8);
            StringU16(const StringU16& other);
            StringU16(StringU16&& other) noexcept;
            ~StringU16();

            StringU16& operator=(const StringU16& other);
            StringU16& operator=(StringU16&& other) noexcept;
            StringU16& operator=(const char16_t* str);

            const char16_t* Data() const { return m_Data; }
            char16_t*       Data() { return m_Data; }

            const char16_t* CStr() const { return m_Data; }

            uint SizeUnits() const { return m_SizeUnits; }
            uint Capacity() const { return m_Capacity; }

            bool Empty() const { return m_SizeUnits == 0; }

            char16_t operator[](uint index) const { return m_Data[index]; }

            void Clear();
            void Reserve(uint newCapacity);
            void ShrinkToFit();

            void Assign(const char16_t* str);
            void Assign(const StringU16& other);
            void Assign(const String& strUtf8);

            void Append(const char16_t* str);
            void Append(const StringU16& other);
            void Append(const String& utf8);

            uint SizeCodePoints() const;

            bool IsValidUtf16() const;

            StringU16 SubstrUnits(uint offset, uint count) const;

            int Compare(const StringU16& other) const;

            bool TryAssign(const String& utf8);
            bool TryToUtf8(String& out) const;

            const char16_t* AsWide() const;

            char16_t*       begin() { return Data(); }
            char16_t*       end() { return Data() + SizeUnits(); }
            const char16_t* begin() const { return Data(); }
            const char16_t* end() const { return Data() + SizeUnits(); }
            const char16_t* cbegin() const { return begin(); }
            const char16_t* cend() const { return end(); }

            String ToUtf8() const;

            bool Equals(const StringU16& other) const;

            StringU16& operator+=(const char16_t* str);
            StringU16& operator+=(const StringU16& other);

            bool operator==(const StringU16& other) const;
            bool operator!=(const StringU16& other) const;
        };
    } // namespace Stdlib
} // namespace krystallic
