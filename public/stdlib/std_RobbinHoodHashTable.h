/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: leopard-bf187
*
*  Description: RobbinHood hash table algorithm, string-based key
*
*  Date: 18.04.2026
*/


#pragma once


#include "std_dll_types.h"
#include "std_rhht_TValueOps.h"



namespace krystallic
{
    namespace Stdlib
    {
        template <typename TValue, typename TValueOps> struct TRHHashPair
        {
            int    m_cost;
            dword  m_hash;
            char   m_id[64];
            TValue m_val;

            TRHHashPair()
            {
                m_cost = 0;
                m_hash = 0;
                m_id[0] = '\0';
                m_val = TValueOps::Null();
            }

            TRHHashPair(const char* id, dword hash, const TValue& value)
            {
                m_cost = 0;
                m_hash = hash;

                uint len = Stdlib::StrLen(id);
                len = (len < 64) ? len : 63;
                Stdlib::StrCopySafe(m_id, len, id);
                m_id[len] = '\0';

                TValueOps::Copy(m_val, value);
            }

            TRHHashPair(const TRHHashPair& other)
            {
                m_cost = other.m_cost;
                m_hash = other.m_hash;

                Stdlib::StrCopy(m_id, other.m_id);

                TValueOps::Copy(m_val, other.m_val);
            }

            TRHHashPair(TRHHashPair&& mv) noexcept
            {
                m_cost = mv.m_cost;
                m_hash = mv.m_hash;

                Stdlib::StrCopy(m_id, mv.m_id);

                TValueOps::Move(m_val, mv.m_val);
            }

            ~TRHHashPair() { MakeNull(); }

            void MakeNull()
            {
                m_cost = 0;
                m_hash = 0;
                m_id[0] = '\0';
                TValueOps::Release(m_val);
            }

            TRHHashPair& operator=(const TRHHashPair& other)
            {
                m_cost = other.m_cost;
                m_hash = other.m_hash;

                Stdlib::StrCopy(m_id, other.m_id);

                TValueOps::Release(m_val);
                TValueOps::Copy(m_val, other.m_val);

                return *this;
            }

            TRHHashPair& operator=(TRHHashPair&& mv) noexcept
            {
                m_cost = mv.m_cost;
                m_hash = mv.m_hash;

                Stdlib::StrCopy(m_id, mv.m_id);

                TValueOps::Release(m_val);
                TValueOps::Move(m_val, mv.m_val);

                return *this;
            }
        };



        template <typename TValue, typename TValueOps> struct TRobbinHoodHashTable
        {
			typedef TRHHashPair<TValue, TValueOps> RHPairType;

            uint        m_tableSize;
            uint        m_currentTableSizeIndex;
            uint        m_numElements;
            RHPairType* m_table;

            TRobbinHoodHashTable() { Reset(); }

            ~TRobbinHoodHashTable() { Reset(); }

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
                        const char* id = oldTable[i].m_id;
                        TValue      value = oldTable[i].m_val;
                        AddElement(id, value);
                    }
                }

                delete[] oldTable;
                return true;
            }


            RHPairType* GetPair(const char* id) const
            {
                if (!id || !m_tableSize)
                    return nullptr;

                dword hash = FNV1A32_Hash(id, Stdlib::StrLen(id));
                uint  index = hash % m_tableSize;
                uint  pos = index;
                int   probe = 0;

                while (m_table[pos].m_hash != 0 && m_table[pos].m_cost >= probe)
                {
                    if (m_table[pos].m_hash == hash && Stdlib::StrEq(m_table[pos].m_id, id))
                        return &m_table[pos];

                    probe++;
                    pos = (pos + 1) % m_tableSize;
                }

                return nullptr;
            }


            bool AddElement(const char* id, const TValue& value)
            {
                if (!id || !TValueOps::IsValid(value) || m_numElements == m_tableSize)
                    return false;

                dword hash = FNV1A32_Hash(id, Stdlib::StrLen(id));
                uint  index = hash % m_tableSize;
                uint  pos = index;

                RHPairType insert(id, hash, value);

                while (m_table[pos].m_hash != 0)
                {
                    if (m_table[pos].m_hash == hash && Stdlib::StrEq(m_table[pos].m_id, id))
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


            bool DelElement(const char* id)
            {
                if (!id || !m_table || m_tableSize == 0)
                    return false;

                dword hash = FNV1A32_Hash(id, Stdlib::StrLen(id));
                uint  index = hash % m_tableSize;
                uint  pos = index;
                int   probe = 0;

                while (m_table[pos].m_hash != 0 && m_table[pos].m_cost >= probe)
                {
                    if (m_table[pos].m_hash == hash && Stdlib::StrEq(m_table[pos].m_id, id))
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


            inline bool HasElement(const char* id) const { return GetPair(id) != 0; }


            bool SetElement(const char* id, const TValue& value)
            {
                if (!id || !TValueOps::IsValid(value))
                    return false;

                RHPairType* pair = GetPair(id);
                if (pair)
                {
                    TValueOps::Release(pair->m_val);
                    pair->m_val = value;
                    TValueOps::Retain(pair->m_val);
                    return true;
                }

                return AddElement(id, value);
            }


            inline TValue GetElement(const char* id)
            {
                RHPairType* p = GetPair(id);
                return p ? p->m_val : TValueOps::Null();
            }


            inline RHPairType* operator[](const char* id) { return GetPair(id); }


            inline const RHPairType* operator[](const char* id) const { return GetPair(id); }


            inline uint GetNumElements() const { return m_numElements; }


            inline uint GetTableSize() const { return m_tableSize; }
        };

    } // namespace Stdlib
} // namespace krystallic
