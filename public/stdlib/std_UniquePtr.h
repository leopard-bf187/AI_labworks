/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @yorunikakeru4
*
*  Description: unique pointer class implementation
*
*  Date: 06.06.2026
*/

#pragma once

#include <cstddef>

#include "std_Allocator.h"

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T> class UniquePtr
        {
        private:
            T*                              m_ptr;
            RefCounted<Common::IAllocator> m_allocator;

            void Destroy()
            {
                if (!m_ptr)
                {
                    return;
                }

                m_ptr->~T();
                m_allocator->Free(m_ptr);
                m_ptr = nullptr;
            }

        public:
            UniquePtr() noexcept : m_ptr(nullptr), m_allocator(g_stdlibDefaultAllocator) {}

            UniquePtr(std::nullptr_t) noexcept : m_ptr(nullptr), m_allocator(g_stdlibDefaultAllocator) {}

            UniquePtr(T* ptr, Common::IAllocator* allocator = nullptr)
                : m_ptr(ptr), m_allocator(allocator ? allocator : g_stdlibDefaultAllocator)
            {
            }

            UniquePtr(const UniquePtr&) = delete;
            UniquePtr& operator=(const UniquePtr&) = delete;

            UniquePtr(UniquePtr&& other) noexcept
                : m_ptr(other.m_ptr), m_allocator(std::move(other.m_allocator))
            {
                other.m_ptr = nullptr;
                other.m_allocator = g_stdlibDefaultAllocator;
            }

            ~UniquePtr() { Destroy(); }

            UniquePtr& operator=(UniquePtr&& other) noexcept
            {
                if (this == &other)
                {
                    return *this;
                }

                Destroy();
                m_ptr = other.m_ptr;
                m_allocator = std::move(other.m_allocator);
                other.m_ptr = nullptr;
                other.m_allocator = g_stdlibDefaultAllocator;
                return *this;
            }

            UniquePtr& operator=(std::nullptr_t)
            {
                Reset();
                return *this;
            }

            T* Get() const { return m_ptr; }

            T* Release()
            {
                T* ptr = m_ptr;
                m_ptr = nullptr;
                return ptr;
            }

            void Reset(T* ptr = nullptr)
            {
                if (m_ptr == ptr)
                {
                    return;
                }

                Destroy();
                m_ptr = ptr;
            }

            T&       operator*() const { return *m_ptr; }
            T*       operator->() const { return m_ptr; }
            explicit operator bool() const { return m_ptr != nullptr; }

            bool operator==(T* other) const { return m_ptr == other; }
            bool operator!=(T* other) const { return m_ptr != other; }
            bool operator==(const UniquePtr& other) const { return m_ptr == other.m_ptr; }
            bool operator!=(const UniquePtr& other) const { return m_ptr != other.m_ptr; }
        };
    } // namespace Stdlib
} // namespace krystallic
