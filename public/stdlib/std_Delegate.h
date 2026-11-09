/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @yorunikakeru4
*
*  Description: non-owning object+method callback wrapper
*
*  Date: 17.08.2026
*/


#pragma once


#include <cassert>
#include <cstddef>
#include <utility>

#include "std_dll_types.h"


namespace krystallic
{
    namespace Stdlib
    {
        template <typename R, typename... Args> class Delegate<R(Args...)>
        {
        private:
            using StubFn = R (*)(void* object, Args... args);

            void*  m_Object;
            StubFn m_Stub;

            template <typename C, R (C::*Method)(Args...)> static R MethodStub(void* object, Args... args) { return (static_cast<C*>(object)->*Method)(std::forward<Args>(args)...); }

            template <typename C, R (C::*Method)(Args...) const> static R ConstMethodStub(void* object, Args... args) { return (static_cast<const C*>(object)->*Method)(std::forward<Args>(args)...); }

        public:
            Delegate() : m_Object(nullptr), m_Stub(nullptr) {}
            Delegate(std::nullptr_t) : m_Object(nullptr), m_Stub(nullptr) {}

            template <typename C, R (C::*Method)(Args...)> void Bind(C* object)
            {
                m_Object = object;
                m_Stub = &MethodStub<C, Method>;
            }

            template <typename C, R (C::*Method)(Args...) const> void Bind(const C* object)
            {
                m_Object = const_cast<C*>(object);
                m_Stub = &ConstMethodStub<C, Method>;
            }

            void Reset()
            {
                m_Object = nullptr;
                m_Stub = nullptr;
            }

            R operator()(Args... args) const
            {
                assert(m_Stub);
                return m_Stub(m_Object, std::forward<Args>(args)...);
            }

            bool  Empty() const { return m_Stub == nullptr; }
            void* GetObject() const { return m_Object; }

            explicit operator bool() const { return m_Stub != nullptr; }
        };
    } // namespace Stdlib
} // namespace krystallic
