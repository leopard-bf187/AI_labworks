/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD & @Ra192192
*
*  Description:
*
*  Date:
*/


#define STDLIB_API_EXPORT
#include "stdlib_dll.h"



namespace krystallic
{
    namespace Stdlib
    {

        void StringU16::AllocateEmpty()
        {
            m_Data = new char16_t[1];
            m_Data[0] = 0;
            m_SizeUnits = 0;
            m_Capacity = 0;
        }

        void StringU16::Reallocate(uint newCapacity)
        {
            char16_t* newData = new char16_t[newCapacity + 1];

            if (m_SizeUnits > 0)
                memcpy(newData, m_Data, m_SizeUnits * sizeof(char16_t));

            newData[m_SizeUnits] = 0;

            delete[] m_Data;
            m_Data = newData;
            m_Capacity = newCapacity;
        }

        void StringU16::EnsureCapacity(uint requiredCapacity)
        {
            if (requiredCapacity <= m_Capacity)
                return;

            Reallocate(GrowCapacity(requiredCapacity));
        }

        bool StringU16::IsPointerInside(const char16_t* ptr) const
        {
            if (!ptr || !m_Data)
                return false;

            return ptr >= m_Data && ptr < (m_Data + m_SizeUnits + 1);
        }

        uint StringU16::GrowCapacity(uint requiredCapacity)
        {
            uint newCapacity = (requiredCapacity < 16) ? 16 : requiredCapacity;
            newCapacity = newCapacity + (newCapacity / 2);
            return newCapacity;
        }

        StringU16::StringU16() : m_Data(nullptr), m_SizeUnits(0), m_Capacity(0)
        {
            AllocateEmpty();
        }

        StringU16::StringU16(const char16_t* str) : m_Data(nullptr), m_SizeUnits(0), m_Capacity(0)
        {
            AllocateEmpty();
            Assign(str);
        }

        StringU16::StringU16(const String& strUtf8) : m_Data(nullptr), m_SizeUnits(0), m_Capacity(0)
        {
            AllocateEmpty();
            Assign(strUtf8);
        }

        StringU16::StringU16(const StringU16& other)
            : m_Data(nullptr), m_SizeUnits(other.m_SizeUnits), m_Capacity(other.m_SizeUnits)
        {
            m_Data = new char16_t[m_Capacity + 1];

            if (m_SizeUnits > 0)
                memcpy(m_Data, other.m_Data, m_SizeUnits * sizeof(char16_t));

            m_Data[m_SizeUnits] = 0;
        }

        StringU16::StringU16(StringU16&& other) noexcept
            : m_Data(other.m_Data), m_SizeUnits(other.m_SizeUnits), m_Capacity(other.m_Capacity)
        {
            other.m_Data = new char16_t[1];
            other.m_Data[0] = 0;
            other.m_SizeUnits = 0;
            other.m_Capacity = 0;
        }

        StringU16::~StringU16()
        {
            delete[] m_Data;
        }

        StringU16& StringU16::operator=(const StringU16& other)
        {
            if (this == &other)
                return *this;

            Assign(other);
            return *this;
        }

        StringU16& StringU16::operator=(StringU16&& other) noexcept
        {
            if (this == &other)
                return *this;

            delete[] m_Data;

            m_Data = other.m_Data;
            m_SizeUnits = other.m_SizeUnits;
            m_Capacity = other.m_Capacity;

            other.m_Data = new char16_t[1];
            other.m_Data[0] = 0;
            other.m_SizeUnits = 0;
            other.m_Capacity = 0;

            return *this;
        }

        StringU16& StringU16::operator=(const char16_t* str)
        {
            Assign(str);
            return *this;
        }

        void StringU16::Clear()
        {
            m_SizeUnits = 0;
            m_Data[0] = 0;
        }

        void StringU16::Reserve(uint newCapacity)
        {
            if (newCapacity <= m_Capacity)
                return;

            Reallocate(newCapacity);
        }

