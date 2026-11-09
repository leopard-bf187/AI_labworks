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


#include "pch.h"


#include <climits>
#include <type_traits>


#include "crypto_lib_enums.h"


#define CRYPTO_VERSION_MAJOR 0x01
#define CRYPTO_VERSION_MINOR 0x02
#define CRYPTO_VERSION_PATCH 0x00


namespace krystallic
{
    namespace Crypto
    {
        static_assert(CHAR_BIT == 8 && sizeof(uint8) == 1);
        static_assert(sizeof(dword) == 4 && sizeof(uint32) == 4);
        static_assert(sizeof(qword) == 8 && sizeof(uint64) == 8);
        static_assert(std::is_unsigned_v<uint64>);

        struct SCRC32Context { dword state; };

        struct SMD4Context
        {
            dword state[4];
            uint64 byteCount;
            uint8 buffer[64];
            uint32 buffered;
        };

        struct SMD5Context
        {
            dword state[4];
            uint64 byteCount;
            uint8 buffer[64];
            uint32 buffered;
        };

        static_assert(std::is_trivially_copyable_v<SCRC32Context>);
        static_assert(std::is_trivially_copyable_v<SMD4Context>);
        static_assert(std::is_trivially_copyable_v<SMD5Context>);
    }
}

