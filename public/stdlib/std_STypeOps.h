/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @yorunikakeru4
*
*  Description: runtime type-erased type operations descriptor STypeOps
*
*  Date: 09.10.2026
*/

#pragma once


#include "std_TypeOps.h"


namespace krystallic
{
    namespace Stdlib
    {
        /// Runtime type-erased description of T built from the same typed operations
        /// that typed containers use. Type-erased storage passes raw addresses; every
        /// function pointer reinterprets them as T before touching the object.
        ///
        /// Storage-required members (validated by IsValidStorageTypeOps): size,
        /// alignment, construct, destroy, moveConstruct and moveAssign.
        /// copyConstruct/copyAssign, isEqual and hash are optional capabilities.
        /// IsValidTypeOps additionally requires isEqual and hash, preserving the
        /// previous complete-descriptor contract for hash-based consumers.
        /// Missing optional operations are null, never uninitialized pointers.
        struct STypeOps
        {
            uint size;
            uint alignment;

            void (*construct)(void* dst);
            void (*destroy)(void* value);

            void (*copyConstruct)(void* dst, const void* src);
            void (*moveConstruct)(void* dst, void* src);

            void (*copyAssign)(void* dst, const void* src);
            void (*moveAssign)(void* dst, void* src);

            bool (*isEqual)(const void* a, const void* b);
            dword(*hash)(const void* value);
        };


        /// Builds STypeOps for T from TOps (TypeOps<T> unless a custom typed policy
        /// with the same member names is supplied). Storage-required operations
        /// are checked at compile time; missing equality/hash operations are null.
        /// The descriptor owns nothing and allocates nothing.
        template <typename T, typename TOps = TypeOps<T>> inline STypeOps BuildTypeOps()
        {
            static_assert(TOps::IS_DEFAULT_CONSTRUCTIBLE, "BuildTypeOps: T must be default-constructible to provide 'construct'");
            static_assert(TOps::IS_MOVE_CONSTRUCTIBLE, "BuildTypeOps: T must be move- or copy-constructible to provide 'moveConstruct'");
            static_assert(TOps::IS_MOVE_ASSIGNABLE, "BuildTypeOps: T must be move- or copy-assignable to provide 'moveAssign'");

            STypeOps ops = {};

            ops.size = TOps::TYPE_SIZE;
            ops.alignment = TOps::TYPE_ALIGNMENT;

            ops.construct = [](void* dst) { TOps::Construct(dst); };
            ops.destroy = [](void* value) { TOps::Destroy(static_cast<T*>(value)); };

            if constexpr (TOps::IS_COPY_CONSTRUCTIBLE)
            {
                ops.copyConstruct = [](void* dst, const void* src) { TOps::CopyConstruct(dst, *static_cast<const T*>(src)); };
            }

            if constexpr (TOps::IS_COPY_ASSIGNABLE)
            {
                ops.copyAssign = [](void* dst, const void* src) { TOps::CopyAssign(*static_cast<T*>(dst), *static_cast<const T*>(src)); };
            }

            ops.moveConstruct = [](void* dst, void* src) { TOps::MoveConstruct(dst, *static_cast<T*>(src)); };
            ops.moveAssign = [](void* dst, void* src) { TOps::MoveAssign(*static_cast<T*>(dst), *static_cast<T*>(src)); };

            if constexpr (TOps::HAS_EQUALITY)
            {
                ops.isEqual = [](const void* a, const void* b) { return TOps::IsEqual(*static_cast<const T*>(a), *static_cast<const T*>(b)); };
            }

            if constexpr (TOps::HAS_HASH)
            {
                ops.hash = [](const void* value) { return TOps::Hash(*static_cast<const T*>(value)); };
            }

            return ops;
        }


        /// Validates operations required for constructing, relocating and
        /// destroying values. Equality, hashing and copying are optional here.
        inline bool IsValidStorageTypeOps(const STypeOps& ops)
        {
            if (ops.size == 0)
            {
                return false;
            }

            if (ops.alignment == 0 || (ops.alignment & (ops.alignment - 1)) != 0)
            {
                return false;
            }

            if (!ops.construct || !ops.destroy || !ops.moveConstruct || !ops.moveAssign)
            {
                return false;
            }

            return true;
        }


        /// Keeps the original strict check for consumers that require both
        /// semantic equality and hashing (for example hash tables).
        inline bool IsValidTypeOps(const STypeOps& ops)
        {
            return IsValidStorageTypeOps(ops) && ops.isEqual && ops.hash;
        }
    } // namespace Stdlib
} // namespace krystallic
