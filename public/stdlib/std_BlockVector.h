#pragma once

#include <cassert>
#include <iterator>
#include <type_traits>

#include "std_dll_types.h"


namespace krystallic
{
    namespace Stdlib
    {
        template <typename T, uint _BlockSize = 64> class BlockVector
        {
        public:
            struct Block
            {
                Block* m_prev;
                Block* m_next;
                T      m_array[_BlockSize];
                Block() : m_prev(nullptr), m_next(nullptr) {}
            };

            class Iterator
            {
            public:
                using iterator_category = std::bidirectional_iterator_tag;
                using value_type = T;
                using difference_type = std::ptrdiff_t;
                using pointer = T*;
                using reference = T&;

            private:
                Block* m_block;
                uint   m_index;
                Block* m_lastBlock;
                uint   m_lastIndex;
                T*     m_array;

            public:
                Iterator(Block* blk, uint idx, Block* endBlk, uint endIdx)
                    : m_block(blk), m_index(idx), m_lastBlock(endBlk), m_lastIndex(endIdx)
                {
                    m_array = m_block ? m_block->m_array : nullptr;
                }

                Iterator(const Iterator& other) = default;
                Iterator& operator=(const Iterator& other) = default;

                reference operator*() { return m_array[m_index]; }
                pointer   operator->() { return &m_array[m_index]; }

                Iterator& operator++()
                {
                    if (m_block == m_lastBlock && m_index == m_lastIndex)
                    {
                        m_block = nullptr;
                        m_index = 0;
                    }
                    else
                    {
                        m_index++;
                        if (m_index >= _BlockSize)
                        {
                            m_block = m_block->m_next;
                            m_array = m_block ? m_block->m_array : nullptr;
                            m_index = 0;
                        }
                    }
                    return *this;
                }

                Iterator& operator--()
                {
                    if (m_block == nullptr)
                    {
                        m_block = m_lastBlock;
                        m_index = m_lastIndex;
                    }
                    else if (m_index == 0)
                    {
                        m_block = m_block->m_prev;
                        m_array = m_block ? m_block->m_array : nullptr;
                        m_index = _BlockSize - 1;
                    }
                    else
                    {
                        m_index--;
                    }
                    return *this;
                }

                Iterator operator++(int)
                {
                    Iterator tmp = *this;
                    ++(*this);
                    return tmp;
                }
                Iterator operator--(int)
                {
                    Iterator tmp = *this;
                    --(*this);
                    return tmp;
                }

                bool operator==(const Iterator& other) const
                {
                    return m_block == other.m_block && m_index == other.m_index;
                }
                bool operator!=(const Iterator& other) const { return !(*this == other); }
            };

            class ConstIterator
            {
            public:
                using iterator_category = std::bidirectional_iterator_tag;
                using value_type = T;
                using difference_type = std::ptrdiff_t;
                using pointer = const T*;
                using reference = const T&;

            private:
                const Block* m_block;
                uint         m_index;
                const Block* m_lastBlock;
                uint         m_lastIndex;
                const T*     m_array;

            public:
                ConstIterator(const Block* blk, uint idx, const Block* endBlk, uint endIdx)
                    : m_block(blk), m_index(idx), m_lastBlock(endBlk), m_lastIndex(endIdx)
                {
                    m_array = m_block ? const_cast<T*>(m_block->m_array) : nullptr;
                }

                ConstIterator(const ConstIterator& other) = default;
                ConstIterator& operator=(const ConstIterator& other) = default;

                reference operator*() const { return m_array[m_index]; }
                pointer   operator->() const { return &m_array[m_index]; }

                ConstIterator& operator++()
                {
                    if (m_block == m_lastBlock && m_index == m_lastIndex)
                    {
                        m_block = nullptr;
                        m_index = 0;
                    }
                    else
                    {
                        m_index++;
                        if (m_index >= _BlockSize)
                        {
                            m_block = m_block->m_next;
                            m_array = m_block ? m_block->m_array : nullptr;
                            m_index = 0;
                        }
                    }
                    return *this;
                }

                ConstIterator& operator--()
                {
                    if (m_block == nullptr)
                    {
                        m_block = m_lastBlock;
                        m_index = m_lastIndex;
                    }
                    else if (m_index == 0)
                    {
                        m_block = m_block->m_prev;
                        m_array = m_block ? m_block->m_array : nullptr;
                        m_index = _BlockSize - 1;
                    }
                    else
                    {
                        m_index--;
                    }
                    return *this;
                }


                ConstIterator operator++(int)
                {
                    ConstIterator tmp = *this;
                    ++(*this);
                    return tmp;
                }
                ConstIterator operator--(int)
                {
                    ConstIterator tmp = *this;
                    --(*this);
                    return tmp;
                }

                bool operator==(const ConstIterator& other) const
                {
                    return m_block == other.m_block && m_index == other.m_index;
                }
                bool operator!=(const ConstIterator& other) const { return !(*this == other); }
            };

        public:
            BlockVector();

            ~BlockVector() { DeleteAllBlocks(); }

            void PushBack(const T& data);
            void PushFront(const T& data);
            void Insert(uint pos, const T& data);


            uint Size() const { return m_numElements; }
            uint Capacity() const { return _BlockSize; }
            uint TotalCapacity() const { return m_numBlocks * _BlockSize; }
            uint NumBlocks() const { return m_numBlocks; }

            uint BlockSize() const { return _BlockSize; }

            void Reset();

            Iterator begin() { return Iterator(m_startBlock, m_startIndex, m_endBlock, m_endIndex); }
            Iterator end() { return Iterator(nullptr, 0, m_endBlock, m_endIndex); }

            ConstIterator cbegin() const { return ConstIterator(m_startBlock, m_startIndex, m_endBlock, m_endIndex); }
            ConstIterator cend() const { return ConstIterator(nullptr, 0, m_endBlock, m_endIndex); }

            Iterator rbegin() { return Iterator(m_endBlock, m_endIndex, m_startBlock, m_startIndex); }
            Iterator rend() { return Iterator(m_startBlock, m_startIndex - 1, m_endBlock, m_endIndex); }

            ConstIterator crbegin() const { return ConstIterator(m_endBlock, m_endIndex, m_startBlock, m_startIndex); }
            ConstIterator crend() const
            {
                return ConstIterator(m_startBlock, m_startIndex - 1, m_endBlock, m_endIndex);
            }

            T&       operator[](uint index);
            const T& operator[](uint index) const;

        private:
            void RecalculateCentral();
            void DeleteAllBlocks();

        private:
            Block* m_startBlock;
            Block* m_endBlock;
            Block* m_centralBlock;
            int32  m_startIndex;
            int32  m_endIndex; //to last elem
            uint   m_numBlocks;
            uint   m_numElements;
            uint   m_capacity;
        };


