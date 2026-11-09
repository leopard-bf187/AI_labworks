/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: data types definitions
*
*  Date: 10.04.2022, 10.07.2025, 02.06.2026
*/

#pragma once


#define		False				0
#define		_false				0
#define		True				1
#define		_true				1


#if defined(KRYSTALLIC_OS_WINNT)
#include <stddef.h>
typedef		wchar_t				wchar;
#else
typedef		unsigned short		wchar;
#endif


typedef		unsigned char	    boolean, _bool, cbool;
typedef		long long			llong;
typedef		long double			ldouble;
typedef		unsigned long		ulong;

// typedef		char				int8;
// typedef		short				int16;
// typedef		int					int32;
// typedef		long long			int64;

// typedef		unsigned char		uchar,   byte, uint8;
// typedef		unsigned short		ushort,  word, uint16;
// typedef		unsigned int		uint,   dword, uint32;
// typedef		unsigned long long	ullong, qword, uint64;

typedef		char				int8;
typedef		short				int16;
typedef		int					int32;

typedef		unsigned char		uchar,   byte, uint8;
typedef		unsigned short		ushort,  word, uint16;
typedef		unsigned int		uint,   dword, uint32;
typedef		unsigned long long	ullong, qword;

#if defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)

#if defined(KRYSTALLIC_ARCH_X86) || defined(KRYSTALLIC_ARCH_ARMv7)
typedef		long long			int64;
typedef		unsigned long long  uint64;
#else
typedef		long			    int64;
typedef		unsigned long   	uint64;
#endif

#else
typedef		long long			int64;
typedef		unsigned long long  uint64;
#endif

typedef		dword				ERRCODE;
typedef		dword				RES;
typedef		float				float32;
typedef		double				float64;

typedef		unsigned char		utf8;
typedef		unsigned short		utf16;
typedef		unsigned int		utf32;

typedef		char			hexB[5];
typedef		char			hexW[7];
typedef		char			hexD[11];
typedef		char			hexQ[19];
