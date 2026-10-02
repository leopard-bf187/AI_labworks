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


namespace krystallic
{
    namespace Stdlib
    {
        // Forward iterator over Unicode code points of a UTF-8 byte range.
        // Invalid UTF-8 asserts in debug and ends iteration in release.
        class CodePointIterator
        {
        private:
            const char* m_Data;
            uint        m_SizeBytes;
            uint        m_Offset;

        public:
            CodePointIterator(const char* data, uint sizeBytes, uint offset)
                : m_Data(data), m_SizeBytes(sizeBytes), m_Offset(offset)
            {
            }

            uint32 operator*() const
            {
                uint   offset = m_Offset;
                uint32 codePoint = 0;
                if (!StrNextCodePointU8(m_Data, m_SizeBytes, offset, codePoint))
                {
                    assert(false && "invalid UTF-8 sequence in CodePointIterator");
                    return 0;
                }
                return codePoint;
            }

            CodePointIterator& operator++()
            {
                if (!StrAdvanceOneCodePointU8(m_Data, m_SizeBytes, m_Offset))
                {
                    assert(m_Offset >= m_SizeBytes && "invalid UTF-8 sequence in CodePointIterator");
                    m_Offset = m_SizeBytes;
                }
                return *this;
            }

            bool operator==(const CodePointIterator& other) const { return m_Offset == other.m_Offset; }
            bool operator!=(const CodePointIterator& other) const { return m_Offset != other.m_Offset; }
        };

        class CodePointRange
        {
        private:
            const char* m_Data;
            uint        m_SizeBytes;

        public:
            CodePointRange(const char* data, uint sizeBytes) : m_Data(data), m_SizeBytes(sizeBytes) {}

            CodePointIterator begin() const { return CodePointIterator(m_Data, m_SizeBytes, 0); }
            CodePointIterator end() const { return CodePointIterator(m_Data, m_SizeBytes, m_SizeBytes); }
        };

        class STDLIB_API StringView
        {
        private:
            const char* m_Data;
            uint        m_SizeBytes;

        public:
            StringView();
            StringView(const char* str);
            StringView(const char* str, uint sizeBytes);

            const char* Data() const { return m_Data; }
            uint        SizeBytes() const { return m_SizeBytes; }
            uint        SizeCodePoints() const;

            bool Empty() const { return m_SizeBytes == 0; }

            char operator[](uint index) const { return m_Data[index]; }

            void Clear()
            {
                m_Data = nullptr;
                m_SizeBytes = 0;
            }

            bool Equals(const StringView& other) const;
            bool StartsWith(const StringView& prefix) const;
            bool EndsWith(const StringView& suffix) const;

            int Compare(const StringView& other) const;

            bool Contains(const StringView& subStr) const;
            bool Contains(char ch) const;

            int FindChar(char ch, uint offset = 0) const;
            int FindLastChar(char ch) const;
            int Find(const StringView& subStr, uint offset = 0) const;

            StringView TrimLeftAscii() const;
            StringView TrimRightAscii() const;
            StringView TrimAscii() const;

            bool IsValidUtf8() const;

            StringView SubstrBytes(uint offset, uint count) const;
            StringView SubstrCodePoints(uint start, uint count) const;

            const char* begin() const { return Data(); }
            const char* end() const { return Data() + SizeBytes(); }
            const char* cbegin() const { return begin(); }
            const char* cend() const { return end(); }

            CodePointRange CodePoints() const { return CodePointRange(Data(), SizeBytes()); }

            bool operator==(const StringView& other) const;
            bool operator!=(const StringView& other) const;
        };
    } // namespace Stdlib
} // namespace krystallic
