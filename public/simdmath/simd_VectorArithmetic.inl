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

#include <cmath>

#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
#include <immintrin.h>
#elif defined(SIMD_ARM_NEON)
#include <arm_neon.h>
#endif

namespace krystallic
{
    namespace SIMDMath
    {
        inline R128x1F __vectorcall VecAbs(R128x1F a)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_and_ps(a.v1, _mm_castsi128_ps(g_AbsMask.v1));
#elif defined(SIMD_ARM_NEON)
            result.v1 = vabsq_f32(a.v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecNeg(R128x1F a)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_xor_ps(a.v1, _mm_set1_ps(-0.0f));
#elif defined(SIMD_ARM_NEON)
            result.v1 = vnegq_f32(a.v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecAdd(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_add_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vaddq_f32(a.v1, b.v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecSub(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_sub_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vsubq_f32(a.v1, b.v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecMul(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_mul_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vmulq_f32(a.v1, b.v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecDiv(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_div_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = neon_vdivq_f32(a.v1, b.v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecScale(R128x1F a, float scale)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_mul_ps(a.v1, _mm_set1_ps(scale));
#elif defined(SIMD_ARM_NEON)
            result.v1 = vmulq_n_f32(a.v1, scale);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecMin(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_min_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vminq_f32(a.v1, b.v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecMax(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_max_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vmaxq_f32(a.v1, b.v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecClamp(R128x1F value, R128x1F minValue, R128x1F maxValue)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_max_ps(value.v1, minValue.v1);
            result.v1 = _mm_min_ps(result.v1, maxValue.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vmaxq_f32(value.v1, minValue.v1);
            result.v1 = vminq_f32(result.v1, maxValue.v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecSaturate(R128x1F value)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            const __m128 minValue = _mm_setzero_ps();
            const __m128 maxValue = _mm_set1_ps(1.0f);
            result.v1 = _mm_max_ps(value.v1, minValue);
            result.v1 = _mm_min_ps(result.v1, maxValue);
#elif defined(SIMD_ARM_NEON)
            const float32x4_t minValue = vdupq_n_f32(0.0f);
            const float32x4_t maxValue = vdupq_n_f32(1.0f);
            result.v1 = vmaxq_f32(value.v1, minValue);
            result.v1 = vminq_f32(result.v1, maxValue);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecSqrt(R128x1F value)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_sqrt_ps(value.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = neon_vsqrtq_f32(value.v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecIsInf(R128x1F a)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            const __m128 temp = _mm_and_ps(a.v1, _mm_castsi128_ps(g_AbsMask.v1));
            result.v1 = _mm_cmpeq_ps(temp, _mm_castsi128_ps(g_Infinity.v1));
#elif defined(SIMD_ARM_NEON)
            const uint32x4_t bits = vreinterpretq_u32_f32(a.v1);
            const uint32x4_t absBits = vandq_u32(bits, vdupq_n_u32(FLOAT_ABS_MASK));
            const uint32x4_t mask = vceqq_u32(absBits, vdupq_n_u32(FLOAT_EXPONENT_MASK));
            result.v1 = vreinterpretq_f32_u32(mask);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecIsNaN(R128x1F a)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_cmpunord_ps(a.v1, a.v1);
#elif defined(SIMD_ARM_NEON)
            const uint32x4_t mask = vmvnq_u32(vceqq_f32(a.v1, a.v1));
            result.v1 = vreinterpretq_f32_u32(mask);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecPow(R128x1F vec, R128x1F power)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            alignas(16) float values[4];
            alignas(16) float powers[4];
            _mm_store_ps(values, vec.v1);
            _mm_store_ps(powers, power.v1);
            result = R128x1F_Set(std::pow(values[0], powers[0]), std::pow(values[1], powers[1]), std::pow(values[2], powers[2]), std::pow(values[3], powers[3]));
#elif defined(SIMD_ARM_NEON)
            result = R128x1F_Set(std::pow(vgetq_lane_f32(vec.v1, 0), vgetq_lane_f32(power.v1, 0)), std::pow(vgetq_lane_f32(vec.v1, 1), vgetq_lane_f32(power.v1, 1)), std::pow(vgetq_lane_f32(vec.v1, 2), vgetq_lane_f32(power.v1, 2)), std::pow(vgetq_lane_f32(vec.v1, 3), vgetq_lane_f32(power.v1, 3)));
#endif
            return result;
        }

        inline R128x1F __vectorcall VecRsqrt(R128x1F value)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(value.v1));
#elif defined(SIMD_ARM_NEON)
            result.v1 = neon_vdivq_f32(vdupq_n_f32(1.0f), neon_vsqrtq_f32(value.v1));
#endif
            return result;
        }

        template <int MASK> inline R128x1F __vectorcall VecDP(R128x1F a, R128x1F b)
        {
            static_assert(MASK >= 0 && MASK <= 0xFF);
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_dp_ps(a.v1, b.v1, MASK);
#elif defined(SIMD_ARM_NEON)
            const float32x4_t product = vmulq_f32(a.v1, b.v1);
            float sum = 0.0f;
            if constexpr ((MASK & 0x10) != 0) sum += vgetq_lane_f32(product, 0);
            if constexpr ((MASK & 0x20) != 0) sum += vgetq_lane_f32(product, 1);
            if constexpr ((MASK & 0x40) != 0) sum += vgetq_lane_f32(product, 2);
            if constexpr ((MASK & 0x80) != 0) sum += vgetq_lane_f32(product, 3);
            result.v1 = vdupq_n_f32(0.0f);
            if constexpr ((MASK & 0x01) != 0) result.v1 = vsetq_lane_f32(sum, result.v1, 0);
            if constexpr ((MASK & 0x02) != 0) result.v1 = vsetq_lane_f32(sum, result.v1, 1);
            if constexpr ((MASK & 0x04) != 0) result.v1 = vsetq_lane_f32(sum, result.v1, 2);
            if constexpr ((MASK & 0x08) != 0) result.v1 = vsetq_lane_f32(sum, result.v1, 3);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecFMAdd(R128x1F a, R128x1F b, R128x1F c)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
#if defined(SIMD_EXT_FMA3)
            result.v1 = _mm_fmadd_ps(a.v1, b.v1, c.v1);
#else
            result.v1 = _mm_add_ps(_mm_mul_ps(a.v1, b.v1), c.v1);
#endif
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64)
            result.v1 = vfmaq_f32(c.v1, a.v1, b.v1);
#else
            result.v1 = vmlaq_f32(c.v1, a.v1, b.v1);
#endif
#endif
            return result;
        }

        inline R128x1F __vectorcall VecFNMAdd(R128x1F a, R128x1F b, R128x1F c)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
#if defined(SIMD_EXT_FMA3)
            result.v1 = _mm_fnmadd_ps(a.v1, b.v1, c.v1);
#else
            result.v1 = _mm_sub_ps(c.v1, _mm_mul_ps(a.v1, b.v1));
#endif
#elif defined(SIMD_ARM_NEON)
#if defined(KRYSTALLIC_ARCH_ARM64)
            result.v1 = vfmsq_f32(c.v1, a.v1, b.v1);
#else
            result.v1 = vmlsq_f32(c.v1, a.v1, b.v1);
#endif
#endif
            return result;
        }

        inline R128x1F VecMergeLow(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_unpacklo_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vzipq_f32(a.v1, b.v1).val[0];
#endif
            return result;
        }

        inline R128x1F VecMergeHigh(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_unpackhi_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vzipq_f32(a.v1, b.v1).val[1];
#endif
            return result;
        }

        inline R128x1F __vectorcall VecXor(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_xor_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(a.v1), vreinterpretq_u32_f32(b.v1)));
#endif
            return result;
        }

        inline R128x1F __vectorcall VecOr(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_or_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(a.v1), vreinterpretq_u32_f32(b.v1)));
#endif
            return result;
        }

        inline R128x1F __vectorcall VecAndNot(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_andnot_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vreinterpretq_f32_u32(vbicq_u32(vreinterpretq_u32_f32(b.v1), vreinterpretq_u32_f32(a.v1)));
#endif
            return result;
        }

        inline R128x1F __vectorcall VecAnd(R128x1F a, R128x1F b)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_and_ps(a.v1, b.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(a.v1), vreinterpretq_u32_f32(b.v1)));
#endif
            return result;
        }

        inline R128x1F __vectorcall VecRcp(R128x1F a)
        {
            R128x1F result{};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            result.v1 = _mm_div_ps(_mm_set1_ps(1.0f), a.v1);
#elif defined(SIMD_ARM_NEON)
            result.v1 = neon_vdivq_f32(vdupq_n_f32(1.0f), a.v1);
#endif
            return result;
        }
    }
}
