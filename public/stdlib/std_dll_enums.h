/*
*  Copyright (c) BytesForge 2022-2025
*
*  Authors: @leopard-bf187 & @Ra192192
*
*  Description: list class declaration source code
*
*  Date: 24.04.2025
*/


#pragma once


#include "../pch.h"


namespace krystallic
{
    namespace Stdlib
    {
        enum EAtomicMemoryOrder
        {
            ATOMIC_MEMORY_ORDER_RELAXED = 0,
            ATOMIC_MEMORY_ORDER_ACQUIRE,
            ATOMIC_MEMORY_ORDER_RELEASE,
            ATOMIC_MEMORY_ORDER_ACQ_REL,
            ATOMIC_MEMORY_ORDER_SEQ_CST
        };

        enum EAtomicCounterOpResult
        {
            ATOMIC_COUNTER_OP_OK = 0,
            ATOMIC_COUNTER_OP_FAIL,
            ATOMIC_COUNTER_OP_INVALID_ARG,
            ATOMIC_COUNTER_OP_NOT_SUPPORTED
        };
    } // namespace Stdlib
} // namespace krystallic