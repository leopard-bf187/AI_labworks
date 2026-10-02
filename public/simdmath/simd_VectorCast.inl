#pragma once

#include <cstring>
#include "simd_library.h"

namespace krystallic
{
    namespace SIMDMath
    {
        inline R128x1I __vectorcall Cast128x1FI(R128x1F_Arg0 v) noexcept
        {
            R128x1I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castps_si128(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_s32_f32(v.v1);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x1D __vectorcall Cast128x1FD(R128x1F_Arg0 v) noexcept
        {
            R128x1D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castps_pd(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x1F __vectorcall Cast128x1IF(R128x1I_Arg0 v) noexcept
        {
            R128x1F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castsi128_ps(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_f32_s32(v.v1);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x1D __vectorcall Cast128x1ID(R128x1I_Arg0 v) noexcept
        {
            R128x1D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castsi128_pd(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x1F __vectorcall Cast128x1DF(R128x1D_Arg0 v) noexcept
        {
            R128x1F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castpd_ps(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x1I __vectorcall Cast128x1DI(R128x1D_Arg0 v) noexcept
        {
            R128x1I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castpd_si128(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x2I __vectorcall Cast128x2FI(R128x2F_Arg0 v) noexcept
        {
            R128x2I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castps_si128(v.v1);
            r.v2 = _mm_castps_si128(v.v2);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_s32_f32(v.v1);
            r.v2 = vreinterpretq_s32_f32(v.v2);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x2D __vectorcall Cast128x2FD(R128x2F_Arg0 v) noexcept
        {
            R128x2D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castps_pd(v.v1);
            r.v2 = _mm_castps_pd(v.v2);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x2F __vectorcall Cast128x2IF(R128x2I_Arg0 v) noexcept
        {
            R128x2F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castsi128_ps(v.v1);
            r.v2 = _mm_castsi128_ps(v.v2);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_f32_s32(v.v1);
            r.v2 = vreinterpretq_f32_s32(v.v2);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x2D __vectorcall Cast128x2ID(R128x2I_Arg0 v) noexcept
        {
            R128x2D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castsi128_pd(v.v1);
            r.v2 = _mm_castsi128_pd(v.v2);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x2F __vectorcall Cast128x2DF(R128x2D_Arg0 v) noexcept
        {
            R128x2F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castpd_ps(v.v1);
            r.v2 = _mm_castpd_ps(v.v2);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x2I __vectorcall Cast128x2DI(R128x2D_Arg0 v) noexcept
        {
            R128x2I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castpd_si128(v.v1);
            r.v2 = _mm_castpd_si128(v.v2);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x3I __vectorcall Cast128x3FI(R128x3F_Arg0 v) noexcept
        {
            R128x3I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castps_si128(v.v1);
            r.v2 = _mm_castps_si128(v.v2);
            r.v3 = _mm_castps_si128(v.v3);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_s32_f32(v.v1);
            r.v2 = vreinterpretq_s32_f32(v.v2);
            r.v3 = vreinterpretq_s32_f32(v.v3);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x3D __vectorcall Cast128x3FD(R128x3F_Arg0 v) noexcept
        {
            R128x3D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castps_pd(v.v1);
            r.v2 = _mm_castps_pd(v.v2);
            r.v3 = _mm_castps_pd(v.v3);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x3F __vectorcall Cast128x3IF(R128x3I_Arg0 v) noexcept
        {
            R128x3F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castsi128_ps(v.v1);
            r.v2 = _mm_castsi128_ps(v.v2);
            r.v3 = _mm_castsi128_ps(v.v3);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_f32_s32(v.v1);
            r.v2 = vreinterpretq_f32_s32(v.v2);
            r.v3 = vreinterpretq_f32_s32(v.v3);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x3D __vectorcall Cast128x3ID(R128x3I_Arg0 v) noexcept
        {
            R128x3D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castsi128_pd(v.v1);
            r.v2 = _mm_castsi128_pd(v.v2);
            r.v3 = _mm_castsi128_pd(v.v3);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x3F __vectorcall Cast128x3DF(R128x3D_Arg0 v) noexcept
        {
            R128x3F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castpd_ps(v.v1);
            r.v2 = _mm_castpd_ps(v.v2);
            r.v3 = _mm_castpd_ps(v.v3);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x3I __vectorcall Cast128x3DI(R128x3D_Arg0 v) noexcept
        {
            R128x3I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castpd_si128(v.v1);
            r.v2 = _mm_castpd_si128(v.v2);
            r.v3 = _mm_castpd_si128(v.v3);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x4I __vectorcall Cast128x4FI(R128x4F_Arg0 v) noexcept
        {
            R128x4I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castps_si128(v.v1);
            r.v2 = _mm_castps_si128(v.v2);
            r.v3 = _mm_castps_si128(v.v3);
            r.v4 = _mm_castps_si128(v.v4);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_s32_f32(v.v1);
            r.v2 = vreinterpretq_s32_f32(v.v2);
            r.v3 = vreinterpretq_s32_f32(v.v3);
            r.v4 = vreinterpretq_s32_f32(v.v4);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x4D __vectorcall Cast128x4FD(R128x4F_Arg0 v) noexcept
        {
            R128x4D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castps_pd(v.v1);
            r.v2 = _mm_castps_pd(v.v2);
            r.v3 = _mm_castps_pd(v.v3);
            r.v4 = _mm_castps_pd(v.v4);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
            memcpy(&r.v4, &v.v4, sizeof(r.v4));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x4F __vectorcall Cast128x4IF(R128x4I_Arg0 v) noexcept
        {
            R128x4F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castsi128_ps(v.v1);
            r.v2 = _mm_castsi128_ps(v.v2);
            r.v3 = _mm_castsi128_ps(v.v3);
            r.v4 = _mm_castsi128_ps(v.v4);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_f32_s32(v.v1);
            r.v2 = vreinterpretq_f32_s32(v.v2);
            r.v3 = vreinterpretq_f32_s32(v.v3);
            r.v4 = vreinterpretq_f32_s32(v.v4);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x4D __vectorcall Cast128x4ID(R128x4I_Arg0 v) noexcept
        {
            R128x4D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castsi128_pd(v.v1);
            r.v2 = _mm_castsi128_pd(v.v2);
            r.v3 = _mm_castsi128_pd(v.v3);
            r.v4 = _mm_castsi128_pd(v.v4);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
            memcpy(&r.v4, &v.v4, sizeof(r.v4));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x4F __vectorcall Cast128x4DF(R128x4D_Arg0 v) noexcept
        {
            R128x4F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castpd_ps(v.v1);
            r.v2 = _mm_castpd_ps(v.v2);
            r.v3 = _mm_castpd_ps(v.v3);
            r.v4 = _mm_castpd_ps(v.v4);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
            memcpy(&r.v4, &v.v4, sizeof(r.v4));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R128x4I __vectorcall Cast128x4DI(R128x4D_Arg0 v) noexcept
        {
            R128x4I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_castpd_si128(v.v1);
            r.v2 = _mm_castpd_si128(v.v2);
            r.v3 = _mm_castpd_si128(v.v3);
            r.v4 = _mm_castpd_si128(v.v4);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
            memcpy(&r.v4, &v.v4, sizeof(r.v4));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x1I __vectorcall Cast256x1FI(R256x1F_Arg0 v) noexcept
        {
            R256x1I r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castps_si128(v.v1);
            r.v2 = _mm_castps_si128(v.v2);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castps_si256(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_s32_f32(v.v1);
            r.v2 = vreinterpretq_s32_f32(v.v2);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x1D __vectorcall Cast256x1FD(R256x1F_Arg0 v) noexcept
        {
            R256x1D r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castps_pd(v.v1);
            r.v2 = _mm_castps_pd(v.v2);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castps_pd(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x1F __vectorcall Cast256x1IF(R256x1I_Arg0 v) noexcept
        {
            R256x1F r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castsi128_ps(v.v1);
            r.v2 = _mm_castsi128_ps(v.v2);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castsi256_ps(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_f32_s32(v.v1);
            r.v2 = vreinterpretq_f32_s32(v.v2);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x1D __vectorcall Cast256x1ID(R256x1I_Arg0 v) noexcept
        {
            R256x1D r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castsi128_pd(v.v1);
            r.v2 = _mm_castsi128_pd(v.v2);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castsi256_pd(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x1F __vectorcall Cast256x1DF(R256x1D_Arg0 v) noexcept
        {
            R256x1F r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castpd_ps(v.v1);
            r.v2 = _mm_castpd_ps(v.v2);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castpd_ps(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x1I __vectorcall Cast256x1DI(R256x1D_Arg0 v) noexcept
        {
            R256x1I r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castpd_si128(v.v1);
            r.v2 = _mm_castpd_si128(v.v2);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castpd_si256(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x2I __vectorcall Cast256x2FI(R256x2F_Arg0 v) noexcept
        {
            R256x2I r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castps_si128(v.v1);
            r.v2 = _mm_castps_si128(v.v2);
            r.v3 = _mm_castps_si128(v.v3);
            r.v4 = _mm_castps_si128(v.v4);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castps_si256(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_s32_f32(v.v1);
            r.v2 = vreinterpretq_s32_f32(v.v2);
            r.v3 = vreinterpretq_s32_f32(v.v3);
            r.v4 = vreinterpretq_s32_f32(v.v4);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x2D __vectorcall Cast256x2FD(R256x2F_Arg0 v) noexcept
        {
            R256x2D r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castps_pd(v.v1);
            r.v2 = _mm_castps_pd(v.v2);
            r.v3 = _mm_castps_pd(v.v3);
            r.v4 = _mm_castps_pd(v.v4);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castps_pd(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
            memcpy(&r.v4, &v.v4, sizeof(r.v4));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x2F __vectorcall Cast256x2IF(R256x2I_Arg0 v) noexcept
        {
            R256x2F r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castsi128_ps(v.v1);
            r.v2 = _mm_castsi128_ps(v.v2);
            r.v3 = _mm_castsi128_ps(v.v3);
            r.v4 = _mm_castsi128_ps(v.v4);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castsi256_ps(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vreinterpretq_f32_s32(v.v1);
            r.v2 = vreinterpretq_f32_s32(v.v2);
            r.v3 = vreinterpretq_f32_s32(v.v3);
            r.v4 = vreinterpretq_f32_s32(v.v4);
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x2D __vectorcall Cast256x2ID(R256x2I_Arg0 v) noexcept
        {
            R256x2D r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castsi128_pd(v.v1);
            r.v2 = _mm_castsi128_pd(v.v2);
            r.v3 = _mm_castsi128_pd(v.v3);
            r.v4 = _mm_castsi128_pd(v.v4);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castsi256_pd(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
            memcpy(&r.v4, &v.v4, sizeof(r.v4));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x2F __vectorcall Cast256x2DF(R256x2D_Arg0 v) noexcept
        {
            R256x2F r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castpd_ps(v.v1);
            r.v2 = _mm_castpd_ps(v.v2);
            r.v3 = _mm_castpd_ps(v.v3);
            r.v4 = _mm_castpd_ps(v.v4);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castpd_ps(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
            memcpy(&r.v4, &v.v4, sizeof(r.v4));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


        inline R256x2I __vectorcall Cast256x2DI(R256x2D_Arg0 v) noexcept
        {
            R256x2I r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_castpd_si128(v.v1);
            r.v2 = _mm_castpd_si128(v.v2);
            r.v3 = _mm_castpd_si128(v.v3);
            r.v4 = _mm_castpd_si128(v.v4);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_castpd_si256(v.v1);
#elif defined(SIMD_ARM_NEON)
            memcpy(&r.v1, &v.v1, sizeof(r.v1));
            memcpy(&r.v2, &v.v2, sizeof(r.v2));
            memcpy(&r.v3, &v.v3, sizeof(r.v3));
            memcpy(&r.v4, &v.v4, sizeof(r.v4));
#elif defined(SIMD_NONE)
            memcpy(&r, &v, sizeof(r));
#endif
            return r;
        }


    } // namespace SIMDMath
} // namespace krystallic
