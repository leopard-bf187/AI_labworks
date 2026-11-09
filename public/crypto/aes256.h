/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 
*
*  Description: AES-256 library
*
*  Date: 22.06.2025
*/


#pragma once

#include "../pch.h"

#if defined(__SSE2__) || defined(_M_AMD64) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#include <immintrin.h>
#else
typedef struct { unsigned char bytes[16]; } __m128i;
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef		uint8		AES_SRC_KEY[32];

typedef struct _AES_ENC_KEY
{
    __m128i m_key[16];
} AES_ENC_KEY, AES_DEC_KEY;

typedef		char		GAME_KEY[30];
typedef		uint8		DATA_BLOCK[16];


// hash functions
dword FNV1A32_Hash(const void* key, const uint32 len);
qword FNV1A64_Hash(const void* key, const uint64 len);

void          CRC32_InitChecksum(unsigned long* crcvalue);
void          CRC32_UpdateChecksum(unsigned long* crcvalue, const void* data, int length);
void          CRC32_FinishChecksum(unsigned long* crcvalue);
unsigned long CRC32_BlockChecksum(const void* data, int length);


// AES functions
//extern void generate_aes256_key(uint8 key[32]);
void aes256_key_expansion(const AES_SRC_KEY key, AES_ENC_KEY* round_keys);
void aes256_invert_keys(const AES_ENC_KEY* enc_keys, AES_DEC_KEY* dec_keys);
void aes256_encrypt_block(const uint8* in, uint8* out, AES_ENC_KEY* round_keys);
void aes256_decrypt_block(const uint8* in, uint8* out, const AES_DEC_KEY* dec_keys);


// game key @leopard
void InitKeyData(char* name, char* surname, char* nick, AES_SRC_KEY key);
void InitEncryptionData(qword hash1, qword hash2, DATA_BLOCK data);
void GetGameKey(uint8* enc1, uint8* enc2, GAME_KEY key);
//void GameKey_KeyGen();
//void GameKey_KeyReg();


// cdkey advanced @leopard

typedef struct _CDKEY
{
    char m_key[32];
} CDKEY;


typedef struct _CDKEY_USER_DATA
{
    char m_name[64];
    char m_surname[64];
    char m_username[64];
    char m_email[64];
} CDKEY_USER_DATA;


typedef	struct _CDKEY_ENC_USER_DATA
{
    char data[256];
} CDKEY_ENC_DATA;


typedef struct _CDKEY_CRYPTO_DATA
{
    union
    {
        struct
        {
            qword m_hash1;
            qword m_hash2;
            qword m_hash3;
            qword m_hash4;
        } m_hashParts;
        qword       m_hash[4];
        AES_SRC_KEY m_aesKey;
    };
    AES_ENC_KEY m_encKey;
    AES_DEC_KEY m_decKey;
    dword       m_crc32;
} CDKEY_CRYPTO_DATA;


void InputUserData(CDKEY_USER_DATA* data);
void GenerateEncodingKeys(CDKEY_USER_DATA* data, CDKEY_CRYPTO_DATA* outCrypto);
void EncodeUserData(CDKEY_USER_DATA* data, CDKEY_CRYPTO_DATA* crypto, CDKEY_ENC_DATA* outEncoded);
void DecodeUserData(CDKEY_ENC_DATA* encoded, CDKEY_CRYPTO_DATA* crypto, CDKEY_USER_DATA* outData);
void GenerateCDKey(CDKEY_ENC_DATA* encoded, CDKEY_CRYPTO_DATA* crypto, CDKEY* outKey);
int  ValidateCDKey(CDKEY_USER_DATA* data, CDKEY* key);

void PrintUserData(CDKEY_USER_DATA* user, CDKEY* key);
void PrintCryptoData(CDKEY_CRYPTO_DATA* crypto, int flags);
void PrintEncoded(CDKEY_ENC_DATA* encoded);
void PrintCDKeyDataToFile(CDKEY_USER_DATA* user, CDKEY* cdkey, CDKEY_CRYPTO_DATA* crypto, int flags, CDKEY_ENC_DATA* encoded);



#ifdef __cplusplus
}
#endif


