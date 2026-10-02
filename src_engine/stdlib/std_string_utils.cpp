/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD & @Ra192192
*
*  Description: Updated C-style string functions
*
*  Date: 15.04.2026
*/


#define STDLIB_API_EXPORT
#include "stdlib_dll.h"



namespace krystallic
{
    namespace Stdlib
    {
        uint StrLen(const char* str)
        {
            if (!str)
                return 0;

            uint len = 0;
            while (str[len] != '\0')
                ++len;
            return len;
        }

        uint StrLen(const char16_t* str)
        {
            if (!str)
                return 0;

            uint len = 0;
            while (str[len] != 0)
                ++len;
            return len;
        }

        uint StrLen(const char32_t* str)
        {
            if (!str)
                return 0;

            uint len = 0;
            while (str[len] != 0)
                ++len;
            return len;
        }

        uint StrLen(const uint32* str)
        {
            if (!str)
                return 0;

            uint len = 0;
            while (str[len] != 0)
                ++len;
            return len;
        }


        char* StrCopy(char* dst, const char* src)
        {
            if (!dst || !src)
                return dst;

            char* out = dst;
            while ((*dst++ = *src++) != '\0')
            {
            }
            return out;
        }

        char* StrCopyN(char* dst, const char* src, uint count)
        {
            if (!dst || !src)
                return dst;

            uint i = 0;
            for (; i < count && src[i] != '\0'; ++i)
                dst[i] = src[i];

            for (; i < count; ++i)
                dst[i] = '\0';

            return dst;
        }

        char* StrCopySafe(char* dst, uint dstSize, const char* src)
        {
            if (!dst || dstSize == 0)
                return dst;

            if (!src)
            {
                dst[0] = '\0';
                return dst;
            }

            uint i = 0;
            for (; i + 1 < dstSize && src[i] != '\0'; ++i)
                dst[i] = src[i];

            dst[i] = '\0';
            return dst;
        }



        char* StrCat(char* dst, const char* src)
        {
            if (!dst || !src)
                return dst;

            char* out = dst;
            while (*dst)
                ++dst;

            while ((*dst++ = *src++) != '\0')
            {
            }

            return out;
        }

        char* StrCatSafe(char* dst, uint dstSize, const char* src)
        {
            if (!dst || dstSize == 0)
                return dst;

            if (!src)
                return dst;

            uint dstLen = StrLen(dst);
            if (dstLen >= dstSize)
            {
                dst[dstSize - 1] = '\0';
                return dst;
            }

            uint i = 0;
            while ((dstLen + i + 1) < dstSize && src[i] != '\0')
            {
                dst[dstLen + i] = src[i];
                ++i;
            }

            dst[dstLen + i] = '\0';
            return dst;
        }



        int StrCmp(const char* a, const char* b)
        {
            if (a == b)
                return 0;

            if (!a)
                return -1;

            if (!b)
                return 1;

            while (*a && (*a == *b))
            {
                ++a;
                ++b;
            }

            return (unsigned char) (*a) - (unsigned char) (*b);
        }

        int StrNCmp(const char* a, const char* b, uint count)
        {
            if (count == 0)
                return 0;

            if (a == b)
                return 0;

            if (!a)
                return -1;

            if (!b)
                return 1;

            for (uint i = 0; i < count; ++i)
            {
                unsigned char ca = (unsigned char) a[i];
                unsigned char cb = (unsigned char) b[i];

                if (ca != cb)
                    return (int) ca - (int) cb;

                if (ca == '\0')
                    return 0;
            }

            return 0;
        }



        bool StrEq(const char* a, const char* b)
        {
            return StrCmp(a, b) == 0;
        }



        bool StrStartsWith(const char* str, const char* prefix)
        {
            if (!str || !prefix)
                return false;

            while (*prefix)
            {
                if (*str != *prefix)
                    return false;
                ++str;
                ++prefix;
            }

            return true;
        }

