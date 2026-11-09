/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @yorunikakeru4 & @leopard-bf187
*
*  Description: allocator-aware Robin Hood hash table over TypeOps policies
*
*  Date: 09.10.2026
*/

#pragma once


#include "std_Allocator.h"
#include "std_TypeOps.h"


namespace krystallic
{
    namespace Stdlib
    {
        /// Robin Hood hash table over raw bucket storage allocated through
        /// Common::IAllocator. TKeyOps/TValueOps follow the TypeOps member
        /// contract (std_TypeOps.h): lifetime (CopyConstruct/MoveConstruct/
        /// MoveAssign/Destroy) plus Hash/IsEqual on the key side.
        ///
        /// Bucket occupancy:
        /// - a dedicated state byte (BUCKET_EMPTY/BUCKET_OCCUPIED) marks
        ///   occupancy, so `hash == 0` is an ordinary hash value and an empty
        ///   bucket is never inferred from a zero hash or a zeroed T value;
        /// - a freed bucket only flips its state byte, so no Null() policy on
        ///   T is required to release a bucket;
        /// - contract: IsEqual(a, b) implies Hash(a) == Hash(b) — enforced by
        ///   the key policy, the table never re-derives hashes.
        ///
        /// Load factor and probe distance:
        /// - Capacity() is the bucket count and Size() <= Capacity() is the
        ///   hard limit: Add/Set return false once the table is full
        ///   (load factor 1.0); keep Size() <= 75% of Capacity()
        ///   (RECOMMENDED_LOAD_FACTOR_PERCENT) for healthy probe distances;
        /// - every occupied bucket stores `dib` — its distance from the ideal
        ///   bucket `Hash % Capacity`. Insert is Robin Hood: the in-flight
        ///   entry steals a bucket from a richer (smaller-dib) resident and
        ///   the resident continues with dib + 1. Lookup walks the probe
        ///   sequence while the stored dib is >= the probe count, which is
        ///   the early-out that keeps lookups allocation-free;
        /// - deletion is backward shift: the hole walks forward through the
        ///   cluster, moving each entry one bucket toward its home (dib - 1)
        ///   and stops at an empty bucket or a cluster head (dib == 0).
        ///
        /// Growth contract: manual only. Resize/Reserve are the growth
        /// operations — there is no hidden automatic growth, so Add/Set fail
        /// with false when there is no room. Resize(newSize) rehashes into
        /// exactly newSize buckets and requires newSize > Size(); when the
        /// bucket allocation fails the original table is kept untouched.
        /// Lookup operations (Contains/Find/ProbeDistance) never allocate.
        ///
        /// Both a failing Add/Set (no room, duplicate key on Add) and a
        /// failing Resize preserve the previous table state; Set replaces the
        /// value of an existing key, Add refuses duplicates.
        template <typename TKey, typename TValue, typename TKeyOps = TypeOps<TKey>, typename TValueOps = TypeOps<TValue>>
        class RobinHoodHashTable
        {
        public:
            /// Documented soft load factor: keep Size() at or below this
            /// percentage of Capacity() when calling Resize manually.
            static constexpr uint RECOMMENDED_LOAD_FACTOR_PERCENT = 75;

            /// Occupancy state byte of a bucket. Distinct from the hash so a
            /// zero hash stays a valid hash value.
            static constexpr uint8 BUCKET_EMPTY = 0;
            static constexpr uint8 BUCKET_OCCUPIED = 1;

            /// Public view of a stored key/value pair. The key, hash and DIB
            /// are read-only: changing them would break the lookup invariants.
            /// A returned SPair* is invalidated by relocation (Add/Remove/
            /// Resize), and hash + DIB are not persistent identifiers.
            struct SPair
            {
            public:
                const TKey& GetKey() const { return *reinterpret_cast<const TKey*>(keyStorage); }
                TValue& GetValue() { return *reinterpret_cast<TValue*>(valueStorage); }
                const TValue& GetValue() const { return *reinterpret_cast<const TValue*>(valueStorage); }
                dword GetHash() const { return hash; }
                uint32 GetDib() const { return dib; }

            private:
                friend class RobinHoodHashTable;

                alignas(TKey) uint8 keyStorage[sizeof(TKey)];
                alignas(TValue) uint8 valueStorage[sizeof(TValue)];
                dword  hash;
                uint32 dib;
                uint8  state;
            };

            static constexpr uint NO_BUCKET = ~0u;

