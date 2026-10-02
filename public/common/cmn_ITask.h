/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: ITask interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"
#include "cmn_IAsyncOperation.h"

namespace krystallic
{
    namespace Common
    {
        struct ITask : public IAsyncOperation
        {
            virtual ERRCODE Execute() = 0;
        };
    } // namespace Common
} // namespace krystallic