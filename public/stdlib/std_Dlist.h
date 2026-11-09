/*
*  Copyright (c) BytesForge 2022-2026
*
*  Authors: @leopard-bf187 & @Ra192192 & @yorunikakeru4
*
*  Description: allocator-aware doubly linked list
*
*  Date: 11.05.2025, 03.06.2026
*/

#pragma once

#include "std_List.h"

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T> class DList : public List<T>
        {
        public:
            DList() : List<T>() {}

            explicit DList(Common::IAllocator* allocator) : List<T>(allocator) {}

            explicit DList(uint reserveCount, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : List<T>(reserveCount, allocator)
            {
            }

            DList(std::initializer_list<T> values, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : List<T>(values, allocator)
            {
            }

            DList(const DList<T>& other, Common::IAllocator* allocator = g_stdlibDefaultAllocator)
                : List<T>(other, allocator)
            {
            }

            DList(DList<T>&& other) noexcept : List<T>(std::move(other)) {}

            DList<T>& operator=(const DList<T>& other)
            {
                List<T>::operator=(other);
                return *this;
            }

            DList<T>& operator=(DList<T>&& other) noexcept
            {
                List<T>::operator=(std::move(other));
                return *this;
            }

            DList<T>& operator=(std::initializer_list<T> values)
            {
                List<T>::operator=(values);
                return *this;
            }
        };
    } // namespace Stdlib
} // namespace krystallic