            static_assert(TKeyOps::HAS_HASH, "RobinHoodHashTable: TKeyOps must provide Hash (equal keys must hash equal)");
            static_assert(TKeyOps::HAS_EQUALITY, "RobinHoodHashTable: TKeyOps must provide IsEqual");
            static_assert(TKeyOps::IS_COPY_CONSTRUCTIBLE, "RobinHoodHashTable: keys must be copy-constructible through TKeyOps");
            static_assert(TValueOps::IS_COPY_CONSTRUCTIBLE, "RobinHoodHashTable: values must be copy-constructible through TValueOps");
            static_assert(TKeyOps::IS_MOVE_CONSTRUCTIBLE, "RobinHoodHashTable: keys must be move- or copy-constructible through TKeyOps");
            static_assert(TKeyOps::IS_MOVE_ASSIGNABLE, "RobinHoodHashTable: keys must be move- or copy-assignable through TKeyOps");
            static_assert(TValueOps::IS_MOVE_CONSTRUCTIBLE, "RobinHoodHashTable: values must be move- or copy-constructible through TValueOps");
            static_assert(TValueOps::IS_MOVE_ASSIGNABLE, "RobinHoodHashTable: values must be move- or copy-assignable through TValueOps");

            RobinHoodHashTable(Common::IAllocator* allocator = nullptr) : m_buckets(nullptr), m_capacity(0), m_size(0), m_allocator(allocator ? allocator : g_stdlibDefaultAllocator) {}

            /// Copies other into a table of the same capacity. Without an
            /// explicit allocator the copy keeps the source allocator. A
            /// failed bucket allocation leaves the copy empty instead of
            /// throwing (the engine builds without exceptions).
            RobinHoodHashTable(const RobinHoodHashTable& other, Common::IAllocator* allocator = nullptr) : m_buckets(nullptr), m_capacity(0), m_size(0), m_allocator(allocator ? allocator : AllocatorOf(other.m_allocator))
            {
                if (other.m_size == 0)
                {
                    return;
                }

                SPair* buckets = AllocateBucketsWith(m_allocator.Get(), other.m_capacity);
                if (!buckets)
                {
                    return;
                }

                m_buckets = buckets;
                m_capacity = other.m_capacity;
                CopyEntriesFrom(other);
            }

            RobinHoodHashTable(RobinHoodHashTable&& other) noexcept : m_buckets(other.m_buckets), m_capacity(other.m_capacity), m_size(other.m_size), m_allocator(std::move(other.m_allocator))
            {
                other.m_buckets = nullptr;
                other.m_capacity = 0;
                other.m_size = 0;
                other.m_allocator = g_stdlibDefaultAllocator;
            }

            ~RobinHoodHashTable() { Reset(); }

            /// Copies other, adopting the source allocator when it differs.
            /// Bucket storage is allocated before the current contents are
            /// released, so a failed allocation keeps this table unchanged.
            RobinHoodHashTable& operator=(const RobinHoodHashTable& other)
            {
                if (this == &other)
                {
                    return *this;
                }

                if (other.m_size == 0)
                {
                    if (m_allocator != other.m_allocator)
                    {
                        Reset();
                        m_allocator = other.m_allocator;
                    }
                    else
                    {
                        Clear();
                    }
                    return *this;
                }

                SPair* buckets = AllocateBucketsWith(AllocatorOf(other.m_allocator), other.m_capacity);
                if (!buckets)
                {
                    return *this;
                }

                FreeOwnStorage();
                m_allocator = other.m_allocator;
                m_buckets = buckets;
                m_capacity = other.m_capacity;
                m_size = 0;
                CopyEntriesFrom(other);
                return *this;
            }

            RobinHoodHashTable& operator=(RobinHoodHashTable&& other) noexcept
            {
                if (this == &other)
                {
                    return *this;
                }

                Reset();

                m_buckets = other.m_buckets;
                m_capacity = other.m_capacity;
                m_size = other.m_size;
                m_allocator = std::move(other.m_allocator);

                other.m_buckets = nullptr;
                other.m_capacity = 0;
                other.m_size = 0;
                other.m_allocator = g_stdlibDefaultAllocator;
                return *this;
            }

            /// Rehashes every entry into exactly newSize buckets. newSize must
            /// be greater than Size(). When the new bucket array cannot be
            /// allocated the current table stays untouched and false is
            /// returned; entries are moved, not copied, so rehashing does not
            /// bump ref counts or copy-construct values.
            bool Resize(uint newSize)
            {
                if (newSize == 0 || newSize <= m_size)
                {
                    return false;
                }

                SPair* buckets = AllocateBucketsWith(m_allocator.Get(), newSize);
                if (!buckets)
                {
                    return false;
                }

                RehashInto(buckets, newSize);
                return true;
            }

