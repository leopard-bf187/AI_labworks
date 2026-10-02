/*
*  Authors:  Leonid (@LeoParD) Parmacli  &&  Victor (@RisovoePole) Anisimov
*
*  Description:
*
*  Date: 22.03.2026
*/


#pragma once


#include "simd_library.h"
#include "constants.h"
#include "simd_registers_SSE42.h"



namespace krystallic
{
    namespace SIMDMath
    {
        // --------------------------- 128bit (SSE4 size) registers --------------------------- //
//----------------------FLOAT
        inline R128x1F R128x1F_Zero() noexcept
        {
            R128x1F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_ps();
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_f32(0.0f);
#endif
            return res;
        }

        inline R128x1F R128x1F_One(float v) noexcept
        {
            R128x1F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_ps(v);
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_f32(v);
#endif
            return res;
        }

        inline R128x1F R128x1F_Set(float v1, float v2, float v3, float v4) noexcept
        {
            R128x1F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_ps(v1, v2, v3, v4);
#elif defined(SIMD_ARM_NEON)
            const float tmp1[4] = { v1, v2, v3, v4 };
            res.v1 = vld1q_f32(tmp1);
#endif
            return res;
        }

        inline R128x2F R128x2F_Zero() noexcept
        {
            R128x2F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_ps();
            res.v2 = _mm_setzero_ps();
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_f32(0.0f);
            res.v2 = vdupq_n_f32(0.0f);
#endif
            return res;
        }

        inline R128x2F R128x2F_One(float v) noexcept
        {
            R128x2F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_ps(v);
            res.v2 = _mm_set1_ps(v);
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_f32(v);
            res.v2 = vdupq_n_f32(v);
#endif
            return res;
        }
        inline R128x2F R128x2F_Set(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8) noexcept
        {
            R128x2F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_ps(v1, v2, v3, v4);
            res.v2 = _mm_setr_ps(v5, v6, v7, v8);
#elif defined(SIMD_ARM_NEON)
            const float tmp1[4] = { v1, v2, v3, v4 };
            res.v1 = vld1q_f32(tmp1);
            const float tmp2[4] = { v5, v6, v7, v8 };
            res.v2 = vld1q_f32(tmp2);
#endif
            return res;
        }

        inline R128x3F R128x3F_Zero() noexcept
        {
            R128x3F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_ps();
            res.v2 = _mm_setzero_ps();
            res.v3 = _mm_setzero_ps();
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_f32(0.0f);
            res.v2 = vdupq_n_f32(0.0f);
            res.v3 = vdupq_n_f32(0.0f);
#endif
            return res;
        }

        inline R128x3F R128x3F_One(float v) noexcept
        {
            R128x3F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_ps(v);
            res.v2 = _mm_set1_ps(v);
            res.v3 = _mm_set1_ps(v);
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_f32(v);
            res.v2 = vdupq_n_f32(v);
            res.v3 = vdupq_n_f32(v);
#endif
            return res;
        }

        inline R128x3F R128x3F_Set(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12) noexcept
        {
            R128x3F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_ps(v1, v2, v3, v4);
            res.v2 = _mm_setr_ps(v5, v6, v7, v8);
            res.v3 = _mm_setr_ps(v9, v10, v11, v12);
#elif defined(SIMD_ARM_NEON)
            const float tmp1[4] = { v1, v2, v3, v4 };
            res.v1 = vld1q_f32(tmp1);
            const float tmp2[4] = { v5, v6, v7, v8 };
            res.v2 = vld1q_f32(tmp2);
            const float tmp3[4] = { v9, v10, v11, v12 };
            res.v3 = vld1q_f32(tmp3);
#endif
            return res;
        }

        inline R128x4F R128x4F_Zero() noexcept
        {
            R128x4F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_ps();
            res.v2 = _mm_setzero_ps();
            res.v3 = _mm_setzero_ps();
            res.v4 = _mm_setzero_ps();
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_f32(0.0f);
            res.v2 = vdupq_n_f32(0.0f);
            res.v3 = vdupq_n_f32(0.0f);
            res.v4 = vdupq_n_f32(0.0f);
#endif
            return res;
        }
        inline R128x4F R128x4F_One(float v) noexcept
        {
            R128x4F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_ps(v);
            res.v2 = _mm_set1_ps(v);
            res.v3 = _mm_set1_ps(v);
            res.v4 = _mm_set1_ps(v);
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_f32(v);
            res.v2 = vdupq_n_f32(v);
            res.v3 = vdupq_n_f32(v);
            res.v4 = vdupq_n_f32(v);
#endif
            return res;
        }

        inline R128x4F R128x4F_Set(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15, float v16) noexcept
        {
            R128x4F res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_ps(v1, v2, v3, v4);
            res.v2 = _mm_setr_ps(v5, v6, v7, v8);
            res.v3 = _mm_setr_ps(v9, v10, v11, v12);
            res.v4 = _mm_setr_ps(v13, v14, v15, v16);
#elif defined(SIMD_ARM_NEON)
            const float tmp1[4] = { v1, v2, v3, v4 };
            res.v1 = vld1q_f32(tmp1);
            const float tmp2[4] = { v5, v6, v7, v8 };
            res.v2 = vld1q_f32(tmp2);
            const float tmp3[4] = { v9, v10, v11, v12 };
            res.v3 = vld1q_f32(tmp3);
            const float tmp4[4] = { v13, v14, v15, v16 };
            res.v4 = vld1q_f32(tmp4);
#endif
            return res;
        }
        //----------------------INTEGER

