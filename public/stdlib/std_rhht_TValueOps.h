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
        template <typename TValue> struct TValueOps_Default
        {
            inline static TValue Null() { return TValue(); }

            inline static bool IsValid(const TValue& v) { return true; }

            inline static void Retain(TValue& v) {}

            inline static void Release(TValue& v) { v = TValueOps_Default::Null(); }

            inline static void Copy(TValue& dst, const TValue& src)
            {
                dst = src;
                TValueOps_Default::Retain(dst);
            }

            inline static void Move(TValue& dst, TValue& src)
            {
                dst = src;
                src = TValueOps_Default::Null();
            }
        };


        template <typename TValue> struct TValueOps_RefCountedPtr
        {
            inline static TValue Null() { return nullptr; }

            inline static bool IsValid(const TValue& v) { return v != nullptr; }

            inline static void Retain(TValue& v)
            {
                if (v)
                    v->IncRef();
            }

            inline static void Release(TValue& v)
            {
                if (v)
                {
                    v->Delete();
                    v = TValueOps_RefCountedPtr::Null();
                }
            }

            inline static void Copy(TValue& dst, const TValue& src)
            {
                dst = src;
                TValueOps_RefCountedPtr::Retain(dst);
            }

            inline static void Move(TValue& dst, TValue& src)
            {
                dst = src;
                src = TValueOps_RefCountedPtr::Null();
            }
        };


        template <typename TValue> struct TValueOps_RawPtr
        {
            inline static TValue Null() { return nullptr; }

            inline static bool IsValid(const TValue& v) { return v != nullptr; }

            inline static void Retain(TValue& v) {}

            inline static void Release(TValue& v) { v = TValueOps_RawPtr::Null(); }

            inline static void Copy(TValue& dst, const TValue& src) { dst = src; }

            inline static void Move(TValue& dst, TValue& src)
            {
                dst = src;
                src = TValueOps_RawPtr::Null();
            }
        };
    } // namespace Stdlib
} // namespace krystallic
