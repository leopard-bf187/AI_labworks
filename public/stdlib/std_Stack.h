/*
*  Copyright (c) BytesForge 2022-2026
*
*  Authors: @leopard-bf187 & @Ra192192 & @yorunikakeru4
*
*  Description: stack adapter over Vector
*
*  Date: 14.06.2025, 03.06.2026
*/

#pragma once

#include "std_Vector.h"

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T> class Stack
        {
        private:
            Vector<T> m_Data;

        public:
            Stack() : m_Data() {}

            explicit Stack(Common::IAllocator* allocator) : m_Data(allocator) {}

            explicit Stack(uint reserveCount, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : m_Data(reserveCount, allocator)
            {
            }

            Stack(std::initializer_list<T> values, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : m_Data(values, allocator)
            {
            }

            Stack(const Stack<T>& other, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : m_Data(other.m_Data, allocator)
            {
            }

            Stack(Stack<T>&& other) noexcept : m_Data(std::move(other.m_Data)) {}

            ~Stack() = default;

            Stack<T>& operator=(const Stack<T>& other)
            {
                m_Data = other.m_Data;
                return *this;
            }

            Stack<T>& operator=(Stack<T>&& other) noexcept
            {
                if (this != &other)
                {
                    m_Data = std::move(other.m_Data);
                }
                return *this;
            }

            Stack<T>& operator=(std::initializer_list<T> values)
            {
                m_Data = values;
                return *this;
            }

            void Push(const T& value) { m_Data.PushBack(value); }

            void Push(T&& value) { m_Data.PushBack(std::move(value)); }

            T Pop() { return m_Data.PopBack(); }

            bool TryPop(T* outValue) { return m_Data.TryPopBack(outValue); }

            T& Top() { return m_Data.Back(); }

            const T& Top() const { return m_Data.Back(); }

            uint Size() const { return m_Data.Size(); }

            bool Empty() const { return m_Data.Empty(); }

            void Clear() { m_Data.Clear(); }

            void Reset() { m_Data.Reset(); }

            uint Allocated() const { return m_Data.Allocated(); }

            Common::IAllocator* GetAllocator() { return m_Data.GetAllocator(); }
        };
    } // namespace Stdlib
} // namespace krystallic