        bool StrEndsWith(const char* str, const char* suffix)
        {
            if (!str || !suffix)
                return false;

            uint strLen = StrLen(str);
            uint suffixLen = StrLen(suffix);

            if (suffixLen > strLen)
                return false;

            return StrCmp(str + (strLen - suffixLen), suffix) == 0;
        }



        const char* StrFindChar(const char* str, char ch)
        {
            if (!str)
                return nullptr;

            while (*str)
            {
                if (*str == ch)
                    return str;
                ++str;
            }

            return nullptr;
        }

        const char* StrFindChar(const char* str, uint offset, char ch)
        {
            if (!str)
                return nullptr;

            uint len = StrLen(str);
            if (offset >= len)
                return nullptr;

            str += offset;
            while (*str)
            {
                if (*str == ch)
                    return str;
                ++str;
            }

            return nullptr;
        }

        const char* StrFindLastChar(const char* str, char ch)
        {
            if (!str)
                return nullptr;

            const char* last = nullptr;
            while (*str)
            {
                if (*str == ch)
                    last = str;
                ++str;
            }

            return last;
        }

        const char* StrFindStr(const char* str, const char* subStr)
        {
            if (!str || !subStr)
                return nullptr;

            if (*subStr == '\0')
                return str;

            for (; *str; ++str)
            {
                const char* a = str;
                const char* b = subStr;

                while (*a && *b && (*a == *b))
                {
                    ++a;
                    ++b;
                }

                if (*b == '\0')
                    return str;
            }

            return nullptr;
        }

        const char* StrFindStr(const char* str, uint offset, const char* subStr)
        {
            if (!str || !subStr)
                return nullptr;

            uint len = StrLen(str);
            if (offset >= len)
                return nullptr;

            return StrFindStr(str + offset, subStr);
        }



        uint StrSpan(const char* str, const char* accept)
        {
            if (!str || !accept)
                return 0;

            uint count = 0;
            while (*str)
            {
                if (!StrFindChar(accept, *str))
                    break;

                ++count;
                ++str;
            }

            return count;
        }

        uint StrCSpan(const char* str, const char* reject)
        {
            if (!str || !reject)
                return 0;

            uint count = 0;
            while (*str)
            {
                if (StrFindChar(reject, *str))
                    break;

                ++count;
                ++str;
            }

            return count;
        }



        bool StrIsAscii(const char* str)
        {
            if (!str)
                return false;

            while (*str)
            {
                if (((unsigned char) *str) > 0x7F)
                    return false;
                ++str;
            }

            return true;
        }

        bool StrIsAsciiDigit(char ch)
        {
            return ch >= '0' && ch <= '9';
        }

        bool StrIsAsciiLower(char ch)
        {
            return ch >= 'a' && ch <= 'z';
        }

        bool StrIsAsciiUpper(char ch)
        {
            return ch >= 'A' && ch <= 'Z';
        }

        bool StrIsAsciiAlpha(char ch)
        {
            return StrIsAsciiLower(ch) || StrIsAsciiUpper(ch);
        }

        bool StrIsAsciiAlphaNum(char ch)
        {
            return StrIsAsciiAlpha(ch) || StrIsAsciiDigit(ch);
        }

        bool StrIsAsciiSpace(char ch)
        {
            return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\v' || ch == '\f';
        }



        char StrToLowerAscii(char ch)
        {
            if (StrIsAsciiUpper(ch))
                return (char) (ch - 'A' + 'a');
            return ch;
        }

        char StrToUpperAscii(char ch)
        {
            if (StrIsAsciiLower(ch))
                return (char) (ch - 'a' + 'A');
            return ch;
        }



        void StrToLowerAscii(char* str)
        {
            if (!str)
                return;

            while (*str)
            {
                *str = StrToLowerAscii(*str);
                ++str;
            }
        }

        void StrToUpperAscii(char* str)
        {
            if (!str)
                return;

            while (*str)
            {
                *str = StrToUpperAscii(*str);
                ++str;
            }
        }



        char* StrTrimLeftAscii(char* str)
        {
            if (!str)
                return nullptr;

            while (*str && StrIsAsciiSpace(*str))
                ++str;

            return str;
        }

