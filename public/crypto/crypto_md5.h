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
        // Legacy compatibility hash; not suitable for passwords or signatures.
        // Context must be initialized; data may be null only when size is zero.
        void MD5Init(SMD5Context* context);
        void MD5Update(SMD5Context* context, const void* data, uint64 size);

        // Writes 16 digest bytes in algorithm order and resets context to zero.
        // Digest must be valid and must not overlap context. Reinitialize before reuse.
        void MD5Final(SMD5Context* context, uint8 digest[16]);
        void MD5Hash(const void* data, uint64 size, uint8 digest[16]);
    }
}