        inline R128x1I R128x1I_Zero() noexcept
        {
            R128x1I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_si128();
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_s32(0);
#endif
            return res;
        }
        inline R128x1I R128x1I_One(int v) noexcept
        {
            R128x1I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_epi32(v);
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_s32(v);
#endif
            return res;
        }
        inline R128x1I R128x1I_Set(int v1, int v2, int v3, int v4) noexcept
        {
            R128x1I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_epi32(v1, v2, v3, v4);
#elif defined(SIMD_ARM_NEON)
            const int32_t tmp1[4] = { v1, v2, v3, v4 };
            res.v1 = vld1q_s32(tmp1);
#endif
            return res;
        }

        inline R128x2I R128x2I_Zero() noexcept
        {
            R128x2I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_si128();
            res.v2 = _mm_setzero_si128();
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_s32(0);
            res.v2 = vdupq_n_s32(0);
#endif
            return res;
        }
        inline R128x2I R128x2I_One(int v) noexcept
        {
            R128x2I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_epi32(v);
            res.v2 = _mm_set1_epi32(v);
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_s32(v);
            res.v2 = vdupq_n_s32(v);
#endif
            return res;
        }
        inline R128x2I R128x2I_Set(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8) noexcept
        {
            R128x2I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_epi32(v1, v2, v3, v4);
            res.v2 = _mm_setr_epi32(v5, v6, v7, v8);
#elif defined(SIMD_ARM_NEON)
            const int32_t tmp1[4] = { v1, v2, v3, v4 };
            res.v1 = vld1q_s32(tmp1);
            const int32_t tmp2[4] = { v5, v6, v7, v8 };
            res.v2 = vld1q_s32(tmp2);
#endif
            return res;
        }

        inline R128x3I R128x3I_Zero() noexcept
        {
            R128x3I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_si128();
            res.v2 = _mm_setzero_si128();
            res.v3 = _mm_setzero_si128();
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_s32(0);
            res.v2 = vdupq_n_s32(0);
            res.v3 = vdupq_n_s32(0);
#endif
            return res;
        }

        inline R128x3I R128x3I_One(int v) noexcept
        {
            R128x3I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_epi32(v);
            res.v2 = _mm_set1_epi32(v);
            res.v3 = _mm_set1_epi32(v);
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_s32(v);
            res.v2 = vdupq_n_s32(v);
            res.v3 = vdupq_n_s32(v);
#endif
            return res;
        }

        inline R128x3I R128x3I_Set(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9, int v10, int v11, int v12) noexcept
        {
            R128x3I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_epi32(v1, v2, v3, v4);
            res.v2 = _mm_setr_epi32(v5, v6, v7, v8);
            res.v3 = _mm_setr_epi32(v9, v10, v11, v12);
#elif defined(SIMD_ARM_NEON)
            const int32_t tmp1[4] = { v1, v2, v3, v4 };
            res.v1 = vld1q_s32(tmp1);
            const int32_t tmp2[4] = { v5, v6, v7, v8 };
            res.v2 = vld1q_s32(tmp2);
            const int32_t tmp3[4] = { v9, v10, v11, v12 };
            res.v3 = vld1q_s32(tmp3);
#endif
            return res;
        }

        inline R128x4I R128x4I_Zero() noexcept
        {
            R128x4I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_si128();
            res.v2 = _mm_setzero_si128();
            res.v3 = _mm_setzero_si128();
            res.v4 = _mm_setzero_si128();
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_s32(0);
            res.v2 = vdupq_n_s32(0);
            res.v3 = vdupq_n_s32(0);
            res.v4 = vdupq_n_s32(0);
#endif
            return res;
        }
        inline R128x4I R128x4I_One(int v) noexcept
        {
            R128x4I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_epi32(v);
            res.v2 = _mm_set1_epi32(v);
            res.v3 = _mm_set1_epi32(v);
            res.v4 = _mm_set1_epi32(v);
#elif defined(SIMD_ARM_NEON)
            res.v1 = vdupq_n_s32(v);
            res.v2 = vdupq_n_s32(v);
            res.v3 = vdupq_n_s32(v);
            res.v4 = vdupq_n_s32(v);
#endif
            return res;
        }

        inline R128x4I R128x4I_Set(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9, int v10, int v11, int v12, int v13, int v14, int v15, int v16) noexcept
        {
            R128x4I res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_epi32(v1, v2, v3, v4);
            res.v2 = _mm_setr_epi32(v5, v6, v7, v8);
            res.v3 = _mm_setr_epi32(v9, v10, v11, v12);
            res.v4 = _mm_setr_epi32(v13, v14, v15, v16);
#elif defined(SIMD_ARM_NEON)
            const int32_t tmp1[4] = { v1, v2, v3, v4 };
            res.v1 = vld1q_s32(tmp1);
            const int32_t tmp2[4] = { v5, v6, v7, v8 };
            res.v2 = vld1q_s32(tmp2);
            const int32_t tmp3[4] = { v9, v10, v11, v12 };
            res.v3 = vld1q_s32(tmp3);
            const int32_t tmp4[4] = { v13, v14, v15, v16 };
            res.v4 = vld1q_s32(tmp4);
#endif
            return res;
        }


