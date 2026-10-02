/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IStreamable interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IStreamable
        {
            virtual ERRCODE StreamIn(IInputStream* stream, uint64 bytesToRead, uint64* outRead) = 0;
            virtual ERRCODE StreamOut(IOutputStream* stream, uint64 bytesToWrite, uint64* outWritten) = 0;
            virtual bool    IsStreaming() const = 0;
            virtual float   GetStreamingProgress() const = 0;
        };
    } // namespace Common
} // namespace krystallic