            /// Grows the table so it holds at least minElements entries.
            /// Never shrinks: a table that already has the capacity is left
            /// as is. Part of the manual growth contract with Resize.
            bool Reserve(uint minElements)
            {
                if (minElements <= m_capacity)
                {
                    return true;
                }

                return Resize(minElements);
            }

            /// Inserts a new key/value pair with Robin Hood insertion.
            /// Returns false when the table is full (no automatic growth) or
            /// the key is already present; on false the table is unchanged.
            bool Add(const TKey& key, const TValue& value)
            {
                if (m_capacity == 0 || m_size >= m_capacity)
                {
                    return false;
                }

                const dword hash = TKeyOps::Hash(key);
                if (FindInternal(key, hash) != nullptr)
                {
                    return false;
                }

                // The in-flight entry lives on the stack: Add allocates only
                // the bucket array, never per-entry scratch storage.
                alignas(SPair) uint8 carrierStore[sizeof(SPair)];
                SPair& carrier = *reinterpret_cast<SPair*>(carrierStore);
                TKeyOps::CopyConstruct(KeyOf(carrier), key);
                TValueOps::CopyConstruct(ValueOf(carrier), value);
                carrier.hash = hash;
                carrier.dib = 0;
                carrier.state = BUCKET_OCCUPIED;

                PlaceEntry(carrier);
                ++m_size;
                return true;
            }

            /// Inserts the pair when the key is absent, otherwise replaces
            /// the stored value of the existing key. Returns false only when
            /// the key is absent and the table has no room; a replace works
            /// even on a full table.
            bool Set(const TKey& key, const TValue& value)
            {
                if (TValue* found = Find(key))
                {
                    TValueOps::Destroy(found);
                    TValueOps::CopyConstruct(found, value);
                    return true;
                }

                return Add(key, value);
            }

            /// Removes the key with backward-shift deletion and returns
            /// true. Returns false when the key is absent.
            bool Remove(const TKey& key)
            {
                if (m_capacity == 0)
                {
                    return false;
                }

                const dword hash = TKeyOps::Hash(key);
                const uint  pos = FindBucketIndex(key, hash);
                if (pos == NO_BUCKET)
                {
                    return false;
                }

                Backshift(pos);
                --m_size;
                return true;
            }

            bool Contains(const TKey& key) const { return Find(key) != nullptr; }

            TValue* Find(const TKey& key)
            {
                if (m_capacity == 0)
                    return nullptr;

                return FindInternal(key, TKeyOps::Hash(key));
            }

            const TValue* Find(const TKey& key) const
            {
                if (m_capacity == 0)
                    return nullptr;

                return FindInternal(key, TKeyOps::Hash(key));
            }

            /// Uses a precomputed hash; the caller must pass TKeyOps::Hash(key).
            TValue* Find(const TKey& key, dword hash) { return FindInternal(key, hash); }
            const TValue* Find(const TKey& key, dword hash) const { return FindInternal(key, hash); }

            /// Returns the first pair with this hash, if present. A hash alone
            /// does not distinguish different keys that collide.
            TValue* FindByHash(dword hash)
            {
                SPair* pair = FindPairByHash(hash);
                return pair ? ValueOf(*pair) : nullptr;
            }

            const TValue* FindByHash(dword hash) const
            {
                const SPair* pair = FindPairByHash(hash);
                return pair ? ValueOf(*pair) : nullptr;
            }

            /// Direct O(1) access to the current bucket at Ideal(hash) + dib.
            /// Both fields must describe the pair in the current table state.
            TValue* FindByHash(dword hash, uint32 dib)
            {
                SPair* pair = FindPairByHash(hash, dib);
                return pair ? ValueOf(*pair) : nullptr;
            }

            const TValue* FindByHash(dword hash, uint32 dib) const
            {
                const SPair* pair = FindPairByHash(hash, dib);
                return pair ? ValueOf(*pair) : nullptr;
            }

            SPair* FindPair(const TKey& key)
            {
                if (m_capacity == 0)
                    return nullptr;

                return FindPairInternal(key, TKeyOps::Hash(key));
            }

            const SPair* FindPair(const TKey& key) const
            {
                if (m_capacity == 0)
                    return nullptr;

                return FindPairInternal(key, TKeyOps::Hash(key));
            }

