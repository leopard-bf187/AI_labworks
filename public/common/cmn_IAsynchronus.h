/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IAsynchronous interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"
#include "cmn_IWaitable.h"

namespace krystallic
{
    namespace Common
    {
        struct IAsynchronous : public IWaitable
        {
            virtual EAsyncState GetState() const = 0;
            virtual bool        IsPending() const = 0;
            virtual bool        IsRunning() const = 0;
            virtual bool        IsCompleted() const = 0;
            virtual bool        IsFailed() const = 0;
            virtual bool        IsCancelled() const = 0;
            virtual bool        IsFinished() const = 0;
            virtual ERRCODE     GetResultCode() const = 0;
        };
    } // namespace Common
} // namespace krystallic