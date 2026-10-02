/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 
*
*  Description: AES-256 encrypting library
*
*  Date: 22.06.2025
*/


#include "crypto.h"
#include <stdio.h>
#include <stdlib.h>


void generate_aes256_key(uint8 key[32])
{
    FILE* f = fopen("/dev/urandom", "rb");
    fread(key, 1, 32, f);
    fclose(f);
}


// Макрос генерации одного раунда ключа
#define AES_256_KEY_EXP(tmp1, tmp2, k1, k2, rcon)                                                \
    tmp1 = _mm_aeskeygenassist_si128(k2, rcon);                                                                        \
    tmp2 = _mm_shuffle_epi32(tmp1, 0xff);                                                                              \
    tmp1 = _mm_slli_si128(k1, 0x4);                                                                                    \
    k1 = _mm_xor_si128(k1, tmp1);                                                                                      \
    tmp1 = _mm_slli_si128(tmp1, 0x4);                                                                                  \
    k1 = _mm_xor_si128(k1, tmp1);                                                                                      \
    tmp1 = _mm_slli_si128(tmp1, 0x4);                                                                                  \
    k1 = _mm_xor_si128(k1, tmp1);                                                                                      \
    k1 = _mm_xor_si128(k1, tmp2);                                                                                      \
    tmp1 = _mm_slli_si128(k2, 0x4);                                                                                    \
    k2 = _mm_xor_si128(k2, tmp1);                                                                                      \
    tmp1 = _mm_slli_si128(tmp1, 0x4);                                                                                  \
    k2 = _mm_xor_si128(k2, tmp1);                                                                                      \
    tmp1 = _mm_slli_si128(tmp1, 0x4);                                                                                  \
    k2 = _mm_xor_si128(k2, tmp1)



void aes256_key_expansion(const AES_SRC_KEY key, AES_ENC_KEY* round_keys)
{
    __m128i k1 = _mm_loadu_si128((const __m128i*) (key));
    __m128i k2 = _mm_loadu_si128((const __m128i*) (key + 16));
    __m128i tmp1, tmp2;

    round_keys->m_key[0] = k1;
    round_keys->m_key[1] = k2;

    AES_256_KEY_EXP(tmp1, tmp2, k1, k2, 0x01);
    round_keys->m_key[2] = k1;
    round_keys->m_key[3] = k2;
    AES_256_KEY_EXP(tmp1, tmp2, k1, k2, 0x02);
    round_keys->m_key[4] = k1;
    round_keys->m_key[5] = k2;
    AES_256_KEY_EXP(tmp1, tmp2, k1, k2, 0x04);
    round_keys->m_key[6] = k1;
    round_keys->m_key[7] = k2;
    AES_256_KEY_EXP(tmp1, tmp2, k1, k2, 0x08);
    round_keys->m_key[8] = k1;
    round_keys->m_key[9] = k2;
    AES_256_KEY_EXP(tmp1, tmp2, k1, k2, 0x10);
    round_keys->m_key[10] = k1;
    round_keys->m_key[11] = k2;
    AES_256_KEY_EXP(tmp1, tmp2, k1, k2, 0x20);
    round_keys->m_key[12] = k1;
    round_keys->m_key[13] = k2;
    AES_256_KEY_EXP(tmp1, tmp2, k1, k2, 0x40);
    round_keys->m_key[14] = k1;
}



void aes256_invert_keys(const AES_ENC_KEY* enc_keys, AES_DEC_KEY* dec_keys)
{
    dec_keys->m_key[0] = enc_keys->m_key[14]; // первый ключ дешифрования = последний ключ шифрования
    dec_keys->m_key[14] = enc_keys->m_key[0]; // последний ключ дешифрования = первый шифрования

    for (int i = 1; i < 14; ++i)
    {
        dec_keys->m_key[i] = _mm_aesimc_si128(enc_keys->m_key[14 - i]); // InverseMixColumns
    }
}



void aes256_encrypt_block(const uint8* in, uint8* out, AES_ENC_KEY* round_keys)
{
    __m128i block = _mm_loadu_si128((__m128i*) in);
    block = _mm_xor_si128(block, round_keys->m_key[0]);
    for (int i = 1; i < 14; ++i)
    {
        block = _mm_aesenc_si128(block, round_keys->m_key[i]);
    }
    block = _mm_aesenclast_si128(block, round_keys->m_key[14]);
    _mm_storeu_si128((__m128i*) out, block);
}



void aes256_decrypt_block(const uint8* in, uint8* out, const AES_DEC_KEY* dec_keys)
{
    __m128i block = _mm_loadu_si128((__m128i*) in);
    block = _mm_xor_si128(block, dec_keys->m_key[0]);

    for (int i = 1; i < 14; ++i)
    {
        block = _mm_aesdec_si128(block, dec_keys->m_key[i]);
    }

    block = _mm_aesdeclast_si128(block, dec_keys->m_key[14]);
    _mm_storeu_si128((__m128i*) out, block);
}
