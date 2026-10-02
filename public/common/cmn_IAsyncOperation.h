/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IAsyncOperation interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"
#include "cmn_IAsynchronus.h"

namespace krystallic
{
    namespace Common
    {
        struct IAsyncOperation : public IAsynchronous
        {
            virtual ERRCODE Cancel() = 0;
            virtual ERRCODE SetCallback(IAsyncCallback* callback) = 0;
        };
    } // namespace Common
} // namespace krystallic