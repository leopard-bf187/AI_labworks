/*
*  Authors:  Leonid (@LeoParD) Parmacli  &&  Victor (@RisovoePole) Anisimov
*
*  Description:
*
*  Date: 05.01.2026
*/


#pragma once

#include "../pch.h"

//#include <emmintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _MSC_VER
    #ifndef __vectorcall
        #define __vectorcall
    #endif
#endif


#ifdef _MSC_VER
    #include <intrin.h>
#endif

//#if defined(__aarch64__) || defined(__arm__) || defined(_M_ARM64) || defined(_M_ARM)
//#   ifndef __SIMDMATH_USE_NEON__
//#       define __SIMDMATH_USE_NEON__ 1
//#   endif
//#   include <arm_neon.h>
//#elif defined(__x86_64__) || defined(__i386__) || defined(_M_X64) || defined(_M_IX86)
//#   ifndef __SIMDMATH_USE_SSE4__
//#       define __SIMDMATH_USE_SSE4__ 1
//#   endif
//#   include <xmmintrin.h>
//#   include <immintrin.h>
//#endif


#if defined(SIMD_AVX) || defined(SIMD_AVX2)
#include <immintrin.h>
#elif defined(SIMD_SSE42)
#include <nmmintrin.h>
#elif defined(SIMD_ARM_NEON)
#include <arm_neon.h>
#endif


#define IsFloatNaN(x)  ((*(unsigned int*)&(x) & 0x7f800000) == 0x7f800000 && (*(unsigned int*)&(x) & 0x007fffff) != 0)
#define IsFloatInf(x)  ((*(unsigned int*)&(x) & 0x7fffffff) == 0x7f800000)



//#define __SIMDMATH_WINDOWS__
//#define __SIMDMATH_LINUX__
//#define __SIMDMATH_MACOS__
//
//#define __SIMDMATH_X86__
//#define __SIMDMATH_X64__
//#define __SIMDMATH_ARMv7__
//#define __SIMDMATH_ARMv8__
//#define __SIMDMATH_ARM64__

// #define __SIMDMATH_USE_SSE4__ 1
// #define __SIMDMATH_USE_AVX__ 0
// #define __SIMDMATH_USE_AVX2__ 0
// #define __SIMDMATH_USE_NEON__ 1


#ifndef _SM_SHUFFLE
#define _SM_SHUFFLE(x, y, z, w) ((x) | ((y) << 2) | ((z) << 4) | ((w) << 6))
#endif

#if defined SIMD_SSE42

using vec4f = __m128;
using vec4i = __m128i;
using vec4ui = __m128i;
using vec4d = __m128d;

#ifndef _SM_PERMUTE
#define _SM_PERMUTE(v, x, y, z, w) _mm_shuffle_ps((v), (v), _SM_SHUFFLE(x, y, z, w))
#endif

#ifndef _set_vec4f
#define _set_vec4f(x, y, z, w) _mm_setr_ps(x,y,z,w)
#endif

#ifndef _set1_vec4f
#define _set1_vec4f(x) _mm_set1_ps(x)
#endif

#ifndef _set_vec4i
#define _set_vec4i(x, y, z, w) _mm_setr_epi32(x,y,z,w)
#endif

#ifndef _set1_vec4i
#define _set1_vec4i(x) _mm_set1_epi32(x)
#endif

#ifndef _set_vec4ui
#define _set_vec4ui(x, y, z, w) _mm_setr_epi32(x,y,z,w)
#endif

#ifndef _set1_vec4ui
#define _set1_vec4ui(x) _mm_set1_epi32(x)
#endif

#elif defined(SIMD_AVX) || defined(SIMD_AVX2)

using vec4f = __m128;
#ifndef _SM_PERMUTE
#define _SM_PERMUTE(v, x, y, z, w) _mm_permute_ps((v), _SM_SHUFFLE(x, y, z, w))
#endif

