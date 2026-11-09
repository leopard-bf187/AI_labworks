#pragma once
#include "std_dll_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


    STDLIB_API extern _bool hexB_init(hexB out);
    STDLIB_API extern _bool hexW_init(hexW out);
    STDLIB_API extern _bool hexD_init(hexD out);
    STDLIB_API extern _bool hexQ_init(hexQ out);


    STDLIB_API extern _bool num_to_hexB(int8 val, hexB out);
    STDLIB_API extern _bool num_to_hexW(int16 val, hexW out);
    STDLIB_API extern _bool num_to_hexD(int32 val, hexD out);
    STDLIB_API extern _bool num_to_hexQ(int64 val, hexQ out);
    STDLIB_API extern _bool unum_to_hexB(uint8 val, hexB out);
    STDLIB_API extern _bool unum_to_hexW(uint16 val, hexW out);
    STDLIB_API extern _bool unum_to_hexD(uint32 val, hexD out);
    STDLIB_API extern _bool unum_to_hexQ(uint64 val, hexQ out);


    STDLIB_API extern _bool hexB_to_num(hexB val, int8* out);
    STDLIB_API extern _bool hexW_to_num(hexW val, int16* out);
    STDLIB_API extern _bool hexD_to_num(hexD val, int32* out);
    STDLIB_API extern _bool hexQ_to_num(hexQ val, int64* out);
    STDLIB_API extern _bool hexB_to_unum(hexB val, uint8* out);
    STDLIB_API extern _bool hexW_to_unum(hexW val, uint16* out);
    STDLIB_API extern _bool hexD_to_unum(hexD val, uint32* out);
    STDLIB_API extern _bool hexQ_to_unum(hexQ val, uint64* out);


#ifdef __cplusplus
}
#endif