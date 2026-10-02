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
        inline R128x1F __vectorcall VecLerp(R128x1F a, R128x1F b, float t)
        {
            R128x1F result; // A + (B-A)*t
#if defined(SIMD_SSE42) || defined(SIMD_AVX)

            result.v1 = _mm_sub_ps(b.v1, a.v1);
            result.v1 = _mm_mul_ps(result.v1, _mm_set1_ps(t));
            result.v1 = _mm_add_ps(result.v1, a.v1);

#elif defined(SIMD_ARM_NEON)
            result.v1 = vsubq_f32(b.v1, a.v1);
            result.v1 = vmulq_f32(result.v1, vdupq_n_f32(t));
            result.v1 = vaddq_f32(result.v1, a.v1);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecInBounds(R128x1F value, R128x1F bounds)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            R128x2F temp;
            temp.v1 = _mm_cmple_ps(value.v1, bounds.v1);
            bounds.v1 = _mm_xor_ps(bounds.v1, _mm_castsi128_ps(_mm_set1_epi32(0x80000000))); //make bounds vec negative
            temp.v2 = _mm_cmple_ps(bounds.v1, value.v1);

            result.v1 = _mm_and_ps(temp.v1, temp.v2);
		
#elif defined(SIMD_ARM_NEON)

            uint32x4_t temp1 = vcleq_f32(value.v1, bounds.v1);
            uint32x4_t temp2 = vreinterpretq_u32_f32(vnegq_f32(bounds.v1));
            temp2 = vcleq_f32(vreinterpretq_f32_u32(temp2), value.v1);

            temp1 = vandq_u32(temp1, temp2);
            result.v1 = vreinterpretq_f32_u32(temp2);
#endif
            return result;
        }
    } // namespace SIMDMath
} // namespace krystallic