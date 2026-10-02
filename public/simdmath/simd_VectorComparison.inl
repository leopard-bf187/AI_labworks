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
        inline R128x1F __vectorcall VecEqual(R128x1F a, R128x1F b)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            result.v1 = _mm_cmpeq_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            uint32x4_t temp = vceqq_f32(a.v1, b.v1);
            result.v1 = vreinterpretq_f32_u32(temp);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecNotEqual(R128x1F a, R128x1F b)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            result.v1 = _mm_cmpneq_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            uint32x4_t mask = vmvnq_u32(vceqq_f32(a.v1, b.v1));
            result.v1 = vreinterpretq_f32_u32(mask);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecGreater(R128x1F a, R128x1F b)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            result.v1 = _mm_cmpgt_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            uint32x4_t temp = vcgtq_f32(a.v1, b.v1);
            result.v1 = vreinterpretq_f32_u32(temp);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecGreaterOrEqual(R128x1F a, R128x1F b)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            result.v1 = _mm_cmpge_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            uint32x4_t temp = vcgeq_f32(a.v1, b.v1);
            result.v1 = vreinterpretq_f32_u32(temp);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecLess(R128x1F a, R128x1F b)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            result.v1 = _mm_cmplt_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            uint32x4_t temp = vcltq_f32(a.v1, b.v1);
            result.v1 = vreinterpretq_f32_u32(temp);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecLessOrEqual(R128x1F a, R128x1F b)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            result.v1 = _mm_cmple_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            uint32x4_t temp = vcleq_f32(a.v1, b.v1);
            result.v1 = vreinterpretq_f32_u32(temp);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecNearEqual(R128x1F a, R128x1F b, float epsilon)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            result.v1 = _mm_sub_ps(a.v1, b.v1); //get diff
            result = VecAbs(result);

            R128x1F eps;
            eps.v1 = _mm_set1_ps(epsilon);

            result = VecLessOrEqual(result, eps);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vsubq_f32(a.v1, b.v1); //get diff
            result = VecAbs(result);

            R128x1F eps;
            eps.v1 = vdupq_n_f32(epsilon);

            result = VecLessOrEqual(result, eps);
#endif
            return result;
        }

    } // namespace SIMDMath

} // namespace krystallic