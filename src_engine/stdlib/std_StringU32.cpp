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

        StringU32::StringU32() : m_Data(nullptr), m_SizeCodePoints(0), m_Capacity(0)
        {
            AllocateEmpty();
        }

        StringU32::StringU32(const uint32* str) : m_Data(nullptr), m_SizeCodePoints(0), m_Capacity(0)
        {
            AllocateEmpty();
            Assign(str);
        }

        StringU32::StringU32(const String& strUtf8) : m_Data(nullptr), m_SizeCodePoints(0), m_Capacity(0)
        {
            AllocateEmpty();
            Assign(strUtf8);
        }

        StringU32::StringU32(const StringU16& strUtf16) : m_Data(nullptr), m_SizeCodePoints(0), m_Capacity(0)
        {
            AllocateEmpty();
            Assign(strUtf16);
        }

        StringU32::StringU32(const StringU32& other)
            : m_Data(nullptr), m_SizeCodePoints(other.m_SizeCodePoints), m_Capacity(other.m_SizeCodePoints)
        {
            m_Data = new uint32[m_Capacity + 1];

            if (m_SizeCodePoints > 0)
                memcpy(m_Data, other.m_Data, m_SizeCodePoints * sizeof(uint32));

            m_Data[m_SizeCodePoints] = 0;
        }

        StringU32::StringU32(StringU32&& other) noexcept
            : m_Data(other.m_Data), m_SizeCodePoints(other.m_SizeCodePoints), m_Capacity(other.m_Capacity)
        {
            other.m_Data = new uint32[1];
            other.m_Data[0] = 0;
            other.m_SizeCodePoints = 0;
            other.m_Capacity = 0;
        }

        StringU32::~StringU32()
        {
            delete[] m_Data;
        }

        StringU32& StringU32::operator=(const StringU32& other)
        {
            if (this == &other)
                return *this;

            Assign(other);
            return *this;
        }

        StringU32& StringU32::operator=(StringU32&& other) noexcept
        {
            if (this == &other)
                return *this;

            delete[] m_Data;

            m_Data = other.m_Data;
            m_SizeCodePoints = other.m_SizeCodePoints;
            m_Capacity = other.m_Capacity;

            other.m_Data = new uint32[1];
            other.m_Data[0] = 0;
            other.m_SizeCodePoints = 0;
            other.m_Capacity = 0;

            return *this;
        }

        StringU32& StringU32::operator=(const uint32* str)
        {
            Assign(str);
            return *this;
        }

        void StringU32::Clear()
        {
            m_SizeCodePoints = 0;
            m_Data[0] = 0;
        }

        void StringU32::Reserve(uint newCapacity)
        {
            if (newCapacity <= m_Capacity)
                return;

            Reallocate(newCapacity);
        }

        void StringU32::ShrinkToFit()
        {
            if (m_Capacity == m_SizeCodePoints)
                return;

            Reallocate(m_SizeCodePoints);
        }

        void StringU32::Assign(const uint32* str)
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
                StringU32 temp(str);
                Assign(temp);
                return;
            }

            uint sizeCodePoints = StrLen(str);

            EnsureCapacity(sizeCodePoints);

            memcpy(m_Data, str, sizeCodePoints * sizeof(uint32));
            m_Data[sizeCodePoints] = 0;
            m_SizeCodePoints = sizeCodePoints;
        }

        void StringU32::Assign(const String& strUtf8)
        {
            Clear();

            if (strUtf8.Empty())
                return;

            uint        byteOffset = 0;
            const uint  byteLen = strUtf8.SizeBytes();
            const char* utf8 = strUtf8.Data();

            while (byteOffset < byteLen)
            {
                uint32 cp = 0;
                if (!StrNextCodePointU8(utf8, byteLen, byteOffset, cp))
                {
                    Clear();
                    return;
                }

                Append(cp);
            }
        }

        void StringU32::Assign(const StringU16& strUtf16)
        {
            Clear();

            if (strUtf16.Empty())
                return;

            uint            unitOffset = 0;
            const uint      unitLen = strUtf16.SizeUnits();
            const char16_t* utf16 = strUtf16.Data();

            while (unitOffset < unitLen)
            {
                uint32 cp = 0;
                if (!StrNextCodePointU16(utf16, unitLen, unitOffset, cp))
                {
                    Clear();
                    return;
                }

                Append(cp);
            }
        }

        void StringU32::Assign(const StringU32& other)
        {
            if (this == &other)
                return;

            EnsureCapacity(other.m_SizeCodePoints);

            if (other.m_SizeCodePoints > 0)
                memcpy(m_Data, other.m_Data, other.m_SizeCodePoints * sizeof(uint32));

            m_Data[other.m_SizeCodePoints] = 0;
            m_SizeCodePoints = other.m_SizeCodePoints;
        }

        void StringU32::Append(uint32 codePoint)
        {
            if (codePoint > 0x10FFFF)
                return;

            if (codePoint >= 0xD800 && codePoint <= 0xDFFF)
                return;

            EnsureCapacity(m_SizeCodePoints + 1);

            m_Data[m_SizeCodePoints] = codePoint;
            ++m_SizeCodePoints;
            m_Data[m_SizeCodePoints] = 0;
        }

        void StringU32::Append(const uint32* str)
        {
            if (!str || str[0] == 0)
                return;

            if (IsPointerInside(str))
            {
                StringU32 temp(str);
                Append(temp);
                return;
            }

            uint appendCount = StrLen(str);
            uint newSize = m_SizeCodePoints + appendCount;

            EnsureCapacity(newSize);

            memcpy(m_Data + m_SizeCodePoints, str, appendCount * sizeof(uint32));
            m_SizeCodePoints = newSize;
            m_Data[m_SizeCodePoints] = 0;
        }

        void StringU32::Append(const StringU32& other)
        {
            if (other.Empty())
                return;

            if (this == &other)
            {
                StringU32 temp(other);
                Append(temp);
                return;
            }

            uint newSize = m_SizeCodePoints + other.m_SizeCodePoints;

            EnsureCapacity(newSize);

            memcpy(m_Data + m_SizeCodePoints, other.m_Data, other.m_SizeCodePoints * sizeof(uint32));
            m_SizeCodePoints = newSize;
            m_Data[m_SizeCodePoints] = 0;
        }

        void StringU32::Append(const String& utf8)
        {
            if (utf8.Empty())
                return;

            const char* bytes = utf8.Data();
            const uint  byteLen = utf8.SizeBytes();

            // Pass 1: validate and count code points so the append either
            // happens fully or not at all.
            uint byteOffset = 0;
            uint cpCount = 0;
            while (byteOffset < byteLen)
            {
                uint32 cp = 0;
                if (!StrNextCodePointU8(bytes, byteLen, byteOffset, cp))
                    return;

                ++cpCount;
            }

            EnsureCapacity(m_SizeCodePoints + cpCount);

            byteOffset = 0;
            while (byteOffset < byteLen)
            {
                uint32 cp = 0;
                StrNextCodePointU8(bytes, byteLen, byteOffset, cp);
                m_Data[m_SizeCodePoints++] = cp;
            }

            m_Data[m_SizeCodePoints] = 0;
        }

        void StringU32::Append(const StringU16& utf16)
        {
            if (utf16.Empty())
                return;

            const char16_t* units = utf16.Data();
            const uint      unitLen = utf16.SizeUnits();

            uint unitOffset = 0;
            uint cpCount = 0;
            while (unitOffset < unitLen)
            {
                uint32 cp = 0;
                if (!StrNextCodePointU16(units, unitLen, unitOffset, cp))
                    return;

                ++cpCount;
            }

            EnsureCapacity(m_SizeCodePoints + cpCount);

            unitOffset = 0;
            while (unitOffset < unitLen)
            {
                uint32 cp = 0;
                StrNextCodePointU16(units, unitLen, unitOffset, cp);
                m_Data[m_SizeCodePoints++] = cp;
            }

            m_Data[m_SizeCodePoints] = 0;
        }

        int StringU32::Find(uint32 codePoint, uint offset) const
        {
            for (uint i = offset; i < m_SizeCodePoints; ++i)
            {
                if (m_Data[i] == codePoint)
                    return (int) i;
            }

            return -1;
        }

        bool StringU32::Contains(uint32 codePoint) const
        {
            return Find(codePoint) >= 0;
        }

        StringU32 StringU32::Substr(uint offset, uint count) const
        {
            StringU32 result;

            if (offset >= m_SizeCodePoints)
                return result;

            const uint remain = m_SizeCodePoints - offset;
            if (count > remain)
                count = remain;

            result.AppendCodePoints(m_Data + offset, count);
            return result;
        }

        void StringU32::RemoveAt(uint index)
        {
            if (index >= m_SizeCodePoints)
                return;

            const uint tail = m_SizeCodePoints - index - 1;
            if (tail > 0)
                memmove(m_Data + index, m_Data + index + 1, tail * sizeof(uint32));

            --m_SizeCodePoints;
            m_Data[m_SizeCodePoints] = 0;
        }

        void StringU32::Insert(uint index, uint32 codePoint)
        {
            if (index > m_SizeCodePoints)
                return;

            if (!IsValidUnicodeScalarValue(codePoint))
                return;

            EnsureCapacity(m_SizeCodePoints + 1);

            const uint tail = m_SizeCodePoints - index;
            if (tail > 0)
                memmove(m_Data + index + 1, m_Data + index, tail * sizeof(uint32));

            m_Data[index] = codePoint;
            ++m_SizeCodePoints;
            m_Data[m_SizeCodePoints] = 0;
        }

        bool StringU32::IsValid() const
        {
            for (uint i = 0; i < m_SizeCodePoints; ++i)
            {
                if (!IsValidUnicodeScalarValue(m_Data[i]))
                    return false;
            }

            return true;
        }

        int StringU32::Compare(const StringU32& other) const
        {
            const uint commonLen =
                m_SizeCodePoints < other.m_SizeCodePoints ? m_SizeCodePoints : other.m_SizeCodePoints;

            for (uint i = 0; i < commonLen; ++i)
            {
                if (m_Data[i] != other.m_Data[i])
                    return m_Data[i] < other.m_Data[i] ? -1 : 1;
            }

            if (m_SizeCodePoints < other.m_SizeCodePoints)
                return -1;

            if (m_SizeCodePoints > other.m_SizeCodePoints)
                return 1;

            return 0;
        }

        bool StringU32::TryToUtf8(String& out) const
        {
            // Pass 1: validate and count target bytes so out is never touched
            // on failure.
            uint totalBytes = 0;
            for (uint i = 0; i < m_SizeCodePoints; ++i)
            {
                char utf8Bytes[4];
                uint utf8Len = 0;
                if (!StrCodePointToUtf8(m_Data[i], utf8Bytes, utf8Len))
                    return false;

                totalBytes += utf8Len;
            }

            // Raw path: write straight into out's buffer (friend access).
            out.Clear();
            out.EnsureCapacity(totalBytes);

            char* dst = out.Ptr();
            uint  written = 0;

            for (uint i = 0; i < m_SizeCodePoints; ++i)
            {
                uint utf8Len = 0;
                StrCodePointToUtf8(m_Data[i], dst + written, utf8Len);
                written += utf8Len;
            }

            dst[written] = '\0';
            out.m_SizeBytes = written;
            out.m_SizeCodePoints = m_SizeCodePoints;

            return true;
        }

        bool StringU32::TryToUtf16(StringU16& out) const
        {
            uint totalUnits = 0;
            for (uint i = 0; i < m_SizeCodePoints; ++i)
            {
                char16_t units[2];
                uint     unitCount = 0;
                if (!StrCodePointToUtf16(m_Data[i], units, unitCount))
                    return false;

                totalUnits += unitCount;
            }

            // Raw path: write straight into out's buffer (friend access).
            out.Clear();
            out.EnsureCapacity(totalUnits);

            char16_t* dst = out.m_Data;
            uint      written = 0;

            for (uint i = 0; i < m_SizeCodePoints; ++i)
            {
                uint unitCount = 0;
                StrCodePointToUtf16(m_Data[i], dst + written, unitCount);
                written += unitCount;
            }

            dst[written] = 0;
            out.m_SizeUnits = written;

            return true;
        }

        String StringU32::ToUtf8() const
        {
            String result;
            TryToUtf8(result);
            return result;
        }

        StringU16 StringU32::ToUtf16() const
        {
            StringU16 result;
            TryToUtf16(result);
            return result;
        }

        void StringU32::AppendCodePoints(const uint32* data, uint count)
        {
            if (count == 0)
                return;

            const uint newSize = m_SizeCodePoints + count;
            EnsureCapacity(newSize);

            memcpy(m_Data + m_SizeCodePoints, data, count * sizeof(uint32));
            m_SizeCodePoints = newSize;
            m_Data[m_SizeCodePoints] = 0;
        }

        bool StringU32::Equals(const StringU32& other) const
        {
            if (m_SizeCodePoints != other.m_SizeCodePoints)
                return false;

            if (m_SizeCodePoints == 0)
                return true;

            return memcmp(m_Data, other.m_Data, m_SizeCodePoints * sizeof(uint32)) == 0;
        }

        StringU32& StringU32::operator+=(uint32 codePoint)
        {
            Append(codePoint);
            return *this;
        }

        StringU32& StringU32::operator+=(const uint32* str)
        {
            Append(str);
            return *this;
        }

        StringU32& StringU32::operator+=(const StringU32& other)
        {
            Append(other);
            return *this;
        }

        bool StringU32::operator==(const StringU32& other) const
        {
            return Equals(other);
        }

        bool StringU32::operator!=(const StringU32& other) const
        {
            return !Equals(other);
        }

        void StringU32::AllocateEmpty()
        {
            m_Data = new uint32[1];
            m_Data[0] = 0;
            m_SizeCodePoints = 0;
            m_Capacity = 0;
        }

        void StringU32::Reallocate(uint newCapacity)
        {
            uint32* newData = new uint32[newCapacity + 1];

            if (m_SizeCodePoints > 0)
                memcpy(newData, m_Data, m_SizeCodePoints * sizeof(uint32));

            newData[m_SizeCodePoints] = 0;

            delete[] m_Data;
            m_Data = newData;
            m_Capacity = newCapacity;
        }

        void StringU32::EnsureCapacity(uint requiredCapacity)
        {
            if (requiredCapacity <= m_Capacity)
                return;

            Reallocate(GrowCapacity(requiredCapacity));
        }

        bool StringU32::IsPointerInside(const uint32* ptr) const
        {
            if (!ptr || !m_Data)
                return false;

            return ptr >= m_Data && ptr < (m_Data + m_SizeCodePoints + 1);
        }

        uint StringU32::GrowCapacity(uint requiredCapacity)
        {
            uint newCapacity = (requiredCapacity < 16) ? 16 : requiredCapacity;
            newCapacity = newCapacity + (newCapacity / 2);
            return newCapacity;
        }
    } // namespace Stdlib
} // namespace krystallic
