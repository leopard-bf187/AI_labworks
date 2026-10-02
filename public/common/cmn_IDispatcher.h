/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IDispatcher interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IDispatcher : IBase
        {
            virtual ERRCODE  Post(ITask* task) = 0;
            virtual ERRCODE  Send(ITask* task) = 0;
            virtual ERRCODE  ProcessTasks(uint32 maxTasks) = 0;
            virtual ThreadID GetOwnerThreadID() const = 0;
            virtual bool     IsOwnerThread() const = 0;
        };
    } // namespace Common
} // namespace krystallic