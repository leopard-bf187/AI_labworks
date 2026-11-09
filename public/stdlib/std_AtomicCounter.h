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
        template <> class AtomicCounter<int32>
        {
        private:
            volatile int32 m_value;

        public:
            AtomicCounter() : m_value(0) {}
            explicit AtomicCounter(int32 value) : m_value(value) {}

            int32 Load(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST) const
            {
                return AtomicImpl::Load32(&m_value, order);
            }

            void Store(int32 value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                AtomicImpl::Store32(&m_value, value, order);
            }

            int32 Increment(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::Increment32(&m_value, order);
            }

            int32 Decrement(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::Decrement32(&m_value, order);
            }

            int32 Add(int32 delta, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::Add32(&m_value, delta, order);
            }

            int32 Exchange(int32 value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::Exchange32(&m_value, value, order);
            }

            bool CompareExchange(int32&             expected,
                                 int32              desired,
                                 EAtomicMemoryOrder successOrder = ATOMIC_MEMORY_ORDER_SEQ_CST,
                                 EAtomicMemoryOrder failureOrder = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::CompareExchange32(&m_value, expected, desired, successOrder, failureOrder);
            }
        };

        template <> class AtomicCounter<uint32>
        {
        private:
            volatile uint32 m_value;

        public:
            AtomicCounter() : m_value(0) {}
            explicit AtomicCounter(uint32 value) : m_value(value) {}

            uint32 Load(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST) const
            {
                return AtomicImpl::LoadU32(&m_value, order);
            }

            void Store(uint32 value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                AtomicImpl::StoreU32(&m_value, value, order);
            }

            uint32 Increment(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::IncrementU32(&m_value, order);
            }

            uint32 Decrement(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::DecrementU32(&m_value, order);
            }

            uint32 Add(uint32 delta, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::AddU32(&m_value, delta, order);
            }

            uint32 Exchange(uint32 value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::ExchangeU32(&m_value, value, order);
            }

            bool CompareExchange(uint32&            expected,
                                 uint32             desired,
                                 EAtomicMemoryOrder successOrder = ATOMIC_MEMORY_ORDER_SEQ_CST,
                                 EAtomicMemoryOrder failureOrder = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::CompareExchangeU32(&m_value, expected, desired, successOrder, failureOrder);
            }
        };

        template <> class AtomicCounter<int64>
        {
        private:
            volatile int64 m_value;

        public:
            AtomicCounter() : m_value(0) {}
            explicit AtomicCounter(int64 value) : m_value(value) {}

            int64 Load(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST) const
            {
                return AtomicImpl::Load64(&m_value, order);
            }

            void Store(int64 value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                AtomicImpl::Store64(&m_value, value, order);
            }

            int64 Increment(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::Increment64(&m_value, order);
            }

            int64 Decrement(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::Decrement64(&m_value, order);
            }

            int64 Add(int64 delta, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::Add64(&m_value, delta, order);
            }

            int64 Exchange(int64 value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::Exchange64(&m_value, value, order);
            }

            bool CompareExchange(int64&             expected,
                                 int64              desired,
                                 EAtomicMemoryOrder successOrder = ATOMIC_MEMORY_ORDER_SEQ_CST,
                                 EAtomicMemoryOrder failureOrder = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::CompareExchange64(&m_value, expected, desired, successOrder, failureOrder);
            }
        };

        template <> class AtomicCounter<uint64>
        {
        private:
            volatile uint64 m_value;

        public:
            AtomicCounter() : m_value(0) {}
            explicit AtomicCounter(uint64 value) : m_value(value) {}

            uint64 Load(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST) const
            {
                return AtomicImpl::LoadU64(&m_value, order);
            }

            void Store(uint64 value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                AtomicImpl::StoreU64(&m_value, value, order);
            }

            uint64 Increment(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::IncrementU64(&m_value, order);
            }

            uint64 Decrement(EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::DecrementU64(&m_value, order);
            }

            uint64 Add(uint64 delta, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::AddU64(&m_value, delta, order);
            }

            uint64 Exchange(uint64 value, EAtomicMemoryOrder order = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::ExchangeU64(&m_value, value, order);
            }

            bool CompareExchange(uint64&            expected,
                                 uint64             desired,
                                 EAtomicMemoryOrder successOrder = ATOMIC_MEMORY_ORDER_SEQ_CST,
                                 EAtomicMemoryOrder failureOrder = ATOMIC_MEMORY_ORDER_SEQ_CST)
            {
                return AtomicImpl::CompareExchangeU64(&m_value, expected, desired, successOrder, failureOrder);
            }
        };
    } // namespace Stdlib
} // namespace krystallic
