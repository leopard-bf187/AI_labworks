#pragma once
#ifndef __kx_str_conv_h__
#define	__kx_str_conv_h__


#include "std_dll_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // declarations
    STDLIB_API _bool str_to_int8(char* str, int8* outer);
    STDLIB_API _bool str_to_int16(char* str, int16* outer);
    STDLIB_API _bool str_to_int32(char* str, int32* outer);
    STDLIB_API _bool str_to_int64(char* str, int64* outer);
    STDLIB_API _bool str_to_uint8(char* str, uint8* outer);
    STDLIB_API _bool str_to_uint16(char* str, uint16* outer);
    STDLIB_API _bool str_to_uint32(char* str, uint32* outer);
    STDLIB_API _bool str_to_uint64(char* str, uint64* outer);

    STDLIB_API _bool int8_to_str(int8 val, char* str);
    STDLIB_API _bool int16_to_str(int16 val, char* str);
    STDLIB_API _bool int32_to_str(int32 val, char* str);
    STDLIB_API _bool int64_to_str(int64 val, char* str);
    STDLIB_API _bool uint8_to_str(uint8 val, char* str);
    STDLIB_API _bool uint16_to_str(uint16 val, char* str);
    STDLIB_API _bool uint32_to_str(uint32 val, char* str);
    STDLIB_API _bool uint64_to_str(uint64 val, char* str);

    STDLIB_API float  str_to_float(char* str);
    STDLIB_API double str_to_double(char* str);


#ifdef __cplusplus
}
#endif

#endif