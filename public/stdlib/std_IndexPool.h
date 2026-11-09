/*
*  Copyright (c) BytesForge 2022-2026
*
*  Authors: @leopard-bf187 & @yorunikakeru4
*
*  Description: index pool declaration source code
*
*  Date: 09.10.2026
*/

#pragma once

#include "std_Vector.h"

#include <cassert>
#include <type_traits>

namespace krystallic
{
    namespace Stdlib
    {
        template <typename T = uint> class IndexPool
        {
        private:
            static_assert(std::is_unsigned_v<T>, "IndexPool requires an unsigned index type");

            static constexpr T InvalidIndex = static_cast<T>(~static_cast<T>(0));

            Vector<T> m_Free;
            T         m_NextIndex;

        private:
            bool ContainsFree(T index) const
            {
                for (uint i = 0; i < m_Free.Size(); ++i)
                {
                    if (m_Free[i] == index)
                    {
                        return true;
                    }
                }
                return false;
            }

        public:
            IndexPool() : m_Free(), m_NextIndex(0) {}

            explicit IndexPool(Common::IAllocator* allocator) : m_Free(allocator), m_NextIndex(0) {}

            T Acquire()
            {
                if (!m_Free.Empty())
                {
                    return m_Free.PopBack();
                }

                if (m_NextIndex == InvalidIndex)
                {
                    return InvalidIndex;
                }

                const T index = m_NextIndex;
                ++m_NextIndex;
                return index;
            }

            void Release(T index)
            {
                assert(index < m_NextIndex);

#ifndef NDEBUG
                if (ContainsFree(index))
                {
                    return;
                }
#endif

                m_Free.PushBack(index);
            }

            void Reset()
            {
                m_Free.Reset();
                m_NextIndex = 0;
            }

            uint FreeCount() const { return m_Free.Size(); }

            T NextIndex() const { return m_NextIndex; }

            bool Empty() const { return m_Free.Empty(); }

            Common::IAllocator* GetAllocator() const { return const_cast<Vector<T>&>(m_Free).GetAllocator(); }
        };
    } // namespace Stdlib
} // namespace krystallic