        void StrTrimRightAscii(char* str)
        {
            if (!str || !*str)
                return;

            uint len = StrLen(str);
            while (len > 0 && StrIsAsciiSpace(str[len - 1]))
            {
                str[len - 1] = '\0';
                --len;
            }
        }

        char* StrTrimAscii(char* str)
        {
            if (!str)
                return nullptr;

            char* left = StrTrimLeftAscii(str);
            if (*left == '\0')
                return left;

            StrTrimRightAscii(left);
            return left;
        }



        int MemCmpN(const char* a, const char* b, uint len)
        {
            if (len == 0 || a == b)
                return 0;

            if (!a)
                return -1;

            if (!b)
                return 1;

            return std::memcmp(a, b, len);
        }

        const char* FindCharN(const char* str, uint len, char ch, uint offset)
        {
            if (!str || offset >= len)
                return nullptr;

            return (const char*) std::memchr(str + offset, (unsigned char) ch, len - offset);
        }

        const char* FindLastCharN(const char* str, uint len, char ch)
        {
            if (!str)
                return nullptr;

            for (uint i = len; i > 0; --i)
            {
                if (str[i - 1] == ch)
                    return str + (i - 1);
            }

            return nullptr;
        }

        const char* FindStrN(const char* str, uint len, const char* sub, uint subLen, uint offset)
        {
            if (!str || !sub || offset > len)
                return nullptr;

            if (subLen == 0)
                return str + offset;

            if (subLen > len - offset)
                return nullptr;

            const char* cursor = str + offset;
            const char* last = str + (len - subLen);

            while (cursor <= last)
            {
                cursor = (const char*) std::memchr(cursor, (unsigned char) sub[0], (uint) (last - cursor) + 1);
                if (!cursor)
                    return nullptr;

                if (std::memcmp(cursor, sub, subLen) == 0)
                    return cursor;

                ++cursor;
            }

            return nullptr;
        }



        uint StrLenU8(const char* str)
        {
            if (!str)
                return 0;

            uint len = 0;
            while (*str)
            {
                unsigned char c = (unsigned char) *str;
                if ((c & 0xC0) != 0x80)
                    ++len;
                ++str;
            }
            return len;
        }

        uint StrLenU8(const char* str, uint byteLen)
        {
            if (!str || byteLen == 0)
                return 0;

            uint len = 0;
            for (uint i = 0; i < byteLen; ++i)
            {
                unsigned char c = (unsigned char) str[i];
                if ((c & 0xC0) != 0x80)
                    ++len;
            }
            return len;
        }

        bool StrIsValidU8(const char* str)
        {
            if (!str)
                return false;

            const unsigned char* s = (const unsigned char*) str;

            while (*s)
            {
                if (*s <= 0x7F)
                {
                    ++s;
                    continue;
                }

                if ((*s & 0xE0) == 0xC0)
                {
                    if ((s[1] & 0xC0) != 0x80)
                        return false;

                    uint32 cp = ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);

                    if (cp < 0x80)
                        return false;

                    s += 2;
                    continue;
                }

                if ((*s & 0xF0) == 0xE0)
                {
                    if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80)
                        return false;

                    uint32 cp = ((s[0] & 0x0F) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);

                    if (cp < 0x800)
                        return false;

                    if (cp >= 0xD800 && cp <= 0xDFFF)
                        return false;

                    s += 3;
                    continue;
                }

                if ((*s & 0xF8) == 0xF0)
                {
                    if ((s[1] & 0xC0) != 0x80 || (s[2] & 0xC0) != 0x80 || (s[3] & 0xC0) != 0x80)
                        return false;

                    uint32 cp = ((s[0] & 0x07) << 18) | ((s[1] & 0x3F) << 12) | ((s[2] & 0x3F) << 6) | (s[3] & 0x3F);

                    if (cp < 0x10000 || cp > 0x10FFFF)
                        return false;

                    s += 4;
                    continue;
                }

                return false;
            }