        void StringU16::ShrinkToFit()
        {
            if (m_Capacity == m_SizeUnits)
                return;

            Reallocate(m_SizeUnits);
        }

        void StringU16::Assign(const char16_t* str)
        {
            if (!str || str[0] == 0)
            {
                Clear();
                return;
            }

            if (str == m_Data)
                return;

            if (IsPointerInside(str))
            {
                StringU16 temp(str);
                Assign(temp);
                return;
            }

            uint sizeUnits = StrLen(str);

            EnsureCapacity(sizeUnits);

            memcpy(m_Data, str, sizeUnits * sizeof(char16_t));
            m_Data[sizeUnits] = 0;
            m_SizeUnits = sizeUnits;
        }

        void StringU16::Assign(const StringU16& other)
        {
            if (this == &other)
                return;

            EnsureCapacity(other.m_SizeUnits);

            if (other.m_SizeUnits > 0)
                memcpy(m_Data, other.m_Data, other.m_SizeUnits * sizeof(char16_t));

            m_Data[other.m_SizeUnits] = 0;
            m_SizeUnits = other.m_SizeUnits;
        }

        void StringU16::Assign(const String& strUtf8)
        {
            Clear();
            Append(strUtf8);
        }

        void StringU16::Append(const char16_t* str)
        {
            if (!str || str[0] == 0)
                return;

            if (IsPointerInside(str))
            {
                StringU16 temp(str);
                Append(temp);
                return;
            }

            uint appendUnits = StrLen(str);
            uint newSizeUnits = m_SizeUnits + appendUnits;

            EnsureCapacity(newSizeUnits);

            memcpy(m_Data + m_SizeUnits, str, appendUnits * sizeof(char16_t));
            m_SizeUnits = newSizeUnits;
            m_Data[m_SizeUnits] = 0;
        }

        void StringU16::Append(const StringU16& other)
        {
            if (other.Empty())
                return;

            if (this == &other)
            {
                StringU16 temp(other);
                Append(temp);
                return;
            }

            uint newSizeUnits = m_SizeUnits + other.m_SizeUnits;

            EnsureCapacity(newSizeUnits);

            memcpy(m_Data + m_SizeUnits, other.m_Data, other.m_SizeUnits * sizeof(char16_t));
            m_SizeUnits = newSizeUnits;
            m_Data[m_SizeUnits] = 0;
        }

        void StringU16::Append(const String& utf8)
        {
            if (utf8.Empty())
                return;

            const char* bytes = utf8.Data();
            const uint  byteLen = utf8.SizeBytes();

            // Pass 1: validate and count required UTF-16 units so the append
            // either happens fully or not at all.
            uint byteOffset = 0;
            uint unitCount = 0;
            while (byteOffset < byteLen)
            {
                uint32 cp = 0;
                if (!StrNextCodePointU8(bytes, byteLen, byteOffset, cp))
                    return;

                unitCount += (cp <= 0xFFFF) ? 1 : 2;
            }

            EnsureCapacity(m_SizeUnits + unitCount);

            byteOffset = 0;
            while (byteOffset < byteLen)
            {
                uint32 cp = 0;
                StrNextCodePointU8(bytes, byteLen, byteOffset, cp);

                char16_t units[2];
                uint     count = 0;
                StrCodePointToUtf16(cp, units, count);

                m_Data[m_SizeUnits] = units[0];
                if (count == 2)
                    m_Data[m_SizeUnits + 1] = units[1];

                m_SizeUnits += count;
            }

            m_Data[m_SizeUnits] = 0;
        }

        uint StringU16::SizeCodePoints() const
        {
            return StrCountCodePointsU16(m_Data, m_SizeUnits);
        }

        bool StringU16::IsValidUtf16() const
        {
            return StrIsValidU16(m_Data, m_SizeUnits);
        }

        StringU16 StringU16::SubstrUnits(uint offset, uint count) const
        {
            StringU16 result;

            if (offset >= m_SizeUnits)
                return result;

            const uint remain = m_SizeUnits - offset;
            if (count > remain)
                count = remain;

            result.AssignUnits(m_Data + offset, count);
            return result;
        }

