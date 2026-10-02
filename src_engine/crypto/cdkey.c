/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 
*
*  Description: cdkey function
*
*  Date: 22.06.2025
*/


#include "crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static const char g_Base32[33] = "ABC2DEFG3HIJK4LMN05PQRS6TUVW7XYZ";


// inner
int IndexOfBase32(char s)
{
    for (int i = 0; i < sizeof(g_Base32); i++)
        if (s == g_Base32[i])
            return i;
    return 0xff;
}


void FormatKey(char raw[28], CDKEY* out)
{
    char* p = out->m_key;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
            *p++ = raw[i * 5 + j];
        *p++ = '-';
    }
    out->m_key[29] = raw[25];
    out->m_key[30] = raw[26];
    out->m_key[31] = 0;
}


void UnformatKey(CDKEY* fmt, char raw[28])
{
    char* p = fmt->m_key;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
            raw[i * 5 + j] = *p++;
        p++;
    }
    p--;
    raw[25] = *p++;
    raw[26] = *p++;
    raw[27] = 0;
}


// outer
void InputUserData(CDKEY_USER_DATA* data)
{
    if (!data)
        return;

    memset(data, 0, sizeof(CDKEY_USER_DATA));

    printf("Type your data: \n");
    printf("  Name:      ");
    scanf(" %s", data->m_name);
    printf("  Surname:   ");
    scanf(" %s", data->m_surname);
    printf("  User name: ");
    scanf(" %s", data->m_username);
    printf("  Email:     ");
    scanf(" %s", data->m_email);
}


void GenerateEncodingKeys(CDKEY_USER_DATA* data, CDKEY_CRYPTO_DATA* outCrypto)
{
    if (!data || !outCrypto)
        return;

    outCrypto->m_hash[0] = FNV1A64_Hash(data->m_name, strlen(data->m_name));
    outCrypto->m_hash[1] = FNV1A64_Hash(data->m_username, strlen(data->m_username));
    outCrypto->m_hash[2] = FNV1A64_Hash(data->m_surname, strlen(data->m_surname));
    outCrypto->m_hash[3] = FNV1A64_Hash(data->m_email, strlen(data->m_email));

    aes256_key_expansion(outCrypto->m_aesKey, &outCrypto->m_encKey);
    aes256_invert_keys(&outCrypto->m_encKey, &outCrypto->m_decKey);

    outCrypto->m_crc32 = CRC32_BlockChecksum(data, sizeof(CDKEY_USER_DATA));
}


void EncodeUserData(CDKEY_USER_DATA* data, CDKEY_CRYPTO_DATA* crypto, CDKEY_ENC_DATA* outEncoded)
{
    if (!data || !crypto || !outEncoded)
        return;

    memset(outEncoded, 0, sizeof(CDKEY_ENC_DATA));

    for (int i = 0; i < 16; i++)
    {
        uint8* src = ((uint8*) data) + i * 16;
        uint8* dst = ((uint8*) outEncoded) + i * 16;
        aes256_encrypt_block(src, dst, &crypto->m_encKey);
    }
}


void DecodeUserData(CDKEY_ENC_DATA* encoded, CDKEY_CRYPTO_DATA* crypto, CDKEY_USER_DATA* outData)
{
    if (!encoded || !crypto || !outData)
        return;

    memset(outData, 0, sizeof(CDKEY_USER_DATA));

    for (int i = 0; i < 16; i++)
    {
        uint8* src = ((uint8*) encoded) + i * 16;
        uint8* dst = ((uint8*) outData) + i * 16;
        aes256_decrypt_block(src, dst, &crypto->m_decKey);
    }
}


void GenerateCDKey(CDKEY_ENC_DATA* encoded, CDKEY_CRYPTO_DATA* crypto, CDKEY* outKey)
{
    char raw[28] = {0};
    memset(raw, 'X', sizeof(raw));
    raw[27] = 0;
    int   p = 0;
    dword crc = crypto->m_crc32;
    byte  tail = crc & 0x03;
    byte  mask = (crc & 0x7C) >> 2;
    dword shift = 0;
    dword bits = 0;
    dword block = 0;
    byte  hash = 0;

    for (int i = 0; i < 4; i++)
    {
        block = *(dword*) ((char*) encoded + i * 64);
        hash = crypto->m_hash[i] & 0x1Full;
        for (int j = 0; j < 4; j++)
        {
            shift = (27 - j * 5);
            bits = (0xF8000000 >> (j * 5));
            raw[p++] = g_Base32[((block & bits) >> shift) ^ hash];
        }
        raw[p++] = g_Base32[hash];
    }

    for (int i = 0; i < 5; i++)
    {
        shift = (27 - i * 5);
        bits = (0xF8000000 >> (i * 5));
        raw[p++] = g_Base32[((crc & bits) >> shift) ^ mask];
    }
    raw[p++] = g_Base32[mask];
    raw[p++] = g_Base32[tail];

    FormatKey(raw, outKey);
}


int ValidateCDKey(CDKEY_USER_DATA* data, CDKEY* key)
{
    CDKEY_USER_DATA   dec;
    CDKEY_CRYPTO_DATA crypto;
    CDKEY_ENC_DATA    enc;

    dword crc = 0;
    dword tail = 0;
    dword mask = 0;
    dword shift = 0;
    dword block = 0;
    byte  hash = 0;

    char raw[28] = {0};
    int  p = 0;

    UnformatKey(key, raw);
    GenerateEncodingKeys(data, &crypto);
    EncodeUserData(data, &crypto, &enc);
    DecodeUserData(&enc, &crypto, &dec);

    for (int i = 0; i < 4; i++)
    {
        dword* verBlock = ((dword*) &enc + i * 16);
        hash = IndexOfBase32(raw[p + 4]);
        block = 0;
        for (int j = 0; j < 4; j++)
        {
            shift = (27 - j * 5);
            block |= (IndexOfBase32((raw[p++])) ^ hash) << shift;
        }

        if ((*verBlock & block) != block)
            return 0;

        if ((crypto.m_hash[i] & 0x1Full) != hash)
            return 0;

        *verBlock = ((*verBlock << 20) >> 20) | block;

        p++;
    }

    mask = IndexOfBase32(raw[p + 5]);
    tail = IndexOfBase32(raw[p + 6]);
    for (int i = 0; i < 5; i++)
    {
        shift = (27 - i * 5);
        crc |= (IndexOfBase32((raw[p++])) ^ mask) << shift;
    }
    crc |= (mask << 2);
    crc |= tail;

    if (crypto.m_crc32 != crc)
        return 0;

    return !memcmp(data, &dec, sizeof(CDKEY_USER_DATA));
}