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


#if defined(__linux__)


namespace krystallic
{
    namespace Stdlib
    {
        namespace AtomicImpl
        {

            static int MapMemoryOrder(EAtomicMemoryOrder order)
            {
                switch (order)
                {
                    case ATOMIC_MEMORY_ORDER_RELAXED:
                        return __ATOMIC_RELAXED;
                    case ATOMIC_MEMORY_ORDER_ACQUIRE:
                        return __ATOMIC_ACQUIRE;
                    case ATOMIC_MEMORY_ORDER_RELEASE:
                        return __ATOMIC_RELEASE;
                    case ATOMIC_MEMORY_ORDER_ACQ_REL:
                        return __ATOMIC_ACQ_REL;
                    case ATOMIC_MEMORY_ORDER_SEQ_CST:
                        return __ATOMIC_SEQ_CST;
                    default:
                        return __ATOMIC_SEQ_CST;
                }
            }

            int32 Load32(const volatile int32* value, EAtomicMemoryOrder order)
            {
                return __atomic_load_n(value, MapMemoryOrder(order));
            }

            void Store32(volatile int32* value, int32 newValue, EAtomicMemoryOrder order)
            {
                __atomic_store_n(value, newValue, MapMemoryOrder(order));
            }

            int32 Increment32(volatile int32* value, EAtomicMemoryOrder order)
            {
                return __atomic_add_fetch(value, 1, MapMemoryOrder(order));
            }

            int32 Decrement32(volatile int32* value, EAtomicMemoryOrder order)
            {
                return __atomic_sub_fetch(value, 1, MapMemoryOrder(order));
            }

            int32 Add32(volatile int32* value, int32 delta, EAtomicMemoryOrder order)
            {
                return __atomic_add_fetch(value, delta, MapMemoryOrder(order));
            }

            int32 Exchange32(volatile int32* value, int32 newValue, EAtomicMemoryOrder order)
            {
                return __atomic_exchange_n(value, newValue, MapMemoryOrder(order));
            }

            bool CompareExchange32(volatile int32*    value,
                                   int32&             expected,
                                   int32              desired,
                                   EAtomicMemoryOrder successOrder,
                                   EAtomicMemoryOrder failureOrder)
            {
                return __atomic_compare_exchange_n(
                    value, &expected, desired, false, MapMemoryOrder(successOrder), MapMemoryOrder(failureOrder));
            }

            uint32 LoadU32(const volatile uint32* value, EAtomicMemoryOrder order)
            {
                return __atomic_load_n(value, MapMemoryOrder(order));
            }

            void StoreU32(volatile uint32* value, uint32 newValue, EAtomicMemoryOrder order)
            {
                __atomic_store_n(value, newValue, MapMemoryOrder(order));
            }

            uint32 IncrementU32(volatile uint32* value, EAtomicMemoryOrder order)
            {
                return __atomic_add_fetch(value, 1u, MapMemoryOrder(order));
            }

            uint32 DecrementU32(volatile uint32* value, EAtomicMemoryOrder order)
            {
                return __atomic_sub_fetch(value, 1u, MapMemoryOrder(order));
            }

            uint32 AddU32(volatile uint32* value, uint32 delta, EAtomicMemoryOrder order)
            {
                return __atomic_add_fetch(value, delta, MapMemoryOrder(order));
            }

            uint32 ExchangeU32(volatile uint32* value, uint32 newValue, EAtomicMemoryOrder order)
            {
                return __atomic_exchange_n(value, newValue, MapMemoryOrder(order));
            }

            bool CompareExchangeU32(volatile uint32*   value,
                                    uint32&            expected,
                                    uint32             desired,
                                    EAtomicMemoryOrder successOrder,
                                    EAtomicMemoryOrder failureOrder)
            {
                return __atomic_compare_exchange_n(
                    value, &expected, desired, false, MapMemoryOrder(successOrder), MapMemoryOrder(failureOrder));
            }

            int64 Load64(const volatile int64* value, EAtomicMemoryOrder order)
            {
                return __atomic_load_n(value, MapMemoryOrder(order));
            }

            void Store64(volatile int64* value, int64 newValue, EAtomicMemoryOrder order)
            {
                __atomic_store_n(value, newValue, MapMemoryOrder(order));
            }

            int64 Increment64(volatile int64* value, EAtomicMemoryOrder order)
            {
                return __atomic_add_fetch(value, 1ll, MapMemoryOrder(order));
            }

            int64 Decrement64(volatile int64* value, EAtomicMemoryOrder order)
            {
                return __atomic_sub_fetch(value, 1ll, MapMemoryOrder(order));
            }

            int64 Add64(volatile int64* value, int64 delta, EAtomicMemoryOrder order)
            {
                return __atomic_add_fetch(value, delta, MapMemoryOrder(order));
            }

            int64 Exchange64(volatile int64* value, int64 newValue, EAtomicMemoryOrder order)
            {
                return __atomic_exchange_n(value, newValue, MapMemoryOrder(order));
            }

            bool CompareExchange64(volatile int64*    value,
                                   int64&             expected,
                                   int64              desired,
                                   EAtomicMemoryOrder successOrder,
                                   EAtomicMemoryOrder failureOrder)
            {
                return __atomic_compare_exchange_n(
                    value, &expected, desired, false, MapMemoryOrder(successOrder), MapMemoryOrder(failureOrder));
            }

            uint64 LoadU64(const volatile uint64* value, EAtomicMemoryOrder order)
            {
                return __atomic_load_n(value, MapMemoryOrder(order));
            }

            void StoreU64(volatile uint64* value, uint64 newValue, EAtomicMemoryOrder order)
            {
                __atomic_store_n(value, newValue, MapMemoryOrder(order));
            }

            uint64 IncrementU64(volatile uint64* value, EAtomicMemoryOrder order)
            {
                return __atomic_add_fetch(value, 1ull, MapMemoryOrder(order));
            }

            uint64 DecrementU64(volatile uint64* value, EAtomicMemoryOrder order)
            {
                return __atomic_sub_fetch(value, 1ull, MapMemoryOrder(order));
            }

            uint64 AddU64(volatile uint64* value, uint64 delta, EAtomicMemoryOrder order)
            {
                return __atomic_add_fetch(value, delta, MapMemoryOrder(order));
            }

            uint64 ExchangeU64(volatile uint64* value, uint64 newValue, EAtomicMemoryOrder order)
            {
                return __atomic_exchange_n(value, newValue, MapMemoryOrder(order));
            }

            bool CompareExchangeU64(volatile uint64*   value,
                                    uint64&            expected,
                                    uint64             desired,
                                    EAtomicMemoryOrder successOrder,
                                    EAtomicMemoryOrder failureOrder)
            {
                return __atomic_compare_exchange_n(
                    value, &expected, desired, false, MapMemoryOrder(successOrder), MapMemoryOrder(failureOrder));
            }

        } // namespace AtomicImpl
    } // namespace Stdlib
} // namespace krystallic


#endif
