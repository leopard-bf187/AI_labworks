/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @yorunikakeru4
*
*  Description: MakeUnique, MakeShared and pointer cast utilities
*
*  Date: 06.06.2026
*/

#pragma once

#include <new>
#include <utility>

#include "std_UniquePtr.h"
#include "std_SharedPtr.h"

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T, typename... Args> UniquePtr<T> MakeUnique(Common::IAllocator* allocator, Args&&... args)
        {
            allocator = allocator ? allocator : g_stdlibDefaultAllocator;
            void* storage = allocator->Alloc(sizeof(T), alignof(T));
            T*    ptr = new (storage) T(std::forward<Args>(args)...);
            return UniquePtr<T>(ptr, allocator);
        }

        template <typename T, typename... Args> SharedPtr<T> MakeShared(Common::IAllocator* allocator, Args&&... args)
        {
            allocator = allocator ? allocator : g_stdlibDefaultAllocator;
            void* storage = allocator->Alloc(sizeof(T), alignof(T));
            T*    ptr = new (storage) T(std::forward<Args>(args)...);
            return SharedPtr<T>(ptr, allocator);
        }

        template <typename T, typename U> SharedPtr<T> StaticPointerCast(const SharedPtr<U>& ptr)
        {
            if (!ptr.m_ctrl)
            {
                return SharedPtr<T>();
            }
            return SharedPtr<T>(static_cast<T*>(ptr.m_ptr), ptr.m_ctrl);
        }

        template <typename T, typename U> SharedPtr<T> ReinterpretPointerCast(const SharedPtr<U>& ptr)
        {
            if (!ptr.m_ctrl)
            {
                return SharedPtr<T>();
            }
            return SharedPtr<T>(reinterpret_cast<T*>(ptr.m_ptr), ptr.m_ctrl);
        }

        template <typename T, typename U> SharedPtr<T> ConstPointerCast(const SharedPtr<U>& ptr)
        {
            if (!ptr.m_ctrl)
            {
                return SharedPtr<T>();
            }
            return SharedPtr<T>(const_cast<T*>(ptr.m_ptr), ptr.m_ctrl);
        }
    } // namespace Stdlib
} // namespace krystallic
