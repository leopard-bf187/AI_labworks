#pragma once


#include "pch.h"


#include "base.h"
#include "global_defs.h"
#include "crypto.h"


#include "std_dll_enums.h"


#define KRSW(t, a, b)                                                                                                                                                                                                                               \
    do                                                                                                                                                                                                                                                   \
    {                                                                                                                                                                                                                                                    \
        t tmp = a;                                                                                                                                                                                                                                       \
        a = b;                                                                                                                                                                                                                                           \
        b = tmp;                                                                                                                                                                                                                                         \
    } while (0)


#define		FILESYS_VERSION		0x02


#if defined(KRYSTALLIC_OS_WINNT)
#if defined(STDLIB_API_EXPORT)
#define STDLIB_API __declspec(dllexport)
#else
#define STDLIB_API __declspec(dllimport)
#endif
#elif defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)
#define STDLIB_API __attribute__((visibility("default")))
#endif


namespace krystallic
{
    namespace Stdlib
    {
        template <typename T> class AtomicCounter;
        typedef AtomicCounter<int32>	AtomicCounter32;
        typedef AtomicCounter<uint32>	AtomicCounterU32;
        typedef AtomicCounter<int64>	AtomicCounter64;
        typedef AtomicCounter<uint64>	AtomicCounterU64;

        template <typename T, uint capacity> class Array;
        template <typename T, uint row, uint col> class Array2D;

        template <typename T> class Vector;
        template <typename T, uint _BlockSize> class BlockVector;

        template <typename T> class Stack;
        template <typename T> class List;
        template <typename T> class DList;
        template <typename T> class Queue;
        template <typename T> class Deque;
        template <typename T> class Set;

        template <typename> class Function;
        template <typename R, typename... Args> class Function<R(Args...)>;

        template <typename> class Delegate;
        template <typename R, typename... Args> class Delegate<R(Args...)>;

        template <typename> class Callable;
        template <typename R, typename... Args> class Callable<R(Args...)>;

        template <typename TValue, typename TValueOps> struct TRobbinHoodHashTable;
        template <typename TKey, typename TValue, typename TKeyOps, typename TValueOps> struct TRobbinHoodHashTableKV;

        template <typename T> class SharedPtr;
        template <typename T> class UniquePtr;
        template <typename T> class WeakPtr;

        class String;
        class StringView;
        class StringU16;
        class StringU32;

        template <typename T> inline void Swap(T& a, T& b)
        {
            T tmp = a;
            a = b;
            b = tmp;
        }


        inline uint GetTableSizeByObjCount(uint numObjects, uint* idx)
        {
            if (numObjects <= 16)
            {
                *idx = 0;
                return 31;
            }

            if (numObjects >= 17 && numObjects <= 32)
            {
                *idx = 1;
                return 61;
            }

            if (numObjects >= 33 && numObjects <= 64)
            {
                *idx = 2;
                return 127;
            }

            if (numObjects >= 65 && numObjects <= 160)
            {
                *idx = 3;
                return 251;
            }

            if (numObjects >= 161 && numObjects <= 350)
            {
                *idx = 4;
                return 509;
            }

            if (numObjects >= 351 && numObjects <= 700)
            {
                *idx = 5;
                return 1021;
            }

            if (numObjects >= 701 && numObjects <= 1500)
            {
                *idx = 6;
                return 2039;
            }

            if (numObjects >= 1501 && numObjects <= 3000)
            {
                *idx = 7;
                return 4093;
            }

            if (numObjects >= 3001 && numObjects <= 6000)
            {
                *idx = 8;
                return 8191;
            }

            if (numObjects >= 6001 && numObjects <= 12000)
            {
                *idx = 9;
                return 16381;
            }

            if (numObjects >= 12001 && numObjects <= 25000)
            {
                *idx = 10;
                return 32749;
            }

            if (numObjects >= 25001 && numObjects <= 50000)
            {
                *idx = 11;
                return 65521;
            }

            return 0;
        }
    } // namespace Stdlib
} // namespace krystallic



namespace krystallic
{
    namespace Stdlib
    {

        STDLIB_API uint StrLen(const char* str);
        STDLIB_API uint StrLen(const char16_t* str);
        STDLIB_API uint StrLen(const char32_t* str);
        STDLIB_API uint StrLen(const uint32* str);

        STDLIB_API char* StrCopy(char* dst, const char* src);
        STDLIB_API char* StrCopyN(char* dst, const char* src, uint count);

        STDLIB_API char* StrCopySafe(char* dst, uint dstSize, const char* src);
        STDLIB_API char* StrCat(char* dst, const char* src);
        STDLIB_API char* StrCatSafe(char* dst, uint dstSize, const char* src);

        STDLIB_API int StrCmp(const char* a, const char* b);
        STDLIB_API int StrNCmp(const char* a, const char* b, uint count);

        STDLIB_API bool StrEq(const char* a, const char* b);

        STDLIB_API bool StrStartsWith(const char* str, const char* prefix);
        STDLIB_API bool StrEndsWith(const char* str, const char* suffix);

        STDLIB_API const char* StrFindChar(const char* str, char ch);
        STDLIB_API const char* StrFindChar(const char* str, uint offset, char ch);
        STDLIB_API const char* StrFindLastChar(const char* str, char ch);
        STDLIB_API const char* StrFindStr(const char* str, const char* subStr);
        STDLIB_API const char* StrFindStr(const char* str, uint offset, const char* subStr);

        STDLIB_API uint StrSpan(const char* str, const char* accept);
        STDLIB_API uint StrCSpan(const char* str, const char* reject);

