/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 
*
*  Description: hash library
*
*  Date: 22.06.2025
*/


#pragma once


#include "../pch.h"


#ifdef __cplusplus
extern "C" {
#endif



// fnv1a
dword FNV1A32_Hash(const void* key, const uint32 len);
qword FNV1A64_Hash(const void* key, const uint64 len);


/*
CRC32 MD4 MD5
Sources imported from DOOM 3 BFG Edition
*/


// crc32
void          CRC32_InitChecksum(unsigned long* crcvalue);
void          CRC32_UpdateChecksum(unsigned long* crcvalue, const void* data, int length);
void          CRC32_FinishChecksum(unsigned long* crcvalue);
unsigned long CRC32_BlockChecksum(const void* data, int length);



// md4
typedef struct _MD4_CTX
{
    unsigned int  state[4];
    unsigned int  count[2];
    unsigned char buffer[64];
} MD4_CTX;

void          MD4_Init(MD4_CTX* context);
void          MD4_Update(MD4_CTX* context, const unsigned char* input, unsigned int inputLen);
void          MD4_Final(MD4_CTX* context, unsigned char digest[16]);
unsigned long MD4_BlockChecksum(const void* data, int length);



// md5
typedef struct _MD5_CTX
{
    unsigned int  state[4];
    unsigned int  bits[2];
    unsigned char in[64];
} MD5_CTX;

void         MD5_Init(MD5_CTX* ctx);
void         MD5_Update(MD5_CTX* context, const unsigned char* input, unsigned int inputLen);
void         MD5_Final(MD5_CTX* context, unsigned char digest[16]);
unsigned int MD5_BlockChecksum(const void* data, uint length);



#ifdef __cplusplus
}
#endif

