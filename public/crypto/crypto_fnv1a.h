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
		// Non-cryptographic byte hashes. Null data is valid only for size == 0.
		// Alignment must be a nonzero power of two; it is a hint, never dereferenced
		// as a stronger-aligned pointer and never changes the result.
		dword FNV1A32(const void* data, uint64 size, uint32 alignment = 1);
		qword FNV1A64(const void* data, uint64 size, uint32 alignment = 1);

		// Hashes object representation, including padding. Not semantic serialization:
		// pointers, endian differences and indeterminate padding are caller concerns.
		template<typename T>
		inline dword FNV1A32(const T& value)
		{
			return FNV1A32(static_cast<const void*>(&value), static_cast<uint64>(sizeof(T)), static_cast<uint32>(alignof(T)));
		}

		template<typename T>
		inline qword FNV1A64(const T& value)
		{
			return FNV1A64(static_cast<const void*>(&value), static_cast<uint64>(sizeof(T)), static_cast<uint32>(alignof(T)));
		}
	}
}