        //----------------------DOUBLE

        inline R128x1D R128x1D_Zero() noexcept
        {
            R128x1D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_pd();
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            res.v1 = vdupq_n_f64(0.0);
#else
            res.v1.lo = 0.0;
            res.v1.hi = 0.0;
#endif
#endif
            return res;
        }
        inline R128x1D R128x1D_One(double v) noexcept
        {
            R128x1D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_pd(v);
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            res.v1 = vdupq_n_f64(v);
#else
            res.v1.lo = v;
            res.v1.hi = v;
#endif
#endif
            return res;
        }
        inline R128x1D R128x1D_Set(double v1, double v2) noexcept
        {
            R128x1D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_pd(v1, v2);
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            const double tmp1[2] = { v1, v2 };
            res.v1 = vld1q_f64(tmp1);
#else
            res.v1.lo = v1;
            res.v1.hi = v2;
#endif
#endif
            return res;
        }

        inline R128x2D R128x2D_Zero() noexcept
        {
            R128x2D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_pd();
            res.v2 = _mm_setzero_pd();
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            res.v1 = vdupq_n_f64(0.0);
            res.v2 = vdupq_n_f64(0.0);
#else
            res.v1.lo = 0.0;
            res.v1.hi = 0.0;
            res.v2.lo = 0.0;
            res.v2.hi = 0.0;
#endif
#endif
            return res;
        }
        inline R128x2D R128x2D_One(double v) noexcept
        {
            R128x2D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_pd(v);
            res.v2 = _mm_set1_pd(v);
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            res.v1 = vdupq_n_f64(v);
            res.v2 = vdupq_n_f64(v);
#else
            res.v1.lo = v;
            res.v1.hi = v;
            res.v2.lo = v;
            res.v2.hi = v;
#endif
#endif
            return res;
        }
        inline R128x2D R128x2D_Set(double v1, double v2, double v3, double v4) noexcept
        {
            R128x2D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_pd(v1, v2);
            res.v2 = _mm_setr_pd(v3, v4);
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            const double tmp1[2] = { v1, v2 };
            res.v1 = vld1q_f64(tmp1);
            const double tmp2[2] = { v3, v4 };
            res.v2 = vld1q_f64(tmp2);
#else
            res.v1.lo = v1;
            res.v1.hi = v2;
            res.v2.lo = v3;
            res.v2.hi = v4;
#endif
#endif
            return res;
        }

        inline R128x3D R128x3D_Zero() noexcept
        {
            R128x3D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_pd();
            res.v2 = _mm_setzero_pd();
            res.v3 = _mm_setzero_pd();
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            res.v1 = vdupq_n_f64(0.0);
            res.v2 = vdupq_n_f64(0.0);
            res.v3 = vdupq_n_f64(0.0);
#else
            res.v1.lo = 0.0;
            res.v1.hi = 0.0;
            res.v2.lo = 0.0;
            res.v2.hi = 0.0;
            res.v3.lo = 0.0;
            res.v3.hi = 0.0;
#endif
#endif
            return res;
        }

        inline R128x3D R128x3D_One(double v) noexcept
        {
            R128x3D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_pd(v);
            res.v2 = _mm_set1_pd(v);
            res.v3 = _mm_set1_pd(v);
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            res.v1 = vdupq_n_f64(v);
            res.v2 = vdupq_n_f64(v);
            res.v3 = vdupq_n_f64(v);
#else
            res.v1.lo = v;
            res.v1.hi = v;
            res.v2.lo = v;
            res.v2.hi = v;
            res.v3.lo = v;
            res.v3.hi = v;
#endif
#endif
            return res;
        }


        inline R128x3D R128x3D_Set(double v1, double v2, double v3, double v4, double v5, double v6) noexcept
        {
            R128x3D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_pd(v1, v2);
            res.v2 = _mm_setr_pd(v3, v4);
            res.v3 = _mm_setr_pd(v5, v6);
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            const double tmp1[2] = { v1, v2 };
            res.v1 = vld1q_f64(tmp1);
            const double tmp2[2] = { v3, v4 };
            res.v2 = vld1q_f64(tmp2);
            const double tmp3[2] = { v5, v6 };
            res.v3 = vld1q_f64(tmp3);
#else
            res.v1.lo = v1;
            res.v1.hi = v2;
            res.v2.lo = v3;
            res.v2.hi = v4;
            res.v3.lo = v5;
            res.v3.hi = v6;
#endif
#endif
            return res;
        }


        //        inline R128x3F R128x3F_Set(double v1, double v2, double v3, double v4, double v5, double v6) noexcept
        //        {
        //            R128x3D res = {};
        //#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
        //            res.v1 = _mm_setr_pd(v1, v2);
        //            res.v2 = _mm_setr_pd(v3, v4);
        //            res.v3 = _mm_setr_pd(v5, v6);
        //#elif defined(SIMD_ARM_NEON)
        //#endif
        //             return res;
        //        }