            SPair* FindPair(const TKey& key, dword hash) { return FindPairInternal(key, hash); }
            const SPair* FindPair(const TKey& key, dword hash) const { return FindPairInternal(key, hash); }

            /// Returns the first pair with this hash, if present.
            SPair* FindPairByHash(dword hash)
            {
                const uint pos = FindBucketIndexByHash(hash);
                return pos == NO_BUCKET ? nullptr : &m_buckets[pos];
            }

            const SPair* FindPairByHash(dword hash) const
            {
                const uint pos = FindBucketIndexByHash(hash);
                return pos == NO_BUCKET ? nullptr : &m_buckets[pos];
            }

            /// Locates a bucket directly and verifies its current hash and DIB.
            SPair* FindPairByHash(dword hash, uint32 dib)
            {
                const uint pos = FindBucketIndexByHash(hash, dib);
                return pos == NO_BUCKET ? nullptr : &m_buckets[pos];
            }

            const SPair* FindPairByHash(dword hash, uint32 dib) const
            {
                const uint pos = FindBucketIndexByHash(hash, dib);
                return pos == NO_BUCKET ? nullptr : &m_buckets[pos];
            }

            /// Returns the stored probe distance of key (0 when the entry
            /// sits in its ideal bucket), or NO_BUCKET when the key is absent.
            /// Introspection helper: probe distances are otherwise internal.
            uint ProbeDistance(const TKey& key) const
            {
                if (m_capacity == 0)
                    return NO_BUCKET;

                const uint pos = FindBucketIndex(key, TKeyOps::Hash(key));
                return pos == NO_BUCKET ? NO_BUCKET : m_buckets[pos].dib;
            }

            /// Destroys every entry but keeps the bucket storage, so the
            /// table stays ready for inserts without re-allocating.
            void Clear()
            {
                for (uint i = 0; i < m_capacity; ++i)
                {
                    if (m_buckets[i].state == BUCKET_OCCUPIED)
                    {
                        DestroyEntry(m_buckets[i]);
                        m_buckets[i].state = BUCKET_EMPTY;
                    }
                }
                m_size = 0;
            }

            /// Clears the table and releases the bucket storage. The
            /// allocator is kept; Capacity() becomes 0.
            void Reset()
            {
                Clear();
                if (m_buckets)
                {
                    m_allocator->Free(m_buckets);
                    m_buckets = nullptr;
                }
                m_capacity = 0;
            }

            uint Size() const { return m_size; }

            /// RefCounted::Get() is const-preserving; the container always
            /// hands out a mutable allocator pointer (Vector behaves the same).
            static Common::IAllocator* AllocatorOf(const RefCounted<Common::IAllocator>& allocator) { return const_cast<Common::IAllocator*>(allocator.Get()); }

            uint Capacity() const { return m_capacity; }

            Common::IAllocator* GetAllocator() const { return AllocatorOf(m_allocator); }

        private:

            SPair* m_buckets;
            uint     m_capacity;
            uint     m_size;

            RefCounted<Common::IAllocator> m_allocator;

            static TKey* KeyOf(SPair& bucket) { return reinterpret_cast<TKey*>(bucket.keyStorage); }

            static const TKey* KeyOf(const SPair& bucket) { return reinterpret_cast<const TKey*>(bucket.keyStorage); }

            static TValue* ValueOf(SPair& bucket) { return reinterpret_cast<TValue*>(bucket.valueStorage); }

            static const TValue* ValueOf(const SPair& bucket) { return reinterpret_cast<const TValue*>(bucket.valueStorage); }

            uint Ideal(dword hash) const { return static_cast<uint>(hash % m_capacity); }

            uint Next(uint pos) const { return pos + 1 == m_capacity ? 0 : pos + 1; }

            /// Allocates and initializes a raw bucket array (all buckets
            /// empty, no T constructed). Returns nullptr when the allocator
            /// refuses or the byte count would overflow.
            static SPair* AllocateBucketsWith(Common::IAllocator* allocator, uint capacity)
            {
                if (capacity == 0 || capacity > ~uint(0) / static_cast<uint>(sizeof(SPair)))
                    return nullptr;

                void* memory = allocator->Alloc(capacity * static_cast<uint>(sizeof(SPair)), alignof(SPair));
                if (!memory)
                    return nullptr;

                SPair* buckets = static_cast<SPair*>(memory);
                for (uint i = 0; i < capacity; ++i)
                {
                    buckets[i].hash = 0;
                    buckets[i].dib = 0;
                    buckets[i].state = BUCKET_EMPTY;
                }

                return buckets;
            }

