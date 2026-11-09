/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: Common API enumeration definitions
*
*  Date: 02.06.2026
*/

#pragma once


#include "../pch.h"


namespace krystallic
{
    namespace Common
    {
        enum EEventListenerPriority
        {
            EVENT_LISTENER_PRIOR_LOWEST,
            EVENT_LISTENER_PRIOR_VERY_LOW,
            EVENT_LISTENER_PRIOR_LOW,
            EVENT_LISTENER_PRIOR_NORMAL,
            EVENT_LISTENER_PRIOR_MEDIUM,
            EVENT_LISTENER_PRIOR_HIGH,
            EVENT_LISTENER_PRIOR_VERY_HIGH,
            EVENT_LISTENER_PRIOR_HIGHEST,
        };

        enum EEventListenerResult : ERRCODE
        {
            EVENT_LISTENER_OK = 0,
            EVENT_LISTENER_ERROR,
            EVENT_LISTENER_INVALID_EVENT = 0x80000000ul,
            EVENT_LISTENER_INTERRUPT_EVENT_HANDLING,
            EVENT_LISTENER_INTERRUPT_EVENT_HANDLING_REMOVE_THIS,
        };

        enum EAsyncState : uint32
        {
            ASYNC_STATE_PENDING,
            ASYNC_STATE_RUNNING,
            ASYNC_STATE_COMPLETED,
            ASYNC_STATE_FAILED,
            ASYNC_STATE_CANCELLED
        };

        enum ESerializerMode : uint32
        {
            SERIALIZER_MODE_READ,
            SERIALIZER_MODE_WRITE
        };

        enum ESerializerError : ERRCODE
        {
            SER_ERR_OK,
            SER_ERR_INVALID_ARGUMENT,
            SER_ERR_NOT_SUPPORTED,
            SER_ERR_NOT_IMPLEMENTED,
            SER_ERR_NOT_FOUND,
            SER_ERR_BACKEND_ERR,
            SER_ERR_INVALID_VAR_TO_READ,
            SER_ERR_INVALID_VAR_TO_WRITE,
            SER_ERR_NULL_VALUE,
            SER_ERR_INVALID_SERIALIZER_MODE,
        };
    } // namespace Common
} // namespace krystallic