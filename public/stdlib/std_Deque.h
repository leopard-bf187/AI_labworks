/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @yorunikakeru4
*
*  Description: deque class declaration source code
*
*  Date: 03.06.2026
*/

#pragma once

#include "std_Vector.h"

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T> class Deque
        {
        private:
            Vector<T> m_Data;

        public:
            Deque() : m_Data() {}

            explicit Deque(Common::IAllocator* allocator) : m_Data(allocator) {}

            explicit Deque(uint reserveCount, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : m_Data(reserveCount, allocator)
            {
            }

            Deque(std::initializer_list<T> values, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : m_Data(values, allocator)
            {
            }

            Deque(const Deque<T>& other, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : m_Data(other.m_Data, allocator)
            {
            }

            Deque(Deque<T>&& other) noexcept : m_Data(std::move(other.m_Data)) {}

            ~Deque() = default;

            Deque<T>& operator=(const Deque<T>& other)
            {
                m_Data = other.m_Data;
                return *this;
            }

            Deque<T>& operator=(Deque<T>&& other) noexcept
            {
                if (this != &other)
                {
                    m_Data = std::move(other.m_Data);
                }
                return *this;
            }

            Deque<T>& operator=(std::initializer_list<T> values)
            {
                m_Data = values;
                return *this;
            }

            uint Size() const { return m_Data.Size(); }

            bool Empty() const { return m_Data.Empty(); }

            void Clear() { m_Data.Clear(); }

            void Reset() { m_Data.Reset(); }

            uint Allocated() const { return m_Data.Allocated(); }

            Common::IAllocator* GetAllocator() { return m_Data.GetAllocator(); }

            void Reserve(uint numElements) { m_Data.Reserve(numElements); }

            void Resize(uint numElements) { m_Data.Resize(numElements); }

            void Resize(uint numElements, const T& value) { m_Data.Resize(numElements, value); }

            void ShrinkToFit() { m_Data.ShrinkToFit(); }

            T* Data() { return m_Data.Data(); }

            const T* Data() const { return m_Data.Data(); }

            uint GetCapacity() const { return m_Data.GetCapacity(); }

            void PushBack(const T& value) { m_Data.PushBack(value); }

            void PushBack(T&& value) { m_Data.PushBack(std::move(value)); }

            void PushFront(const T& value) { m_Data.PushFront(value); }

            void PushFront(T&& value) { m_Data.PushFront(std::move(value)); }

            T PopBack() { return m_Data.PopBack(); }

            T PopFront() { return m_Data.PopFront(); }

            bool TryPopBack(T* outValue) { return m_Data.TryPopBack(outValue); }

            bool TryPopFront(T* outValue) { return m_Data.TryPopFront(outValue); }

            void Insert(uint pos, const T& value) { m_Data.Insert(pos, value); }

            void Insert(uint pos, T&& value) { m_Data.Insert(pos, std::move(value)); }

            void Insert(std::initializer_list<T> values, uint pos) { m_Data.Insert(values, pos); }

            void Insert(const Deque<T>& other, uint pos)
            {
                uint insertPos = pos;
                for (uint i = 0; i < other.Size(); ++i)
                {
                    m_Data.Insert(insertPos, other[i]);
                    ++insertPos;
                }
            }

            void Erase(uint pos) { m_Data.Erase(pos); }

            void Append(std::initializer_list<T> values) { m_Data.Append(values); }

            void Append(const Deque<T>& other)
            {
                for (uint i = 0; i < other.Size(); ++i)
                {
                    m_Data.PushBack(other[i]);
                }
            }

            template <typename... Args> T& EmplaceBack(Args&&... args)
            {
                return m_Data.EmplaceBack(std::forward<Args>(args)...);
            }

            template <typename... Args> T& EmplaceFront(Args&&... args)
            {
                return m_Data.EmplaceFront(std::forward<Args>(args)...);
            }

            T& Front() { return m_Data.Front(); }

            const T& Front() const { return m_Data.Front(); }

            T& Back() { return m_Data.Back(); }

            const T& Back() const { return m_Data.Back(); }

            T* TryFront() { return m_Data.TryFront(); }

            const T* TryFront() const { return m_Data.TryFront(); }

            T* TryBack() { return m_Data.TryBack(); }

            const T* TryBack() const { return m_Data.TryBack(); }

            T& operator[](uint index) { return m_Data[index]; }

            const T& operator[](uint index) const { return m_Data[index]; }
        };
    } // namespace Stdlib
} // namespace krystallic