        template <typename T, uint _BlockSize> void BlockVector<T, _BlockSize>::DeleteAllBlocks()
        {
            Block* current = m_startBlock;
            while (current)
            {
                Block* m_next = current->m_next;
                delete current;
                current = m_next;
            }
            m_startBlock = m_endBlock = m_centralBlock = nullptr;
            m_numBlocks = m_numElements = 0;
            m_startIndex = m_endIndex = 0;
        }


        template <typename T, uint _BlockSize> void BlockVector<T, _BlockSize>::RecalculateCentral()
        {
            uint   mid = m_numBlocks / 2;
            Block* current = m_startBlock;
            for (uint i = 0; i < mid; ++i)
                current = current->m_next;
            m_centralBlock = current;
        }


        template <typename T, uint _BlockSize> BlockVector<T, _BlockSize>::BlockVector()
        {
            m_capacity = _BlockSize;
            m_centralBlock = new Block;
            m_startBlock = m_endBlock = m_centralBlock;
            m_startIndex = m_endIndex = 0;
            m_numBlocks = 1;
            m_numElements = 0;
        }


        template <typename T, uint _BlockSize> void BlockVector<T, _BlockSize>::PushBack(const T& data)
        {
            if (m_numElements == 0)
            {
                m_endBlock->m_array[m_endIndex] = data;
                m_numElements++;
                return;
            }

            if (m_endIndex == _BlockSize - 1)
            {
                Block* newBlock = new Block;
                newBlock->m_prev = m_endBlock;
                m_endBlock->m_next = newBlock;
                m_endBlock = newBlock;
                m_endIndex = 0;
                m_numBlocks++;
                RecalculateCentral();
            }
            else
            {
                m_endIndex++;
            }

            m_endBlock->m_array[m_endIndex] = data;
            m_numElements++;
        }


