/*
*  Copyright (c) BytesForge 2022-2026
*
*  Authors: @leopard-bf187 & @Ra192192 & @yorunikakeru4
*
*  Description: vector class declaration source code
*
*  Date: 31.07.2025, 03.06.2026
*/

#pragma once

#include "std_Allocator.h"

#include <initializer_list>
#include <type_traits>
#include <utility>

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T> class Vector
        {
        private:
            T*                              m_Data;
            uint                            m_Size;
            uint                            m_Capacity;
            RefCounted<Common::IAllocator> m_Allocator;

        private:
            static inline uint AlignCapacity(uint capacity)
            {
                const uint base = 16;
                return ((capacity + base - 1) / base) * base;
            }

            static inline uint GrowCapacity(uint currentCapacity, uint requiredCapacity)
            {
                uint capacity = currentCapacity == 0 ? 16 : currentCapacity;
                while (capacity < requiredCapacity)
                {
                    capacity = AlignCapacity(capacity * 3 / 2);
                }
                return capacity;
            }

            T* Allocate(uint capacity)
            {
                if (capacity == 0)
                {
                    return nullptr;
                }

                return reinterpret_cast<T*>(m_Allocator->Alloc(capacity * sizeof(T), alignof(T)));
            }

            void FreeStorage(T* data)
            {
                if (data)
                {
                    m_Allocator->Free(data);
                }
            }

            void DestroyRange(uint from, uint to)
            {
                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    for (uint i = from; i < to; ++i)
                    {
                        m_Data[i].~T();
                    }
                }
            }

            void Reallocate(uint newCapacity)
            {
                T* newData = Allocate(newCapacity);
                for (uint i = 0; i < m_Size; ++i)
                {
                    new (&newData[i]) T(std::move(m_Data[i]));
                }

                DestroyRange(0, m_Size);
                FreeStorage(m_Data);
                m_Data = newData;
                m_Capacity = newCapacity;
            }

            void EnsureCapacity(uint requiredCapacity)
            {
                if (requiredCapacity <= m_Capacity)
                {
                    return;
                }

                Reallocate(GrowCapacity(m_Capacity, requiredCapacity));
            }

        public:
            Vector() : m_Data(nullptr), m_Size(0), m_Capacity(0), m_Allocator(g_stdlibDefaultAllocator) {}

            explicit Vector(Common::IAllocator* allocator)
                : m_Data(nullptr), m_Size(0), m_Capacity(0),
                  m_Allocator(allocator ? allocator : g_stdlibDefaultAllocator)
            {
            }

            explicit Vector(uint reserveCount, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : Vector(allocator)
            {
                Reserve(reserveCount);
            }

            Vector(std::initializer_list<T> values, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : Vector(allocator)
            {
                Append(values);
            }

            Vector(const Vector<T>& other, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : Vector(allocator)
            {
                Reserve(other.m_Size);
                for (uint i = 0; i < other.m_Size; ++i)
                {
                    EmplaceBack(other.m_Data[i]);
                }
            }

            Vector(Vector<T>&& other) noexcept
                : m_Data(other.m_Data), m_Size(other.m_Size), m_Capacity(other.m_Capacity),
                  m_Allocator(std::move(other.m_Allocator))
            {
                other.m_Data = nullptr;
                other.m_Size = 0;
                other.m_Capacity = 0;
                other.m_Allocator = g_stdlibDefaultAllocator;
            }

            ~Vector() { Reset(); }

            Vector<T>& operator=(const Vector<T>& other)
            {
                if (this == &other)
                {
                    return *this;
                }

                if (m_Allocator != other.m_Allocator)
                {
                    Reset();
                    m_Allocator = other.m_Allocator;
                }
                else
                {
                    Clear();
                }

                Reserve(other.m_Size);
                for (uint i = 0; i < other.m_Size; ++i)
                {
                    EmplaceBack(other.m_Data[i]);
                }
                return *this;
            }

            Vector<T>& operator=(Vector<T>&& other) noexcept
            {
                if (this == &other)
                {
                    return *this;
                }

                Reset();

                m_Data = other.m_Data;
                m_Size = other.m_Size;
                m_Capacity = other.m_Capacity;
                m_Allocator = std::move(other.m_Allocator);

                other.m_Data = nullptr;
                other.m_Size = 0;
                other.m_Capacity = 0;
                other.m_Allocator = g_stdlibDefaultAllocator;
                return *this;
            }

            Vector<T>& operator=(std::initializer_list<T> values)
            {
                Clear();
                Reserve(static_cast<uint>(values.size()));
                Append(values);
                return *this;
            }

            uint Size() const { return m_Size; }

            bool Empty() const { return m_Size == 0; }

            uint Allocated() const { return m_Capacity * sizeof(T); }

            Common::IAllocator* GetAllocator() { return m_Allocator.Get(); }

            void Clear()
            {
                DestroyRange(0, m_Size);
                m_Size = 0;
            }

            void Reset()
            {
                Clear();
                FreeStorage(m_Data);
                m_Data = nullptr;
                m_Capacity = 0;
            }

            void Reserve(uint numElements)
            {
                if (numElements > m_Capacity)
                {
                    Reallocate(GrowCapacity(m_Capacity, numElements));
                }
            }

            void Resize(uint numElements)
            {
                if (numElements < m_Size)
                {
                    DestroyRange(numElements, m_Size);
                    m_Size = numElements;
                    return;
                }

                EnsureCapacity(numElements);
                while (m_Size < numElements)
                {
                    new (&m_Data[m_Size]) T();
                    ++m_Size;
                }
            }

            void Resize(uint numElements, const T& value)
            {
                if (numElements < m_Size)
                {
                    DestroyRange(numElements, m_Size);
                    m_Size = numElements;
                    return;
                }

                EnsureCapacity(numElements);
                while (m_Size < numElements)
                {
                    new (&m_Data[m_Size]) T(value);
                    ++m_Size;
                }
            }

            void ShrinkToFit()
            {
                if (m_Size == m_Capacity)
                {
                    return;
                }

                if (m_Size == 0)
                {
                    Reset();
                    return;
                }

                Reallocate(m_Size);
            }

            void PushBack(const T& value) { EmplaceBack(value); }

            void PushBack(T&& value) { EmplaceBack(std::move(value)); }

            void PushFront(const T& value) { EmplaceFront(value); }

            void PushFront(T&& value) { EmplaceFront(std::move(value)); }

            T PopBack()
            {
                assert(m_Size > 0);
                --m_Size;
                T value = std::move(m_Data[m_Size]);
                m_Data[m_Size].~T();
                return value;
            }

            T PopFront()
            {
                assert(m_Size > 0);
                T value = std::move(m_Data[0]);
                Erase(0);
                return value;
            }

            bool TryPopBack(T* outValue)
            {
                if (Empty())
                {
                    return false;
                }

                T value = PopBack();
                if (outValue)
                {
                    *outValue = std::move(value);
                }
                return true;
            }

            bool TryPopFront(T* outValue)
            {
                if (Empty())
                {
                    return false;
                }

                T value = PopFront();
                if (outValue)
                {
                    *outValue = std::move(value);
                }
                return true;
            }

            void Insert(uint pos, const T& value)
            {
                assert(pos <= m_Size);
                EnsureCapacity(m_Size + 1);
                for (uint i = m_Size; i > pos; --i)
                {
                    new (&m_Data[i]) T(std::move(m_Data[i - 1]));
                    m_Data[i - 1].~T();
                }
                new (&m_Data[pos]) T(value);
                ++m_Size;
            }

            void Insert(uint pos, T&& value)
            {
                assert(pos <= m_Size);
                EnsureCapacity(m_Size + 1);
                for (uint i = m_Size; i > pos; --i)
                {
                    new (&m_Data[i]) T(std::move(m_Data[i - 1]));
                    m_Data[i - 1].~T();
                }
                new (&m_Data[pos]) T(std::move(value));
                ++m_Size;
            }

            void Insert(std::initializer_list<T> values, uint pos)
            {
                assert(pos <= m_Size);
                uint insertPos = pos;
                for (const T& value : values)
                {
                    Insert(insertPos, value);
                    ++insertPos;
                }
            }

            void Insert(const Vector<T>& other, uint pos)
            {
                assert(pos <= m_Size);
                uint insertPos = pos;
                for (uint i = 0; i < other.m_Size; ++i)
                {
                    Insert(insertPos, other.m_Data[i]);
                    ++insertPos;
                }
            }

            void Erase(uint pos)
            {
                assert(pos < m_Size);
                m_Data[pos].~T();
                for (uint i = pos + 1; i < m_Size; ++i)
                {
                    new (&m_Data[i - 1]) T(std::move(m_Data[i]));
                    m_Data[i].~T();
                }
                --m_Size;
            }

            void Append(std::initializer_list<T> values)
            {
                Reserve(m_Size + static_cast<uint>(values.size()));
                for (const T& value : values)
                {
                    EmplaceBack(value);
                }
            }

            void Append(const Vector<T>& other)
            {
                Reserve(m_Size + other.m_Size);
                for (uint i = 0; i < other.m_Size; ++i)
                {
                    EmplaceBack(other.m_Data[i]);
                }
            }

            template <typename... Args> T& EmplaceBack(Args&&... args)
            {
                EnsureCapacity(m_Size + 1);
                new (&m_Data[m_Size]) T(std::forward<Args>(args)...);
                ++m_Size;
                return m_Data[m_Size - 1];
            }

            template <typename... Args> T& EmplaceFront(Args&&... args)
            {
                EnsureCapacity(m_Size + 1);
                for (uint i = m_Size; i > 0; --i)
                {
                    new (&m_Data[i]) T(std::move(m_Data[i - 1]));
                    m_Data[i - 1].~T();
                }
                new (&m_Data[0]) T(std::forward<Args>(args)...);
                ++m_Size;
                return m_Data[0];
            }

            T& Front()
            {
                assert(m_Size > 0);
                return m_Data[0];
            }

            const T& Front() const
            {
                assert(m_Size > 0);
                return m_Data[0];
            }

            T& Back()
            {
                assert(m_Size > 0);
                return m_Data[m_Size - 1];
            }

            const T& Back() const
            {
                assert(m_Size > 0);
                return m_Data[m_Size - 1];
            }

            T* TryFront() { return Empty() ? nullptr : &m_Data[0]; }

            const T* TryFront() const { return Empty() ? nullptr : &m_Data[0]; }

            T* TryBack() { return Empty() ? nullptr : &m_Data[m_Size - 1]; }

            const T* TryBack() const { return Empty() ? nullptr : &m_Data[m_Size - 1]; }

            T* Data() { return m_Data; }

            const T* Data() const { return m_Data; }

            uint GetCapacity() const { return m_Capacity; }

            T& operator[](uint index)
            {
                assert(index < m_Size);
                return m_Data[index];
            }

            const T& operator[](uint index) const
            {
                assert(index < m_Size);
                return m_Data[index];
            }

            class Iterator
            {
            private:
                T* m_Ptr = nullptr;
                explicit Iterator(T* ptr) : m_Ptr(ptr) {}
                friend class Vector;

            public:
                using iterator_category = std::random_access_iterator_tag;
                using value_type = T;
                using difference_type = ptrdiff_t;
                using pointer = T*;
                using reference = T&;

                Iterator() = default;
                Iterator(const Iterator&) = default;
                Iterator& operator=(const Iterator&) = default;

                reference operator*() const { return *m_Ptr; }
                pointer   operator->() const { return m_Ptr; }

                Iterator& operator++()
                {
                    ++m_Ptr;
                    return *this;
                }

                Iterator operator++(int)
                {
                    Iterator tmp = *this;
                    ++m_Ptr;
                    return tmp;
                }

                Iterator& operator--()
                {
                    --m_Ptr;
                    return *this;
                }

                Iterator operator--(int)
                {
                    Iterator tmp = *this;
                    --m_Ptr;
                    return tmp;
                }

                Iterator operator+(difference_type n) const { return Iterator(m_Ptr + n); }
                Iterator operator-(difference_type n) const { return Iterator(m_Ptr - n); }

                Iterator& operator+=(difference_type n)
                {
                    m_Ptr += n;
                    return *this;
                }

                Iterator& operator-=(difference_type n)
                {
                    m_Ptr -= n;
                    return *this;
                }

                reference operator[](difference_type n) const { return m_Ptr[n]; }

                difference_type operator-(const Iterator& other) const { return m_Ptr - other.m_Ptr; }

                bool operator==(const Iterator& other) const { return m_Ptr == other.m_Ptr; }
                bool operator!=(const Iterator& other) const { return m_Ptr != other.m_Ptr; }
                bool operator<(const Iterator& other) const { return m_Ptr < other.m_Ptr; }
                bool operator>(const Iterator& other) const { return m_Ptr > other.m_Ptr; }
                bool operator<=(const Iterator& other) const { return m_Ptr <= other.m_Ptr; }
                bool operator>=(const Iterator& other) const { return m_Ptr >= other.m_Ptr; }
            };

            class ConstIterator
            {
            private:
                const T* m_Ptr = nullptr;
                explicit ConstIterator(const T* ptr) : m_Ptr(ptr) {}
                friend class Vector;

            public:
                using iterator_category = std::random_access_iterator_tag;
                using value_type = T;
                using difference_type = ptrdiff_t;
                using pointer = const T*;
                using reference = const T&;

                ConstIterator() = default;
                ConstIterator(const ConstIterator&) = default;
                ConstIterator& operator=(const ConstIterator&) = default;
                ConstIterator(const Iterator& it) : m_Ptr(it.m_Ptr) {}

                reference operator*() const { return *m_Ptr; }
                pointer   operator->() const { return m_Ptr; }

                ConstIterator& operator++()
                {
                    ++m_Ptr;
                    return *this;
                }

                ConstIterator operator++(int)
                {
                    ConstIterator tmp = *this;
                    ++m_Ptr;
                    return tmp;
                }

                ConstIterator& operator--()
                {
                    --m_Ptr;
                    return *this;
                }

                ConstIterator operator--(int)
                {
                    ConstIterator tmp = *this;
                    --m_Ptr;
                    return tmp;
                }

                ConstIterator operator+(difference_type n) const { return ConstIterator(m_Ptr + n); }
                ConstIterator operator-(difference_type n) const { return ConstIterator(m_Ptr - n); }

                ConstIterator& operator+=(difference_type n)
                {
                    m_Ptr += n;
                    return *this;
                }

                ConstIterator& operator-=(difference_type n)
                {
                    m_Ptr -= n;
                    return *this;
                }

                reference operator[](difference_type n) const { return m_Ptr[n]; }

                difference_type operator-(const ConstIterator& other) const { return m_Ptr - other.m_Ptr; }

                bool operator==(const ConstIterator& other) const { return m_Ptr == other.m_Ptr; }
                bool operator!=(const ConstIterator& other) const { return m_Ptr != other.m_Ptr; }
                bool operator<(const ConstIterator& other) const { return m_Ptr < other.m_Ptr; }
                bool operator>(const ConstIterator& other) const { return m_Ptr > other.m_Ptr; }
                bool operator<=(const ConstIterator& other) const { return m_Ptr <= other.m_Ptr; }
                bool operator>=(const ConstIterator& other) const { return m_Ptr >= other.m_Ptr; }
            };

            class ReverseIterator
            {
            private:
                T* m_Ptr = nullptr;
                explicit ReverseIterator(T* ptr) : m_Ptr(ptr) {}
                friend class Vector;

            public:
                using iterator_category = std::random_access_iterator_tag;
                using value_type = T;
                using difference_type = ptrdiff_t;
                using pointer = T*;
                using reference = T&;

                ReverseIterator() = default;
                ReverseIterator(const ReverseIterator&) = default;
                ReverseIterator& operator=(const ReverseIterator&) = default;

                reference operator*() const { return *(m_Ptr - 1); }
                pointer   operator->() const { return m_Ptr - 1; }

                ReverseIterator& operator++()
                {
                    --m_Ptr;
                    return *this;
                }

                ReverseIterator operator++(int)
                {
                    ReverseIterator tmp = *this;
                    --m_Ptr;
                    return tmp;
                }

                ReverseIterator& operator--()
                {
                    ++m_Ptr;
                    return *this;
                }

                ReverseIterator operator--(int)
                {
                    ReverseIterator tmp = *this;
                    ++m_Ptr;
                    return tmp;
                }

                ReverseIterator operator+(difference_type n) const { return ReverseIterator(m_Ptr - n); }
                ReverseIterator operator-(difference_type n) const { return ReverseIterator(m_Ptr + n); }

                ReverseIterator& operator+=(difference_type n)
                {
                    m_Ptr -= n;
                    return *this;
                }

                ReverseIterator& operator-=(difference_type n)
                {
                    m_Ptr += n;
                    return *this;
                }

                reference operator[](difference_type n) const { return *(m_Ptr - 1 - n); }

                difference_type operator-(const ReverseIterator& other) const { return other.m_Ptr - m_Ptr; }

                bool operator==(const ReverseIterator& other) const { return m_Ptr == other.m_Ptr; }
                bool operator!=(const ReverseIterator& other) const { return m_Ptr != other.m_Ptr; }
                bool operator<(const ReverseIterator& other) const { return m_Ptr > other.m_Ptr; }
                bool operator>(const ReverseIterator& other) const { return m_Ptr < other.m_Ptr; }
                bool operator<=(const ReverseIterator& other) const { return m_Ptr >= other.m_Ptr; }
                bool operator>=(const ReverseIterator& other) const { return m_Ptr <= other.m_Ptr; }
            };

            class ConstReverseIterator
            {
            private:
                const T* m_Ptr = nullptr;
                explicit ConstReverseIterator(const T* ptr) : m_Ptr(ptr) {}
                friend class Vector;

            public:
                using iterator_category = std::random_access_iterator_tag;
                using value_type = T;
                using difference_type = ptrdiff_t;
                using pointer = const T*;
                using reference = const T&;

                ConstReverseIterator() = default;
                ConstReverseIterator(const ConstReverseIterator&) = default;
                ConstReverseIterator& operator=(const ConstReverseIterator&) = default;
                ConstReverseIterator(const ReverseIterator& it) : m_Ptr(it.m_Ptr) {}

                reference operator*() const { return *(m_Ptr - 1); }
                pointer   operator->() const { return m_Ptr - 1; }

                ConstReverseIterator& operator++()
                {
                    --m_Ptr;
                    return *this;
                }

                ConstReverseIterator operator++(int)
                {
                    ConstReverseIterator tmp = *this;
                    --m_Ptr;
                    return tmp;
                }

                ConstReverseIterator& operator--()
                {
                    ++m_Ptr;
                    return *this;
                }

                ConstReverseIterator operator--(int)
                {
                    ConstReverseIterator tmp = *this;
                    ++m_Ptr;
                    return tmp;
                }

                ConstReverseIterator operator+(difference_type n) const { return ConstReverseIterator(m_Ptr - n); }
                ConstReverseIterator operator-(difference_type n) const { return ConstReverseIterator(m_Ptr + n); }

                ConstReverseIterator& operator+=(difference_type n)
                {
                    m_Ptr -= n;
                    return *this;
                }

                ConstReverseIterator& operator-=(difference_type n)
                {
                    m_Ptr += n;
                    return *this;
                }

                reference operator[](difference_type n) const { return *(m_Ptr - 1 - n); }

                difference_type operator-(const ConstReverseIterator& other) const { return other.m_Ptr - m_Ptr; }

                bool operator==(const ConstReverseIterator& other) const { return m_Ptr == other.m_Ptr; }
                bool operator!=(const ConstReverseIterator& other) const { return m_Ptr != other.m_Ptr; }
                bool operator<(const ConstReverseIterator& other) const { return m_Ptr > other.m_Ptr; }
                bool operator>(const ConstReverseIterator& other) const { return m_Ptr < other.m_Ptr; }
                bool operator<=(const ConstReverseIterator& other) const { return m_Ptr >= other.m_Ptr; }
                bool operator>=(const ConstReverseIterator& other) const { return m_Ptr <= other.m_Ptr; }
            };

            Iterator begin() { return Iterator(m_Data); }
            Iterator end() { return Iterator(m_Data + m_Size); }

            ConstIterator begin() const { return ConstIterator(m_Data); }
            ConstIterator end() const { return ConstIterator(m_Data + m_Size); }

            ConstIterator cbegin() const { return ConstIterator(m_Data); }
            ConstIterator cend() const { return ConstIterator(m_Data + m_Size); }

            ReverseIterator rbegin() { return ReverseIterator(m_Data + m_Size); }
            ReverseIterator rend() { return ReverseIterator(m_Data); }

            ConstReverseIterator rbegin() const { return ConstReverseIterator(m_Data + m_Size); }
            ConstReverseIterator rend() const { return ConstReverseIterator(m_Data); }

            ConstReverseIterator crbegin() const { return ConstReverseIterator(m_Data + m_Size); }
            ConstReverseIterator crend() const { return ConstReverseIterator(m_Data); }
        };
    } // namespace Stdlib
} // namespace krystallic
