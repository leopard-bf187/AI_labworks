/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IBuffer interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IBuffer : public IBase
        {
            virtual ~IBuffer() = default;
            virtual void* GetBufferPointer() = 0;
            virtual uint  GetBufferSize() = 0;
        };
    } // namespace Common
} // namespace krystallic