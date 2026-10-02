/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @yorunikakeru4
*
*  Description: Common API types declarations and definitions 
*
*  Date: 02.06.2026
*/

#pragma once

#include "../pch.h"
#include "../base.h"
#include "cmn_enums.h"

namespace krystallic
{
    namespace Common
    {
        typedef uint32 ThreadID;

        struct IAsyncCallback;
        struct IAsynchronous;
        struct IAsyncOperation;
        struct IBuffer;
        struct ICancallable;
        struct IAllocator;
        struct IDispatcher;
        struct IEvent;
        struct IEventListener;
        struct IEventManager;
        struct IEventPool;
        struct IExecutor;
        struct IInputStream;
        struct IOutputStream;
        struct IProgressable;
        struct ISerializable;
        struct ISerializer;
        struct IStreamable;
        struct ITask;
        struct IWaitable;
        struct IError;

        struct STaskDesc
        {
            const char* name;
            void*       userData;
        };

    } // namespace Common
} // namespace krystallic
