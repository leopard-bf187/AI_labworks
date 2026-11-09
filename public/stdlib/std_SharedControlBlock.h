/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @yorunikakeru4
*
*  Description: shared pointer control block
*
*  Date: 06.06.2026
*/

#pragma once

#include "std_Allocator.h"

namespace krystallic
{
    namespace Stdlib
    {
        struct SharedControlBlock
        {
            Common::IAllocator* allocator;
            uint                strongRefs;
            uint                weakRefs;
            void*               object;

            void (*destroyObject)(void* object, Common::IAllocator* allocator);
            void (*destroyBlock)(SharedControlBlock* block, Common::IAllocator* allocator);
        };
    } // namespace Stdlib
} // namespace krystallic