            static void DestroyEntry(SPair& bucket)
            {
                TKeyOps::Destroy(KeyOf(bucket));
                TValueOps::Destroy(ValueOf(bucket));
            }

            /// Moves src (live) into dst (raw storage) including metadata and
            /// leaves src moved-from but alive; the caller destroys src.
            static void MoveInto(SPair& dst, SPair& src)
            {
                TKeyOps::MoveConstruct(KeyOf(dst), *KeyOf(src));
                TValueOps::MoveConstruct(ValueOf(dst), *ValueOf(src));
                dst.hash = src.hash;
                dst.dib = src.dib;
                dst.state = BUCKET_OCCUPIED;
            }

            /// Swaps two live entries through a raw scratch bucket: the
            /// content travels src -> scratch -> dst and both endpoints end
            /// alive with exchanged hash/dib metadata.
            static void SwapBucket(SPair& a, SPair& b)
            {
                alignas(SPair) uint8 scratchStore[sizeof(SPair)];
                SPair* scratch = reinterpret_cast<SPair*>(scratchStore);

                MoveInto(*scratch, a);
                DestroyEntry(a);
                MoveInto(a, b);
                DestroyEntry(b);
                MoveInto(b, *scratch);
                DestroyEntry(*scratch);
            }

            /// Robin Hood placement of a live in-flight entry: steals buckets
            /// from richer residents until an empty bucket shows up, then
            /// moves the entry in and destroys the in-flight (moved-from)
            /// copy. Requires a free bucket in the table; does not touch
            /// m_size (callers account for it).
            void PlaceEntry(SPair& incoming)
            {
                uint pos = Ideal(incoming.hash);
                while (m_buckets[pos].state == BUCKET_OCCUPIED)
                {
                    if (m_buckets[pos].dib < incoming.dib)
                        SwapBucket(m_buckets[pos], incoming);

                    ++incoming.dib;
                    pos = Next(pos);
                }

                MoveInto(m_buckets[pos], incoming);
                DestroyEntry(incoming);
            }

            /// Rehashes one entry out of the storage being drained. The old
            /// bucket itself acts as the in-flight carrier (its storage dies
            /// with the drained array), so an entry is moved exactly once
            /// unless Robin Hood swaps happen.
            void RehashEntry(SPair& source)
            {
                source.dib = 0;
                uint pos = Ideal(source.hash);
                while (m_buckets[pos].state == BUCKET_OCCUPIED)
                {
                    if (m_buckets[pos].dib < source.dib)
                        SwapBucket(m_buckets[pos], source);

                    ++source.dib;
                    pos = Next(pos);
                }

                MoveInto(m_buckets[pos], source);
                DestroyEntry(source);
                ++m_size;
            }

            void RehashInto(SPair* buckets, uint capacity)
            {
                SPair* old = m_buckets;
                const uint oldCapacity = m_capacity;

                m_buckets = buckets;
                m_capacity = capacity;
                m_size = 0;

                if (old)
                {
                    for (uint i = 0; i < oldCapacity; ++i)
                        if (old[i].state == BUCKET_OCCUPIED)
                            RehashEntry(old[i]);

                    m_allocator->Free(old);
                }
            }

            void CopyEntriesFrom(const RobinHoodHashTable& other)
            {
                for (uint i = 0; i < other.m_capacity; ++i)
                {
                    const SPair& source = other.m_buckets[i];
                    if (source.state != BUCKET_OCCUPIED)
                        continue;

                    alignas(SPair) uint8 carrierStore[sizeof(SPair)];
                    SPair& carrier = *reinterpret_cast<SPair*>(carrierStore);
                    TKeyOps::CopyConstruct(KeyOf(carrier), *KeyOf(source));
                    TValueOps::CopyConstruct(ValueOf(carrier), *ValueOf(source));
                    carrier.hash = source.hash;
                    carrier.dib = 0;
                    carrier.state = BUCKET_OCCUPIED;

                    PlaceEntry(carrier);
                    ++m_size;
                }
            }

            void FreeOwnStorage()
            {
                if (!m_buckets)
                {
                    m_size = 0;
                    m_capacity = 0;
                    return;
                }

                Clear();
                m_allocator->Free(m_buckets);
                m_buckets = nullptr;
                m_capacity = 0;
            }

