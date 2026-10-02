/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @yorunikakeru4
*
*  Description: lightweight free/static function pointer wrapper
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
        template <typename R, typename... Args> class Function<R(Args...)>
        {
        private:
            using FnPtr = R (*)(Args...);

            FnPtr m_Fn;

        public:
            Function() : m_Fn(nullptr) {}
            Function(std::nullptr_t) : m_Fn(nullptr) {}
            Function(FnPtr fn) : m_Fn(fn) {}

            Function(const Function& other) = default;
            Function(Function&& other) noexcept : m_Fn(other.m_Fn) { other.m_Fn = nullptr; }

            Function& operator=(const Function& other) = default;

            Function& operator=(Function&& other) noexcept
            {
                m_Fn = other.m_Fn;
                other.m_Fn = nullptr;
                return *this;
            }

            Function& operator=(std::nullptr_t)
            {
                m_Fn = nullptr;
                return *this;
            }

            Function& operator=(FnPtr fn)
            {
                m_Fn = fn;
                return *this;
            }

            R operator()(Args... args) const
            {
                assert(m_Fn);
                return m_Fn(std::forward<Args>(args)...);
            }

            bool  Empty() const { return m_Fn == nullptr; }
            void  Reset() { m_Fn = nullptr; }
            FnPtr Get() const { return m_Fn; }

            explicit operator bool() const { return m_Fn != nullptr; }
        };
    } // namespace Stdlib
} // namespace krystallic
