/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @yorunikakeru4
*
*  Description: Common allocator interface
*
*  Date: 03.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IAllocator : IBase
        {
            virtual void* Alloc(uint size, uint alignment = alignof(void*)) = 0;
            virtual void* Realloc(void* ptr, uint size, uint alignment = alignof(void*)) = 0;
            virtual void  Free(void* ptr) = 0;

            inline static SGuid GUID()
            {
                return {0x46fc68b1, 0x4f48, 0x4f5e, {0xa1, 0x3d, 0x11, 0x49, 0x6f, 0xf4, 0xc9, 0x80}};
            }
        };


    } 
}
