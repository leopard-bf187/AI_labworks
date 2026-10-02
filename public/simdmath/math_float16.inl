/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors:  @leopard-bf187 && @RisovoePole
*
*  Description:
*
*  Date: 11.08.2026
*/


#pragma once


#include "math_library.h"


namespace krystallic
{
	namespace SIMDMath
	{
        inline constexpr Float16::Float16() noexcept : bits(0x0000)
        {}

        inline Float16::Float16(Float32 value) noexcept
        {
            F32ToF16(&value, 1, this);
        }


        inline Float16& Float16::operator=(Float32 value) noexcept
        {
            F32ToF16(&value, 1, this);
            return *this;
        }


        inline Float16::operator float() const noexcept
        {
            Float32 result;
            F16ToF32(this, 1, &result);
            return result;
        }


        inline constexpr word Float16::ToBits() const noexcept
        {
            return bits;
        }


        inline constexpr Float16 Float16::FromBits(word value) noexcept
        {
            Float16 result;
            result.bits = value;
            return result;
        }


        inline void F32ToF16(const Float32* src, int num, Float16* dst)
        {
            for (int i = 0; i < num; ++i)
            {
                dword srcBits;
                memcpy(&srcBits, &src[i], sizeof(srcBits));

                const dword sign = (srcBits >> 16) & 0x8000u;
                const dword exponent = (srcBits >> 23) & 0xFFu;
                const dword mantissa = srcBits & 0x007FFFFFu;

                // Inf / NaN
                if (exponent == 0xFFu)
                {
                    if (mantissa == 0)
                    {
                        dst[i].bits = static_cast<word>(sign | 0x7C00u);
                    }
                    else
                    {
                        dword halfMantissa = mantissa >> 13;

                        if (halfMantissa == 0)
                            halfMantissa = 1;

                        dst[i].bits = static_cast<word>(sign | 0x7C00u | halfMantissa);
                    }

                    continue;
                }

                const int e = static_cast<int>(exponent) - 127;

                // Overflow -> Inf
                if (e > 15)
                {
                    dst[i].bits = static_cast<word>(sign | 0x7C00u);
                    continue;
                }

                // Normal Float16
                if (e >= -14)
                {
                    dword halfExponent = static_cast<dword>(e + 15);
                    dword halfMantissa = mantissa >> 13;

                    const dword remainder = mantissa & 0x1FFFu;

                    // Round to nearest, ties to even
                    if (remainder > 0x1000u || (remainder == 0x1000u && (halfMantissa & 1u)))
                    {
                        ++halfMantissa;

                        if (halfMantissa == 0x400u)
                        {
                            halfMantissa = 0;
                            ++halfExponent;

                            if (halfExponent >= 31)
                            {
                                dst[i].bits = static_cast<word>(sign | 0x7C00u);
                                continue;
                            }
                        }
                    }

                    dst[i].bits = static_cast<word>(sign | (halfExponent << 10) | halfMantissa);
                    continue;
                }

                // Too small even for a Float16 subnormal
                if (e < -25)
                {
                    dst[i].bits = static_cast<word>(sign);
                    continue;
                }

                // Float16 subnormal
                const dword significand = mantissa | 0x00800000u;
                const int shift = -e - 1;

                dword halfMantissa = significand >> shift;
                const dword mask = (1u << shift) - 1u;
                const dword remainder = significand & mask;
                const dword halfway = 1u << (shift - 1);

                // Round to nearest, ties to even
                if (remainder > halfway || (remainder == halfway && (halfMantissa & 1u)))
                    ++halfMantissa;

                // Rounding may turn the largest subnormal into the smallest normal
                if (halfMantissa >= 0x400u)
                    dst[i].bits = static_cast<word>(sign | 0x0400u);
                else
                    dst[i].bits = static_cast<word>(sign | halfMantissa);
            }
        }


        inline void F16ToF32(const Float16* src, int num, Float32* dst)
        {
            for (int i = 0; i < num; ++i)
            {
                const dword srcBits = static_cast<dword>(src[i].bits);

                const dword sign = (srcBits & 0x8000u) << 16;
                const dword exponent = (srcBits >> 10) & 0x1Fu;
                const dword mantissa = srcBits & 0x03FFu;

                dword dstBits;

                if (exponent == 0)
                {
                    if (mantissa == 0)
                    {
                        // ±0
                        dstBits = sign;
                    }
                    else
                    {
                        // Float16 subnormal -> Float32 normal
                        dword m = mantissa;
                        int e = -14;

                        while ((m & 0x0400u) == 0)
                        {
                            m <<= 1;
                            --e;
                        }

                        m &= 0x03FFu;

                        const dword floatExponent = static_cast<dword>(e + 127);
                        dstBits = sign | (floatExponent << 23) | (m << 13);
                    }
                }
                else if (exponent == 0x1Fu)
                {
                    // Inf / NaN
                    dstBits = sign | 0x7F800000u | (mantissa << 13);

                    if (mantissa != 0)
                        dstBits |= 0x00400000u;
                }
                else
                {
                    // Normal Float16 -> normal Float32
                    const dword floatExponent = exponent + 112u;
                    dstBits = sign | (floatExponent << 23) | (mantissa << 13);
                }

                memcpy(&dst[i], &dstBits, sizeof(dstBits));
            }
        }
	}
}