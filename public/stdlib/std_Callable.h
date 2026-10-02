/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @yorunikakeru4
*
*  Description: owning, allocator-backed, type-erased callable wrapper with SOO
*
*  Date: 17.08.2026
*/


#pragma once


#include <cassert>
#include <cstddef>
#include <new>
#include <type_traits>
#include <utility>

#include "std_Allocator.h"
#include "std_dll_types.h"


namespace krystallic
{
    namespace Stdlib
    {
        template <typename R, typename... Args> class Callable<R(Args...)>
        {
        private:
            static constexpr uint SOO_SIZE = 32;
            static constexpr uint SOO_ALIGN = alignof(void*);

            struct SOps
            {
                R (*Call)(void* object, Args... args);
                void* (*Copy)(void* inlineStorage, const void* srcObject, Common::IAllocator* allocator, bool dstInline);
                void (*Move)(void* dstObject, void* srcObject);
                void (*Destroy)(void* object, Common::IAllocator* allocator, bool isInline);
            };

            template <typename F> static constexpr bool FitsInline() { return sizeof(F) <= SOO_SIZE && alignof(F) <= SOO_ALIGN; }

            template <typename F> static R CallImpl(void* object, Args... args) { return (*static_cast<F*>(object))(std::forward<Args>(args)...); }

            template <typename F> static void* CopyImpl(void* inlineStorage, const void* srcObject, Common::IAllocator* allocator, bool dstInline)
            {
                void* dst = dstInline ? inlineStorage : allocator->Alloc(sizeof(F), alignof(F));
                return new (dst) F(*static_cast<const F*>(srcObject));
            }

            template <typename F> static void MoveImpl(void* dstObject, void* srcObject) { new (dstObject) F(std::move(*static_cast<F*>(srcObject))); }

            template <typename F> static void DestroyImpl(void* object, Common::IAllocator* allocator, bool isInline)
            {
                static_cast<F*>(object)->~F();
                if (!isInline)
                {
                    allocator->Free(object);
                }
            }

            template <typename F> static const SOps* GetOps()
            {
                static const SOps ops{&CallImpl<F>, &CopyImpl<F>, &MoveImpl<F>, &DestroyImpl<F>};
                return &ops;
            }

            void*               m_Object;
            const SOps*         m_Ops;
            Common::IAllocator* m_Allocator; // caller must keep the allocator alive for this Callable's lifetime
            alignas(SOO_ALIGN) unsigned char m_Storage[SOO_SIZE];
            bool m_UsesInlineStorage;

            // Takes ownership of `other`'s callable object, leaving it empty. Used by
            // both the move constructor and move assignment (after Reset()).
            void AdoptFrom(Callable& other)
            {
                m_Ops = other.m_Ops;
                m_Allocator = other.m_Allocator;
                m_UsesInlineStorage = other.m_UsesInlineStorage;

                if (!m_Ops)
                {
                    m_Object = nullptr;
                }
                else if (m_UsesInlineStorage)
                {
                    // The inline object physically lives inside `other`'s storage array,
                    // at a different address than ours — it must be relocated by value.
                    m_Object = &m_Storage[0];
                    m_Ops->Move(m_Object, other.m_Object);
                    m_Ops->Destroy(other.m_Object, other.m_Allocator, true);
                }
                else
                {
                    // Heap-allocated: no relocation needed, just steal the pointer.
                    m_Object = other.m_Object;
                }

                other.m_Object = nullptr;
                other.m_Ops = nullptr;
                other.m_UsesInlineStorage = false;
            }

        public:
            Callable() : m_Object(nullptr), m_Ops(nullptr), m_Allocator(nullptr), m_UsesInlineStorage(false) {}
            Callable(std::nullptr_t) : Callable() {}

            template <typename F, typename = std::enable_if_t<!std::is_same_v<std::decay_t<F>, Callable>>> Callable(F&& fn, Common::IAllocator* allocator = nullptr) : m_Allocator(allocator ? allocator : g_stdlibDefaultAllocator)
            {
                using TStored = std::decay_t<F>;
                m_UsesInlineStorage = FitsInline<TStored>();
                void* storage = m_UsesInlineStorage ? static_cast<void*>(&m_Storage[0]) : m_Allocator->Alloc(sizeof(TStored), alignof(TStored));
                m_Object = new (storage) TStored(std::forward<F>(fn));
                m_Ops = GetOps<TStored>();
            }

            Callable(const Callable& other) : m_Ops(other.m_Ops), m_Allocator(other.m_Allocator), m_UsesInlineStorage(other.m_UsesInlineStorage)
            {
                m_Object = m_Ops ? m_Ops->Copy(&m_Storage[0], other.m_Object, m_Allocator, m_UsesInlineStorage) : nullptr;
            }

            Callable(Callable&& other) noexcept : m_Object(nullptr), m_Ops(nullptr), m_Allocator(nullptr), m_UsesInlineStorage(false) { AdoptFrom(other); }

            Callable& operator=(const Callable& other)
            {
                if (this == &other)
                    return *this;

                Reset();
                m_Ops = other.m_Ops;
                m_Allocator = other.m_Allocator;
                m_UsesInlineStorage = other.m_UsesInlineStorage;
                m_Object = m_Ops ? m_Ops->Copy(&m_Storage[0], other.m_Object, m_Allocator, m_UsesInlineStorage) : nullptr;
                return *this;
            }

            Callable& operator=(Callable&& other) noexcept
            {
                if (this == &other)
                    return *this;

                Reset();
                AdoptFrom(other);
                return *this;
            }

            Callable& operator=(std::nullptr_t)
            {
                Reset();
                return *this;
            }

            ~Callable() { Reset(); }

            R operator()(Args... args) const
            {
                assert(m_Ops);
                return m_Ops->Call(m_Object, std::forward<Args>(args)...);
            }

            bool Empty() const { return m_Ops == nullptr; }

            void Reset()
            {
                if (m_Ops)
                {
                    m_Ops->Destroy(m_Object, m_Allocator, m_UsesInlineStorage);
                }
                m_Object = nullptr;
                m_Ops = nullptr;
                m_UsesInlineStorage = false;
            }

            explicit operator bool() const { return m_Ops != nullptr; }
        };
    } // namespace Stdlib
} // namespace krystallic
