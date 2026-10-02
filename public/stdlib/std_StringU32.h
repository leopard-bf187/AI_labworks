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
#include "std_StringU16.h"


namespace krystallic
{
    namespace Stdlib
    {
        class STDLIB_API StringU32
        {
        private:
            uint32* m_Data;
            uint    m_SizeCodePoints;
            uint    m_Capacity;

        private:
            void        AllocateEmpty();
            void        Reallocate(uint newCapacity);
            void        EnsureCapacity(uint requiredCapacity);
            bool        IsPointerInside(const uint32* ptr) const;
            static uint GrowCapacity(uint requiredCapacity);

            // Raw code point path for conversions: data must not alias this
            // string's buffer.
            void AppendCodePoints(const uint32* data, uint count);

        public:
            StringU32();
            StringU32(const uint32* str);
            StringU32(const String& strUtf8);
            StringU32(const StringU16& strUtf16);
            StringU32(const StringU32& other);
            StringU32(StringU32&& other) noexcept;
            ~StringU32();

            StringU32& operator=(const StringU32& other);
            StringU32& operator=(StringU32&& other) noexcept;
            StringU32& operator=(const uint32* str);

            const uint32* Data() const { return m_Data; }
            uint32*       Data() { return m_Data; }

            const uint32* CStr() const { return m_Data; }

            uint SizeCodePoints() const { return m_SizeCodePoints; }
            uint Capacity() const { return m_Capacity; }

            bool Empty() const { return m_SizeCodePoints == 0; }

            uint32 operator[](uint index) const { return m_Data[index]; }

            void Clear();
            void Reserve(uint newCapacity);
            void ShrinkToFit();

            void Assign(const uint32* str);
            void Assign(const String& strUtf8);
            void Assign(const StringU16& strUtf16);
            void Assign(const StringU32& other);

            void Append(uint32 codePoint);
            void Append(const uint32* str);
            void Append(const StringU32& other);
            void Append(const String& utf8);
            void Append(const StringU16& utf16);

            int Find(uint32 codePoint, uint offset = 0) const;

            bool Contains(uint32 codePoint) const;

            StringU32 Substr(uint offset, uint count) const;

            void RemoveAt(uint index);

            void Insert(uint index, uint32 codePoint);

            bool IsValid() const;

            int Compare(const StringU32& other) const;

            bool TryToUtf8(String& out) const;
            bool TryToUtf16(StringU16& out) const;

            uint32*       begin() { return Data(); }
            uint32*       end() { return Data() + SizeCodePoints(); }
            const uint32* begin() const { return Data(); }
            const uint32* end() const { return Data() + SizeCodePoints(); }
            const uint32* cbegin() const { return begin(); }
            const uint32* cend() const { return end(); }

            String    ToUtf8() const;
            StringU16 ToUtf16() const;

            bool Equals(const StringU32& other) const;

            StringU32& operator+=(uint32 codePoint);
            StringU32& operator+=(const uint32* str);
            StringU32& operator+=(const StringU32& other);

            bool operator==(const StringU32& other) const;
            bool operator!=(const StringU32& other) const;
        };
    } // namespace Stdlib
} // namespace krystallic
