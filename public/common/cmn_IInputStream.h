/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IInputStream interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IInputStream
        {
            virtual ERRCODE Read(void* buffer, uint64 size, uint64* outRead) = 0;
            virtual ERRCODE Seek(uint64 offset) = 0;
            virtual uint64  Tell() const = 0;
            virtual uint64  GetSize() const = 0;
            virtual bool    IsEOF() const = 0;
        };
    } // namespace Common
} // namespace krystallic