/*
*  Copyright (c) BytesForge 2022-2026
*
*  Authors: @leopard-bf187 & @Ra192192 & @yorunikakeru4
*
*  Description: queue adapter over Deque
*
*  Date: 11.06.2025, 03.06.2026
*/

#pragma once

#include "std_Deque.h"

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T> class Queue
        {
        private:
            Deque<T> m_Data;

        public:
            Queue() : m_Data() {}

            explicit Queue(Common::IAllocator* allocator) : m_Data(allocator) {}

            explicit Queue(uint reserveCount, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : m_Data(reserveCount, allocator)
            {
            }

            Queue(std::initializer_list<T> values, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : m_Data(values, allocator)
            {
            }

            Queue(const Queue<T>& other, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : m_Data(other.m_Data, allocator)
            {
            }

            Queue(Queue<T>&& other) noexcept : m_Data(std::move(other.m_Data)) {}

            ~Queue() = default;

            Queue<T>& operator=(const Queue<T>& other)
            {
                m_Data = other.m_Data;
                return *this;
            }

            Queue<T>& operator=(Queue<T>&& other) noexcept
            {
                if (this != &other)
                {
                    m_Data = std::move(other.m_Data);
                }
                return *this;
            }

            Queue<T>& operator=(std::initializer_list<T> values)
            {
                m_Data = values;
                return *this;
            }

            void Push(const T& value) { m_Data.PushBack(value); }

            void Push(T&& value) { m_Data.PushBack(std::move(value)); }

            T Pop() { return m_Data.PopFront(); }

            bool TryPop(T* outValue) { return m_Data.TryPopFront(outValue); }

            T& Front() { return m_Data.Front(); }

            const T& Front() const { return m_Data.Front(); }

            T& Back() { return m_Data.Back(); }

            const T& Back() const { return m_Data.Back(); }

            uint Size() const { return m_Data.Size(); }

            bool Empty() const { return m_Data.Empty(); }

            void Clear() { m_Data.Clear(); }

            void Reset() { m_Data.Reset(); }

            uint Allocated() const { return m_Data.Allocated(); }

            Common::IAllocator* GetAllocator() { return m_Data.GetAllocator(); }
        };
    } // namespace Stdlib
} // namespace krystallic