        STDLIB_API bool StrIsAscii(const char* str);
        STDLIB_API bool StrIsAsciiDigit(char ch);
        STDLIB_API bool StrIsAsciiLower(char ch);
        STDLIB_API bool StrIsAsciiUpper(char ch);
        STDLIB_API bool StrIsAsciiAlpha(char ch);
        STDLIB_API bool StrIsAsciiAlphaNum(char ch);
        STDLIB_API bool StrIsAsciiSpace(char ch);

        STDLIB_API char StrToLowerAscii(char ch);
        STDLIB_API char StrToUpperAscii(char ch);

        STDLIB_API void StrToLowerAscii(char* str);
        STDLIB_API void StrToUpperAscii(char* str);

        STDLIB_API char* StrTrimLeftAscii(char* str);
        STDLIB_API void  StrTrimRightAscii(char* str);
        STDLIB_API char* StrTrimAscii(char* str);

        STDLIB_API int MemCmpN(const char* a, const char* b, uint len);

        STDLIB_API const char* FindCharN(const char* str, uint len, char ch, uint offset = 0);
        STDLIB_API const char* FindLastCharN(const char* str, uint len, char ch);
        STDLIB_API const char* FindStrN(const char* str, uint len, const char* sub, uint subLen, uint offset = 0);

        STDLIB_API uint StrLenU8(const char* str);
        STDLIB_API uint StrLenU8(const char* str, uint byteLen);
        STDLIB_API bool StrIsValidU8(const char* str);
        STDLIB_API bool StrIsValidU8(const char* str, uint byteLen);
        STDLIB_API uint StrCountCodePointsU8(const char* str, uint byteLen);
        STDLIB_API bool StrAdvanceOneCodePointU8(const char* str, uint byteLen, uint& offset);
        STDLIB_API bool StrRetreatOneCodePointU8(const char* str, uint byteLen, uint& offset);

        STDLIB_API bool StrIsValidU16(const char16_t* str, uint unitLen);
        STDLIB_API uint StrCountCodePointsU16(const char16_t* str, uint unitLen);

        STDLIB_API bool IsValidUnicodeScalarValue(uint32 cp);
        STDLIB_API uint StrUtf8SequenceLength(unsigned char leadByte);
        STDLIB_API bool StrNextCodePointU8(const char* str, uint byteLen, uint& byteOffset, uint32& outCodePoint);
        STDLIB_API bool StrCodePointToUtf8(uint32 codePoint, char outBytes[4], uint& outByteLen);
        STDLIB_API bool StrCodePointToUtf16(uint32 codePoint, char16_t outUnits[2], uint& outUnitCount);
        STDLIB_API bool StrNextCodePointU16(const char16_t* str, uint unitLen, uint& unitOffset, uint32& outCodePoint);


        namespace AtomicImpl
        {
            STDLIB_API int32 Load32(const volatile int32* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API void  Store32(volatile int32* value, int32 newValue, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API int32 Increment32(volatile int32* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API int32 Decrement32(volatile int32* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API int32 Add32(volatile int32* value, int32 delta, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API int32 Exchange32(volatile int32* value, int32 newValue, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API bool  CompareExchange32(volatile int32* value, int32& expected, int32 desired, EAtomicMemoryOrder successOrder = ATOMIC_MEMORY_ORDER_SEQ_CST, EAtomicMemoryOrder failureOrder = ATOMIC_MEMORY_ORDER_SEQ_CST);

            STDLIB_API uint32 LoadU32(const volatile uint32* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API void   StoreU32(volatile uint32* value, uint32 newValue, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API uint32 IncrementU32(volatile uint32* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API uint32 DecrementU32(volatile uint32* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API uint32 AddU32(volatile uint32* value, uint32 delta, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API uint32 ExchangeU32(volatile uint32* value, uint32 newValue, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API bool   CompareExchangeU32(volatile uint32* value, uint32& expected, uint32 desired, EAtomicMemoryOrder successOrder = ATOMIC_MEMORY_ORDER_SEQ_CST, EAtomicMemoryOrder failureOrder = ATOMIC_MEMORY_ORDER_SEQ_CST);

            STDLIB_API int64 Load64(const volatile int64* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API void  Store64(volatile int64* value, int64 newValue, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API int64 Increment64(volatile int64* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API int64 Decrement64(volatile int64* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API int64 Add64(volatile int64* value, int64 delta, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API int64 Exchange64(volatile int64* value, int64 newValue, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API bool  CompareExchange64(volatile int64* value, int64& expected, int64 desired, EAtomicMemoryOrder successOrder = ATOMIC_MEMORY_ORDER_SEQ_CST, EAtomicMemoryOrder failureOrder = ATOMIC_MEMORY_ORDER_SEQ_CST);

            STDLIB_API uint64 LoadU64(const volatile uint64* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API void   StoreU64(volatile uint64* value, uint64 newValue, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API uint64 IncrementU64(volatile uint64* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API uint64 DecrementU64(volatile uint64* value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API uint64 AddU64(volatile uint64* value, uint64 delta, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API uint64 ExchangeU64(volatile uint64* value, uint64 newValue, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST);
            STDLIB_API bool   CompareExchangeU64(volatile uint64* value, uint64& expected, uint64 desired, EAtomicMemoryOrder successOrder = ATOMIC_MEMORY_ORDER_SEQ_CST, EAtomicMemoryOrder failureOrder = ATOMIC_MEMORY_ORDER_SEQ_CST);
        } // namespace AtomicImpl
    } // namespace Stdlib
} // namespace krystallic
