/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 
*
*  Description: test game key functions
*
*  Date: 22.06.2025
*/

#include "crypto.h"


void InitKeyData(char* name, char* surname, char* nick, AES_SRC_KEY key)
{
    memset(key, '_', sizeof(uint8) * 32);

    int len = strlen(name);
    strncpy((char*) key, name, (len > 11) ? 11 : len);

    len = strlen(surname);
    strncpy((char*) key + 11, surname, (len > 11) ? 11 : len);

    len = strlen(nick);
    strncpy((char*) key + 22, nick, (len > 10) ? 10 : len);
}


void InitEncryptionData(qword hash1, qword hash2, DATA_BLOCK data)
{
#pragma warning(disable : 4244)

    data[0x0] = (hash1 & 0xf000000000000000) >> 60;
    data[0x1] = (hash2 & 0x0f00000000000000) >> 56;
    data[0x2] = (hash1 & 0x00f0000000000000) >> 52;
    data[0x3] = (hash2 & 0x000f000000000000) >> 48;
    data[0x4] = (hash1 & 0x0000f00000000000) >> 44;
    data[0x5] = (hash2 & 0x00000f0000000000) >> 40;
    data[0x6] = (hash1 & 0x000000f000000000) >> 36;
    data[0x7] = (hash2 & 0x0000000f00000000) >> 32;
    data[0x8] = (hash1 & 0x00000000f0000000) >> 28;
    data[0x9] = (hash2 & 0x000000000f000000) >> 24;
    data[0xa] = (hash1 & 0x0000000000f00000) >> 20;
    data[0xb] = (hash2 & 0x00000000000f0000) >> 16;
    data[0xc] = (hash1 & 0x000000000000f000) >> 12;
    data[0xd] = (hash2 & 0x0000000000000f00) >> 8;
    data[0xe] = (hash1 & 0x00000000000000f0) >> 4;
    data[0xf] = (hash2 & 0x000000000000000f) >> 0;
}


void GetGameKey(uint8* enc1, uint8* enc2, GAME_KEY key)
{
    const char table[] = "AB0CDE1FG2HIJ3KLM4NO5PQ6RS7TU8VWX9YZ";
    memset(key, '-', sizeof(key[0]) * 30);

    uint8 enc[32];

    for (int i = 0, j = 0; i < 16; i++)
    {
        enc[j++] = enc1[i];
        enc[j++] = enc2[j];
    }

    for (int i = 0; i < 5; i++)
    {
        int bp = i * 6;
        key[bp] = table[enc[bp++] % 36];
        key[bp] = table[enc[bp++] % 36];
        key[bp] = table[enc[bp++] % 36];
        key[bp] = table[enc[bp++] % 36];
        key[bp] = table[enc[bp++] % 36];
        key[bp++] = '-';
    }

    key[29] = 0;
}