            return true;
        }

        bool StrIsValidU8(const char* str, uint byteLen)
        {
            if (!str)
                return byteLen == 0;

            uint   offset = 0;
            uint32 codePoint = 0;
            while (offset < byteLen)
            {
                if (!StrNextCodePointU8(str, byteLen, offset, codePoint))
                    return false;
            }

            return true;
        }

        uint StrCountCodePointsU8(const char* str, uint byteLen)
        {
            return StrLenU8(str, byteLen);
        }

        bool StrAdvanceOneCodePointU8(const char* str, uint byteLen, uint& offset)
        {
            uint   next = offset;
            uint32 codePoint = 0;
            if (!StrNextCodePointU8(str, byteLen, next, codePoint))
                return false;

            offset = next;
            return true;
        }

        bool StrRetreatOneCodePointU8(const char* str, uint byteLen, uint& offset)
        {
            if (!str || offset == 0 || offset > byteLen)
                return false;

            uint pos = offset;
            uint steps = 0;
            while (pos > 0 && steps < 4)
            {
                --pos;
                ++steps;
                if (((unsigned char) str[pos] & 0xC0) != 0x80)
                    break;
            }

            const uint seqLen = StrUtf8SequenceLength((unsigned char) str[pos]);
            if (seqLen == 0 || seqLen != offset - pos)
                return false;

            uint   check = pos;
            uint32 codePoint = 0;
            if (!StrNextCodePointU8(str, byteLen, check, codePoint) || check != offset)
                return false;

            offset = pos;
            return true;
        }

        bool StrIsValidU16(const char16_t* str, uint unitLen)
        {
            if (!str)
                return unitLen == 0;

            uint   offset = 0;
            uint32 codePoint = 0;
            while (offset < unitLen)
            {
                if (!StrNextCodePointU16(str, unitLen, offset, codePoint))
                    return false;
            }

            return true;
        }

        uint StrCountCodePointsU16(const char16_t* str, uint unitLen)
        {
            if (!str)
                return 0;

            uint   count = 0;
            uint   offset = 0;
            uint32 codePoint = 0;
            while (offset < unitLen)
            {
                if (!StrNextCodePointU16(str, unitLen, offset, codePoint))
                    ++offset;

                ++count;
            }

            return count;
        }

        bool IsValidUnicodeScalarValue(uint32 cp)
        {
            if (cp > 0x10FFFF)
                return false;

            return cp < 0xD800 || cp > 0xDFFF;
        }

        uint StrUtf8SequenceLength(unsigned char leadByte)
        {
            if (leadByte <= 0x7F)
                return 1;

            if ((leadByte & 0xE0) == 0xC0)
                return 2;

            if ((leadByte & 0xF0) == 0xE0)
                return 3;

            if ((leadByte & 0xF8) == 0xF0)
                return 4;

            return 0;
        }

        bool StrNextCodePointU8(const char* str, uint byteLen, uint& byteOffset, uint32& outCodePoint)
        {
            if (!str || byteOffset >= byteLen)
                return false;

            const unsigned char* s = (const unsigned char*) str;
            const unsigned char  c = s[byteOffset];

            if (c <= 0x7F)
            {
                outCodePoint = c;
                byteOffset += 1;
                return true;
            }

            if ((c & 0xE0) == 0xC0)
            {
                if (byteOffset + 1 >= byteLen)
                    return false;

                unsigned char c1 = s[byteOffset + 1];
                if ((c1 & 0xC0) != 0x80)
                    return false;

                uint32 cp = ((c & 0x1F) << 6) | (c1 & 0x3F);

                if (cp < 0x80)
                    return false;

                outCodePoint = cp;
                byteOffset += 2;
                return true;
            }

            if ((c & 0xF0) == 0xE0)
            {
                if (byteOffset + 2 >= byteLen)
                    return false;

                unsigned char c1 = s[byteOffset + 1];
                unsigned char c2 = s[byteOffset + 2];

                if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80)
                    return false;

                uint32 cp = ((c & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);

                if (cp < 0x800)
                    return false;

                if (cp >= 0xD800 && cp <= 0xDFFF)
                    return false;

                outCodePoint = cp;
                byteOffset += 3;
                return true;
            }

            if ((c & 0xF8) == 0xF0)
            {
                if (byteOffset + 3 >= byteLen)
                    return false;

                unsigned char c1 = s[byteOffset + 1];
                unsigned char c2 = s[byteOffset + 2];
                unsigned char c3 = s[byteOffset + 3];

                if ((c1 & 0xC0) != 0x80 || (c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80)
                    return false;

                uint32 cp = ((c & 0x07) << 18) | ((c1 & 0x3F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);

                if (cp < 0x10000 || cp > 0x10FFFF)
                    return false;

                outCodePoint = cp;
                byteOffset += 4;
                return true;
            }

            return false;
        }

        bool StrCodePointToUtf8(uint32 codePoint, char outBytes[4], uint& outByteLen)
        {
            outByteLen = 0;

            if (!outBytes)
                return false;

            if (codePoint <= 0x7F)
            {
                outBytes[0] = (char) codePoint;
                outByteLen = 1;
                return true;
            }

            if (codePoint <= 0x7FF)
            {
                outBytes[0] = (char) (0xC0 | (codePoint >> 6));
                outBytes[1] = (char) (0x80 | (codePoint & 0x3F));
                outByteLen = 2;
                return true;
            }

            if (codePoint >= 0xD800 && codePoint <= 0xDFFF)
                return false;

            if (codePoint <= 0xFFFF)
            {
                outBytes[0] = (char) (0xE0 | (codePoint >> 12));
                outBytes[1] = (char) (0x80 | ((codePoint >> 6) & 0x3F));
                outBytes[2] = (char) (0x80 | (codePoint & 0x3F));
                outByteLen = 3;
                return true;
            }

            if (codePoint <= 0x10FFFF)
            {
                outBytes[0] = (char) (0xF0 | (codePoint >> 18));
                outBytes[1] = (char) (0x80 | ((codePoint >> 12) & 0x3F));
                outBytes[2] = (char) (0x80 | ((codePoint >> 6) & 0x3F));
                outBytes[3] = (char) (0x80 | (codePoint & 0x3F));
                outByteLen = 4;
                return true;
            }

            return false;
        }

        bool StrCodePointToUtf16(uint32 codePoint, char16_t outUnits[2], uint& outUnitCount)
        {
            outUnitCount = 0;

            if (!outUnits)
                return false;

            if (codePoint > 0x10FFFF)
                return false;

            if (codePoint >= 0xD800 && codePoint <= 0xDFFF)
                return false;

            if (codePoint <= 0xFFFF)
            {
                outUnits[0] = (char16_t) codePoint;
                outUnitCount = 1;
                return true;
            }

            codePoint -= 0x10000;
            outUnits[0] = (char16_t) (0xD800 + (codePoint >> 10));
            outUnits[1] = (char16_t) (0xDC00 + (codePoint & 0x3FF));
            outUnitCount = 2;
            return true;
        }

        bool StrNextCodePointU16(const char16_t* str, uint unitLen, uint& unitOffset, uint32& outCodePoint)
        {
            if (!str || unitOffset >= unitLen)
                return false;

            uint32 u0 = (uint32) str[unitOffset];

            if (u0 >= 0xD800 && u0 <= 0xDBFF)
            {
                if (unitOffset + 1 >= unitLen)
                    return false;

                uint32 u1 = (uint32) str[unitOffset + 1];
                if (u1 < 0xDC00 || u1 > 0xDFFF)
                    return false;

                outCodePoint = 0x10000 + (((u0 - 0xD800) << 10) | (u1 - 0xDC00));
                unitOffset += 2;
                return true;
            }

            if (u0 >= 0xDC00 && u0 <= 0xDFFF)
                return false;

            outCodePoint = u0;
            unitOffset += 1;
            return true;
        }
    } // namespace Stdlib
} // namespace krystallic
