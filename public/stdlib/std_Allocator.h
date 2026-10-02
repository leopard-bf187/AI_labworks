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
        //    void*  Alloc(uint size, uint alignment = alignof(void*)) override;
        //    void   Free(void* ptr) override;
        //    uint32 IncRef() override;
        //    uint32 Delete() override;
        //    uint32 QueryIFace(const SGuid& guid, void** iface) override;
        //};

        extern "C" STDLIB_API Common::IAllocator* g_stdlibDefaultAllocator;
    } // namespace Stdlib
} // namespace krystallic
