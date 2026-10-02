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



namespace krystallic
{
    namespace SIMDMath
    {
        inline R128x1F VecSetX(R128x1F v, float x) noexcept
        {
            R128x1F result;

#if defined(SIMD_SSE42) || defined(SIMD_AVX)

            result.v1 = _mm_move_ss(v.v1, _mm_set_ss(x));

#elif defined(SIMD_ARM_NEON)

            result.v1 = vsetq_lane_f32(x, v.v1, 0);

#endif
            return result;
        }


        inline R128x1F VecSetY(R128x1F v, float y) noexcept
        {
            R128x1F result;

#if defined(SIMD_SSE42) || defined(SIMD_AVX)

            // result.v1 = _SM_PERMUTE(v.v1, 1, 0, 2, 3);
            // result.v1 = _mm_move_ss(result.v1, _mm_set_ss(y));
            // result.v1 = _SM_PERMUTE(result.v1, 1, 0, 2, 3);

            result.v1 = _mm_blend_ps(v.v1, _mm_set1_ps(y), 0b0010);
#elif defined(SIMD_ARM_NEON)

            result.v1 = vsetq_lane_f32(y, v.v1, 1);

#endif

            return result;
        }


        inline R128x1F VecSetZ(R128x1F v, float z) noexcept
        {
            R128x1F result;

#if defined(SIMD_SSE42) || defined(SIMD_AVX)

            // result.v1 = _SM_PERMUTE(v.v1, 2, 1, 0, 3);
            // result.v1 = _mm_move_ss(result.v1, _mm_set_ss(z));
            // result.v1 = _SM_PERMUTE(result.v1, 2, 1, 0, 3);

            result.v1 = _mm_blend_ps(v.v1, _mm_set1_ps(z), 0b0100);

#elif defined(SIMD_ARM_NEON)

            result.v1 = vsetq_lane_f32(z, v.v1, 2);

#endif

            return result;
        }


        inline R128x1F VecSetW(R128x1F v, float w) noexcept
        {
            R128x1F result;

#if defined(SIMD_SSE42) || defined(SIMD_AVX)

            // result.v1 = _SM_PERMUTE(v.v1, 3, 1, 2, 0);
            // result.v1 = _mm_move_ss(result.v1, _mm_set_ss(w));
            // result.v1 = _SM_PERMUTE(result.v1, 3, 1, 2, 0);

            result.v1 = _mm_blend_ps(v.v1, _mm_set1_ps(w), 0b1000);
#elif defined(SIMD_ARM_NEON)

            result.v1 = vsetq_lane_f32(w, v.v1, 3);

#endif

            return result;
        }


        inline float VecGetX(R128x1F v) noexcept
        {
#if defined(SIMD_SSE42) || defined(SIMD_AVX)

            return _mm_cvtss_f32(v.v1);

#elif defined(SIMD_ARM_NEON)

            return vgetq_lane_f32(v.v1, 0);

#endif
        }


        inline float VecGetY(R128x1F v) noexcept
        {
#if defined(SIMD_SSE42) || defined(SIMD_AVX)

            __m128 temp = _SM_PERMUTE(v.v1, 1, 1, 1, 1);
            return _mm_cvtss_f32(temp);

#elif defined(SIMD_ARM_NEON)

            return vgetq_lane_f32(v.v1, 1);

#endif
        }


        inline float VecGetZ(R128x1F v) noexcept
        {
#if defined(SIMD_SSE42) || defined(SIMD_AVX)

            __m128 temp = _SM_PERMUTE(v.v1, 2, 2, 2, 2);
            return _mm_cvtss_f32(temp);

#elif defined(SIMD_ARM_NEON)

            return vgetq_lane_f32(v.v1, 2);

#endif
        }


        inline float VecGetW(R128x1F v) noexcept
        {
#if defined(SIMD_SSE42) || defined(SIMD_AVX)

            __m128 temp = _SM_PERMUTE(v.v1, 3, 3, 3, 3);
            return _mm_cvtss_f32(temp);

#elif defined(SIMD_ARM_NEON)

            return vgetq_lane_f32(v.v1, 3);

#endif
        }


        inline int VecMask(R128x1F v) noexcept
        {
            int r = 0;

#if defined(SIMD_SSE42)
            r = _mm_movemask_ps(v.v1);
#elif defined(SIMD_AVX)
            // ...
#elif defined(SIMD_AVX2)
            // ...
#elif defined(SIMD_ARM_NEON)
            uint32x4_t bits = vreinterpretq_u32_f32(v.v1);
            r |= (vgetq_lane_u32(bits, 0) >> 31) << 0;
            r |= (vgetq_lane_u32(bits, 1) >> 31) << 1;
            r |= (vgetq_lane_u32(bits, 2) >> 31) << 2;
            r |= (vgetq_lane_u32(bits, 3) >> 31) << 3;
#endif

            return r;
        }


        template <int MASK> R128x1F VecBlend(R128x1F a, R128x1F b)
        {
            R128x1F result;
#if defined(SIMD_SSE42)
            result.v1 = _mm_blend_ps(a.v1, b.v1, MASK);
#elif defined(SIMD_AVX)
            // ...
#elif defined(SIMD_AVX2)
            // ...
#elif defined(SIMD_ARM_NEON)
            // ...
#endif
            return result;
        }


        template <int X, int Y, int Z, int W> R128x1F VecShuffle(R128x1F a, R128x1F b)
        {
            R128x1F result;
#if defined(SIMD_SSE42)
                result.v1 = _mm_shuffle_ps(a.v1, b.v1, _MM_SHUFFLE(W, Z, Y, X));
#elif defined(SIMD_AVX)
            // ...
#elif defined(SIMD_AVX2)
            // ...
#elif defined(SIMD_ARM_NEON)
            // ...
#endif
            return result;
        }

    } // namespace SIMDMath


} // namespace krystallic