        inline R128x4D R128x4D_Zero() noexcept
        {
            R128x4D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setzero_pd();
            res.v2 = _mm_setzero_pd();
            res.v3 = _mm_setzero_pd();
            res.v4 = _mm_setzero_pd();
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            res.v1 = vdupq_n_f64(0.0);
            res.v2 = vdupq_n_f64(0.0);
            res.v3 = vdupq_n_f64(0.0);
            res.v4 = vdupq_n_f64(0.0);
#else
            res.v1.lo = 0.0;
            res.v1.hi = 0.0;
            res.v2.lo = 0.0;
            res.v2.hi = 0.0;
            res.v3.lo = 0.0;
            res.v3.hi = 0.0;
            res.v4.lo = 0.0;
            res.v4.hi = 0.0;
#endif
#endif
            return res;
        }
        inline R128x4D R128x4D_One(double v) noexcept
        {
            R128x4D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_set1_pd(v);
            res.v2 = _mm_set1_pd(v);
            res.v3 = _mm_set1_pd(v);
            res.v4 = _mm_set1_pd(v);
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            res.v1 = vdupq_n_f64(v);
            res.v2 = vdupq_n_f64(v);
            res.v3 = vdupq_n_f64(v);
            res.v4 = vdupq_n_f64(v);
#else
            res.v1.lo = v;
            res.v1.hi = v;
            res.v2.lo = v;
            res.v2.hi = v;
            res.v3.lo = v;
            res.v3.hi = v;
            res.v4.lo = v;
            res.v4.hi = v;
#endif
#endif
            return res;
        }

        inline R128x4D R128x4D_Set(double v1, double v2, double v3, double v4, double v5, double v6, double v7, double v8) noexcept
        {
            R128x4D res = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            res.v1 = _mm_setr_pd(v1, v2);
            res.v2 = _mm_setr_pd(v3, v4);
            res.v3 = _mm_setr_pd(v5, v6);
            res.v4 = _mm_setr_pd(v7, v8);
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
            const double tmp1[2] = { v1, v2 };
            res.v1 = vld1q_f64(tmp1);
            const double tmp2[2] = { v3, v4 };
            res.v2 = vld1q_f64(tmp2);
            const double tmp3[2] = { v5, v6 };
            res.v3 = vld1q_f64(tmp3);
            const double tmp4[2] = { v7, v8 };
            res.v4 = vld1q_f64(tmp4);
#else
            res.v1.lo = v1;
            res.v1.hi = v2;
            res.v2.lo = v3;
            res.v2.hi = v4;
            res.v3.lo = v5;
            res.v3.hi = v6;
            res.v4.lo = v7;
            res.v4.hi = v8;
#endif
#endif
            return res;
        }

        // --------------------------- 256 bit (AVX size) registers --------------------------- //

    //----------------------FLOAT
    /*
            R256x1F R256x1F_Zero() noexcept
            {
               R256x1F res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x1F R256x1F_One(float v) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x1F R256x1F_Set(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }


            R256x2F R256x2F_Zero() noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x2F R256x2F_One(float v) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x2F R256x2F_Set(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15, float v16) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }



    //----------------------INTEGER


            R256x1I R256x1I_Zero() noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x1I R256x1I_One(int v) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x1I R256x1I_Set(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }


            R256x2I R256x2I_Zero() noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x2I R256x2I_One(int v) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x2I R256x2I_Set(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9, int v10, int v11, int v12, int v13, int v14, int v15, int v16) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }



    //----------------------DOUBLE


            R256x1D R256x1D_Zero() noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x1D R256x1D_One(double v) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x1D R256x1D_Set(double v1, double v2, double v3, double v4) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }


            R256x2D R256x2D_Zero() noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x2D R256x2D_One(double v) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

            R256x2D R256x2D_Set(double v1, double v2, double v3, double v4, double v5, double v6, double v7, double v8) noexcept
            {
               XX res;
    #if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)

    #elif defined(SIMD_ARM_NEON)
    #endif
                return res;
            }

    */
    //-----------------------------------------------------




        inline R128x1F __vectorcall R128x1F_LoadRGBA(const CColorRGBA& v) noexcept
        {
            R128x1F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            result.v1 = _mm_set_ps(v.a, v.b, v.g, v.r);
#elif defined(SIMD_ARM_NEON)
            const float tmp[4] = { v.r, v.g, v.b, v.a };
            result.v1 = vld1q_f32(tmp);
#endif
            return result;
        }


        inline R128x1F __vectorcall R128x1F_LoadHSLA(const CColorHSLA& v) noexcept
        {
            R128x1F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(v.a, v.l, v.s, v.h);
#elif defined(SIMD_ARM_NEON)
            const float tmp[4] = { v.h, v.s, v.l, v.a };
            result.v1 = vld1q_f32(tmp);
#endif
            return result;
        }


        inline R128x1F __vectorcall R128x1F_LoadHSVA(const CColorHSVA& v) noexcept
        {
            R128x1F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(v.a, v.v, v.s, v.h);
#elif defined(SIMD_ARM_NEON)
            const float tmp[4] = { v.h, v.s, v.v, v.a };
            result.v1 = vld1q_f32(tmp);
#endif
            return result;
        }