/*
void GameKey_KeyGen()
{
	uint8 key[32];

	char name[64] = { 0 };
	char surname[64] = { 0 };
	char nick[64] = { 0 };
	char email[64] = { 0 };
	uint64 hash1 = 0;
	uint64 hash2 = 0;
	uint64 hash3 = 0;
	uint64 hash4 = 0;

	printf("Name:     "); scanf(" %s", name);
	printf("Surname:  "); scanf(" %s", surname);
	printf("Nickname: "); scanf(" %s", nick);
	printf("Email:    "); scanf(" %s", email);
	printf("\n\n");

	InitKeyData(name, surname, nick, key);

	__m128i enc_keys[16];
	__m128i dec_keys[16];
	aes256_key_expansion(key, enc_keys);
	aes256_invert_keys(enc_keys, dec_keys);

	hash1 = FNV1A64_Hash(name, strlen(name));
	hash2 = FNV1A64_Hash(surname, strlen(surname));
	hash3 = FNV1A64_Hash(nick, strlen(nick));
	hash4 = FNV1A64_Hash(email, strlen(email));

	uint8 data1[16] = { 0 };
	uint8 data2[16] = { 0 };
	uint8 encrypted1[16] = { 0 };
	uint8 encrypted2[16] = { 0 };
	InitEncryptionData(hash1, hash3, data1);
	aes256_encrypt_block(data1, encrypted1, enc_keys);

	InitEncryptionData(hash2, hash4, data2);
	aes256_encrypt_block(data2, encrypted2, enc_keys);

	uint8 decrypted1[16];
	uint8 decrypted2[16];

	aes256_decrypt_block(encrypted1, decrypted1, dec_keys);
	aes256_decrypt_block(encrypted2, decrypted2, dec_keys);

	char gameKey[30] = { 0 };
	GetGameKey(encrypted1, encrypted2, gameKey);
	printf("Game key: %s\n\n", gameKey);


	FILE* fp = fopen("gameKey.txt", "w");
	fprintf(fp, "Name:     %s\n", name);
	fprintf(fp, "Surname:  %s\n", surname);
	fprintf(fp, "Nickname: %s\n", nick);
	fprintf(fp, "Email:    %s\n", email);
	fprintf(fp, "Game key: %s\n\n", gameKey);

	fprintf(fp, "Ecnoded:\n");
	for (int i = 0; i < 16; i++)
		fprintf(fp, "%02X ", encrypted1[i]);
	fprintf(fp, "\n");
	for (int i = 0; i < 16; i++)
		fprintf(fp, "%02X ", encrypted2[i]);
	fprintf(fp, "\n\n");

	fprintf(fp, "Decoded:\n");
	for (int i = 0; i < 16; i++)
		fprintf(fp, "%02X ", data1[i]);
	fprintf(fp, "\n");
	for (int i = 0; i < 16; i++)
		fprintf(fp, "%02X ", decrypted1[i]);
	fprintf(fp, "\n");
	for (int i = 0; i < 16; i++)
		fprintf(fp, "%02X ", data2[i]);
	fprintf(fp, "\n");
	for (int i = 0; i < 16; i++)
		fprintf(fp, "%02X ", decrypted2[i]);
	fprintf(fp, "\n\n");

	fprintf(fp, "Hash:\n");
	fprintf(fp, "1: %16llX\n2: %16llX\n3: %16llX\n4: %16llX", hash1, hash2, hash3, hash4);

	fclose(fp);
}



void GameKey_KeyReg()
{
	uint8 key[32];
	char name[64] = { 0 };
	char surname[64] = { 0 };
	char nick[64] = { 0 };
	char email[64] = { 0 };
	char userKey[64] = { 0 };
	uint64 hash1 = 0;
	uint64 hash2 = 0;
	uint64 hash3 = 0;
	uint64 hash4 = 0;

	printf("Name:     "); scanf(" %s", name);
	printf("Surname:  "); scanf(" %s", surname);
	printf("Nickname: "); scanf(" %s", nick);
	printf("Email:    "); scanf(" %s", email);
	printf("Game Key: "); scanf(" %s", userKey);
	printf("\n\n");

	InitKeyData(name, surname, nick, key);

	__m128i enc_keys[16];
	__m128i dec_keys[16];
	aes256_key_expansion(key, enc_keys);
	aes256_invert_keys(enc_keys, dec_keys);

	hash1 = FNV1A64_Hash(name, strlen(name));
	hash2 = FNV1A64_Hash(surname, strlen(surname));
	hash3 = FNV1A64_Hash(nick, strlen(nick));
	hash4 = FNV1A64_Hash(email, strlen(email));

	uint8 data1[16] = { 0 };
	uint8 data2[16] = { 0 };
	uint8 encrypted1[16] = { 0 };
	uint8 encrypted2[16] = { 0 };
	InitEncryptionData(hash1, hash3, data1);
	aes256_encrypt_block(data1, encrypted1, enc_keys);

	InitEncryptionData(hash2, hash4, data2);
	aes256_encrypt_block(data2, encrypted2, enc_keys);

	uint8 decrypted1[16];
	uint8 decrypted2[16];

	aes256_decrypt_block(encrypted1, decrypted1, dec_keys);
	aes256_decrypt_block(encrypted2, decrypted2, dec_keys);

	char gameKey[30] = { 0 };
	GetGameKey(encrypted1, encrypted2, gameKey);

	//printf("%s\n", gameKey);
	//printf("%s\n", userKey);

	printf("Ecnoded:\n");
	for (int i = 0; i < 16; i++)
		printf("%02X ", encrypted1[i]);
	printf("\n");
	for (int i = 0; i < 16; i++)
		printf("%02X ", encrypted2[i]);
	printf("\n\n");

	printf("Decoded:\n");
	for (int i = 0; i < 16; i++)
		printf("%02X ", data1[i]);
	printf("\n");
	for (int i = 0; i < 16; i++)
		printf("%02X ", decrypted1[i]);
	printf("\n");
	for (int i = 0; i < 16; i++)
		printf("%02X ", data2[i]);
	printf("\n");
	for (int i = 0; i < 16; i++)
		printf("%02X ", decrypted2[i]);
	printf("\n\n");

	printf("Hash:\n");
	printf("1: %16llX\n2: %16llX\n3: %16llX\n4: %16llX\n\n", hash1, hash2, hash3, hash4);


	if (!strcmp(userKey, gameKey))
		printf("\n\nRegistration Success.\n\n");
	else
		printf("\nRegistration Failed. Invalid key or data\n\n");

}*/