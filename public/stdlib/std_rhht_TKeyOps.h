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


namespace krystallic
{
    namespace Stdlib
    {
        template <typename TKey> struct TKeyOps_Default
        {
            inline static TKey Null() { return TKey(); }

            inline static bool IsValid(const TKey& key) { return true; }

            inline static dword Hash(const TKey& key) { return FNV1A32_Hash(&key, sizeof(TKey)); }

            inline static bool IsEqual(const TKey& a, const TKey& b) { return a == b; }

            inline static void Retain(TKey& key) {}

            inline static void Release(TKey& key) { key = TKeyOps_Default::Null(); }

            inline static void Copy(TKey& dst, const TKey& src)
            {
                dst = src;
                TKeyOps_Default::Retain(dst);
            }

            inline static void Move(TKey& dst, TKey& src)
            {
                dst = src;
                src = TKeyOps_Default::Null();
            }
        };



        template <typename TKey> struct TKeyOps_RefCountedPtr
        {
            inline static TKey Null() { return nullptr; }

            inline static bool IsValid(const TKey& key) { return key != nullptr; }

            inline static dword Hash(const TKey& key)
            {
                struct
                {
                    uint64 p;
                    SGuid  g;
                } data;

                data.p = reinterpret_cast<uint64>(key);
                data.g = TKey::GUID();
                return FNV1A32_Hash(&data.g, sizeof(SGuid));
            }

            inline static bool IsEqual(const TKey& a, const TKey& b)
            {
                if (a == nullptr && b == nullptr)
                    return true;
                if (a == nullptr || b == nullptr)
                    return false;

                IBase* p1 = nullptr;
                IBase* p2 = nullptr;
                a->QueryBase(&p1);
                b->QueryBase(&p2);

                bool eq = (p1 == p2);

                p1->Delete();
                p2->Delete();

                return eq;
            }

            inline static void Retain(TKey& key)
            {
                if (key)
                    key->IncRef();
            }

            inline static void Release(TKey& key)
            {
                if (key)
                {
                    key->Delete();
                    key = TKeyOps_RefCountedPtr::Null();
                }
            }

            inline static void Copy(TKey& dst, const TKey& src)
            {
                dst = src;
                TKeyOps_RefCountedPtr::Retain(dst);
            }

            inline static void Move(TKey& dst, TKey& src)
            {
                dst = src;
                src = TKeyOps_RefCountedPtr::Null();
            }
        };



        template <typename TKey> struct TKeyOps_RawPtr
        {
            inline static TKey Null() { return nullptr; }

            inline static bool IsValid(const TKey& key) { return key != nullptr; }

            inline static dword Hash(const TKey& key) { return FNV1A32_Hash(&key, sizeof(TKey)); }

            inline static bool IsEqual(const TKey& a, const TKey& b) { return a == b; }

            inline static void Retain(TKey& key) {}

            inline static void Release(TKey& key) { key = TKeyOps_RawPtr::Null(); }

            inline static void Copy(TKey& dst, const TKey& src)
            {
                dst = src;
                TKeyOps_RawPtr::Retain(dst);
            }

            inline static void Move(TKey& dst, TKey& src)
            {
                dst = src;
                src = TKeyOps_RawPtr::Null();
            }
        };
    } // namespace Stdlib
} // namespace krystallic
