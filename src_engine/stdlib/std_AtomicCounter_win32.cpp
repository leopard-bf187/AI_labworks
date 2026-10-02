/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors:
*
*  Description:
*
*  Date:
*/


#define STDLIB_API_EXPORT
#include "stdlib_dll.h"


#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


namespace krystallic
{
    namespace Stdlib
    {
        namespace AtomicImpl
        {

            static LONG MapMemoryOrder(EAtomicMemoryOrder order)
            {
                (void) order;
                return 0;
            }

            int32 Load32(const volatile int32* value, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                return (int32) InterlockedCompareExchange((volatile LONG*) value, 0, 0);
            }

            void Store32(volatile int32* value, int32 newValue, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                InterlockedExchange((volatile LONG*) value, (LONG) newValue);
            }

            int32 Increment32(volatile int32* value, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                return (int32) InterlockedIncrement((volatile LONG*) value);
            }

            int32 Decrement32(volatile int32* value, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                return (int32) InterlockedDecrement((volatile LONG*) value);
            }

            int32 Add32(volatile int32* value, int32 delta, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                return (int32) InterlockedExchangeAdd((volatile LONG*) value, (LONG) delta) + delta;
            }

            int32 Exchange32(volatile int32* value, int32 newValue, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                return (int32) InterlockedExchange((volatile LONG*) value, (LONG) newValue);
            }

            bool CompareExchange32(volatile int32*    value,
                                   int32&             expected,
                                   int32              desired,
                                   EAtomicMemoryOrder successOrder,
                                   EAtomicMemoryOrder failureOrder)
            {
                (void) MapMemoryOrder(successOrder);
                (void) MapMemoryOrder(failureOrder);

                LONG previous = InterlockedCompareExchange((volatile LONG*) value, (LONG) desired, (LONG) expected);
                if ((int32) previous == expected)
                    return true;

                expected = (int32) previous;
                return false;
            }

            uint32 LoadU32(const volatile uint32* value, EAtomicMemoryOrder order)
            {
                return (uint32) Load32((const volatile int32*) value, order);
            }

            void StoreU32(volatile uint32* value, uint32 newValue, EAtomicMemoryOrder order)
            {
                Store32((volatile int32*) value, (int32) newValue, order);
            }

            uint32 IncrementU32(volatile uint32* value, EAtomicMemoryOrder order)
            {
                return (uint32) Increment32((volatile int32*) value, order);
            }

            uint32 DecrementU32(volatile uint32* value, EAtomicMemoryOrder order)
            {
                return (uint32) Decrement32((volatile int32*) value, order);
            }

            uint32 AddU32(volatile uint32* value, uint32 delta, EAtomicMemoryOrder order)
            {
                return (uint32) Add32((volatile int32*) value, (int32) delta, order);
            }

            uint32 ExchangeU32(volatile uint32* value, uint32 newValue, EAtomicMemoryOrder order)
            {
                return (uint32) Exchange32((volatile int32*) value, (int32) newValue, order);
            }

            bool CompareExchangeU32(volatile uint32*   value,
                                    uint32&            expected,
                                    uint32             desired,
                                    EAtomicMemoryOrder successOrder,
                                    EAtomicMemoryOrder failureOrder)
            {
                int32 expectedSigned = (int32) expected;
                bool  result = CompareExchange32(
                    (volatile int32*) value, expectedSigned, (int32) desired, successOrder, failureOrder);
                expected = (uint32) expectedSigned;
                return result;
            }

            int64 Load64(const volatile int64* value, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                return (int64) InterlockedCompareExchange64((volatile LONGLONG*) value, 0, 0);
            }

            void Store64(volatile int64* value, int64 newValue, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                InterlockedExchange64((volatile LONGLONG*) value, (LONGLONG) newValue);
            }

            int64 Increment64(volatile int64* value, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                return (int64) InterlockedIncrement64((volatile LONGLONG*) value);
            }

            int64 Decrement64(volatile int64* value, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                return (int64) InterlockedDecrement64((volatile LONGLONG*) value);
            }

            int64 Add64(volatile int64* value, int64 delta, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                return (int64) InterlockedExchangeAdd64((volatile LONGLONG*) value, (LONGLONG) delta) + delta;
            }

            int64 Exchange64(volatile int64* value, int64 newValue, EAtomicMemoryOrder order)
            {
                (void) MapMemoryOrder(order);
                return (int64) InterlockedExchange64((volatile LONGLONG*) value, (LONGLONG) newValue);
            }

            bool CompareExchange64(volatile int64*    value,
                                   int64&             expected,
                                   int64              desired,
                                   EAtomicMemoryOrder successOrder,
                                   EAtomicMemoryOrder failureOrder)
            {
                (void) MapMemoryOrder(successOrder);
                (void) MapMemoryOrder(failureOrder);

                LONGLONG previous =
                    InterlockedCompareExchange64((volatile LONGLONG*) value, (LONGLONG) desired, (LONGLONG) expected);
                if ((int64) previous == expected)
                    return true;

                expected = (int64) previous;
                return false;
            }

            uint64 LoadU64(const volatile uint64* value, EAtomicMemoryOrder order)
            {
                return (uint64) Load64((const volatile int64*) value, order);
            }

            void StoreU64(volatile uint64* value, uint64 newValue, EAtomicMemoryOrder order)
            {
                Store64((volatile int64*) value, (int64) newValue, order);
            }

            uint64 IncrementU64(volatile uint64* value, EAtomicMemoryOrder order)
            {
                return (uint64) Increment64((volatile int64*) value, order);
            }

            uint64 DecrementU64(volatile uint64* value, EAtomicMemoryOrder order)
            {
                return (uint64) Decrement64((volatile int64*) value, order);
            }

            uint64 AddU64(volatile uint64* value, uint64 delta, EAtomicMemoryOrder order)
            {
                return (uint64) Add64((volatile int64*) value, (int64) delta, order);
            }

            uint64 ExchangeU64(volatile uint64* value, uint64 newValue, EAtomicMemoryOrder order)
            {
                return (uint64) Exchange64((volatile int64*) value, (int64) newValue, order);
            }

            bool CompareExchangeU64(volatile uint64*   value,
                                    uint64&            expected,
                                    uint64             desired,
                                    EAtomicMemoryOrder successOrder,
                                    EAtomicMemoryOrder failureOrder)
            {
                int64 expectedSigned = (int64) expected;
                bool  result = CompareExchange64(
                    (volatile int64*) value, expectedSigned, (int64) desired, successOrder, failureOrder);
                expected = (uint64) expectedSigned;
                return result;
            }

        } // namespace AtomicImpl
    } // namespace Stdlib
} // namespace krystallic


#endif