        int StringU16::Compare(const StringU16& other) const
        {
            const uint commonLen = m_SizeUnits < other.m_SizeUnits ? m_SizeUnits : other.m_SizeUnits;

            for (uint i = 0; i < commonLen; ++i)
            {
                const uint32 a = (uint32) m_Data[i];
                const uint32 b = (uint32) other.m_Data[i];

                if (a != b)
                    return a < b ? -1 : 1;
            }

            if (m_SizeUnits < other.m_SizeUnits)
                return -1;

            if (m_SizeUnits > other.m_SizeUnits)
                return 1;

            return 0;
        }

        bool StringU16::TryAssign(const String& utf8)
        {
            if (!StrIsValidU8(utf8.Data(), utf8.SizeBytes()))
                return false;

            Clear();
            Append(utf8);
            return true;
        }

        bool StringU16::TryToUtf8(String& out) const
        {
            // Pass 1: validate and count target bytes so out is never touched
            // on failure.
            uint unitOffset = 0;
            uint totalBytes = 0;
            while (unitOffset < m_SizeUnits)
            {
                uint32 cp = 0;
                if (!StrNextCodePointU16(m_Data, m_SizeUnits, unitOffset, cp))
                    return false;

                char utf8Bytes[4];
                uint utf8Len = 0;
                if (!StrCodePointToUtf8(cp, utf8Bytes, utf8Len))
                    return false;

                totalBytes += utf8Len;
            }

            // Raw path: write straight into out's buffer (friend access).
            out.Clear();
            out.EnsureCapacity(totalBytes);

            char* dst = out.Ptr();
            uint  written = 0;
            uint  codePoints = 0;

            unitOffset = 0;
            while (unitOffset < m_SizeUnits)
            {
                uint32 cp = 0;
                StrNextCodePointU16(m_Data, m_SizeUnits, unitOffset, cp);

                uint utf8Len = 0;
                StrCodePointToUtf8(cp, dst + written, utf8Len);

                written += utf8Len;
                ++codePoints;
            }

            dst[written] = '\0';
            out.m_SizeBytes = written;
            out.m_SizeCodePoints = codePoints;

            return true;
        }

        const char16_t* StringU16::AsWide() const
        {
            return m_Data;
        }

        String StringU16::ToUtf8() const
        {
            String result;
            TryToUtf8(result);
            return result;
        }

        void StringU16::AppendUnits(const char16_t* units, uint count)
        {
            if (count == 0)
                return;

            const uint newSizeUnits = m_SizeUnits + count;
            EnsureCapacity(newSizeUnits);

            memcpy(m_Data + m_SizeUnits, units, count * sizeof(char16_t));
            m_SizeUnits = newSizeUnits;
            m_Data[m_SizeUnits] = 0;
        }

        void StringU16::AssignUnits(const char16_t* units, uint count)
        {
            EnsureCapacity(count);

            if (count > 0)
                memcpy(m_Data, units, count * sizeof(char16_t));

            m_SizeUnits = count;
            m_Data[m_SizeUnits] = 0;
        }

        bool StringU16::Equals(const StringU16& other) const
        {
            if (m_SizeUnits != other.m_SizeUnits)
                return false;

            if (m_SizeUnits == 0)
                return true;

            return memcmp(m_Data, other.m_Data, m_SizeUnits * sizeof(char16_t)) == 0;
        }

        StringU16& StringU16::operator+=(const char16_t* str)
        {
            Append(str);
            return *this;
        }

        StringU16& StringU16::operator+=(const StringU16& other)
        {
            Append(other);
            return *this;
        }

        bool StringU16::operator==(const StringU16& other) const
        {
            return Equals(other);
        }

        bool StringU16::operator!=(const StringU16& other) const
        {
            return !Equals(other);
        }
    } // namespace Stdlib
} // namespace krystallic
