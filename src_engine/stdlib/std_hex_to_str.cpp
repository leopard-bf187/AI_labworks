#define STDLIB_API_EXPORT
#include "stdlib_dll.h"


using namespace krystallic::Stdlib;


_bool hexB_init(hexB out)
{
    StrCopy(out, "0x00");
    return _true;
}


_bool hexW_init(hexW out)
{
    StrCopy(out, "0x0000");
    return _true;
}


_bool hexD_init(hexD out)
{
    StrCopy(out, "0x00000000");
    return _true;
}


_bool hexQ_init(hexQ out)
{
    StrCopy(out, "0x0000000000000000");
    return _true;
}


_bool num_to_hexB(int8 val, hexB out)
{
    uint8 u_val = (uint8) val;
    return unum_to_hexB(u_val, out);
    ;
}


_bool num_to_hexW(int16 val, hexW out)
{
    uint16 u_val = (uint16) val;
    return unum_to_hexW(u_val, out);
}


_bool num_to_hexD(int32 val, hexD out)
{
    uint32 u_val = (uint32) val;
    return unum_to_hexD(u_val, out);
}


_bool num_to_hexQ(int64 val, hexQ out)
{
    uint64 u_val = (uint64) val;
    return unum_to_hexQ(u_val, out);
}


_bool unum_to_hexB(uint8 val, hexB out)
{
    int i = 0;
    int rest = 0;

    hexB_init(out);

    while (val > 0)
    {
        rest = val % 16;
        if (rest < 10)
        {
            out[3 - i] = char(48 + rest);
        }
        else
        {
            out[3 - i] = char(55 + rest);
        }
        val /= 16;
        i++;
    }
    return _true;
}


_bool unum_to_hexW(uint16 val, hexW out)
{
    int i = 0;
    int rest = 0;

    hexW_init(out);

    while (val > 0)
    {
        rest = val % 16;
        if (rest < 10)
        {
            out[5 - i] = char(48 + rest);
        }
        else
        {
            out[5 - i] = char(55 + rest);
        }
        val /= 16;
        i++;
    }
    return _true;
}


_bool unum_to_hexD(uint32 val, hexD out)
{
    int i = 0;
    int rest = 0;

    hexD_init(out);

    while (val > 0)
    {
        rest = val % 16;
        if (rest < 10)
        {
            out[9 - i] = char(48 + rest);
        }
        else
        {
            out[9 - i] = char(55 + rest);
        }
        val /= 16;
        i++;
    }
    return _true;
}


_bool unum_to_hexQ(uint64 val, hexQ out)
{
    int i = 0;
    int rest = 0;

    hexQ_init(out);

    while (val > 0)
    {
        rest = val % 16;
        if (rest < 10)
        {
            out[17 - i] = char(48 + rest);
        }
        else
        {
            out[17 - i] = char(55 + rest);
        }
        val /= 16;
        i++;
    }
    return _true;
}


_bool hexB_to_num(hexB val, int8* out)
{
    return hexB_to_unum(val, (uint8*) out);
}


_bool hexW_to_num(hexW val, int16* out)
{
    return hexW_to_unum(val, (uint16*) out);
}


_bool hexD_to_num(hexD val, int32* out)
{
    return hexD_to_unum(val, (uint32*) out);
}


_bool hexQ_to_num(hexQ val, int64* out)
{
    return hexQ_to_unum(val, (uint64*) out);
}


_bool hexB_to_unum(hexB val, uint8* out)
{
    int val_int;
    int p = 1;
    int i = 0;
    int result = 0;

    if (!out)
        return _false;

    for (i = 0; i < 2; i++)
    {
        if (val[3 - i] < 58)
        {
            val_int = (val[3 - i] - 48);
        }
        else
        {
            val_int = val[3 - i] - 55;
        }

        p *= 16;
        result += val_int * p;
    }


    *out = result;
    return _true;
}


_bool hexW_to_unum(hexW val, uint16* out)
{
    int val_int;
    int result = 0;
    int p = 1;
    int i = 0;

    if (!out)
        return _false;

    for (i = 0; i < 4; i++)
    {
        if (val[5 - i] < 58)
        {
            val_int = (val[5 - i] - 48);
        }
        else
        {
            val_int = val[5 - i] - 55;
        }

        p *= 16;
        result += val_int * p;
    }
    *out = result;
    return _true;
}


_bool hexD_to_unum(hexD val, uint32* out)
{
    int val_int = 0;
    int result = 0;
    int p = 1;
    int i = 0;

    if (!out)
        return _false;

    for (i = 0; i < 8; i++)
    {
        if (val[9 - i] < 58)
        {
            val_int = (val[9 - i] - 48);
        }
        else
        {
            val_int = val[9 - i] - 55;
        }

        p *= 16;
        result += val_int * p;
    }

    *out = result;
    return _true;
}


_bool hexQ_to_unum(hexQ val, uint64* out)
{
    int val_int;
    int result = 0;
    int p = 1;
    int i = 0;

    if (!out)
        return _false;

    for (i = 0; i < 16; i++)
    {
        if (val[17 - i] < 58)
        {
            val_int = (val[17 - i] - 48);
        }
        else
        {
            val_int = val[17 - i] - 55;
        }

        p *= 16;
        result += val_int * p;
    }

    *out = result;
    return _true;
}