/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 19.04.2026
*/


#pragma once


#include "std_dll_types.h"
#include "std_rhht_TKeyOps.h"
#include "std_rhht_TValueOps.h"

namespace krystallic
{
    namespace Stdlib
    {

        template <typename TKey, typename TValue, typename TKeyOps, typename TValueOps> struct TRHHashPairKV
        {
            int    m_cost;
            dword  m_hash;
            TKey   m_key;
            TValue m_val;

            TRHHashPairKV()
            {
                m_cost = 0;
                m_hash = 0;
                m_key = TKeyOps::Null();
                m_val = TValueOps::Null();
            }

            TRHHashPairKV(const TKey& key, dword hash, const TValue& value)
            {
                m_cost = 0;
                m_hash = hash;
                TKeyOps::Copy(m_key, key);
                TValueOps::Copy(m_val, value);
            }

            TRHHashPairKV(const TRHHashPairKV& other)
            {
                m_cost = other.m_cost;
                m_hash = other.m_hash;
                TKeyOps::Copy(m_key, other.m_key);
                TValueOps::Copy(m_val, other.m_val);
            }

            TRHHashPairKV(TRHHashPairKV&& mv) noexcept
            {
                m_cost = mv.m_cost;
                m_hash = mv.m_hash;
                TKeyOps::Move(m_key, mv.m_key);
                TValueOps::Move(m_val, mv.m_val);
            }

            ~TRHHashPairKV() { MakeNull(); }

            void MakeNull()
            {
                m_cost = 0;
                m_hash = 0;
                TKeyOps::Release(m_key);
                TValueOps::Release(m_val);
            }

            TRHHashPairKV& operator=(const TRHHashPairKV& other)
            {
                m_cost = other.m_cost;
                m_hash = other.m_hash;
                TKeyOps::Release(m_key);
                TValueOps::Release(m_val);
                TKeyOps::Copy(m_key, other.m_key);
                TValueOps::Copy(m_val, other.m_val);
                return *this;
            }

            TRHHashPairKV& operator=(TRHHashPairKV&& mv) noexcept
            {
                m_cost = mv.m_cost;
                m_hash = mv.m_hash;
                TKeyOps::Release(m_key);
                TValueOps::Release(m_val);
                TKeyOps::Move(m_key, mv.m_key);
                TValueOps::Move(m_val, mv.m_val);
                return *this;
            }
        };

        template <typename TKey, typename TValue, typename TKeyOps, typename TValueOps> struct TRobbinHoodHashTableKV
        {
			typedef TRHHashPairKV<TKey, TValue, TKeyOps, TValueOps> RHPairType;

            uint        m_tableSize;
            uint        m_currentTableSizeIndex;
            uint        m_numElements;
            RHPairType* m_table;

            TRobbinHoodHashTableKV() { Reset(); }

            ~TRobbinHoodHashTableKV() { Reset(); }

            bool Resize(uint newSize)
            {
                if (newSize == 0 || newSize <= m_numElements)
                    return false;

                RHPairType* oldTable = m_table;
                uint        oldSize = m_tableSize;

                m_table = new RHPairType[newSize];

                m_tableSize = newSize;
                m_numElements = 0;

                for (uint i = 0; i < oldSize; ++i)
                {
                    if (oldTable[i].m_hash != 0)
                    {
                        TKey   key = oldTable[i].m_key;
                        TValue value = oldTable[i].m_val;
                        AddElement(key, value);
                    }
                }

                delete[] oldTable;
                return true;
            }


            RHPairType* GetPair(const TKey& key) const
            {
                if (!TKeyOps::IsValid(key) || !m_tableSize)
                    return nullptr;

                dword hash = TKeyOps::Hash(key);
                uint  index = hash % m_tableSize;
                uint  pos = index;
                int   probe = 0;

                while (m_table[pos].m_hash != 0 && m_table[pos].m_cost >= probe)
                {
                    if (m_table[pos].m_hash == hash && TKeyOps::IsEqual(m_table[pos].m_key, key))
                        return &m_table[pos];

                    probe++;
                    pos = (pos + 1) % m_tableSize;
                }

                return nullptr;
            }


            bool AddElement(const TKey& key, const TValue& value)
            {
                if (!TKeyOps::IsValid(key) || !TValueOps::IsValid(value) || m_numElements == m_tableSize)
                    return false;

                dword hash = TKeyOps::Hash(key);
                uint  index = hash % m_tableSize;
                uint  pos = index;

                RHPairType insert(key, hash, value);

                while (m_table[pos].m_hash != 0)
                {
                    if (m_table[pos].m_hash == hash && TKeyOps::IsEqual(m_table[pos].m_key, key))
                        return false;

                    if (m_table[pos].m_cost < insert.m_cost)
                        Stdlib::Swap(m_table[pos], insert);

                    insert.m_cost++;
                    pos = (pos + 1) % m_tableSize;
                }

                m_table[pos] = insert;
                m_numElements++;
                return true;
            }


            bool DelElement(const TKey& key)
            {
                if (!TKeyOps::IsValid(key) || !m_table || m_tableSize == 0)
                    return false;

                dword hash = TKeyOps::Hash(key);
                uint  index = hash % m_tableSize;
                uint  pos = index;
                int   probe = 0;

                while (m_table[pos].m_hash != 0 && m_table[pos].m_cost >= probe)
                {
                    if (m_table[pos].m_hash == hash && TKeyOps::IsEqual(m_table[pos].m_key, key))
                        break;

                    probe++;
                    pos = (pos + 1) % m_tableSize;
                }

                if (m_table[pos].m_hash == 0 || m_table[pos].m_cost < probe)
                    return false;

                uint deletePos = pos;
                uint next = (deletePos + 1) % m_tableSize;

                while (m_table[next].m_hash != 0 && m_table[next].m_cost > 0)
                {
                    m_table[deletePos] = m_table[next];
                    m_table[deletePos].m_cost--;

                    deletePos = next;
                    next = (next + 1) % m_tableSize;
                }

                m_table[deletePos].MakeNull();
                m_numElements--;
                return true;
            }


            void CreateTable(int numObjects)
            {
                if (m_table)
                {
                    delete[] m_table;
                    m_table = nullptr;
                }
                m_tableSize = GetTableSizeByObjCount(numObjects, &m_currentTableSizeIndex);
                m_table = new RHPairType[m_tableSize];
                m_numElements = 0;
            }


            void Reset()
            {
                if (m_table)
                {
                    delete[] m_table;
                    m_table = nullptr;
                }
                m_numElements = 0;
                m_tableSize = 0;
                m_currentTableSizeIndex = 0;
            }


            inline bool HasElement(const TKey& key) const { return GetPair(key) != 0; }


            bool SetElement(const TKey& key, const TValue& value)
            {
                if (!TKeyOps::IsValid(key) || !TValueOps::IsValid(value))
                    return false;

                RHPairType* pair = GetPair(key);
                if (pair)
                {
                    TValueOps::Release(pair->m_val);
                    TValueOps::Copy(pair->m_val, value);
                    return true;
                }

                return AddElement(key, value);
            }


            inline TValue GetElement(const TKey& key)
            {
                RHPairType* p = GetPair(key);
                return p ? p->m_val : TValueOps::Null();
            }


            inline RHPairType* operator[](const TKey& key) { return GetPair(key); }


            inline const RHPairType* operator[](const TKey& key) const { return GetPair(key); }


            inline uint GetNumElements() const { return m_numElements; }


            inline uint GetTableSize() const { return m_tableSize; }
        };
    } // namespace Stdlib
} // namespace krystallic
