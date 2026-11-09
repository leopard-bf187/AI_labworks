/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IAsyncCallback interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IAsyncCallback : IBase
        {
            virtual void OnAsyncCompleted(IAsynchronous* operation, ERRCODE result) = 0;
        };
    } // namespace Common
} // namespace krystallic