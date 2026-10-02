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
        StringView::StringView() : m_Data(nullptr), m_SizeBytes(0)
        {
        }


        StringView::StringView(const char* str) : m_Data(str), m_SizeBytes(StrLen(str))
        {
        }


        StringView::StringView(const char* str, uint sizeBytes)
            : m_Data(str), m_SizeBytes((str && sizeBytes > 0) ? sizeBytes : 0)
        {
            if (!str)
                m_SizeBytes = 0;
        }


        uint StringView::SizeCodePoints() const
        {
            return StrLenU8(m_Data, m_SizeBytes);
        }


        bool StringView::Equals(const StringView& other) const
        {
            if (m_SizeBytes != other.m_SizeBytes)
                return false;

            if (m_SizeBytes == 0)
                return true;

            if (!m_Data || !other.m_Data)
                return false;

            return MemCmpN(m_Data, other.m_Data, m_SizeBytes) == 0;
        }


        bool StringView::StartsWith(const StringView& prefix) const
        {
            if (prefix.m_SizeBytes > m_SizeBytes)
                return false;

            if (prefix.m_SizeBytes == 0)
                return true;

            if (!m_Data || !prefix.m_Data)
                return false;

            return MemCmpN(m_Data, prefix.m_Data, prefix.m_SizeBytes) == 0;
        }


        bool StringView::EndsWith(const StringView& suffix) const
        {
            if (suffix.m_SizeBytes > m_SizeBytes)
                return false;

            if (suffix.m_SizeBytes == 0)
                return true;

            if (!m_Data || !suffix.m_Data)
                return false;

            const char* tail = m_Data + (m_SizeBytes - suffix.m_SizeBytes);
            return MemCmpN(tail, suffix.m_Data, suffix.m_SizeBytes) == 0;
        }


        int StringView::Compare(const StringView& other) const
        {
            const uint commonLen = m_SizeBytes < other.m_SizeBytes ? m_SizeBytes : other.m_SizeBytes;

            const int result = MemCmpN(m_Data, other.m_Data, commonLen);
            if (result != 0)
                return result;

            if (m_SizeBytes < other.m_SizeBytes)
                return -1;

            if (m_SizeBytes > other.m_SizeBytes)
                return 1;

            return 0;
        }


        bool StringView::Contains(const StringView& subStr) const
        {
            return Find(subStr) >= 0;
        }


        bool StringView::Contains(char ch) const
        {
            return FindChar(ch) >= 0;
        }


        int StringView::FindChar(char ch, uint offset) const
        {
            const char* found = FindCharN(m_Data, m_SizeBytes, ch, offset);
            return found ? (int) (found - m_Data) : -1;
        }


        int StringView::FindLastChar(char ch) const
        {
            const char* found = FindLastCharN(m_Data, m_SizeBytes, ch);
            return found ? (int) (found - m_Data) : -1;
        }


        int StringView::Find(const StringView& subStr, uint offset) const
        {
            if (!m_Data || !subStr.m_Data)
            {
                if (subStr.m_SizeBytes == 0 && offset <= m_SizeBytes)
                    return (int) offset;

                return -1;
            }

            const char* found = FindStrN(m_Data, m_SizeBytes, subStr.m_Data, subStr.m_SizeBytes, offset);
            return found ? (int) (found - m_Data) : -1;
        }


        StringView StringView::TrimLeftAscii() const
        {
            uint begin = 0;
            while (begin < m_SizeBytes && StrIsAsciiSpace(m_Data[begin]))
                ++begin;

            return StringView(m_Data + begin, m_SizeBytes - begin);
        }


        StringView StringView::TrimRightAscii() const
        {
            uint end = m_SizeBytes;
            while (end > 0 && StrIsAsciiSpace(m_Data[end - 1]))
                --end;

            return StringView(m_Data, end);
        }


        StringView StringView::TrimAscii() const
        {
            return TrimLeftAscii().TrimRightAscii();
        }


        bool StringView::IsValidUtf8() const
        {
            return StrIsValidU8(m_Data, m_SizeBytes);
        }


        StringView StringView::SubstrBytes(uint offset, uint count) const
        {
            if (!m_Data || offset >= m_SizeBytes)
                return StringView();

            uint remain = m_SizeBytes - offset;
            if (count > remain)
                count = remain;

            return StringView(m_Data + offset, count);
        }


        StringView StringView::SubstrCodePoints(uint start, uint count) const
        {
            uint begin = 0;
            for (uint i = 0; i < start; ++i)
            {
                if (!StrAdvanceOneCodePointU8(m_Data, m_SizeBytes, begin))
                    return StringView();
            }

            uint end = begin;
            for (uint i = 0; i < count; ++i)
            {
                if (!StrAdvanceOneCodePointU8(m_Data, m_SizeBytes, end))
                    break;
            }

            return StringView(m_Data + begin, end - begin);
        }


        bool StringView::operator==(const StringView& other) const
        {
            return Equals(other);
        }


        bool StringView::operator!=(const StringView& other) const
        {
            return !Equals(other);
        }
    } // namespace Stdlib
} // namespace krystallic
