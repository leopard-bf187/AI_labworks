/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IExecutor interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IExecutor : IBase
        {
            virtual ERRCODE Execute(ITask* task) = 0;
            virtual ERRCODE Submit(const STaskDesc* desc, ITask** outTask) = 0;
        };
    } // namespace Common
} // namespace krystallic