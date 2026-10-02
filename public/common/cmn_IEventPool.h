/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IEventPool interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"
#include "stdlib/std_SharedPtr.h"

namespace krystallic
{
    namespace Common
    {
        struct IEventPool : IBase
        {
            virtual ~IEventPool() = default;
            virtual Stdlib::SharedPtr<IEvent> Acquire() = 0;

            inline static SGuid GUID()
            {
                return {0xe2e03ceb, 0xaa8a, 0x42a9, {0xa9, 0xb2, 0x30, 0x40, 0x50, 0x0e, 0x21, 0x1d}};
            }
        };
    } // namespace Common
} // namespace krystallic