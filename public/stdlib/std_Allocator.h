/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @Ra192192 & @yorunikakeru4
*
*  Description: stdlib default allocator declaration
*
*  Date: 03.06.2026
*/

#pragma once

#include "std_dll_types.h"
#include "../common/cmn_IAllocator.h"

namespace krystallic
{
    namespace Stdlib
    {
        //struct STDLIB_API CStdAllocator : Common::IAllocator
        //{
        //    void*  Alloc(uint64 size, uint64 alignment = alignof(void*)) override;
        //    void   Free(void* ptr) override;
        //    uint32 IncRef() override;
        //    uint32 Delete() override;
        //    uint32 QueryIFace(const SGuid& guid, void** iface) override;
        //};

        // Global fallback allocator for Stdlib. Contract: Alloc/Realloc honor a
        // power-of-two `alignment` (asserts and returns nullptr otherwise),
        // Alloc(0, ...) returns nullptr, Free(nullptr) is safe, Realloc keeps
        // min(oldSize, newSize) bytes and leaves the old block valid on failure.
        // Ref-counting must never destroy this static instance.
        // Rule: any Stdlib container that owns dynamic memory gets it through its
        // Common::IAllocator (this one by default), and blocks must never be
        // mixed between different allocator implementations.
        extern "C" STDLIB_API Common::IAllocator* g_stdlibDefaultAllocator;
    } // namespace Stdlib
} // namespace krystallic