#elif defined SIMD_ARM_NEON

using vec4f  = float32x4_t;
using vec4i  = int32x4_t;
using vec4ui = uint32x4_t;

#if defined(KRYSTALLIC_ARCH_ARMv7)
using vec4d = float32x4_t;
#else
using vec4d = float64x2_t;
#endif

template<int X, int Y, int Z, int W>
inline vec4f SM_Permute(vec4f v)
{
    static_assert(X >= 0 && X < 4);
    static_assert(Y >= 0 && Y < 4);
    static_assert(Z >= 0 && Z < 4);
    static_assert(W >= 0 && W < 4);

    vec4f r = vdupq_n_f32(0.0f);

    r = vsetq_lane_f32(vgetq_lane_f32(v, X), r, 0);
    r = vsetq_lane_f32(vgetq_lane_f32(v, Y), r, 1);
    r = vsetq_lane_f32(vgetq_lane_f32(v, Z), r, 2);
    r = vsetq_lane_f32(vgetq_lane_f32(v, W), r, 3);

    return r;
}

inline vec4f _set_vec4f(float x, float y, float z, float w) {
    float arr[4] = {x, y, z, w};
    return vld1q_f32(arr);
}
inline vec4i _set_vec4i(int32_t x, int32_t y, int32_t z, int32_t w) {
    int32_t arr[4] = {x, y, z, w};
    return vld1q_s32(arr);
}
inline vec4ui _set_vec4ui(uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
    uint32_t arr[4] = {x, y, z, w};
    return vld1q_u32(arr);
}

#ifndef _set1_vec4f
#define _set1_vec4f(x) vdupq_n_f32(x)
#endif

#ifndef _set1_vec4i
#define _set1_vec4i(x) vdupq_n_s32(x)
#endif

#ifndef _set1_vec4ui
#define _set1_vec4ui(x) vdupq_n_u32(x)
#endif

#ifndef _SM_PERMUTE
#define _SM_PERMUTE(v, x, y, z, w) SM_Permute<x, y, z, w>(v)
#endif

#endif


namespace krystallic
{
    namespace SIMDMath
    {
        constexpr unsigned int FLOAT_SIGN_MASK = 0x80000000u;
        constexpr unsigned int FLOAT_EXPONENT_MASK = 0x7F800000u;
        constexpr unsigned int FLOAT_MANTISSA_MASK = 0x007FFFFFu;
        constexpr unsigned int FLOAT_ABS_MASK = 0x7FFFFFFFu;

        inline bool HasFloatsOneInf(const float* floats, int numFloats)
        {
            const unsigned char* bytes = reinterpret_cast<const unsigned char*>(floats);

            for (int i = 0; i < numFloats; ++i)
            {
                unsigned int bits =
                    ((unsigned int)bytes[i * 4 + 0]) |
                    ((unsigned int)bytes[i * 4 + 1] << 8) |
                    ((unsigned int)bytes[i * 4 + 2] << 16) |
                    ((unsigned int)bytes[i * 4 + 3] << 24);

                if ((bits & 0x7fffffffu) == 0x7f800000u)
                    return true;
            }

            return false;
        }

        inline bool HasFloatsOneNaN(const float* floats, int numFloats)
        {
            const unsigned char* bytes = reinterpret_cast<const unsigned char*>(floats);

            for (int i = 0; i < numFloats; ++i)
            {
                unsigned int bits =
                    ((unsigned int)bytes[i * 4 + 0]) |
                    ((unsigned int)bytes[i * 4 + 1] << 8) |
                    ((unsigned int)bytes[i * 4 + 2] << 16) |
                    ((unsigned int)bytes[i * 4 + 3] << 24);

                if (((bits & FLOAT_EXPONENT_MASK) == FLOAT_EXPONENT_MASK) && ((bits & FLOAT_MANTISSA_MASK) != 0))
                    return true;
            }

            return false;
        }
    }
}