        inline R128x1F __vectorcall R128x1F_LoadYUVA(const CColorYUVA& v) noexcept
        {
            R128x1F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(v.a, v.v, v.u, v.y);
#elif defined(SIMD_ARM_NEON)
            const float tmp[4] = { v.y, v.u, v.v, v.a };
            result.v1 = vld1q_f32(tmp);
#endif
            return result;
        }


        inline R128x1F __vectorcall R128x1F_LoadVec2(const CVector2& v) noexcept
        {
            R128x1F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(1.0f, 0.0f, v.y, v.x);
#elif defined(SIMD_ARM_NEON)
            const float tmp[4] = { v.x, v.y, 0.0f, 1.0f };
            result.v1 = vld1q_f32(tmp);
#endif
            return result;
        }


        inline R128x1F __vectorcall R128x1F_LoadVec3(const CVector3& v) noexcept
        {
            R128x1F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(1.0f, v.z, v.y, v.x);
#elif defined(SIMD_ARM_NEON)
            const float tmp[4] = { v.x, v.y, v.z, 1.0f };
            result.v1 = vld1q_f32(tmp);
#endif
            return result;
        }


        inline R128x1F __vectorcall R128x1F_LoadVec4(const CVector4& v) noexcept
        {
            R128x1F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(v.w, v.z, v.y, v.x);
#elif defined(SIMD_ARM_NEON)
            const float tmp[4] = { v.x, v.y, v.z, v.w };
            result.v1 = vld1q_f32(tmp);
#endif
            return result;
        }


        inline R128x1F __vectorcall R128x1F_LoadQuat(const CQuaternion& v) noexcept
        {
            R128x1F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(v.w, v.z, v.y, v.x);
#elif defined(SIMD_ARM_NEON)
            const float tmp[4] = { v.x, v.y, v.z, v.w };
            result.v1 = vld1q_f32(tmp);
#endif
            return result;
        }


        inline R128x2F __vectorcall R128x2F_LoadMat22(const CMatrix2x2& v) noexcept
        {
            R128x2F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(0.0f, 0.0f, v._12, v._11);
            result.v2 = _mm_set_ps(0.0f, 0.0f, v._22, v._21);
#elif defined(SIMD_ARM_NEON)
            const float tmp1[4] = { v._11, v._12, 0.0f, 0.0f };
            result.v1 = vld1q_f32(tmp1);
            const float tmp2[4] = { v._21, v._22, 0.0f, 0.0f };
            result.v2 = vld1q_f32(tmp2);
#endif
            return result;
        }

        inline R256x1F __vectorcall R256x1F_LoadMat22(const CMatrix2x2& v) noexcept
        {
            R256x1F result = {};
#if defined(SIMD_SSE42)
            result.v1 = _mm_set_ps(0.0f, 0.0f, v._12, v._11);
            result.v2 = _mm_set_ps(0.0f, 0.0f, v._22, v._21);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm256_setr_ps(v._11, v._12, 0.0f, 0.0f, v._21, v._22, 0.0f, 0.0f);
#elif defined(SIMD_ARM_NEON)
            const float tmp1[4] = { v._11, v._12, 0.0f, 0.0f };
            const float tmp2[4] = { v._21, v._22, 0.0f, 0.0f };
            result.v1 = vld1q_f32(tmp1);
            result.v2 = vld1q_f32(tmp2);
#endif
            return result;
        }


        inline R128x3F __vectorcall R128x3F_LoadMat33(const CMatrix3x3& v) noexcept
        {
            R128x3F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(0.0f, v._13, v._12, v._11);
            result.v2 = _mm_set_ps(0.0f, v._23, v._22, v._21);
            result.v3 = _mm_set_ps(0.0f, v._33, v._32, v._31);
#elif defined(SIMD_ARM_NEON)
            const float tmp1[4] = { v._11, v._12, v._13, 0.0f };
            const float tmp2[4] = { v._21, v._22, v._23, 0.0f };
            const float tmp3[4] = { v._31, v._32, v._33, 0.0f };

            result.v1 = vld1q_f32(tmp1);
            result.v2 = vld1q_f32(tmp2);
            result.v3 = vld1q_f32(tmp3);
#endif
            return result;
        }


