/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IProgressable interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IProgressable
        {
            virtual float  GetProgress() const = 0;
            virtual uint64 GetCompletedWork() const = 0;
            virtual uint64 GetTotalWork() const = 0;
        };
    } // namespace Common
} // namespace krystallic