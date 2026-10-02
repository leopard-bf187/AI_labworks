/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @yorunikakeru4
*
*  Description: weak pointer — non-owning reference to a shared-managed object
*
*  Date: 06.06.2026
*/

#pragma once

#include <cstddef>

#include "std_SharedPtr.h"

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T> class WeakPtr
        {
        private:
            T*                  m_ptr;
            SharedControlBlock* m_ctrl;

            void ReleaseWeakRef()
            {
                if (!m_ctrl)
                {
                    return;
                }

                --m_ctrl->weakRefs;
                if (m_ctrl->strongRefs == 0 && m_ctrl->weakRefs == 0)
                {
                    Common::IAllocator* alloc = m_ctrl->allocator;
                    m_ctrl->destroyBlock(m_ctrl, alloc);
                }

                m_ptr = nullptr;
                m_ctrl = nullptr;
            }

        public:
            WeakPtr() noexcept : m_ptr(nullptr), m_ctrl(nullptr) {}

            WeakPtr(std::nullptr_t) noexcept : m_ptr(nullptr), m_ctrl(nullptr) {}

            WeakPtr(const SharedPtr<T>& shared) : m_ptr(shared.m_ptr), m_ctrl(shared.m_ctrl)
            {
                if (m_ctrl)
                {
                    ++m_ctrl->weakRefs;
                }
            }

            WeakPtr(const WeakPtr& other) : m_ptr(other.m_ptr), m_ctrl(other.m_ctrl)
            {
                if (m_ctrl)
                {
                    ++m_ctrl->weakRefs;
                }
            }

            WeakPtr(WeakPtr&& other) noexcept : m_ptr(other.m_ptr), m_ctrl(other.m_ctrl)
            {
                other.m_ptr = nullptr;
                other.m_ctrl = nullptr;
            }

            ~WeakPtr() { ReleaseWeakRef(); }

            WeakPtr& operator=(const SharedPtr<T>& shared)
            {
                ReleaseWeakRef();
                m_ptr = shared.m_ptr;
                m_ctrl = shared.m_ctrl;
                if (m_ctrl)
                {
                    ++m_ctrl->weakRefs;
                }
                return *this;
            }

            WeakPtr& operator=(const WeakPtr& other)
            {
                if (this == &other)
                {
                    return *this;
                }

                ReleaseWeakRef();
                m_ptr = other.m_ptr;
                m_ctrl = other.m_ctrl;
                if (m_ctrl)
                {
                    ++m_ctrl->weakRefs;
                }
                return *this;
            }

            WeakPtr& operator=(WeakPtr&& other) noexcept
            {
                if (this == &other)
                {
                    return *this;
                }

                ReleaseWeakRef();
                m_ptr = other.m_ptr;
                m_ctrl = other.m_ctrl;
                other.m_ptr = nullptr;
                other.m_ctrl = nullptr;
                return *this;
            }

            WeakPtr& operator=(std::nullptr_t)
            {
                Reset();
                return *this;
            }

            bool Expired() const { return !m_ctrl || m_ctrl->strongRefs == 0; }
            uint UseCount() const { return m_ctrl ? m_ctrl->strongRefs : 0; }

            SharedPtr<T> Lock() const
            {
                if (Expired())
                {
                    return SharedPtr<T>();
                }
                return SharedPtr<T>(m_ptr, m_ctrl);
            }

            void Reset() { ReleaseWeakRef(); }

            explicit operator bool() const { return !Expired(); }
        };
    } // namespace Stdlib
} // namespace krystallic