        inline R256x2F __vectorcall R256x2F_LoadMat33(const CMatrix3x3& v) noexcept
        {
            R256x2F result = {};
#if defined(SIMD_SSE42)
            result.v1 = _mm_set_ps(0.0f, v._13, v._12, v._11);
            result.v2 = _mm_set_ps(0.0f, v._23, v._22, v._21);
            result.v3 = _mm_set_ps(0.0f, v._33, v._32, v._31);
            result.v4 = _mm_setzero_ps();
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm256_setr_ps(v._11, v._12, v._13, 0.0f, v._21, v._22, v._23, 0.0f);
            result.v2 = _mm256_setr_ps(v._31, v._32, v._33, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
#elif defined(SIMD_ARM_NEON)
            const float tmp1[4] = { v._11, v._12, v._13, 0.0f };
            const float tmp2[4] = { v._21, v._22, v._23, 0.0f };
            const float tmp3[4] = { v._31, v._32, v._33, 0.0f };
            result.v1 = vld1q_f32(tmp1);
            result.v2 = vld1q_f32(tmp2);
            result.v3 = vld1q_f32(tmp3);
            result.v4 = vdupq_n_f32(0.0f);
#endif
            return result;
        }


        inline R128x4F __vectorcall R128x4F_LoadMat43(const CMatrix4x3& v) noexcept
        {
            R128x4F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(0.0f, v._13, v._12, v._11);
            result.v2 = _mm_set_ps(0.0f, v._23, v._22, v._21);
            result.v3 = _mm_set_ps(0.0f, v._33, v._32, v._31);
            result.v4 = _mm_set_ps(1.0f, v._43, v._42, v._41);
#elif defined(SIMD_ARM_NEON)
            const float tmp1[4] = { v._11, v._12, v._13, 0.0f };
            const float tmp2[4] = { v._21, v._22, v._23, 0.0f };
            const float tmp3[4] = { v._31, v._32, v._33, 0.0f };
            const float tmp4[4] = { v._41, v._42, v._43, 1.0f };

            result.v1 = vld1q_f32(tmp1);
            result.v2 = vld1q_f32(tmp2);
            result.v3 = vld1q_f32(tmp3);
            result.v4 = vld1q_f32(tmp4);
#endif
            return result;
        }

        inline R256x2F __vectorcall R256x2F_LoadMat43(const CMatrix4x3& v) noexcept
        {
            R256x2F result = {};
#if defined(SIMD_SSE42)
            result.v1 = _mm_set_ps(0.0f, v._13, v._12, v._11);
            result.v2 = _mm_set_ps(0.0f, v._23, v._22, v._21);
            result.v3 = _mm_set_ps(0.0f, v._33, v._32, v._31);
            result.v4 = _mm_set_ps(1.0f, v._43, v._42, v._41);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm256_setr_ps(v._11, v._12, v._13, 0.0f, v._21, v._22, v._23, 0.0f);
            result.v2 = _mm256_setr_ps(v._31, v._32, v._33, 0.0f, v._41, v._42, v._43, 1.0f);
#elif defined(SIMD_ARM_NEON)
            const float tmp1[4] = { v._11, v._12, v._13, 0.0f };
            const float tmp2[4] = { v._21, v._22, v._23, 0.0f };
            const float tmp3[4] = { v._31, v._32, v._33, 0.0f };
            const float tmp4[4] = { v._41, v._42, v._43, 1.0f };
            result.v1 = vld1q_f32(tmp1);
            result.v2 = vld1q_f32(tmp2);
            result.v3 = vld1q_f32(tmp3);
            result.v4 = vld1q_f32(tmp4);
#endif
            return result;
        }

        inline R128x4F __vectorcall R128x4F_LoadMat44(const CMatrix4x4& v) noexcept
        {
            R128x4F result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_set_ps(v._14, v._13, v._12, v._11);
            result.v2 = _mm_set_ps(v._24, v._23, v._22, v._21);
            result.v3 = _mm_set_ps(v._34, v._33, v._32, v._31);
            result.v4 = _mm_set_ps(v._44, v._43, v._42, v._41);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vld1q_f32(v.m[0]);
            result.v2 = vld1q_f32(v.m[1]);
            result.v3 = vld1q_f32(v.m[2]);
            result.v4 = vld1q_f32(v.m[3]);
#endif
            return result;
        }

        inline R256x2F __vectorcall R256x2F_LoadMat44(const CMatrix4x4& v) noexcept
        {
            R256x2F result = {};
#if defined(SIMD_SSE42)
            result.v1 = _mm_loadu_ps(v.m[0]);
            result.v2 = _mm_loadu_ps(v.m[1]);
            result.v3 = _mm_loadu_ps(v.m[2]);
            result.v4 = _mm_loadu_ps(v.m[3]);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm256_loadu_ps(&v.m[0][0]);
            result.v2 = _mm256_loadu_ps(&v.m[2][0]);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vld1q_f32(v.m[0]);
            result.v2 = vld1q_f32(v.m[1]);
            result.v3 = vld1q_f32(v.m[2]);
            result.v4 = vld1q_f32(v.m[3]);
#endif
            return result;
        }


        inline CColorRGBA __vectorcall R128x1F_StoreRGBA(const R128x1F& simd) noexcept
        {
            CColorRGBA result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm_storeu_ps(&result.x, simd.v1);
#elif defined(SIMD_ARM_NEON)
            vst1q_f32(&result.x, simd.v1);
#endif
            return result;
        }


        inline CColorHSLA __vectorcall R128x1F_StoreHSLA(const R128x1F& simd) noexcept
        {
            CColorHSLA result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm_storeu_ps(&result.h, simd.v1);
#elif defined(SIMD_ARM_NEON)
            vst1q_f32(&result.h, simd.v1);
#endif
            return result;
        }


        inline CColorHSVA __vectorcall R128x1F_StoreHSVA(const R128x1F& simd) noexcept
        {
            CColorHSVA result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm_storeu_ps(&result.h, simd.v1);
#elif defined(SIMD_ARM_NEON)
            vst1q_f32(&result.h, simd.v1);
#endif
            return result;
        }


        inline CColorYUVA __vectorcall R128x1F_StoreYUVA(const R128x1F& simd) noexcept
        {
            CColorYUVA result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm_storeu_ps(&result.y, simd.v1);
#elif defined(SIMD_ARM_NEON)
            vst1q_f32(&result.y, simd.v1);
#endif
            return result;
        }


        inline CVector2 __vectorcall R128x1F_StoreVec2(const R128x1F& simd) noexcept
        {
            CVector2 result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm_store_sd(reinterpret_cast<double*>(result.v), _mm_castps_pd(simd.v1));
#elif defined(SIMD_ARM_NEON)
            vst1_f32(result.v, vget_low_f32(simd.v1));
#endif
            return result;
        }


        inline CVector3 __vectorcall R128x1F_StoreVec3(const R128x1F& simd) noexcept
        {
            CVector3 result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm_store_sd(reinterpret_cast<double*>(result.v), _mm_castps_pd(simd.v1));
            __m128 vz = _mm_shuffle_ps(simd.v1, simd.v1, _MM_SHUFFLE(2, 2, 2, 2));
            _mm_store_ss(&result.z, vz);
#elif defined(SIMD_ARM_NEON)
            vst1_f32(result.v, vget_low_f32(simd.v1));
            result.z = vgetq_lane_f32(simd.v1, 2);
#endif
            return result;
        }


        inline CVector4 __vectorcall R128x1F_StoreVec4(const R128x1F& simd) noexcept
        {
            CVector4 result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm_storeu_ps(&result.x, simd.v1);
#elif defined(SIMD_ARM_NEON)
            vst1q_f32(&result.x, simd.v1);
#endif
            return result;
        }


        inline CQuaternion __vectorcall R128x1F_StoreQuat(const R128x1F& simd) noexcept
        {
            CQuaternion result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm_storeu_ps(&result.x, simd.v1);
#elif defined(SIMD_ARM_NEON)
            vst1q_f32(&result.x, simd.v1);
#endif
            return result;
        }


        inline CMatrix2x2 __vectorcall R128x2F_StoreMat22(const R128x2F& simd) noexcept
        {
            CMatrix2x2 result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm_store_sd(reinterpret_cast<double*>(result.m[0]), _mm_castps_pd(simd.v1));
            _mm_store_sd(reinterpret_cast<double*>(result.m[1]), _mm_castps_pd(simd.v2));
#elif defined(SIMD_ARM_NEON)
            vst1_f32(result.m[0], vget_low_f32(simd.v1));
            vst1_f32(result.m[1], vget_low_f32(simd.v2));
#endif
            return result;
        }

        inline CMatrix2x2 __vectorcall R256x1F_StoreMat22(const R256x1F& simd) noexcept
        {
            CMatrix2x2 result = {};
#if defined(SIMD_SSE42)
            __m128 packed = _mm_movelh_ps(simd.v1, simd.v2);
            _mm_storeu_ps(&result.m[0][0], packed);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            __m128 row1 = _mm256_castps256_ps128(simd.v1);
            __m128 row2 = _mm256_extractf128_ps(simd.v1, 1);
            __m128 packed = _mm_movelh_ps(row1, row2);
            _mm_storeu_ps(&result.m[0][0], packed);
#elif defined(SIMD_ARM_NEON)
            float32x2_t row1 = vget_low_f32(simd.v1);
            float32x2_t row2 = vget_low_f32(simd.v2);
            vst1q_f32(&result.m[0][0], vcombine_f32(row1, row2));
#endif
            return result;
        }

        inline CMatrix3x3 __vectorcall R128x3F_StoreMat33(const R128x3F& simd) noexcept
        {
            CMatrix3x3 result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            __m128     temp1 = simd.v1;
            __m128     temp2 = simd.v2;
            __m128     temp3 = simd.v3;
            __m128     temp4 = _mm_shuffle_ps(temp1, temp2, _MM_SHUFFLE(0, 0, 2, 2));

            temp1 = _mm_shuffle_ps(temp1, temp4, _MM_SHUFFLE(2, 0, 1, 0));
            _mm_storeu_ps(&result.m[0][0], temp1);

            temp2 = _mm_shuffle_ps(temp2, temp3, _MM_SHUFFLE(1, 0, 2, 1));
            _mm_storeu_ps(&result.m[1][1], temp2);

            temp3 = _SM_PERMUTE(temp3, 2, 2, 2, 2);
            _mm_store_ss(&result.m[2][2], temp3);
#elif defined(SIMD_ARM_NEON)
            vst1_f32(result.m[0], vget_low_f32(simd.v1));
            vst1_f32(result.m[1], vget_low_f32(simd.v2));
            vst1_f32(result.m[2], vget_low_f32(simd.v3));
            result._13 = vgetq_lane_f32(simd.v1, 2);
            result._23 = vgetq_lane_f32(simd.v2, 2);
            result._33 = vgetq_lane_f32(simd.v3, 2);

#endif
            return result;
        }

        inline CMatrix3x3 __vectorcall R256x2F_StoreMat33(const R256x2F& simd) noexcept
        {
            R128x3F temp = {};
#if defined(SIMD_SSE42)
            temp.v1 = simd.v1;
            temp.v2 = simd.v2;
            temp.v3 = simd.v3;
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            temp.v1 = _mm256_castps256_ps128(simd.v1);
            temp.v2 = _mm256_extractf128_ps(simd.v1, 1);
            temp.v3 = _mm256_castps256_ps128(simd.v2);
#elif defined(SIMD_ARM_NEON)
            temp.v1 = simd.v1;
            temp.v2 = simd.v2;
            temp.v3 = simd.v3;
#endif
            return R128x3F_StoreMat33(temp);
        }

        inline CMatrix4x3 __vectorcall R128x4F_StoreMat43(const R128x4F& simd) noexcept
        {
            CMatrix4x3 result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            __m128     temp1 = simd.v1;
            __m128     temp2 = simd.v2;
            __m128     temp3 = simd.v3;
            __m128     temp4 = simd.v4;
            __m128     temp5 = _mm_shuffle_ps(temp2, temp3, _MM_SHUFFLE(1, 0, 2, 1));

            temp2 = _mm_shuffle_ps(temp2, temp1, _MM_SHUFFLE(2, 2, 0, 0));
            temp1 = _mm_shuffle_ps(temp1, temp2, _MM_SHUFFLE(0, 2, 1, 0));
            temp3 = _mm_shuffle_ps(temp3, temp4, _MM_SHUFFLE(0, 0, 2, 2));
            temp3 = _mm_shuffle_ps(temp3, temp4, _MM_SHUFFLE(2, 1, 2, 0));

            _mm_storeu_ps(&result.m[0][0], temp1);
            _mm_storeu_ps(&result.m[1][1], temp5);
            _mm_storeu_ps(&result.m[2][2], temp3);
#elif defined(SIMD_ARM_NEON)
            vst1_f32(result.m[0], vget_low_f32(simd.v1));
            vst1_f32(result.m[1], vget_low_f32(simd.v2));
            vst1_f32(result.m[2], vget_low_f32(simd.v3));
            vst1_f32(result.m[3], vget_low_f32(simd.v4));
            result._13 = vgetq_lane_f32(simd.v1, 2);
            result._23 = vgetq_lane_f32(simd.v2, 2);
            result._33 = vgetq_lane_f32(simd.v3, 2);
            result._43 = vgetq_lane_f32(simd.v4, 2);
#endif
            return result;
        }

        inline CMatrix4x3 __vectorcall R256x2F_StoreMat43(const R256x2F& simd) noexcept
        {
            R128x4F temp = {};
#if defined(SIMD_SSE42)
            temp.v1 = simd.v1;
            temp.v2 = simd.v2;
            temp.v3 = simd.v3;
            temp.v4 = simd.v4;
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            temp.v1 = _mm256_castps256_ps128(simd.v1);
            temp.v2 = _mm256_extractf128_ps(simd.v1, 1);
            temp.v3 = _mm256_castps256_ps128(simd.v2);
            temp.v4 = _mm256_extractf128_ps(simd.v2, 1);
#elif defined(SIMD_ARM_NEON)
            temp.v1 = simd.v1;
            temp.v2 = simd.v2;
            temp.v3 = simd.v3;
            temp.v4 = simd.v4;
#endif
            return R128x4F_StoreMat43(temp);
        }

        inline CMatrix4x4 __vectorcall R128x4F_StoreMat44(const R128x4F& simd) noexcept
        {
            CMatrix4x4 result = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm_storeu_ps(result.m[0], simd.v1);
            _mm_storeu_ps(result.m[1], simd.v2);
            _mm_storeu_ps(result.m[2], simd.v3);
            _mm_storeu_ps(result.m[3], simd.v4);
#elif defined(SIMD_ARM_NEON)
            vst1q_f32(result.m[0], simd.v1);
            vst1q_f32(result.m[1], simd.v2);
            vst1q_f32(result.m[2], simd.v3);
            vst1q_f32(result.m[3], simd.v4);
#endif
            return result;
        }

        inline CMatrix4x4 __vectorcall R256x2F_StoreMat44(const R256x2F& simd) noexcept
        {
            CMatrix4x4 result = {};
#if defined(SIMD_SSE42)
            _mm_storeu_ps(result.m[0], simd.v1);
            _mm_storeu_ps(result.m[1], simd.v2);
            _mm_storeu_ps(result.m[2], simd.v3);
            _mm_storeu_ps(result.m[3], simd.v4);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            _mm256_storeu_ps(&result.m[0][0], simd.v1);
            _mm256_storeu_ps(&result.m[2][0], simd.v2);
#elif defined(SIMD_ARM_NEON)
            vst1q_f32(result.m[0], simd.v1);
            vst1q_f32(result.m[1], simd.v2);
            vst1q_f32(result.m[2], simd.v3);
            vst1q_f32(result.m[3], simd.v4);
#endif
            return result;
        }

        template <int X, int Y, int Z, int W> inline R128x1F VecPermute(R128x1F v) noexcept
        {
            R128x1F result = {};
            result.v1 = _SM_PERMUTE(v.v1, X, Y, Z, W);
            return result;
        }
    } // namespace SIMDMath
} // namespace krystallic