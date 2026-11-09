/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 08.10.2026
*/


#pragma once


#include "crypto_lib_types.h"


namespace krystallic
{
    namespace Crypto
    {
        // CRC-32/IEEE, reflected polynomial 0xEDB88320; init/final XOR 0xFFFFFFFF.
        // Context must be initialized; data may be null only when size is zero.
        void CRC32Init(SCRC32Context* context);
        void CRC32Update(SCRC32Context* context, const void* data, uint64 size);

        // Finish is non-mutating and repeatable; further Update remains valid.
        dword CRC32Finish(const SCRC32Context* context);
        dword CRC32Hash(const void* data, uint64 size);
    }
}

