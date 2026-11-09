/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IOutputStream interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IOutputStream
        {
            virtual ERRCODE Write(const void* buffer, uint64 size, uint64* outWritten) = 0;
            virtual ERRCODE Seek(uint64 offset) = 0;
            virtual uint64  Tell() const = 0;
            virtual ERRCODE Flush() = 0;
        };
    } // namespace Common
} // namespace krystallic