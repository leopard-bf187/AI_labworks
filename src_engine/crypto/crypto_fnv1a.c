/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 
*
*  Description: FNV1A hashing
*
*  Date: 22.06.2025
*/

#include "crypto.h"


dword FNV1A32_Hash(const void* key, const uint32 len)
{
    const char* data = (char*)key;
    uint32 hash = 0x811c9dc5;
    uint32 prime = 0x1000193;

    for (uint32 i = 0; i < len; ++i) {
        uint8 value = data[i];
        hash = hash ^ value;
        hash *= prime;
    }

    return hash;
}


qword FNV1A64_Hash(const void* key, const uint64 len)
{
    const char* data = (char*)key;
    uint64 hash = 0xcbf29ce484222325;
    uint64 prime = 0x100000001b3;

    for (uint i = 0; i < len; ++i) {
        uint8 value = data[i];
        hash = hash ^ value;
        hash *= prime;
    }

    return hash;
}