/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @yorunikakeru4
*
*  Description: shared pointer with reference-counted control block
*
*  Date: 06.06.2026
*/

#pragma once

#include <cstddef>
#include <new>

#include "std_SharedControlBlock.h"

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T> class WeakPtr;

        template <typename T> class SharedPtr
        {
        private:
            T*                  m_ptr;
            SharedControlBlock* m_ctrl;

            static Common::IAllocator* NormalizeAllocator(Common::IAllocator* allocator)
            {
                return allocator ? allocator : g_stdlibDefaultAllocator;
            }



            void ReleaseRefs()
            {
                if (!m_ctrl)
                {
                    return;
                }

                --m_ctrl->strongRefs;
                if (m_ctrl->strongRefs == 0)
                {
                    Common::IAllocator* alloc = m_ctrl->allocator;
                    m_ctrl->destroyObject(m_ctrl->object, alloc);
                    if (m_ctrl->weakRefs == 0)
                    {
                        m_ctrl->destroyBlock(m_ctrl, alloc);
                    }
                }

                m_ptr = nullptr;
                m_ctrl = nullptr;
            }

            SharedPtr(T* ptr, SharedControlBlock* ctrl) : m_ptr(ptr), m_ctrl(ctrl)
            {
                if (m_ctrl)
                {
                    ++m_ctrl->strongRefs;
                }
            }

        public:
            SharedPtr() noexcept : m_ptr(nullptr), m_ctrl(nullptr) {}

            SharedPtr(std::nullptr_t) noexcept : m_ptr(nullptr), m_ctrl(nullptr) {}

            SharedPtr(T* ptr, Common::IAllocator* allocator = nullptr) : m_ptr(ptr), m_ctrl(nullptr)
            {
                if (!ptr)
                {
                    return;
                }

                allocator = NormalizeAllocator(allocator);
                allocator->IncRef();
                void* ctrlStorage = allocator->Alloc(sizeof(SharedControlBlock), alignof(SharedControlBlock));
                m_ctrl = new (ctrlStorage) SharedControlBlock();
                m_ctrl->allocator = allocator;
                m_ctrl->strongRefs = 1;
                m_ctrl->weakRefs = 0;
                m_ctrl->object = ptr;
                m_ctrl->destroyObject = [](void* object, Common::IAllocator* alloc)
                {
                    static_cast<T*>(object)->~T();
                    alloc->Free(object);
                };
                m_ctrl->destroyBlock = [](SharedControlBlock* block, Common::IAllocator* alloc)
                {
                    alloc->Free(block);
                    alloc->Delete();
                };
            }

            SharedPtr(const SharedPtr& other) : m_ptr(other.m_ptr), m_ctrl(other.m_ctrl)
            {
                if (m_ctrl)
                {
                    ++m_ctrl->strongRefs;
                }
            }

            SharedPtr(SharedPtr&& other) noexcept : m_ptr(other.m_ptr), m_ctrl(other.m_ctrl)
            {
                other.m_ptr = nullptr;
                other.m_ctrl = nullptr;
            }

            ~SharedPtr() { ReleaseRefs(); }

            SharedPtr& operator=(const SharedPtr& other)
            {
                if (this == &other)
                {
                    return *this;
                }

                ReleaseRefs();
                m_ptr = other.m_ptr;
                m_ctrl = other.m_ctrl;
                if (m_ctrl)
                {
                    ++m_ctrl->strongRefs;
                }
                return *this;
            }

            SharedPtr& operator=(SharedPtr&& other) noexcept
            {
                if (this == &other)
                {
                    return *this;
                }

                ReleaseRefs();
                m_ptr = other.m_ptr;
                m_ctrl = other.m_ctrl;
                other.m_ptr = nullptr;
                other.m_ctrl = nullptr;
                return *this;
            }

            SharedPtr& operator=(std::nullptr_t)
            {
                Reset();
                return *this;
            }

            T*   Get() const { return m_ptr; }
            uint UseCount() const { return m_ctrl ? m_ctrl->strongRefs : 0; }
            bool Unique() const { return UseCount() == 1; }

            void Reset() { ReleaseRefs(); }

            T&       operator*() const { return *m_ptr; }
            T*       operator->() const { return m_ptr; }
            explicit operator bool() const { return m_ptr != nullptr; }

            bool operator==(const SharedPtr& other) const { return m_ptr == other.m_ptr; }
            bool operator!=(const SharedPtr& other) const { return m_ptr != other.m_ptr; }
            bool operator==(T* other) const { return m_ptr == other; }
            bool operator!=(T* other) const { return m_ptr != other; }

            SharedControlBlock* GetControlBlock() const { return m_ctrl; }

            template <typename U, typename V> friend SharedPtr<U> StaticPointerCast(const SharedPtr<V>& ptr);
            template <typename U, typename V> friend SharedPtr<U> ReinterpretPointerCast(const SharedPtr<V>& ptr);
            template <typename U, typename V> friend SharedPtr<U> ConstPointerCast(const SharedPtr<V>& ptr);
            template <typename U> friend class WeakPtr;
        };
    } // namespace Stdlib
} // namespace krystallic
