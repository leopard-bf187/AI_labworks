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


namespace krystallic
{
    namespace Stdlib
    {
        class STDLIB_API String
        {
        public:
            static constexpr uint SSO_CAPACITY = 32;

        private:
            // SSO: payloads up to SSO_CAPACITY bytes (plus terminator) live in
            // m_Small; larger strings switch to heap storage in m_Data.
            union
            {
                char* m_Data;
                char  m_Small[SSO_CAPACITY + 1];
            };
            uint m_SizeBytes;
            uint m_SizeCodePoints;
            uint m_Capacity;
            bool m_IsSmall;

        private:
            void               InitSmallEmpty();
            char*              Ptr() { return m_IsSmall ? m_Small : m_Data; }
            const char*        Ptr() const { return m_IsSmall ? m_Small : m_Data; }
            void               Reallocate(uint newCapacity);
            void               EnsureCapacity(uint requiredCapacity);
            bool               IsPointerInside(const char* ptr) const;
            static inline uint GrowCapacity(uint requiredCapacity);

            // Raw paths for conversions: bytes must be valid UTF-8 with the
            // given code point count and must not alias this string's buffer.
            void AppendBytes(const char* bytes, uint byteCount, uint codePointCount);
            void AssignBytes(const char* bytes, uint byteCount, uint codePointCount);

            friend class StringU16;
            friend class StringU32;

        public:
            String();
            String(const char* str);
            String(const StringView& view);
            String(const String& other);
            String(String&& other) noexcept;
            ~String();

            String& operator=(const String& other);
            String& operator=(String&& other) noexcept;
            String& operator=(const char* str);

            const char* Data() const { return Ptr(); }
            char*       Data() { return Ptr(); }

            const char* CStr() const { return Ptr(); }

            uint SizeBytes() const { return m_SizeBytes; }
            uint SizeCodePoints() const { return m_SizeCodePoints; }
            uint Capacity() const { return m_Capacity; }

            bool Empty() const { return m_SizeBytes == 0; }

            char operator[](uint index) const { return Ptr()[index]; }

            StringView View() const { return StringView(Ptr(), m_SizeBytes); }

            void Clear();
            void Reserve(uint newCapacity);
            void ShrinkToFit();

            void Assign(const char* str);
            void Assign(const StringView& view);
            void Assign(const String& other);

            void Append(const char* str);
            void Append(const StringView& view);
            void Append(const String& other);

            bool Equals(const String& other) const;
            bool Equals(const StringView& view) const;

            int Compare(const StringView& other) const;

            bool StartsWith(const StringView& prefix) const;
            bool EndsWith(const StringView& suffix) const;

            bool Contains(const StringView& subStr) const;
            bool Contains(char ch) const;

            int FindChar(char ch, uint offset = 0) const;
            int FindLastChar(char ch) const;
            int Find(const StringView& subStr, uint offset = 0) const;

            void ToLowerAscii();
            void ToUpperAscii();

            void TrimLeftAscii();
            void TrimRightAscii();
            void TrimAscii();

            bool IsValidUtf8() const;

            void AppendCodePoint(uint32 codePoint);

            String SubstrBytes(uint offset, uint count) const;
            String SubstrCodePoints(uint start, uint count) const;

            uint32 AtCodePoint(uint index) const;

            void PopBackBytes(uint count);
            bool PopBackCodePoint();

            void ReserveExact(uint newCapacity);

            bool TryAssign(const char* str);
            bool TryAssign(const StringView& view);
            bool TryAssign(const String& other);

            char*       begin() { return Data(); }
            char*       end() { return Data() + SizeBytes(); }
            const char* begin() const { return Data(); }
            const char* end() const { return Data() + SizeBytes(); }
            const char* cbegin() const { return begin(); }
            const char* cend() const { return end(); }

            CodePointRange CodePoints() const { return CodePointRange(Data(), SizeBytes()); }

            String& operator+=(const char* str);
            String& operator+=(const StringView& view);
            String& operator+=(const String& other);

            bool operator==(const String& other) const;
            bool operator!=(const String& other) const;

            bool operator==(const StringView& view) const;
            bool operator!=(const StringView& view) const;

            // Exact-match overloads: without them a string literal is
            // ambiguous between the String and StringView conversions.
            bool operator==(const char* str) const;
            bool operator!=(const char* str) const;
        };
    } // namespace Stdlib
} // namespace krystallic
