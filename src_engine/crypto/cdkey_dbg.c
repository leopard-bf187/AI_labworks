/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 
*
*  Description: debug print cdkey functions
*
*  Date: 22.06.2025
*/


#include "crypto.h"
#include <stdio.h>
#include <stdlib.h>


void PrintUserData(CDKEY_USER_DATA* user, CDKEY* key)
{
    if (!user)
        return;

    printf("\nUSER DATA: \n");
    printf("    Name:      %s\n", user->m_name);
    printf("    Surname:   %s\n", user->m_surname);
    printf("    User name: %s\n", user->m_username);
    printf("    Email:     %s\n", user->m_email);
    if (key)
        printf("  CDKey:     %s\n", key->m_key);
    printf("\n");
}


void PrintCryptoData(CDKEY_CRYPTO_DATA* crypto, int flags)
{
    printf("\nCRYPTOGRAPHY DATA: \n");

    printf("  Hash:\n");
    printf("    1: %16llX\n", crypto->m_hash[0]);
    printf("    2: %16llX\n", crypto->m_hash[1]);
    printf("    3: %16llX\n", crypto->m_hash[2]);
    printf("    4: %16llX\n", crypto->m_hash[3]);
    printf("  CRC32: %08X\n", crypto->m_crc32);

    if ((flags & 1) == 1)
    {
        printf("  AES_KEY:\n");
        for (int i = 0; i < 2; i++)
        {
            printf("    ");
            for (int j = 0; j < 16; j++)
                printf("%02X ", crypto->m_aesKey[i * 2 + j]);
            printf("\n");
        }
    }

    if ((flags & 2) == 2)
    {
        printf("  AES_ENC_KEY:\n");
        for (int i = 0; i < 16; i++)
        {
            uint8 k[16] = {0};
            _mm_store_si128((__m128i*) k, crypto->m_encKey.m_key[i]);
            printf("    ");
            for (int j = 0; j < 16; j++)
                printf("%02X ", k[j]);
            printf("\n");
        }
    }

    if ((flags & 4) == 4)
    {
        printf("  AES_DEC_KEY:\n");
        for (int i = 0; i < 16; i++)
        {
            uint8 k[16] = {0};
            _mm_store_si128((__m128i*) k, crypto->m_decKey.m_key[i]);
            printf("    ");
            for (int j = 0; j < 16; j++)
                printf("%02X ", k[j]);
            printf("\n");
        }
    }

    printf("\n");
}


void PrintEncoded(CDKEY_ENC_DATA* encoded)
{
    printf("\nENCRYPTED DATA: \n");
    for (int i = 0; i < 16; i++)
    {
        if (i % 4 == 0 && i != 0)
            printf("\n");
        printf("  ");
        for (int j = 0; j < 16; j++)
        {
            printf("%02X ", (unsigned char) encoded->data[i * 16 + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void PrintCDKeyDataToFile(
    CDKEY_USER_DATA* user, CDKEY* cdkey, CDKEY_CRYPTO_DATA* crypto, int flags, CDKEY_ENC_DATA* encoded)
{
    if (!user || !cdkey)
        return;

    FILE* fp = fopen("cdkey.txt", "w");

    fprintf(fp, "USER DATA: \n");
    fprintf(fp, "  Name:      %s\n", user->m_name);
    fprintf(fp, "  Surname:   %s\n", user->m_surname);
    fprintf(fp, "  User name: %s\n", user->m_username);
    fprintf(fp, "  Email:     %s\n", user->m_email);
    fprintf(fp, "  CDKey:     %s\n", cdkey->m_key);
    fprintf(fp, "\n");

    if (crypto)
    {
        fprintf(fp, "\nCRYPTOGRAPHY DATA: \n");

        fprintf(fp, "  Hash:\n");
        fprintf(fp, "    1: %16llX\n", crypto->m_hash[0]);
        fprintf(fp, "    2: %16llX\n", crypto->m_hash[1]);
        fprintf(fp, "    3: %16llX\n", crypto->m_hash[2]);
        fprintf(fp, "    4: %16llX\n", crypto->m_hash[3]);

        fprintf(fp, "  CRC32: %08X\n", crypto->m_crc32);

        if ((flags & 1) == 1)
        {
            fprintf(fp, "  AES_KEY:\n");
            for (int i = 0; i < 2; i++)
            {
                fprintf(fp, "    ");
                for (int j = 0; j < 16; j++)
                    fprintf(fp, "%02X ", crypto->m_aesKey[i * 2 + j]);
                fprintf(fp, "\n");
            }
        }

        if ((flags & 2) == 2)
        {
            fprintf(fp, "  AES_ENC_KEY:\n");
            for (int i = 0; i < 16; i++)
            {
                uint8 k[16] = {0};
                _mm_store_si128((__m128i*) k, crypto->m_encKey.m_key[i]);
                fprintf(fp, "    ");
                for (int j = 0; j < 16; j++)
                    fprintf(fp, "%02X ", k[j]);
                fprintf(fp, "\n");
            }
        }

        if ((flags & 4) == 4)
        {
            fprintf(fp, "  AES_DEC_KEY:\n");
            for (int i = 0; i < 16; i++)
            {
                uint8 k[16] = {0};
                _mm_store_si128((__m128i*) k, crypto->m_decKey.m_key[i]);
                fprintf(fp, "    ");
                for (int j = 0; j < 16; j++)
                    fprintf(fp, "%02X ", k[j]);
                fprintf(fp, "\n");
            }
        }

        fprintf(fp, "\n");
    }

    if (encoded)
    {
        fprintf(fp, "\nENCRYPTED DATA: \n");
        for (int i = 0; i < 16; i++)
        {
            if (i % 4 == 0 && i != 0)
                fprintf(fp, "\n");
            fprintf(fp, "  ");
            for (int j = 0; j < 16; j++)
            {
                fprintf(fp, "%02X ", (unsigned char) encoded->data[i * 16 + j]);
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}