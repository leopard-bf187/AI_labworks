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
        void String::InitSmallEmpty()
        {
            m_IsSmall = true;
            m_Small[0] = '\0';
            m_SizeBytes = 0;
            m_SizeCodePoints = 0;
            m_Capacity = SSO_CAPACITY;
        }

        void String::Reallocate(uint newCapacity)
        {
            if (newCapacity <= SSO_CAPACITY)
            {
                if (m_IsSmall)
                    return;

                char* oldData = m_Data;

                if (m_SizeBytes > 0)
                    memcpy(m_Small, oldData, m_SizeBytes);

                m_Small[m_SizeBytes] = '\0';

                delete[] oldData;
                m_IsSmall = true;
                m_Capacity = SSO_CAPACITY;
                return;
            }

            char* newData = new char[newCapacity + 1];

            if (m_SizeBytes > 0)
                memcpy(newData, Ptr(), m_SizeBytes);

            newData[m_SizeBytes] = '\0';

            if (!m_IsSmall)
                delete[] m_Data;

            m_Data = newData;
            m_IsSmall = false;
            m_Capacity = newCapacity;
        }

        void String::EnsureCapacity(uint requiredCapacity)
        {
            if (requiredCapacity <= m_Capacity)
                return;

            Reallocate(GrowCapacity(requiredCapacity));
        }

        bool String::IsPointerInside(const char* ptr) const
        {
            if (!ptr)
                return false;

            const char* data = Ptr();
            return ptr >= data && ptr < (data + m_SizeBytes + 1);
        }

        inline uint String::GrowCapacity(uint requiredCapacity)
        {
            uint newCapacity = (requiredCapacity < 16) ? 16 : requiredCapacity;
            newCapacity = newCapacity + (newCapacity / 2);
            return newCapacity;
        }

        String::String()
        {
            InitSmallEmpty();
        }

        String::String(const char* str)
        {
            InitSmallEmpty();
            Assign(str);
        }

        String::String(const StringView& view)
        {
            InitSmallEmpty();
            Assign(view);
        }

        String::String(const String& other)
        {
            if (other.m_SizeBytes <= SSO_CAPACITY)
            {
                m_IsSmall = true;
                m_Capacity = SSO_CAPACITY;
            }
            else
            {
                m_Data = new char[other.m_SizeBytes + 1];
                m_IsSmall = false;
                m_Capacity = other.m_SizeBytes;
            }

            char* data = Ptr();
            if (other.m_SizeBytes > 0)
                memcpy(data, other.Ptr(), other.m_SizeBytes);

            data[other.m_SizeBytes] = '\0';
            m_SizeBytes = other.m_SizeBytes;
            m_SizeCodePoints = other.m_SizeCodePoints;
        }

        String::String(String&& other) noexcept
        {
            if (other.m_IsSmall)
            {
                m_IsSmall = true;
                memcpy(m_Small, other.m_Small, other.m_SizeBytes + 1);
            }
            else
            {
                m_IsSmall = false;
                m_Data = other.m_Data;
            }

            m_SizeBytes = other.m_SizeBytes;
            m_SizeCodePoints = other.m_SizeCodePoints;
            m_Capacity = other.m_Capacity;

            other.InitSmallEmpty();
        }

        String::~String()
        {
            if (!m_IsSmall)
                delete[] m_Data;
        }

        String& String::operator=(const String& other)
        {
            if (this == &other)
                return *this;

            Assign(other);
            return *this;
        }

        String& String::operator=(String&& other) noexcept
        {
            if (this == &other)
                return *this;

            if (!m_IsSmall)
                delete[] m_Data;

            if (other.m_IsSmall)
            {
                m_IsSmall = true;
                memcpy(m_Small, other.m_Small, other.m_SizeBytes + 1);
            }
            else
            {
                m_IsSmall = false;
                m_Data = other.m_Data;
            }

            m_SizeBytes = other.m_SizeBytes;
            m_SizeCodePoints = other.m_SizeCodePoints;
            m_Capacity = other.m_Capacity;

            other.InitSmallEmpty();

            return *this;
        }

        String& String::operator=(const char* str)
        {
            Assign(str);
            return *this;
        }

        void String::Clear()
        {
            m_SizeBytes = 0;
            m_SizeCodePoints = 0;
            Ptr()[0] = '\0';
        }

        void String::Reserve(uint newCapacity)
        {
            if (newCapacity <= m_Capacity)
                return;

            Reallocate(newCapacity);
        }

        void String::ShrinkToFit()
        {
            if (m_IsSmall)
                return;

            if (m_SizeBytes <= SSO_CAPACITY)
            {
                Reallocate(m_SizeBytes);
                return;
            }

            if (m_Capacity == m_SizeBytes)
                return;

            Reallocate(m_SizeBytes);
        }

        void String::Assign(const char* str)
        {
            if (!str || str[0] == '\0')
            {
                Clear();
                return;
            }

            if (str == Ptr())
                return;

            if (IsPointerInside(str))
            {
                String temp(str);
                Assign(temp.View());
                return;
            }

            const uint sizeBytes = StrLen(str);
            const uint sizeCodePoints = StrLenU8(str);

            EnsureCapacity(sizeBytes);

            char* data = Ptr();
            memcpy(data, str, sizeBytes);
            data[sizeBytes] = '\0';

            m_SizeBytes = sizeBytes;
            m_SizeCodePoints = sizeCodePoints;
        }

        void String::Assign(const StringView& view)
        {
            if (view.Empty())
            {
                Clear();
                return;
            }

            if (view.Data() == Ptr() && view.SizeBytes() == m_SizeBytes)
                return;

            if (view.Data() && IsPointerInside(view.Data()))
            {
                String temp(view);
                Assign(temp.View());
                return;
            }

            EnsureCapacity(view.SizeBytes());

            char* data = Ptr();
            memcpy(data, view.Data(), view.SizeBytes());
            data[view.SizeBytes()] = '\0';

            m_SizeBytes = view.SizeBytes();
            m_SizeCodePoints = view.SizeCodePoints();
        }

        void String::Assign(const String& other)
        {
            if (this == &other)
                return;

            EnsureCapacity(other.m_SizeBytes);

            char* data = Ptr();
            if (other.m_SizeBytes > 0)
                memcpy(data, other.Ptr(), other.m_SizeBytes);

            data[other.m_SizeBytes] = '\0';

            m_SizeBytes = other.m_SizeBytes;
            m_SizeCodePoints = other.m_SizeCodePoints;
        }

        void String::Append(const char* str)
        {
            if (!str || str[0] == '\0')
                return;

            if (IsPointerInside(str))
            {
                String temp(str);
                Append(temp.View());
                return;
            }

            const uint appendSizeBytes = StrLen(str);
            const uint appendCodePoints = StrLenU8(str);
            const uint newSizeBytes = m_SizeBytes + appendSizeBytes;

            EnsureCapacity(newSizeBytes);

            char* data = Ptr();
            memcpy(data + m_SizeBytes, str, appendSizeBytes);
            m_SizeBytes = newSizeBytes;
            m_SizeCodePoints += appendCodePoints;
            data[m_SizeBytes] = '\0';
        }

        void String::Append(const StringView& view)
        {
            if (view.Empty())
                return;

            if (view.Data() && IsPointerInside(view.Data()))
            {
                String temp(view);
                Append(temp.View());
                return;
            }

            const uint newSizeBytes = m_SizeBytes + view.SizeBytes();

            EnsureCapacity(newSizeBytes);

            char* data = Ptr();
            memcpy(data + m_SizeBytes, view.Data(), view.SizeBytes());
            m_SizeBytes = newSizeBytes;
            m_SizeCodePoints += view.SizeCodePoints();
            data[m_SizeBytes] = '\0';
        }

        void String::Append(const String& other)
        {
            if (other.Empty())
                return;

            if (this == &other)
            {
                String temp(other);
                Append(temp.View());
                return;
            }

            Append(other.View());
        }

        bool String::Equals(const String& other) const
        {
            if (m_SizeBytes != other.m_SizeBytes)
                return false;

            if (m_SizeBytes == 0)
                return true;

            return MemCmpN(Ptr(), other.Ptr(), m_SizeBytes) == 0;
        }

        bool String::Equals(const StringView& view) const
        {
            if (m_SizeBytes != view.SizeBytes())
                return false;

            if (m_SizeBytes == 0)
                return true;

            return MemCmpN(Ptr(), view.Data(), m_SizeBytes) == 0;
        }

        int String::Compare(const StringView& other) const
        {
            return View().Compare(other);
        }

        bool String::Contains(const StringView& subStr) const
        {
            return View().Contains(subStr);
        }

        bool String::Contains(char ch) const
        {
            return View().Contains(ch);
        }

        bool String::StartsWith(const StringView& prefix) const
        {
            return View().StartsWith(prefix);
        }

        bool String::EndsWith(const StringView& suffix) const
        {
            return View().EndsWith(suffix);
        }

        int String::FindChar(char ch, uint offset) const
        {
            return View().FindChar(ch, offset);
        }

        int String::FindLastChar(char ch) const
        {
            return View().FindLastChar(ch);
        }

        int String::Find(const StringView& subStr, uint offset) const
        {
            return View().Find(subStr, offset);
        }

        void String::ToLowerAscii()
        {
            StrToLowerAscii(Ptr());
        }

        void String::ToUpperAscii()
        {
            StrToUpperAscii(Ptr());
        }

        void String::TrimLeftAscii()
        {
            char* data = Ptr();

            uint begin = 0;
            while (begin < m_SizeBytes && StrIsAsciiSpace(data[begin]))
                ++begin;

            if (begin == 0)
                return;

            const uint newSize = m_SizeBytes - begin;
            if (newSize > 0)
                memmove(data, data + begin, newSize);

            m_SizeBytes = newSize;
            m_SizeCodePoints -= begin;
            data[m_SizeBytes] = '\0';
        }

        void String::TrimRightAscii()
        {
            char* data = Ptr();

            uint end = m_SizeBytes;
            while (end > 0 && StrIsAsciiSpace(data[end - 1]))
                --end;

            if (end == m_SizeBytes)
                return;

            m_SizeCodePoints -= m_SizeBytes - end;
            m_SizeBytes = end;
            data[m_SizeBytes] = '\0';
        }

        void String::TrimAscii()
        {
            TrimRightAscii();
            TrimLeftAscii();
        }

        bool String::IsValidUtf8() const
        {
            return View().IsValidUtf8();
        }

        void String::AppendCodePoint(uint32 codePoint)
        {
            char bytes[4];
            uint byteLen = 0;
            if (!StrCodePointToUtf8(codePoint, bytes, byteLen))
                return;

            AppendBytes(bytes, byteLen, 1);
        }

        String String::SubstrBytes(uint offset, uint count) const
        {
            return String(View().SubstrBytes(offset, count));
        }

        String String::SubstrCodePoints(uint start, uint count) const
        {
            return String(View().SubstrCodePoints(start, count));
        }

        uint32 String::AtCodePoint(uint index) const
        {
            const char* data = Ptr();

            uint offset = 0;
            for (uint i = 0; i < index; ++i)
            {
                if (!StrAdvanceOneCodePointU8(data, m_SizeBytes, offset))
                    return 0;
            }

            uint32 codePoint = 0;
            if (!StrNextCodePointU8(data, m_SizeBytes, offset, codePoint))
                return 0;

            return codePoint;
        }

        void String::PopBackBytes(uint count)
        {
            if (count >= m_SizeBytes)
            {
                Clear();
                return;
            }

            char* data = Ptr();
            m_SizeBytes -= count;
            data[m_SizeBytes] = '\0';
            m_SizeCodePoints = StrLenU8(data, m_SizeBytes);
        }

        bool String::PopBackCodePoint()
        {
            char* data = Ptr();

            uint offset = m_SizeBytes;
            if (!StrRetreatOneCodePointU8(data, m_SizeBytes, offset))
                return false;

            m_SizeBytes = offset;
            m_SizeCodePoints -= 1;
            data[m_SizeBytes] = '\0';
            return true;
        }

        void String::ReserveExact(uint newCapacity)
        {
            if (newCapacity <= m_Capacity)
                return;

            Reallocate(newCapacity);
        }

        bool String::TryAssign(const char* str)
        {
            return TryAssign(StringView(str, StrLen(str)));
        }

        bool String::TryAssign(const StringView& view)
        {
            if (!StrIsValidU8(view.Data(), view.SizeBytes()))
                return false;

            Assign(view);
            return true;
        }

        bool String::TryAssign(const String& other)
        {
            return TryAssign(other.View());
        }

        void String::AppendBytes(const char* bytes, uint byteCount, uint codePointCount)
        {
            if (byteCount == 0)
                return;

            const uint newSizeBytes = m_SizeBytes + byteCount;
            EnsureCapacity(newSizeBytes);

            char* data = Ptr();
            memcpy(data + m_SizeBytes, bytes, byteCount);
            m_SizeBytes = newSizeBytes;
            m_SizeCodePoints += codePointCount;
            data[m_SizeBytes] = '\0';
        }

        void String::AssignBytes(const char* bytes, uint byteCount, uint codePointCount)
        {
            EnsureCapacity(byteCount);

            char* data = Ptr();
            if (byteCount > 0)
                memcpy(data, bytes, byteCount);

            m_SizeBytes = byteCount;
            m_SizeCodePoints = codePointCount;
            data[m_SizeBytes] = '\0';
        }

        String& String::operator+=(const char* str)
        {
            Append(str);
            return *this;
        }

        String& String::operator+=(const StringView& view)
        {
            Append(view);
            return *this;
        }

        String& String::operator+=(const String& other)
        {
            Append(other);
            return *this;
        }

        bool String::operator==(const String& other) const
        {
            return Equals(other);
        }

        bool String::operator!=(const String& other) const
        {
            return !Equals(other);
        }

        bool String::operator==(const StringView& view) const
        {
            return Equals(view);
        }

        bool String::operator!=(const StringView& view) const
        {
            return !Equals(view);
        }

        bool String::operator==(const char* str) const
        {
            return Equals(StringView(str, StrLen(str)));
        }

        bool String::operator!=(const char* str) const
        {
            return !Equals(StringView(str, StrLen(str)));
        }
    } // namespace Stdlib
} // namespace krystallic
