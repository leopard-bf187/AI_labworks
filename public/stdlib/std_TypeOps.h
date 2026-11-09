/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @yorunikakeru4
*
*  Description: compile-time type operations policy TypeOps
*
*  Date: 09.10.2026
*/

#pragma once


#include <new>
#include <type_traits>
#include <utility>

#include "std_dll_types.h"
#include "crypto/crypto_fnv1a.h"


namespace krystallic
{
    namespace Stdlib
    {
        namespace TypeOpsImpl
        {
            /// Generic compile-time operations over T: lifetime, copy, move, equality
            /// and hash. Every operation is constrained by the matching capability
            /// flag, so an unavailable operation is a compile-time error at the call
            /// site instead of a hidden byte-wise fallback (move-only types never
            /// copy through memcpy). Short operations are defined in the class body
            /// and are inline-able.
            ///
            /// Hash rules (IsEqual(a, b) implies Hash(a) == Hash(b)):
            /// - a user-provided dword GetHash() const member always wins;
            /// - otherwise value hashing is used only for types whose equal values
            ///   are byte-identical: integers, enums, floats with canonicalized
            ///   signed zero, and trivially copyable types with unique object
            ///   representations (no padding);
            /// - any other type must provide GetHash(), otherwise Hash does not
            ///   exist for it and is a compile-time error.
            ///
            /// Null() is intentionally not part of the model: an empty state belongs
            /// to the container and is never encoded as a "null value" of T.
            template <typename T> struct TGenericOps
            {
                static constexpr uint TYPE_SIZE = sizeof(T);
                static constexpr uint TYPE_ALIGNMENT = alignof(T);

                static constexpr bool IS_DEFAULT_CONSTRUCTIBLE = std::is_default_constructible_v<T>;
                static constexpr bool IS_COPY_CONSTRUCTIBLE = std::is_copy_constructible_v<T>;
                static constexpr bool IS_COPY_ASSIGNABLE = std::is_copy_assignable_v<T>;
                static constexpr bool IS_MOVE_CONSTRUCTIBLE = std::is_move_constructible_v<T> || std::is_copy_constructible_v<T>;
                static constexpr bool IS_MOVE_ASSIGNABLE = std::is_move_assignable_v<T> || std::is_copy_assignable_v<T>;
                static constexpr bool IS_COPYABLE = IS_COPY_CONSTRUCTIBLE && IS_COPY_ASSIGNABLE;

                static constexpr bool HAS_EQUALITY = requires(const T& a, const T& b) { bool(a == b); };
                static constexpr bool HAS_MEMBER_HASH = requires(const T& value) { dword(value.GetHash()); };
                static constexpr bool HAS_VALUE_HASH = std::is_integral_v<T> || std::is_enum_v<T> || (std::is_floating_point_v<T> && sizeof(T) <= sizeof(float64)) || (std::is_trivially_copyable_v<T> && std::has_unique_object_representations_v<T>);
                static constexpr bool HAS_HASH = HAS_MEMBER_HASH || HAS_VALUE_HASH;

                /// Default-constructs T in raw storage (value-initialization, so
                /// trivial types start zeroed instead of with indeterminate bytes).
                static void Construct(void* dst)
                    requires(IS_DEFAULT_CONSTRUCTIBLE)
                {
                    ::new (dst) T();
                }

                /// Destroys the object; the storage itself belongs to the caller.
                static void Destroy(T* value) { value->~T(); }

                static void CopyConstruct(void* dst, const T& src)
                    requires(IS_COPY_CONSTRUCTIBLE)
                {
                    ::new (dst) T(src);
                }

                /// Falls back to the copy constructor for copy-only types (the move
                /// constructor may be deleted), mirroring std::move_if_noexcept.
                static void MoveConstruct(void* dst, T& src)
                    requires(IS_MOVE_CONSTRUCTIBLE)
                {
                    if constexpr (std::is_move_constructible_v<T>)
                    {
                        ::new (dst) T(std::move(src));
                    }
                    else
                    {
                        ::new (dst) T(src);
                    }
                }

                static void CopyAssign(T& dst, const T& src)
                    requires(IS_COPY_ASSIGNABLE)
                {
                    dst = src;
                }

                /// Falls back to copy assignment for copy-only types.
                static void MoveAssign(T& dst, T& src)
                    requires(IS_MOVE_ASSIGNABLE)
                {
                    if constexpr (std::is_move_assignable_v<T>)
                    {
                        dst = std::move(src);
                    }
                    else
                    {
                        dst = src;
                    }
                }

                static bool IsEqual(const T& a, const T& b)
                    requires(HAS_EQUALITY)
                {
                    return a == b;
                }

                static dword Hash(const T& value)
                    requires(HAS_HASH)
                {
                    if constexpr (HAS_MEMBER_HASH)
                    {
                        return static_cast<dword>(value.GetHash());
                    }
                    else if constexpr (std::is_floating_point_v<T>)
                    {
                        // -0.0 and +0.0 compare equal, so normalize signed zero
                        // before reading the representation. NaN never compares
                        // equal to anything, so its invariant is vacuous.
                        T canonical = value;
                        if (canonical == static_cast<T>(0))
                        {
                            canonical = static_cast<T>(0);
                        }

                        return Crypto::FNV1A32(canonical);
                    }
                    else
                    {
                        // Reached only for value-hash types, which are padding-free
                        // by construction: equal values are byte-identical here.
                        return Crypto::FNV1A32(value);
                    }
                }
            };
        } // namespace TypeOpsImpl


