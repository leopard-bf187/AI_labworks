/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: legacy global definitions
*
*  Date: 10.07.2025, 02.06.2026
*/

#pragma once


#include "data_types.h"


// from mmsystem.h
#define MAKE_FOURCC(ch0, ch1, ch2, ch3) ((dword)(byte)(ch0) | ((dword)(byte)(ch1) << 8) | ((dword)(byte)(ch2) << 16) | ((dword)(byte)(ch3) << 24 ))



// macros to free allocated memory of C pointer
#ifndef safe_free
#	define safe_free(p)                                                                                                      \
    {                                                                                                                  \
        if (p)                                                                                                         \
        {                                                                                                              \
            free(p);                                                                                                   \
            p = 0;                                                                                                     \
        }                                                                                                              \
    }
#endif


#ifndef KRENG_C_API

// macros to free allocated memory of C++ pointer
#ifndef safe_del
#	define safe_del(p)                                                                                                       \
    {                                                                                                                  \
        if (p)                                                                                                         \
        {                                                                                                              \
            delete p;                                                                                                  \
            p = 0;                                                                                                     \
        }                                                                                                              \
    }
#endif


// macros to free allocated memory of C++ array of pointers
#ifndef safe_del_arr
#	define safe_del_arr(p)                                                                                                   \
    {                                                                                                                  \
        if (p)                                                                                                         \
        {                                                                                                              \
            delete[] p;                                                                                                \
            p = nullptr;                                                                                               \
        }                                                                                                              \
    }

#endif


// macros to release a C++ COM object of Win32 API
#ifndef safe_release
#	define safe_release(p)                                                                                                   \
    {                                                                                                                  \
        if (p)                                                                                                         \
        {                                                                                                              \
            p->Release();                                                                                              \
            p = 0;                                                                                                     \
        }                                                                                                              \
    }
#endif

// macros to release a C++ Nice Tech Interface class
#ifndef safe_delete
#	define safe_delete(p)                                                                                                    \
    {                                                                                                                  \
        if (p)                                                                                                         \
        {                                                                                                              \
            p->Delete();                                                                                               \
            p = 0;                                                                                                     \
        }                                                                                                              \
    }
#endif


#endif


//defines a keyword that means an input parameter
#ifndef _in
#	define _in
#endif

//defines a keyword that means an output parameter
#ifndef _out 
#	define _out
#endif

//defines a keyword that means an input and output parameter
#ifndef _in_out 
#	define _in_out
#endif

//defines a keyword that means an input and output parameter
#ifndef _in_opt 
#	define _in_opt
#endif

//defines a keyword that means an input and output parameter
#ifndef _out_opt 
#	define _out_opt
#endif

//defines a keyword that means an input and output parameter
#ifndef _in_out_opt 
#	define _in_out_opt
#endif


#define		nill	0
#define		null	0


#ifndef KRENG_C_API
#else
#	define		ntpr	0
#endif


#ifndef KRENG_C_API
#	define abstract_iface	struct
#	define abstract_struct	struct
#	define abstract_class	class
#	define virtual_class	class
#endif


#define PRM_TO_STR(p) #p
#define TO_STR_LITERAL(p) #p


#ifndef ALIGN
#	define ALIGN(n)	__declspec(align(n))
#endif


#ifndef UNREF_PARAM
#	define UNREF_PARAM(p) (p)
#endif


#define		kreng_inl		inline
#define		kreng_finl		__forceinline


#ifndef __global_const
#	define __global_const	extern const
#endif


#ifndef DECL_PTR

#if !defined ptr_size

#if defined __build_x86
#define ptr_size 4
#elif defined __build_x64
#define ptr_size 8
#else 
#define ptr_size 4
#endif

#endif

#if defined _OS_WIN32
#if defined __build_x86
#	define DECL_PTR(type, name) type* __ptr32 name
#elif defined __build_x64
#	define DECL_PTR(type, name) type* __ptr64 name
#else
#	define DECL_PTR(type, name) type* __ptr32 name
#endif
#endif

#ifndef _MSC_VER
#	define __vectorcall
#	define OutputDebugString(s) ((void)0)
#endif

#endif
