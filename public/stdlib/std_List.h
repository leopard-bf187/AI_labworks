/*
*  Copyright (c) BytesForge 2022-2026
*
*  Authors: @leopard-bf187 & @Ra192192 & @yorunikakeru4
*
*  Description: allocator-aware singly linked list
*
*  Date: 24.04.2025, 03.06.2026
*/

#pragma once

#include "std_Allocator.h"

#include <initializer_list>
#include <utility>

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T> class List
        {
        public:
            struct Node
            {
                T     data;
                Node* next;

                template <typename... Args>
                explicit Node(Args&&... args) : data(std::forward<Args>(args)...), next(nullptr)
                {
                }
            };

        private:
            Node*                           m_Head;
            Node*                           m_Tail;
            uint                            m_Size;
            uint                            m_Allocated;
            RefCounted<Common::IAllocator> m_Allocator;

            template <typename... Args> Node* CreateNode(Args&&... args)
            {
                void* storage = m_Allocator->Alloc(sizeof(Node), alignof(Node));
                Node* node = new (storage) Node(std::forward<Args>(args)...);
                m_Allocated += sizeof(Node);
                return node;
            }

            void DestroyNode(Node* node)
            {
                if (!node)
                {
                    return;
                }

                node->~Node();
                m_Allocator->Free(node);
                m_Allocated -= sizeof(Node);
            }

            Node* NodeAt(uint pos) const
            {
                assert(pos < m_Size);
                Node* current = m_Head;
                for (uint i = 0; i < pos; ++i)
                {
                    current = current->next;
                }
                return current;
            }

        public:
            List()
                : m_Head(nullptr), m_Tail(nullptr), m_Size(0), m_Allocated(0),
                  m_Allocator(g_stdlibDefaultAllocator)
            {
            }

            explicit List(Common::IAllocator* allocator)
                : m_Head(nullptr), m_Tail(nullptr), m_Size(0), m_Allocated(0),
                  m_Allocator(allocator ? allocator : g_stdlibDefaultAllocator)
            {
            }

            explicit List(uint reserveCount, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : List(allocator)
            {
                (void) reserveCount;
            }

            List(std::initializer_list<T> values, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : List(allocator)
            {
                Append(values);
            }

            List(const List<T>& other, Common::IAllocator* allocator = g_stdlibDefaultAllocator) : List(allocator)
            {
                Append(other);
            }

            List(List<T>&& other) noexcept
                : m_Head(other.m_Head), m_Tail(other.m_Tail), m_Size(other.m_Size), m_Allocated(other.m_Allocated),
                  m_Allocator(std::move(other.m_Allocator))
            {
                other.m_Head = nullptr;
                other.m_Tail = nullptr;
                other.m_Size = 0;
                other.m_Allocated = 0;
                other.m_Allocator = g_stdlibDefaultAllocator;
            }

            ~List() { Reset(); }

            List<T>& operator=(const List<T>& other)
            {
                if (this == &other)
                {
                    return *this;
                }

                Clear();
                Append(other);
                return *this;
            }

            List<T>& operator=(List<T>&& other) noexcept
            {
                if (this == &other)
                {
                    return *this;
                }

                Reset();

                m_Head = other.m_Head;
                m_Tail = other.m_Tail;
                m_Size = other.m_Size;
                m_Allocated = other.m_Allocated;
                m_Allocator = std::move(other.m_Allocator);

                other.m_Head = nullptr;
                other.m_Tail = nullptr;
                other.m_Size = 0;
                other.m_Allocated = 0;
                other.m_Allocator = g_stdlibDefaultAllocator;
                return *this;
            }

            List<T>& operator=(std::initializer_list<T> values)
            {
                Clear();
                Append(values);
                return *this;
            }

            uint Size() const { return m_Size; }

            bool Empty() const { return m_Size == 0; }

            uint Allocated() const { return m_Allocated; }

            Common::IAllocator* GetAllocator() { return m_Allocator.Get(); }

            void Clear()
            {
                Node* current = m_Head;
                while (current)
                {
                    Node* next = current->next;
                    DestroyNode(current);
                    current = next;
                }

                m_Head = nullptr;
                m_Tail = nullptr;
                m_Size = 0;
            }

            void Reset() { Clear(); }

            void PushBack(const T& value) { EmplaceBack(value); }

            void PushBack(T&& value) { EmplaceBack(std::move(value)); }

            void PushFront(const T& value) { EmplaceFront(value); }

            void PushFront(T&& value) { EmplaceFront(std::move(value)); }

            T PopBack()
            {
                assert(m_Size > 0);
                if (m_Size == 1)
                {
                    return PopFront();
                }

                Node* previous = NodeAt(m_Size - 2);
                Node* node = previous->next;
                T     value = std::move(node->data);
                previous->next = nullptr;
                m_Tail = previous;
                --m_Size;
                DestroyNode(node);
                return value;
            }

            T PopFront()
            {
                assert(m_Size > 0);
                Node* node = m_Head;
                T     value = std::move(node->data);
                m_Head = m_Head->next;
                if (!m_Head)
                {
                    m_Tail = nullptr;
                }
                --m_Size;
                DestroyNode(node);
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
                if (pos == 0)
                {
                    PushFront(value);
                    return;
                }
                if (pos == m_Size)
                {
                    PushBack(value);
                    return;
                }

                Node* previous = NodeAt(pos - 1);
                Node* node = CreateNode(value);
                node->next = previous->next;
                previous->next = node;
                ++m_Size;
            }

            void Insert(uint pos, T&& value)
            {
                assert(pos <= m_Size);
                if (pos == 0)
                {
                    PushFront(std::move(value));
                    return;
                }
                if (pos == m_Size)
                {
                    PushBack(std::move(value));
                    return;
                }

                Node* previous = NodeAt(pos - 1);
                Node* node = CreateNode(std::move(value));
                node->next = previous->next;
                previous->next = node;
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

            void Insert(const List<T>& other, uint pos)
            {
                assert(pos <= m_Size);
                uint insertPos = pos;
                for (Node* current = other.m_Head; current; current = current->next)
                {
                    Insert(insertPos, current->data);
                    ++insertPos;
                }
            }

            void Erase(uint pos)
            {
                assert(pos < m_Size);
                if (pos == 0)
                {
                    (void) PopFront();
                    return;
                }

                Node* previous = NodeAt(pos - 1);
                Node* node = previous->next;
                previous->next = node->next;
                if (node == m_Tail)
                {
                    m_Tail = previous;
                }
                --m_Size;
                DestroyNode(node);
            }

            void Append(std::initializer_list<T> values)
            {
                for (const T& value : values)
                {
                    PushBack(value);
                }
            }

            void Append(const List<T>& other)
            {
                for (Node* current = other.m_Head; current; current = current->next)
                {
                    PushBack(current->data);
                }
            }

            template <typename... Args> T& EmplaceBack(Args&&... args)
            {
                Node* node = CreateNode(std::forward<Args>(args)...);
                if (!m_Head)
                {
                    m_Head = node;
                    m_Tail = node;
                }
                else
                {
                    m_Tail->next = node;
                    m_Tail = node;
                }
                ++m_Size;
                return m_Tail->data;
            }

            template <typename... Args> T& EmplaceFront(Args&&... args)
            {
                Node* node = CreateNode(std::forward<Args>(args)...);
                node->next = m_Head;
                m_Head = node;
                if (!m_Tail)
                {
                    m_Tail = node;
                }
                ++m_Size;
                return m_Head->data;
            }

            T& Front()
            {
                assert(m_Head);
                return m_Head->data;
            }

            const T& Front() const
            {
                assert(m_Head);
                return m_Head->data;
            }

            T& Back()
            {
                assert(m_Tail);
                return m_Tail->data;
            }

            const T& Back() const
            {
                assert(m_Tail);
                return m_Tail->data;
            }

            T* TryFront() { return m_Head ? &m_Head->data : nullptr; }

            const T* TryFront() const { return m_Head ? &m_Head->data : nullptr; }

            T* TryBack() { return m_Tail ? &m_Tail->data : nullptr; }

            const T* TryBack() const { return m_Tail ? &m_Tail->data : nullptr; }

            T& GetElement(uint index) const { return NodeAt(index)->data; }

            Node* GetHead() { return m_Head; }

            Node* GetTail() { return m_Tail; }

            class Iterator
            {
            private:
                Node* m_Node = nullptr;
                explicit Iterator(Node* node) : m_Node(node) {}
                friend class List;

            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = T;
                using difference_type = ptrdiff_t;
                using pointer = T*;
                using reference = T&;

                Iterator() = default;
                Iterator(const Iterator&) = default;
                Iterator& operator=(const Iterator&) = default;

                reference operator*() const { return m_Node->data; }
                pointer   operator->() const { return &m_Node->data; }

                Iterator& operator++()
                {
                    m_Node = m_Node->next;
                    return *this;
                }

                Iterator operator++(int)
                {
                    Iterator tmp = *this;
                    m_Node = m_Node->next;
                    return tmp;
                }

                bool operator==(const Iterator& other) const { return m_Node == other.m_Node; }
                bool operator!=(const Iterator& other) const { return m_Node != other.m_Node; }
            };

            class ConstIterator
            {
            private:
                const Node* m_Node = nullptr;
                explicit ConstIterator(const Node* node) : m_Node(node) {}
                friend class List;

            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = T;
                using difference_type = ptrdiff_t;
                using pointer = const T*;
                using reference = const T&;

                ConstIterator() = default;
                ConstIterator(const ConstIterator&) = default;
                ConstIterator& operator=(const ConstIterator&) = default;
                ConstIterator(const Iterator& it) : m_Node(it.m_Node) {}

                reference operator*() const { return m_Node->data; }
                pointer   operator->() const { return &m_Node->data; }

                ConstIterator& operator++()
                {
                    m_Node = m_Node->next;
                    return *this;
                }

                ConstIterator operator++(int)
                {
                    ConstIterator tmp = *this;
                    m_Node = m_Node->next;
                    return tmp;
                }

                bool operator==(const ConstIterator& other) const { return m_Node == other.m_Node; }
                bool operator!=(const ConstIterator& other) const { return m_Node != other.m_Node; }
            };

            Iterator begin() { return Iterator(m_Head); }
            Iterator end() { return Iterator(nullptr); }

            ConstIterator begin() const { return ConstIterator(m_Head); }
            ConstIterator end() const { return ConstIterator(nullptr); }

            ConstIterator cbegin() const { return ConstIterator(m_Head); }
            ConstIterator cend() const { return ConstIterator(nullptr); }
        };
    } // namespace Stdlib
} // namespace krystallic