        template <typename T, uint _BlockSize> void BlockVector<T, _BlockSize>::PushFront(const T& data)
        {
            if (m_numElements == 0)
            {
                m_startBlock->m_array[m_startIndex] = data;
                m_numElements++;
                return;
            }

            if (m_startIndex == 0)
            {
                Block* newBlock = new Block;
                newBlock->m_next = m_startBlock;
                m_startBlock->m_prev = newBlock;
                m_startBlock = newBlock;
                m_startIndex = _BlockSize - 1;
                m_numBlocks++;
                RecalculateCentral();
            }
            else
            {
                m_startIndex--;
            }

            m_startBlock->m_array[m_startIndex] = data;
            m_numElements++;
        }


        template <typename T, uint _BlockSize> void BlockVector<T, _BlockSize>::Insert(uint pos, const T& data)
        {
            static_assert(std::is_trivially_copyable_v<T>, "Insert(memmove) requires trivially copyable type");

            assert(pos <= m_numElements);

            if (pos == 0)
            {
                PushFront(data);
                return;
            }
            else if (pos == m_numElements)
            {
                PushBack(data);
                return;
            }

            // �������� ����������� ������
            bool fromStart = pos <= m_numElements / 2;

            // ������������ ���� � ������ ������ �����
            uint   idx = pos + m_startIndex;
            Block* cur = m_startBlock;
            while (idx >= _BlockSize)
            {
                idx -= _BlockSize;
                cur = cur->m_next;
            }

            // ��������� ����� ����, ���� ��������� ���� ��������� ��������
            if (m_endIndex == _BlockSize - 1)
            {
                Block* newBlock = new Block;
                newBlock->m_prev = m_endBlock;
                m_endBlock->m_next = newBlock;
                m_endBlock = newBlock;
                m_endIndex = _BlockSize - 1; // ������������ ��� ������
                m_numBlocks++;
            }
            else
            {
                m_endIndex++;
            }

            if (fromStart)
            {
                // --- ����� ��������� ������ �� ������� �� ����� ---
                Block* b = m_endBlock;
                uint   bEnd = m_endIndex;

                while (true)
                {
                    uint start = 0;
                    uint count = bEnd + 1;

                    if (b == cur)
                    {
                        start = idx;
                        count = bEnd - idx + 1;
                    }

                    if (count > 0)
                        memmove(&b->m_array[start + 1], &b->m_array[start], count * sizeof(T));


                    if (b == cur)
                        break;

                    bEnd = _BlockSize - 1;
                    b = b->m_prev;
                }

                cur->m_array[idx] = data;
            }
            else
            {
                // --- ����� ��������� ����� �� ������� �� ������ ---
                // ��� ��������� ����� ������� ������� memmove, �� � �������� ��������
                Block* b = cur;
                uint   startIdx = idx;

                while (true)
                {
                    uint count = _BlockSize - startIdx;
                    if (b == m_endBlock)
                        count = m_endIndex - startIdx + 1;

                    if (count > 0)
                        memmove(&b->m_array[startIdx], &b->m_array[startIdx - 1], count * sizeof(T));

                    if (b == m_endBlock)
                        break;

                    startIdx = 0;
                    b = b->m_next;
                }

                cur->m_array[idx] = data;
            }

            m_numElements++;
            RecalculateCentral();
        }


        template <typename T, uint _BlockSize> void BlockVector<T, _BlockSize>::Reset()
        {
            DeleteAllBlocks();
            m_centralBlock = new Block;
            m_startBlock = m_endBlock = m_centralBlock;
            m_startIndex = m_endIndex = 0;
            m_numBlocks = 1;
            m_numElements = 0;
        }


        template <typename T, uint _BlockSize> T& BlockVector<T, _BlockSize>::operator[](uint index)
        {
            assert(index < m_numElements);
            uint   idx = index + m_startIndex;
            Block* current = m_startBlock;
            while (idx >= _BlockSize)
            {
                current = current->m_next;
                idx -= _BlockSize;
            }
            return current->m_array[idx];
        }


        template <typename T, uint _BlockSize> const T& BlockVector<T, _BlockSize>::operator[](uint index) const
        {
            assert(index < m_numElements);
            uint   idx = index + m_startIndex;
            Block* current = m_startBlock;
            while (idx >= _BlockSize)
            {
                current = current->m_next;
                idx -= _BlockSize;
            }
            return current->m_array[idx];
        }
    } // namespace Stdlib
} // namespace krystallic