        /// Compile-time type operations for T. See TypeOpsImpl::TGenericOps for the
        /// semantics and capability flags of the inherited members. Typed containers
        /// call these directly; STypeOps (std_STypeOps.h) wraps them for type-erased
        /// storage.
        template <typename T> struct TypeOps : TypeOpsImpl::TGenericOps<T>
        {
        };


        /// Raw pointer policy: pointers are non-owning handles. Copy duplicates the
        /// address, move transfers it and resets the source to nullptr so a
        /// moved-from slot reads as empty without a Null() policy, and Destroy is a
        /// no-op. Hash covers the address bytes, which matches pointer equality.
        template <typename T> struct TypeOps<T*> : TypeOpsImpl::TGenericOps<T*>
        {
            static constexpr bool HAS_HASH = true;

            static void Construct(void* dst) { ::new (dst) T*(nullptr); }

            static void Destroy(T** value) { (void) value; }

            static void MoveConstruct(void* dst, T*& src)
            {
                ::new (dst) T*(src);
                src = nullptr;
            }

            static void MoveAssign(T*& dst, T*& src)
            {
                dst = src;
                src = nullptr;
            }

            static dword Hash(T* value) { return Crypto::FNV1A32(value); }
        };


        /// Intrusive ref-counted pointer policy (RefCounted<T>): lifecycle, copy and
        /// move semantics are encoded in the wrapper itself (retain on copy, release
        /// on destroy, moved-from is empty); only hash is specialized here. Hash is
        /// the address of the referenced object, matching the wrapper's pointer
        /// equality, so equal wrappers always hash equal.
        template <typename T> struct TypeOps<RefCounted<T>> : TypeOpsImpl::TGenericOps<RefCounted<T>>
        {
            static constexpr bool HAS_HASH = true;

            static dword Hash(const RefCounted<T>& value)
            {
                const T* object = value.Get();
                return Crypto::FNV1A32(object);
            }
        };


        /// Shared ref-counted pointer policy (SharedPtr<T>): same rules as the
        /// intrusive policy; hash is the address of the referenced object.
        template <typename T> struct TypeOps<SharedPtr<T>> : TypeOpsImpl::TGenericOps<SharedPtr<T>>
        {
            static constexpr bool HAS_HASH = true;

            static dword Hash(const SharedPtr<T>& value)
            {
                const T* object = value.Get();
                return Crypto::FNV1A32(object);
            }
        };
    } // namespace Stdlib
} // namespace krystallic