            uint FindBucketIndex(const TKey& key, dword hash) const
            {
                if (m_capacity == 0)
                    return NO_BUCKET;

                uint pos = Ideal(hash);
                uint probe = 0;

                while (m_buckets[pos].state == BUCKET_OCCUPIED && m_buckets[pos].dib >= probe)
                {
                    if (m_buckets[pos].hash == hash && TKeyOps::IsEqual(*KeyOf(m_buckets[pos]), key))
                        return pos;

                    ++probe;
                    pos = Next(pos);
                }

                return NO_BUCKET;
            }

            /// Finds the first bucket matching the hash, without key equality.
            uint FindBucketIndexByHash(dword hash) const
            {
                if (m_capacity == 0)
                    return NO_BUCKET;

                uint pos = Ideal(hash);
                uint probe = 0;

                while (m_buckets[pos].state == BUCKET_OCCUPIED && m_buckets[pos].dib >= probe)
                {
                    if (m_buckets[pos].hash == hash)
                        return pos;

                    ++probe;
                    pos = Next(pos);
                }

                return NO_BUCKET;
            }

            /// Finds exactly the current bucket at hash's ideal position + DIB.
            uint FindBucketIndexByHash(dword hash, uint32 dib) const
            {
                if (m_capacity == 0 || dib >= m_capacity)
                    return NO_BUCKET;

                const uint ideal = Ideal(hash);
                const uint pos = dib < m_capacity - ideal ? ideal + dib : dib - (m_capacity - ideal);
                const SPair& pair = m_buckets[pos];

                return pair.state == BUCKET_OCCUPIED && pair.hash == hash && pair.dib == dib ? pos : NO_BUCKET;
            }

            TValue* FindInternal(const TKey& key, dword hash)
            {
                const uint pos = FindBucketIndex(key, hash);
                return pos == NO_BUCKET ? nullptr : ValueOf(m_buckets[pos]);
            }

            const TValue* FindInternal(const TKey& key, dword hash) const
            {
                const uint pos = FindBucketIndex(key, hash);
                return pos == NO_BUCKET ? nullptr : ValueOf(m_buckets[pos]);
            }

            SPair* FindPairInternal(const TKey& key, dword hash)
            {
                const uint pos = FindBucketIndex(key, hash);
                return pos == NO_BUCKET ? nullptr : &m_buckets[pos];
            }

            const SPair* FindPairInternal(const TKey& key, dword hash) const
            {
                const uint pos = FindBucketIndex(key, hash);
                return pos == NO_BUCKET ? nullptr : &m_buckets[pos];
            }

            /// Backward-shift deletion: destroys the entry at pos and walks
            /// the hole forward through the cluster, moving every entry one
            /// bucket back toward its home (dib - 1). Stops at an empty
            /// bucket, at a cluster head (dib == 0), or when the hole would
            /// wrap back onto the deleted slot.
            void Backshift(uint pos)
            {
                DestroyEntry(m_buckets[pos]);

                uint cur = pos;
                while (true)
                {
                    const uint next = Next(cur);
                    if (next == pos || m_buckets[next].state != BUCKET_OCCUPIED || m_buckets[next].dib == 0)
                    {
                        break;
                    }

                    MoveInto(m_buckets[cur], m_buckets[next]);
                    // The entry slides one bucket toward its home, so its
                    // stored probe distance shrinks by exactly one (the loop
                    // only moves entries with dib > 0, so this never wraps).
                    m_buckets[cur].dib = m_buckets[next].dib - 1;
                    DestroyEntry(m_buckets[next]);
                    m_buckets[next].state = BUCKET_EMPTY;
                    cur = next;
                }

                m_buckets[cur].state = BUCKET_EMPTY;
            }
        };


        /// Map-style spelling kept for AGENTS.md compatibility
        /// (std::unordered_map replacement): same table under the documented
        /// name, so call sites do not have to rename their template.
        template <typename TKey, typename TValue, typename TKeyOps = TypeOps<TKey>, typename TValueOps = TypeOps<TValue>> using RobbinHoodHashTableKV = RobinHoodHashTable<TKey, TValue, TKeyOps, TValueOps>;


        /// Set-style spelling kept for AGENTS.md compatibility
        /// (std::unordered_set replacement): the element is stored as both
        /// the key and the value, so Add(Element, Element) inserts and Find
        /// returns the stored value copy.
        template <typename T, typename TOps = TypeOps<T>> using RobbinHoodHashTable = RobinHoodHashTable<T, T, TOps, TOps>;
    } // namespace Stdlib
} // namespace krystallic
