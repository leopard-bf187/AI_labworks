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
#include "simd_registers_NEON.h"
#include "simd_helpers_SSE42.inl"

namespace krystallic
{
    namespace SIMDMath
    {
        inline R128x1F __vectorcall VecRound(R128x1F vec)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            result.v1 = _mm_round_ps(vec.v1, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
#elif defined(SIMD_ARM_NEON)
            result.v1 = neon_vrndnq_f32(vec.v1);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecModAngles(R128x1F angles)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            // Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
            result.v1 = _mm_mul_ps(angles.v1, _mm_set1_ps(0.159154943f));
            // Use the inline function due to complexity for rounding
            result = VecRound(result);
            result.v1 = _mm_sub_ps(angles.v1, _mm_mul_ps(result.v1, _mm_set1_ps(6.283185307f)));
#elif defined(SIMD_ARM_NEON)
            // Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
            result.v1 = vmulq_f32(angles.v1, vdupq_n_f32(0.159154943f));
            // Use the inline function due to complexity for rounding
            result = VecRound(result);
            result.v1 = vmlsq_f32(angles.v1, result.v1, g_TwoPi.v1);
#endif
            return result;
        }

        inline vec4f __vectorcall VecSelect(vec4f v1, vec4f v2, vec4f control)
        {
            vec4f  result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            __m128 vTemp1 = _mm_andnot_ps(control, v1);
            __m128 vTemp2 = _mm_and_ps(v2, control);
            result = _mm_or_ps(vTemp1, vTemp2);
#elif defined(SIMD_ARM_NEON)
            result = vbslq_f32(vreinterpretq_u32_f32(control), v2, v1);
#endif
            return result;
        }

        inline R128x1F __vectorcall VecCos(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            // Map v.v1 to x in [-pi,pi].
            __m128 x = VecModAngles(v).v1;

            // Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
            __m128 sign = _mm_and_ps(x, _mm_set1_ps(0x80000000));
            __m128 c = _mm_or_ps(_mm_set1_ps(3.141592654f), sign); // pi when x >= 0, -pi when x < 0
            __m128 absx = _mm_andnot_ps(sign, x);                  // |x|
            __m128 rflx = _mm_sub_ps(c, x);
            __m128 comp = _mm_cmple_ps(absx, _mm_set1_ps(1.570796327f));
            __m128 select0 = _mm_and_ps(comp, x);
            __m128 select1 = _mm_andnot_ps(comp, rflx);
            x = _mm_or_ps(select0, select1);
            select0 = _mm_and_ps(comp, _mm_set1_ps(1.0f));
            select1 = _mm_andnot_ps(comp, _mm_set1_ps(-1.0f));
            sign = _mm_or_ps(select0, select1);

            __m128 x2 = _mm_mul_ps(x, x);

            // Compute polynomial approximation
            const __m128 CC1 =
                _mm_setr_ps(-2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/);
            __m128       vConstantsB = _SM_PERMUTE(CC1, 0, 0, 0, 0);
            const __m128 CC0 = _mm_setr_ps(-0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f);
            __m128       vConstants = _SM_PERMUTE(CC0, 3, 3, 3, 3);
            __m128       Result = SseFMAdd(vConstantsB, x2, vConstants);

            vConstants = _SM_PERMUTE(CC0, 2, 2, 2, 2);
            Result = SseFMAdd(Result, x2, vConstants);

            vConstants = _SM_PERMUTE(CC0, 1, 1, 1, 1);
            Result = SseFMAdd(Result, x2, vConstants);

            vConstants = _SM_PERMUTE(CC0, 0, 0, 0, 0);
            Result = SseFMAdd(Result, x2, vConstants);

            Result = SseFMAdd(Result, x2, _mm_set1_ps(1.0f));
            Result = _mm_mul_ps(Result, sign);
            result.v1 = Result;
#elif defined(SIMD_ARM_NEON)
            // Map v.v1 to x in [-pi,pi].
            vec4f x = VecModAngles(v).v1;

            // Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
            uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(x), g_NegativeZero.v1);
            uint32x4_t c = vorrq_u32(vreinterpretq_u32_f32(g_Pi.v1), sign); // pi when x >= 0, -pi when x < 0
            float32x4_t absx = vabsq_f32(x);
            float32x4_t rflx = vsubq_f32(vreinterpretq_f32_u32(c), x);
            uint32x4_t comp = vcleq_f32(absx, g_HalfPi.v1);
            x = vbslq_f32(comp, x, rflx);
            float32x4_t fsign = vbslq_f32(comp, g_One.v1, g_NegativeOne.v1);

            float32x4_t x2 = vmulq_f32(x, x);

            // Compute polynomial approximation
            const vec4f CC1 = g_CosCoefficients1.v1;
            const vec4f CC0 = g_CosCoefficients0.v1;
            vec4f vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
            result.v1 = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

            vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            result.v1 = vmlaq_f32(g_One.v1, result.v1, x2);
            result.v1 = vmulq_f32(result.v1, fsign);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecACos(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            __m128      nonnegative = _mm_cmpge_ps(v.v1, _mm_set1_ps(0.0f));
            __m128      mvalue = _mm_sub_ps(_mm_set1_ps(0.0f), v.v1);
            __m128      x = _mm_max_ps(v.v1, mvalue); // |v.v1|

            // Compute (1-|v.v1|), clamp to zero to avoid sqrt of negative number.
            __m128 oneMValue = _mm_sub_ps(_mm_set1_ps(1.0f), x);
            __m128 clampOneMValue = _mm_max_ps(_mm_set1_ps(0.0f), oneMValue);
            __m128 root = _mm_sqrt_ps(clampOneMValue); // sqrt(1-|v.v1|)

            // Compute polynomial approximation
            const __m128 AC1 = _mm_setr_ps(+0.0308918810f, -0.0170881256f, +0.0066700901f, -0.0012624911f);
            __m128       vConstantsB = _SM_PERMUTE(AC1, 3, 3, 3, 3);
            __m128       vConstants = _SM_PERMUTE(AC1, 2, 2, 2, 2);
            __m128       t0 = SseFMAdd(vConstantsB, x, vConstants);

            vConstants = _SM_PERMUTE(AC1, 1, 1, 1, 1);
            t0 = SseFMAdd(t0, x, vConstants);

            vConstants = _SM_PERMUTE(AC1, 0, 0, 0, 0);
            t0 = SseFMAdd(t0, x, vConstants);

            const __m128 AC0 = _mm_setr_ps(+1.5707963050f, -0.2145988016f, +0.0889789874f, -0.0501743046f);
            vConstants = _SM_PERMUTE(AC0, 3, 3, 3, 3);
            t0 = SseFMAdd(t0, x, vConstants);

            vConstants = _SM_PERMUTE(AC0, 2, 2, 2, 2);
            t0 = SseFMAdd(t0, x, vConstants);

            vConstants = _SM_PERMUTE(AC0, 1, 1, 1, 1);
            t0 = SseFMAdd(t0, x, vConstants);

            vConstants = _SM_PERMUTE(AC0, 0, 0, 0, 0);
            t0 = SseFMAdd(t0, x, vConstants);
            t0 = _mm_mul_ps(t0, root);

            __m128 t1 = _mm_sub_ps(_mm_set1_ps(3.141592654f), t0);
            t0 = _mm_and_ps(nonnegative, t0);
            t1 = _mm_andnot_ps(nonnegative, t1);
            t0 = _mm_or_ps(t0, t1);
            result.v1 = t0;
#elif defined(SIMD_ARM_NEON)
            uint32x4_t nonnegative = vcgeq_f32(v.v1, g_Zero.v1);
            float32x4_t x = vabsq_f32(v.v1);

            // Compute (1-|v.v1|), clamp to zero to avoid sqrt of negative number.
            float32x4_t oneMValue = vsubq_f32(g_One.v1, x);
            float32x4_t clampOneMValue = vmaxq_f32(g_Zero.v1, oneMValue);
            float32x4_t root = neon_vsqrtq_f32(clampOneMValue);

            // Compute polynomial approximation
            const vec4f AC1 = g_ArcCoefficients1.v1;
            vec4f vConstants = vdupq_lane_f32(vget_high_f32(AC1), 0);
            vec4f t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AC1), 1);

            vConstants = vdupq_lane_f32(vget_low_f32(AC1), 1);
            t0 = vmlaq_f32(vConstants, t0, x);

            vConstants = vdupq_lane_f32(vget_low_f32(AC1), 0);
            t0 = vmlaq_f32(vConstants, t0, x);

            const vec4f AC0 = g_ArcCoefficients0.v1;
            vConstants = vdupq_lane_f32(vget_high_f32(AC0), 1);
            t0 = vmlaq_f32(vConstants, t0, x);

            vConstants = vdupq_lane_f32(vget_high_f32(AC0), 0);
            t0 = vmlaq_f32(vConstants, t0, x);

            vConstants = vdupq_lane_f32(vget_low_f32(AC0), 1);
            t0 = vmlaq_f32(vConstants, t0, x);

            vConstants = vdupq_lane_f32(vget_low_f32(AC0), 0);
            t0 = vmlaq_f32(vConstants, t0, x);
            t0 = vmulq_f32(t0, root);

            float32x4_t t1 = vsubq_f32(g_Pi.v1, t0);
            t0 = vbslq_f32(nonnegative, t0, t1);
            result.v1 = t0;
#endif
            return result;
        }

        inline R128x1F __vectorcall VecSin(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            // Force the value within the bounds of pi
            __m128 x = VecModAngles(v).v1;

            // Map in [-pi/2,pi/2] with sin(y) = sin(x).
            __m128 sign = _mm_and_ps(x, _mm_set1_ps(0x80000000));
            __m128 c = _mm_or_ps(_mm_set1_ps(3.141592654f), sign); // pi when x >= 0, -pi when x < 0
            __m128 absx = _mm_andnot_ps(sign, x);                  // |x|
            __m128 rflx = _mm_sub_ps(c, x);
            __m128 comp = _mm_cmple_ps(absx, _mm_set1_ps(1.570796327f));
            __m128 select0 = _mm_and_ps(comp, x);
            __m128 select1 = _mm_andnot_ps(comp, rflx);
            x = _mm_or_ps(select0, select1);

            __m128 x2 = _mm_mul_ps(x, x);

            // Compute polynomial approximation
            const __m128 SC1 =
                _mm_setr_ps(-2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/);
            __m128       vConstantsB = _SM_PERMUTE(SC1, 0, 0, 0, 0);
            const __m128 SC0 = _mm_setr_ps(-0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f);
            __m128       vConstants = _SM_PERMUTE(SC0, 3, 3, 3, 3);
            result.v1 = SseFMAdd(vConstantsB, x2, vConstants);

            vConstants = _SM_PERMUTE(SC0, 2, 2, 2, 2);
            result.v1 = SseFMAdd(result.v1, x2, vConstants);

            vConstants = _SM_PERMUTE(SC0, 1, 1, 1, 1);
            result.v1 = SseFMAdd(result.v1, x2, vConstants);

            vConstants = _SM_PERMUTE(SC0, 0, 0, 0, 0);
            result.v1 = SseFMAdd(result.v1, x2, vConstants);

            result.v1 = SseFMAdd(result.v1, x2, _mm_set1_ps(1.0f));
            result.v1 = _mm_mul_ps(result.v1, x);
#elif defined(SIMD_ARM_NEON)
            // Force the value within the bounds of pi
            vec4f x = VecModAngles(v).v1;

            // Map in [-pi/2,pi/2] with sin(y) = sin(x).
            uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(x), g_NegativeZero.v1);
            uint32x4_t c = vorrq_u32(vreinterpretq_u32_f32(g_Pi.v1), sign); // pi when x >= 0, -pi when x < 0
            float32x4_t absx = vabsq_f32(x);
            float32x4_t rflx = vsubq_f32(vreinterpretq_f32_u32(c), x);
            uint32x4_t comp = vcleq_f32(absx, g_HalfPi.v1);
            x = vbslq_f32(comp, x, rflx);

            float32x4_t x2 = vmulq_f32(x, x);

            // Compute polynomial approximation
            const vec4f SC1 = g_SinCoefficients1.v1;
            const vec4f SC0 = g_SinCoefficients0.v1;
            vec4f vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
            result.v1 = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

            vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            result.v1 = vmlaq_f32(g_One.v1, result.v1, x2);
            result.v1 = vmulq_f32(result.v1, x);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecASin(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            __m128      nonnegative = _mm_cmpge_ps(v.v1, _mm_set1_ps(0.0f));
            __m128      mvalue = _mm_sub_ps(_mm_set1_ps(0.0f), v.v1);
            __m128      x = _mm_max_ps(v.v1, mvalue); // |v.v1|

            // Compute (1-|v.v1|), clamp to zero to avoid sqrt of negative number.
            __m128 oneMValue = _mm_sub_ps(_mm_set1_ps(1.0f), x);
            __m128 clampOneMValue = _mm_max_ps(_mm_set1_ps(0.0f), oneMValue);
            __m128 root = _mm_sqrt_ps(clampOneMValue); // sqrt(1-|v.v1|)

            // Compute polynomial approximation
            const __m128 AC1 = _mm_setr_ps(+0.0308918810f, -0.0170881256f, +0.0066700901f, -0.0012624911f);
            __m128       vConstantsB = _SM_PERMUTE(AC1, 3, 3, 3, 3);
            __m128       vConstants = _SM_PERMUTE(AC1, 2, 2, 2, 2);
            __m128       t0 = SseFMAdd(vConstantsB, x, vConstants);

            vConstants = _SM_PERMUTE(AC1, 1, 1, 1, 1);
            t0 = SseFMAdd(t0, x, vConstants);

            vConstants = _SM_PERMUTE(AC1, 0, 0, 0, 0);
            t0 = SseFMAdd(t0, x, vConstants);

            const __m128 AC0 = _mm_setr_ps(+1.5707963050f, -0.2145988016f, +0.0889789874f, -0.0501743046f);
            vConstants = _SM_PERMUTE(AC0, 3, 3, 3, 3);
            t0 = SseFMAdd(t0, x, vConstants);

            vConstants = _SM_PERMUTE(AC0, 2, 2, 2, 2);
            t0 = SseFMAdd(t0, x, vConstants);

            vConstants = _SM_PERMUTE(AC0, 1, 1, 1, 1);
            t0 = SseFMAdd(t0, x, vConstants);

            vConstants = _SM_PERMUTE(AC0, 0, 0, 0, 0);
            t0 = SseFMAdd(t0, x, vConstants);
            t0 = _mm_mul_ps(t0, root);

            __m128 t1 = _mm_sub_ps(_mm_set1_ps(3.141592654f), t0);
            t0 = _mm_and_ps(nonnegative, t0);
            t1 = _mm_andnot_ps(nonnegative, t1);
            t0 = _mm_or_ps(t0, t1);
            t0 = _mm_sub_ps(_mm_set1_ps(1.570796327f), t0);
            result.v1 = t0;
#elif defined(SIMD_ARM_NEON)
            uint32x4_t nonnegative = vcgeq_f32(v.v1, g_Zero.v1);
            float32x4_t x = vabsq_f32(v.v1);

            // Compute (1-|v.v1|), clamp to zero to avoid sqrt of negative number.
            float32x4_t oneMValue = vsubq_f32(g_One.v1, x);
            float32x4_t clampOneMValue = vmaxq_f32(g_Zero.v1, oneMValue);
            float32x4_t root = neon_vsqrtq_f32(clampOneMValue);

            // Compute polynomial approximation
            const vec4f AC1 = g_ArcCoefficients1.v1;
            vec4f vConstants = vdupq_lane_f32(vget_high_f32(AC1), 0);
            vec4f t0 = vmlaq_lane_f32(vConstants, x, vget_high_f32(AC1), 1);

            vConstants = vdupq_lane_f32(vget_low_f32(AC1), 1);
            t0 = vmlaq_f32(vConstants, t0, x);

            vConstants = vdupq_lane_f32(vget_low_f32(AC1), 0);
            t0 = vmlaq_f32(vConstants, t0, x);

            const vec4f AC0 = g_ArcCoefficients0.v1;
            vConstants = vdupq_lane_f32(vget_high_f32(AC0), 1);
            t0 = vmlaq_f32(vConstants, t0, x);

            vConstants = vdupq_lane_f32(vget_high_f32(AC0), 0);
            t0 = vmlaq_f32(vConstants, t0, x);

            vConstants = vdupq_lane_f32(vget_low_f32(AC0), 1);
            t0 = vmlaq_f32(vConstants, t0, x);

            vConstants = vdupq_lane_f32(vget_low_f32(AC0), 0);
            t0 = vmlaq_f32(vConstants, t0, x);
            t0 = vmulq_f32(t0, root);

            float32x4_t t1 = vsubq_f32(g_Pi.v1, t0);
            t0 = vbslq_f32(nonnegative, t0, t1);
            t0 = vsubq_f32(g_HalfPi.v1, t0);
            result.v1 = t0;
#endif
            return result;
        }


        inline R128x1F __vectorcall VecTan(R128x1F v)
        {
            R128x1F result;

            static const vec4f TanCoefficients0 = _set_vec4f(1.0f, -4.667168334e-1f, 2.566383229e-2f, -3.118153191e-4f);
            static const vec4f TanCoefficients1 =
                _set_vec4f(4.981943399e-7f, -1.333835001e-1f, 3.424887824e-3f, -1.786170734e-5f);
            static const vec4f TanConstants =
                _set_vec4f(1.570796371f, 6.077100628e-11f, 0.000244140625f, 0.63661977228f /*2 / Pi*/);
            static const vec4ui Mask = _set_vec4ui(0x1, 0x1, 0x1, 0x1);
            vec4f               TwoDivPi = _set1_vec4f(0.63661977228f);
            vec4f               Zero = _set1_vec4f(0.0f);

#if defined(SIMD_SSE42) || defined(SIMD_AVX)

            __m128 C0 = _SM_PERMUTE(TanConstants, 0, 0, 0, 0);
            __m128 C1 = _SM_PERMUTE(TanConstants, 1, 1, 1, 1);
            __m128 Epsilon = _SM_PERMUTE(TanConstants, 2, 2, 2, 2);
            ;

            __m128 VA = _mm_mul_ps(v.v1, TwoDivPi);

            VA = _mm_round_ps(VA, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);

            __m128 VC = SseFNMAdd(VA, C0, v.v1);

            R128x1F tmp1;
            tmp1.v1 = VA;
            __m128 VB = VecAbs(tmp1).v1;

            VC = SseFNMAdd(VA, C1, VC);

            reinterpret_cast<__m128i*>(&VB)[0] = _mm_cvttps_epi32(VB);

            __m128 VC2 = _mm_mul_ps(VC, VC);

            __m128 T7 = _SM_PERMUTE(TanCoefficients1, 3, 3, 3, 3);
            __m128 T6 = _SM_PERMUTE(TanCoefficients1, 2, 2, 2, 2);
            __m128 T4 = _SM_PERMUTE(TanCoefficients1, 0, 0, 0, 0);
            __m128 T3 = _SM_PERMUTE(TanCoefficients0, 3, 3, 3, 3);
            __m128 T5 = _SM_PERMUTE(TanCoefficients1, 1, 1, 1, 1);
            __m128 T2 = _SM_PERMUTE(TanCoefficients0, 2, 2, 2, 2);
            __m128 T1 = _SM_PERMUTE(TanCoefficients0, 1, 1, 1, 1);
            __m128 T0 = _SM_PERMUTE(TanCoefficients0, 0, 0, 0, 0);

            __m128  VBIsEven = _mm_and_ps(VB, _mm_castsi128_ps(Mask));
            __m128i temp = _mm_cmpeq_epi32(_mm_castps_si128(VBIsEven), _mm_castps_si128(Zero));
            VBIsEven = _mm_castsi128_ps(temp);

            __m128 N = SseFMAdd(VC2, T7, T6);
            __m128 D = SseFMAdd(VC2, T4, T3);
            N = SseFMAdd(VC2, N, T5);
            D = SseFMAdd(VC2, D, T2);
            N = _mm_mul_ps(VC2, N);
            D = SseFMAdd(VC2, D, T1);
            N = SseFMAdd(VC, N, VC);

            R128x1F tmp2;
            tmp1.v1 = VC;
            tmp2.v1 = Epsilon;
            __m128 VCNearZero = VecInBounds(tmp1, tmp2).v1;

            D = SseFMAdd(VC2, D, T0);

            N = VecSelect(N, VC, VCNearZero);
            D = VecSelect(D, g_One.v1, VCNearZero);

            __m128 R0 = _mm_sub_ps(_mm_set1_ps(0.0f), N);
            __m128 R1 = _mm_div_ps(N, D);
            R0 = _mm_div_ps(D, R0);

            __m128 VIsZero = _mm_cmpeq_ps(v.v1, Zero);

            result.v1 = VecSelect(R0, R1, VBIsEven);

            result.v1 = VecSelect(result.v1, Zero, VIsZero);
#elif defined(SIMD_ARM_NEON)

            float32x4_t C0 = _SM_PERMUTE(TanConstants, 0, 0, 0, 0);
            float32x4_t C1 = _SM_PERMUTE(TanConstants, 1, 1, 1, 1);
            float32x4_t Epsilon = _SM_PERMUTE(TanConstants, 2, 2, 2, 2);

            float32x4_t VA = vmulq_f32(v.v1, TwoDivPi);

            VA = neon_vrndnq_f32(VA);

            float32x4_t VC = neon_vfmsq_f32(VA, C0, v.v1);

            R128x1F tmp1;
            tmp1.v1 = VA;
            float32x4_t VB = VecAbs(tmp1).v1;

            VC = neon_vfmsq_f32(VA, C1, VC);

            VB = vreinterpretq_f32_u32(vcvtq_u32_f32(VB));

            float32x4_t VC2 = vmulq_f32(VC, VC);

            float32x4_t T7 = _SM_PERMUTE(TanCoefficients1, 3, 3, 3, 3);
            float32x4_t T6 = _SM_PERMUTE(TanCoefficients1, 2, 2, 2, 2);
            float32x4_t T4 = _SM_PERMUTE(TanCoefficients1, 0, 0, 0, 0);
            float32x4_t T3 = _SM_PERMUTE(TanCoefficients0, 3, 3, 3, 3);
            float32x4_t T5 = _SM_PERMUTE(TanCoefficients1, 1, 1, 1, 1);
            float32x4_t T2 = _SM_PERMUTE(TanCoefficients0, 2, 2, 2, 2);
            float32x4_t T1 = _SM_PERMUTE(TanCoefficients0, 1, 1, 1, 1);
            float32x4_t T0 = _SM_PERMUTE(TanCoefficients0, 0, 0, 0, 0);

            float32x4_t VBIsEven = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(VB), Mask));
            VBIsEven = vreinterpretq_f32_u32(vceqq_s32(vreinterpretq_s32_f32(VBIsEven), vreinterpretq_s32_f32(Zero)));

            float32x4_t N = neon_vfmaq_f32(VC2, T7, T6);
            float32x4_t D = neon_vfmaq_f32(VC2, T4, T3);
            N = neon_vfmaq_f32(VC2, N, T5);
            D = neon_vfmaq_f32(VC2, D, T2);
            N = vmulq_f32(VC2, N);
            D = neon_vfmaq_f32(VC2, D, T1);
            N = neon_vfmaq_f32(VC, N, VC);

            R128x1F tmp2;
            tmp1.v1 = VC;
            tmp2.v1 = Epsilon;
            float32x4_t VCNearZero = VecInBounds(tmp1, tmp2).v1;
            D = neon_vfmaq_f32(VC2, D, T0);

            N = VecSelect(N, VC, VCNearZero);
            D = VecSelect(D, g_One.v1, VCNearZero);

            float32x4_t R0 = vnegq_f32(N);
            float32x4_t R1 = neon_vdivq_f32(N, D);
            R0 = neon_vdivq_f32(D, R0);

            float32x4_t VIsZero = vreinterpretq_f32_u32(vceqq_f32(v.v1, Zero));

            result.v1 = VecSelect(R0, R1, VBIsEven);

            result.v1 = VecSelect(result.v1, Zero, VIsZero);

#endif
            return result;
        }


        inline R128x1F __vectorcall VecATan(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            __m128      absV = VecAbs(v).v1;
            __m128      invV = _mm_div_ps(_mm_set1_ps(1.0f), v.v1);
            __m128      comp = _mm_cmpgt_ps(v.v1, _mm_set1_ps(1.0f));
            __m128      select0 = _mm_and_ps(comp, _mm_set1_ps(1.0f));
            __m128      select1 = _mm_andnot_ps(comp, _mm_set1_ps(-1.0f));
            __m128      sign = _mm_or_ps(select0, select1);
            comp = _mm_cmple_ps(absV, _mm_set1_ps(1.0f));
            select0 = _mm_and_ps(comp, _mm_set1_ps(0.0f));
            select1 = _mm_andnot_ps(comp, sign);
            sign = _mm_or_ps(select0, select1);
            select0 = _mm_and_ps(comp, v.v1);
            select1 = _mm_andnot_ps(comp, invV);
            __m128 x = _mm_or_ps(select0, select1);

            __m128 x2 = _mm_mul_ps(x, x);

            // Compute polynomial approximation
            const __m128 TC1 = _mm_setr_ps(-0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f);
            __m128       vConstantsB = _SM_PERMUTE(TC1, 3, 3, 3, 3);
            __m128       vConstants = _SM_PERMUTE(TC1, 2, 2, 2, 2);
            __m128       Result = SseFMAdd(vConstantsB, x2, vConstants);

            vConstants = _SM_PERMUTE(TC1, 1, 1, 1, 1);
            Result = SseFMAdd(Result, x2, vConstants);

            vConstants = _SM_PERMUTE(TC1, 0, 0, 0, 0);
            Result = SseFMAdd(Result, x2, vConstants);

            const __m128 TC0 = _mm_setr_ps(-0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f);
            vConstants = _SM_PERMUTE(TC0, 3, 3, 3, 3);
            Result = SseFMAdd(Result, x2, vConstants);

            vConstants = _SM_PERMUTE(TC0, 2, 2, 2, 2);
            Result = SseFMAdd(Result, x2, vConstants);

            vConstants = _SM_PERMUTE(TC0, 1, 1, 1, 1);
            Result = SseFMAdd(Result, x2, vConstants);

            vConstants = _SM_PERMUTE(TC0, 0, 0, 0, 0);
            Result = SseFMAdd(Result, x2, vConstants);

            Result = SseFMAdd(Result, x2, _mm_set1_ps(1.0f));

            Result = _mm_mul_ps(Result, x);
            __m128 result1 = _mm_mul_ps(sign, _mm_set1_ps(1.570796327f));
            result1 = _mm_sub_ps(result1, Result);

            comp = _mm_cmpeq_ps(sign, _mm_set1_ps(0.0f));
            select0 = _mm_and_ps(comp, Result);
            select1 = _mm_andnot_ps(comp, result1);
            result.v1 = _mm_or_ps(select0, select1);
#elif defined(SIMD_ARM_NEON)
            float32x4_t absV = vabsq_f32(v.v1);
            float32x4_t invV = neon_vdivq_f32(g_One.v1, v.v1);
            uint32x4_t comp = vcgtq_f32(v.v1, g_One.v1);
            float32x4_t sign = vbslq_f32(comp, g_One.v1, g_NegativeOne.v1);
            comp = vcleq_f32(absV, g_One.v1);
            sign = vbslq_f32(comp, g_Zero.v1, sign);
            float32x4_t x = vbslq_f32(comp, v.v1, invV);

            float32x4_t x2 = vmulq_f32(x, x);

            // Compute polynomial approximation
            const float32x4_t TC1 = g_ATanCoefficients1.v1;
            float32x4_t vConstants = vdupq_lane_f32(vget_high_f32(TC1), 0);
            result.v1 = vmlaq_lane_f32(vConstants, x2, vget_high_f32(TC1), 1);

            vConstants = vdupq_lane_f32(vget_low_f32(TC1), 1);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(TC1), 0);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            const float32x4_t TC0 = g_ATanCoefficients0.v1;
            vConstants = vdupq_lane_f32(vget_high_f32(TC0), 1);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            vConstants = vdupq_lane_f32(vget_high_f32(TC0), 0);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(TC0), 1);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(TC0), 0);
            result.v1 = vmlaq_f32(vConstants, result.v1, x2);

            result.v1 = vmlaq_f32(g_One.v1, result.v1, x2);
            result.v1 = vmulq_f32(result.v1, x);

            float32x4_t result1 = vmulq_f32(sign, g_HalfPi.v1);
            result1 = vsubq_f32(result1, result.v1);

            comp = vceqq_f32(sign, g_Zero.v1);
            result.v1 = vbslq_f32(comp, result.v1, result1);

#endif
            return result;
        }


        inline void __vectorcall VecSinCos(R128x1F v, R128x1F* outSin, R128x1F* outCos)
        {
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            // Force the value within the bounds of pi
            __m128 x = VecModAngles(v).v1;

            // Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
            __m128 sign = _mm_and_ps(x, _mm_set1_ps(0x80000000));
            __m128 c = _mm_or_ps(_mm_set1_ps(3.141592654f), sign); // pi when x >= 0, -pi when x < 0
            __m128 absx = _mm_andnot_ps(sign, x);                  // |x|
            __m128 rflx = _mm_sub_ps(c, x);
            __m128 comp = _mm_cmple_ps(absx, _mm_set1_ps(1.570796327f));
            __m128 select0 = _mm_and_ps(comp, x);
            __m128 select1 = _mm_andnot_ps(comp, rflx);
            x = _mm_or_ps(select0, select1);
            select0 = _mm_and_ps(comp, _mm_set1_ps(1.0f));
            select1 = _mm_andnot_ps(comp, _mm_set1_ps(-1.0f));
            sign = _mm_or_ps(select0, select1);

            __m128 x2 = _mm_mul_ps(x, x);

            // Compute polynomial approximation of sine
            const __m128 SC1 =
                _mm_setr_ps(-2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/);
            __m128       vConstantsB = _SM_PERMUTE(SC1, 0, 0, 0, 0);
            const __m128 SC0 = _mm_setr_ps(-0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f);
            __m128       vConstants = _SM_PERMUTE(SC0, 3, 3, 3, 3);
            __m128       Result = SseFMAdd(vConstantsB, x2, vConstants);

            vConstants = _SM_PERMUTE(SC0, 2, 2, 2, 2);
            Result = SseFMAdd(Result, x2, vConstants);

            vConstants = _SM_PERMUTE(SC0, 1, 1, 1, 1);
            Result = SseFMAdd(Result, x2, vConstants);

            vConstants = _SM_PERMUTE(SC0, 0, 0, 0, 0);
            Result = SseFMAdd(Result, x2, vConstants);

            Result = SseFMAdd(Result, x2, _mm_set1_ps(1.0f));
            Result = _mm_mul_ps(Result, x);
            outSin->v1 = Result;

            // Compute polynomial approximation of cosine
            const __m128 CC1 =
                _mm_setr_ps(-2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/);
            vConstantsB = _SM_PERMUTE(CC1, 0, 0, 0, 0);
            const __m128 CC0 = _mm_setr_ps(-0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f);
            vConstants = _SM_PERMUTE(CC0, 3, 3, 3, 3);
            Result = SseFMAdd(vConstantsB, x2, vConstants);

            vConstants = _SM_PERMUTE(CC0, 2, 2, 2, 2);
            Result = SseFMAdd(Result, x2, vConstants);

            vConstants = _SM_PERMUTE(CC0, 1, 1, 1, 1);
            Result = SseFMAdd(Result, x2, vConstants);

            vConstants = _SM_PERMUTE(CC0, 0, 0, 0, 0);
            Result = SseFMAdd(Result, x2, vConstants);

            Result = SseFMAdd(Result, x2, _mm_set1_ps(1.0f));
            Result = _mm_mul_ps(Result, sign);
            outCos->v1 = Result;
#elif defined(SIMD_ARM_NEON)
            // Force the value within the bounds of pi
            float32x4_t x = VecModAngles(v).v1;

            // Map in [-pi/2,pi/2] with cos(y) = sign*cos(x).
            uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(x), g_NegativeZero.v1);
            uint32x4_t c = vorrq_u32(vreinterpretq_u32_f32(g_Pi.v1), sign); // pi when x >= 0, -pi when x < 0
            float32x4_t absx = vabsq_f32(x);
            float32x4_t rflx = vsubq_f32(vreinterpretq_f32_u32(c), x);
            uint32x4_t comp = vcleq_f32(absx, g_HalfPi.v1);
            x = vbslq_f32(comp, x, rflx);
            float32x4_t fsign = vbslq_f32(comp, g_One.v1, g_NegativeOne.v1);

            float32x4_t x2 = vmulq_f32(x, x);

            // Compute polynomial approximation for sine
            const float32x4_t SC1 = g_SinCoefficients1.v1;
            const float32x4_t SC0 = g_SinCoefficients0.v1;
            float32x4_t vConstants = vdupq_lane_f32(vget_high_f32(SC0), 1);
            float32x4_t Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(SC1), 0);

            vConstants = vdupq_lane_f32(vget_high_f32(SC0), 0);
            Result = vmlaq_f32(vConstants, Result, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(SC0), 1);
            Result = vmlaq_f32(vConstants, Result, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(SC0), 0);
            Result = vmlaq_f32(vConstants, Result, x2);

            Result = vmlaq_f32(g_One.v1, Result, x2);
            outSin->v1 = vmulq_f32(Result, x);

            // Compute polynomial approximation for cosine
            const float32x4_t CC1 = g_CosCoefficients1.v1;
            const float32x4_t CC0 = g_CosCoefficients0.v1;
            vConstants = vdupq_lane_f32(vget_high_f32(CC0), 1);
            Result = vmlaq_lane_f32(vConstants, x2, vget_low_f32(CC1), 0);

            vConstants = vdupq_lane_f32(vget_high_f32(CC0), 0);
            Result = vmlaq_f32(vConstants, Result, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(CC0), 1);
            Result = vmlaq_f32(vConstants, Result, x2);

            vConstants = vdupq_lane_f32(vget_low_f32(CC0), 0);
            Result = vmlaq_f32(vConstants, Result, x2);

            Result = vmlaq_f32(g_One.v1, Result, x2);
            outCos->v1 = vmulq_f32(Result, fsign);
#endif
        }


        inline void __vectorcall VecASinACos(R128x1F v, R128x1F* outASin, R128x1F* outACos)
        {
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_ARM_NEON)
            *outASin = VecASin(v);
            *outACos = VecACos(v);
#endif
        }


        inline R128x1F __vectorcall VecExp2(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            __m128i     itrunc = _mm_cvttps_epi32(v.v1);
            __m128      ftrunc = _mm_cvtepi32_ps(itrunc);
            __m128      y = _mm_sub_ps(v.v1, ftrunc);

            __m128 poly = SseFMAdd(_mm_setr_ps(-1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f), y, _mm_setr_ps(+1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f));
            poly = SseFMAdd(poly, y, _mm_setr_ps(-1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f));
            poly = SseFMAdd(poly, y, _mm_setr_ps(+9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f));
            poly = SseFMAdd(poly, y, _mm_setr_ps(-5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f));
            poly = SseFMAdd(poly, y, _mm_setr_ps(+2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f));
            poly = SseFMAdd(poly, y, _mm_setr_ps(-6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f));
            poly = SseFMAdd(poly, y, _mm_set1_ps(1.0f));

            __m128i biased = _mm_add_epi32(itrunc, _mm_set1_epi32(127));
            biased = _mm_slli_epi32(biased, 23);
            __m128 result0 = _mm_div_ps(_mm_castsi128_ps(biased), poly);

            biased = _mm_add_epi32(itrunc, _mm_set1_epi32(253));
            biased = _mm_slli_epi32(biased, 23);
            __m128 result1 = _mm_div_ps(_mm_castsi128_ps(biased), poly);
            result1 = _mm_mul_ps(_mm_set1_ps(0x00800000), result1);

            __m128i comp = _mm_cmplt_epi32(_mm_castps_si128(v.v1), _mm_set1_epi32(0x43000000));
            __m128i select0 = _mm_and_si128(comp, _mm_castps_si128(result0));
            __m128i select1 = _mm_andnot_si128(comp, _mm_set1_epi32(0x7F800000));
            __m128i result2 = _mm_or_si128(select0, select1);

            comp = _mm_cmplt_epi32(itrunc, _mm_set1_epi32(-126));
            select1 = _mm_and_si128(comp, _mm_castps_si128(result1));
            select0 = _mm_andnot_si128(comp, _mm_castps_si128(result0));
            __m128i result3 = _mm_or_si128(select0, select1);

            comp = _mm_cmplt_epi32(_mm_castps_si128(v.v1), _mm_set1_epi32(0xC3160000));
            select0 = _mm_and_si128(comp, result3);
            select1 = _mm_andnot_si128(comp, _mm_set1_epi32(0));
            __m128i result4 = _mm_or_si128(select0, select1);

            __m128i sign = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x80000000));
            comp = _mm_cmpeq_epi32(sign, _mm_set1_epi32(0x80000000));
            select0 = _mm_and_si128(comp, result4);
            select1 = _mm_andnot_si128(comp, result2);
            __m128i result5 = _mm_or_si128(select0, select1);

            __m128i t0 = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x007FFFFF));
            __m128i t1 = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x7F800000));
            t0 = _mm_cmpeq_epi32(t0, _mm_set1_epi32(0));
            t1 = _mm_cmpeq_epi32(t1, _mm_set1_epi32(0x7F800000));
            __m128i isNaN = _mm_andnot_si128(t0, t1);

            select0 = _mm_and_si128(isNaN, _mm_set1_epi32(0x7FC00000));
            select1 = _mm_andnot_si128(isNaN, result5);
            __m128i vResult = _mm_or_si128(select0, select1);

            result.v1 = _mm_castsi128_ps(vResult);
#elif defined(SIMD_ARM_NEON)
            int32x4_t itrunc = vcvtq_s32_f32(v.v1);
            float32x4_t ftrunc = vcvtq_f32_s32(itrunc);
            float32x4_t y = vsubq_f32(v.v1, ftrunc);

            float32x4_t poly = vmlaq_f32(g_ExpEst6.v1, g_ExpEst7.v1, y);
            poly = vmlaq_f32(g_ExpEst5.v1, poly, y);
            poly = vmlaq_f32(g_ExpEst4.v1, poly, y);
            poly = vmlaq_f32(g_ExpEst3.v1, poly, y);
            poly = vmlaq_f32(g_ExpEst2.v1, poly, y);
            poly = vmlaq_f32(g_ExpEst1.v1, poly, y);
            poly = vmlaq_f32(g_One.v1, poly, y);

            int32x4_t biased = vaddq_s32(itrunc, g_ExponentBias.v1);
            biased = vshlq_n_s32(biased, 23);
            float32x4_t result0 = neon_vdivq_f32(vreinterpretq_f32_s32(biased), poly);

            biased = vaddq_s32(itrunc, g_253.v1);
            biased = vshlq_n_s32(biased, 23);
            float32x4_t result1 = neon_vdivq_f32(vreinterpretq_f32_s32(biased), poly);
            result1 = vmulq_f32(vreinterpretq_f32_s32(g_MinNormal.v1), result1);

            uint32x4_t comp = vcltq_s32(vreinterpretq_s32_f32(v.v1), g_Bin128.v1);
            float32x4_t result2 = vbslq_f32(comp, result0, vreinterpretq_f32_s32(g_Infinity.v1));

            comp = vcltq_s32(itrunc, g_SubnormalExponent.v1);
            float32x4_t result3 = vbslq_f32(comp, result1, result0);

            comp = vcltq_s32(vreinterpretq_s32_f32(v.v1), vreinterpretq_s32_u32(g_BinNeg150.v1));
            float32x4_t result4 = vbslq_f32(comp, result3, g_Zero.v1);

            int32x4_t sign = vandq_s32(vreinterpretq_s32_f32(v.v1), vreinterpretq_s32_u32(g_NegativeZero.v1));
            comp = vceqq_s32(sign, vreinterpretq_s32_u32(g_NegativeZero.v1));
            float32x4_t result5 = vbslq_f32(comp, result4, result2);

            int32x4_t t0 = vandq_s32(vreinterpretq_s32_f32(v.v1), g_QNaNTest.v1);
            int32x4_t t1 = vandq_s32(vreinterpretq_s32_f32(v.v1), g_Infinity.v1);
            t0 = vreinterpretq_s32_u32(vceqq_s32(t0, vreinterpretq_s32_f32(g_Zero.v1)));
            t1 = vreinterpretq_s32_u32(vceqq_s32(t1, g_Infinity.v1));
            int32x4_t isNaN = vbicq_s32(t1, t0);

            result.v1 = vbslq_f32(vreinterpretq_u32_s32(isNaN), vreinterpretq_f32_s32(g_QNaN.v1), result5);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecExp10(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            // exp10(v.v1) = exp2(vin*log2(10))
            R128x1F Vten;
            Vten.v1 = _mm_mul_ps(g_Lg10.v1, v.v1);
            result = VecExp2(Vten);
#elif defined(SIMD_ARM_NEON)
            R128x1F Vten;
            Vten.v1 = vmulq_f32(g_Lg10.v1, v.v1);
            result = VecExp2(Vten);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecExpE(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            // expE(v.v1) = exp2(vin*log2(e))
            R128x1F Ve;
            Ve.v1 = _mm_mul_ps(g_LgE.v1, v.v1);
            result = VecExp2(Ve);
#elif defined(SIMD_ARM_NEON)
            R128x1F Ve;
            Ve.v1 = vmulq_f32(g_LgE.v1, v.v1);
            result = VecExp2(Ve);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecLog2(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            __m128i     rawBiased = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x7F800000));
            __m128i     trailing = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x007FFFFF));
            __m128i     isExponentZero = _mm_cmpeq_epi32(_mm_setzero_si128(), rawBiased);

            // Compute exponent and significand for normals.
            __m128i biased = _mm_srli_epi32(rawBiased, 23);
            __m128i exponentNor = _mm_sub_epi32(biased, _mm_set1_epi32(127));
            __m128i trailingNor = trailing;

            // Compute exponent and significand for subnormals.
            __m128i leading = VecGetLeadingBit(trailing);
            __m128i shift = _mm_sub_epi32(_mm_set1_epi32(23), leading);
            __m128i exponentSub = _mm_sub_epi32(_mm_set1_epi32(-126), shift);
            __m128i trailingSub = VecMultiSrlEpi32(trailing, shift);
            trailingSub = _mm_and_si128(trailingSub, _mm_set1_epi32(0x007FFFFF));

            __m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
            __m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
            __m128i e = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isExponentZero, trailingSub);
            select1 = _mm_andnot_si128(isExponentZero, trailingNor);
            __m128i t = _mm_or_si128(select0, select1);

            // Compute the approximation.
            __m128i tmp = _mm_or_si128(_mm_set1_epi32(0x3F800000), t);
            __m128  y = _mm_sub_ps(_mm_castsi128_ps(tmp), _mm_set1_ps(1.0f));

            __m128 log2 = SseFMAdd(_mm_set1_ps(-0.010578f), y, _mm_set1_ps(+0.057148f));
            log2 = SseFMAdd(log2, y, _mm_set1_ps(-0.145700f));
            log2 = SseFMAdd(log2, y, _mm_set1_ps(+0.248590f));
            log2 = SseFMAdd(log2, y, _mm_set1_ps(-0.350295f));
            log2 = SseFMAdd(log2, y, g_LogEst2.v1);
            log2 = SseFMAdd(log2, y, g_LogEst1.v1);
            log2 = SseFMAdd(log2, y, g_LogEst0.v1);
            log2 = SseFMAdd(log2, y, _mm_cvtepi32_ps(e));

            __m128i isInfinite = _mm_and_si128(_mm_castps_si128(v.v1), g_AbsMask.v1);
            isInfinite = _mm_cmpeq_epi32(isInfinite, _mm_set1_epi32(0x7F800000));

            __m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(v.v1), _mm_setzero_si128());
            __m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(v.v1), _mm_set1_epi32(0x7F800000));
            __m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

            __m128i isZero = _mm_and_si128(_mm_castps_si128(v.v1), g_AbsMask.v1);
            isZero = _mm_cmpeq_epi32(isZero, _mm_setzero_si128());

            __m128i t0 = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x007FFFFF));
            __m128i t1 = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x7F800000));
            t0 = _mm_cmpeq_epi32(t0, _mm_setzero_si128());
            t1 = _mm_cmpeq_epi32(t1, _mm_set1_epi32(0x7F800000));
            __m128i isNaN = _mm_andnot_si128(t0, t1);

            select0 = _mm_and_si128(isInfinite, _mm_set1_epi32(0x7F800000));
            select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
            __m128i res = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isZero, g_NegInfinity.v1);
            select1 = _mm_andnot_si128(isZero, g_NegQNaN.v1);
            tmp = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isPositive, res);
            select1 = _mm_andnot_si128(isPositive, tmp);
            res = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isNaN, g_QNaN.v1);
            select1 = _mm_andnot_si128(isNaN, res);
            res = _mm_or_si128(select0, select1);

            result.v1 = _mm_castsi128_ps(res);
#elif defined(SIMD_ARM_NEON)
            int32x4_t rawBiased = vandq_s32(vreinterpretq_s32_f32(v.v1), g_Infinity.v1);
            int32x4_t trailing = vandq_s32(vreinterpretq_s32_f32(v.v1), g_QNaNTest.v1);
            uint32x4_t isExponentZero = vceqq_s32(vreinterpretq_s32_f32(g_Zero.v1), rawBiased);

            // Compute exponent and significand for normals.
            int32x4_t biased = vshrq_n_s32(rawBiased, 23);
            int32x4_t exponentNor = vsubq_s32(biased, g_ExponentBias.v1);
            int32x4_t trailingNor = trailing;

            // Compute exponent and significand for subnormals.
            int32x4_t leading = GetLeadingBit(trailing);
            int32x4_t shift = vsubq_s32(g_NumTrailing.v1, leading);
            int32x4_t exponentSub = vsubq_s32(g_SubnormalExponent.v1, shift);
            int32x4_t trailingSub = vshlq_s32(trailing, shift);
            trailingSub = vandq_s32(trailingSub, g_QNaNTest.v1);
            int32x4_t e = vbslq_s32(isExponentZero, exponentSub, exponentNor);
            int32x4_t t = vbslq_s32(isExponentZero, trailingSub, trailingNor);

            // Compute the approximation.
            int32x4_t tmp = vorrq_s32(vreinterpretq_s32_f32(g_One.v1), t);
            float32x4_t y = vsubq_f32(vreinterpretq_f32_s32(tmp), g_One.v1);

            float32x4_t log2 = vmlaq_f32(g_LogEst6.v1, g_LogEst7.v1, y);
            log2 = vmlaq_f32(g_LogEst5.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst4.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst3.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst2.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst1.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst0.v1, log2, y);
            log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

            uint32x4_t isInfinite = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_AbsMask.v1));
            isInfinite = vceqq_u32(isInfinite, vreinterpretq_u32_s32(g_Infinity.v1));

            uint32x4_t isGreaterZero = vcgtq_f32(v.v1, g_Zero.v1);
            uint32x4_t isNotFinite = vcgtq_f32(v.v1, vreinterpretq_f32_s32(g_Infinity.v1));
            uint32x4_t isPositive = vbicq_u32(isGreaterZero, isNotFinite);

            uint32x4_t isZero = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_AbsMask.v1));
            isZero = vceqq_u32(isZero, vreinterpretq_u32_f32(g_Zero.v1));

            uint32x4_t t0 = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_QNaNTest.v1));
            uint32x4_t t1 = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_Infinity.v1));
            t0 = vceqq_u32(t0, vreinterpretq_u32_f32(g_Zero.v1));
            t1 = vceqq_u32(t1, vreinterpretq_u32_s32(g_Infinity.v1));
            uint32x4_t isNaN = vbicq_u32(t1, t0);

            result.v1 = vbslq_f32(isInfinite, vreinterpretq_f32_s32(g_Infinity.v1), log2);
            float32x4_t tmp2 =
                vbslq_f32(isZero, vreinterpretq_f32_u32(g_NegInfinity.v1), vreinterpretq_f32_u32(g_NegQNaN.v1));
            result.v1 = vbslq_f32(isPositive, result.v1, tmp2);
            result.v1 = vbslq_f32(isNaN, vreinterpretq_f32_s32(g_QNaN.v1), result.v1);
#endif
            return result;
        }


        inline R128x1F __vectorcall VecLog10(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            __m128i     rawBiased = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x7F800000));
            __m128i     trailing = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x007FFFFF));
            __m128i     isExponentZero = _mm_cmpeq_epi32(_mm_setzero_si128(), rawBiased);

            // Compute exponent and significand for normals.
            __m128i biased = _mm_srli_epi32(rawBiased, 23);
            __m128i exponentNor = _mm_sub_epi32(biased, _mm_set1_epi32(127));
            __m128i trailingNor = trailing;

            // Compute exponent and significand for subnormals.
            __m128i leading = VecGetLeadingBit(trailing);
            __m128i shift = _mm_sub_epi32(_mm_set1_epi32(23), leading);
            __m128i exponentSub = _mm_sub_epi32(_mm_set1_epi32(-126), shift);
            __m128i trailingSub = VecMultiSrlEpi32(trailing, shift);
            trailingSub = _mm_and_si128(trailingSub, _mm_set1_epi32(0x007FFFFF));

            __m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
            __m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
            __m128i e = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isExponentZero, trailingSub);
            select1 = _mm_andnot_si128(isExponentZero, trailingNor);
            __m128i t = _mm_or_si128(select0, select1);

            // Compute the approximation.
            __m128i tmp = _mm_or_si128(_mm_set1_epi32(0x3F800000), t);
            __m128  y = _mm_sub_ps(_mm_castsi128_ps(tmp), _mm_set1_ps(1.0f));

            __m128 log2 = SseFMAdd(_mm_set1_ps(-0.010578f), y, _mm_set1_ps(+0.057148f));
            log2 = SseFMAdd(log2, y, _mm_set1_ps(-0.145700f));
            log2 = SseFMAdd(log2, y, _mm_set1_ps(+0.248590f));
            log2 = SseFMAdd(log2, y, _mm_set1_ps(-0.350295f));
            log2 = SseFMAdd(log2, y, g_LogEst2.v1);
            log2 = SseFMAdd(log2, y, g_LogEst1.v1);
            log2 = SseFMAdd(log2, y, g_LogEst0.v1);
            log2 = SseFMAdd(log2, y, _mm_cvtepi32_ps(e));

            log2 = _mm_mul_ps(g_InvLg10.v1, log2);

            __m128i isInfinite = _mm_and_si128(_mm_castps_si128(v.v1), g_AbsMask.v1);
            isInfinite = _mm_cmpeq_epi32(isInfinite, _mm_set1_epi32(0x7F800000));

            __m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(v.v1), _mm_setzero_si128());
            __m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(v.v1), _mm_set1_epi32(0x7F800000));
            __m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

            __m128i isZero = _mm_and_si128(_mm_castps_si128(v.v1), g_AbsMask.v1);
            isZero = _mm_cmpeq_epi32(isZero, _mm_setzero_si128());

            __m128i t0 = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x007FFFFF));
            __m128i t1 = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x7F800000));
            t0 = _mm_cmpeq_epi32(t0, _mm_setzero_si128());
            t1 = _mm_cmpeq_epi32(t1, _mm_set1_epi32(0x7F800000));
            __m128i isNaN = _mm_andnot_si128(t0, t1);

            select0 = _mm_and_si128(isInfinite, _mm_set1_epi32(0x7F800000));
            select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
            __m128i res = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isZero, g_NegInfinity.v1);
            select1 = _mm_andnot_si128(isZero, g_NegQNaN.v1);
            tmp = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isPositive, res);
            select1 = _mm_andnot_si128(isPositive, tmp);
            res = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isNaN, g_QNaN.v1);
            select1 = _mm_andnot_si128(isNaN, res);
            res = _mm_or_si128(select0, select1);

            result.v1 = _mm_castsi128_ps(res);
#elif defined(SIMD_ARM_NEON)
            int32x4_t rawBiased = vandq_s32(vreinterpretq_s32_f32(v.v1), g_Infinity.v1);
            int32x4_t trailing = vandq_s32(vreinterpretq_s32_f32(v.v1), g_QNaNTest.v1);
            uint32x4_t isExponentZero = vceqq_s32(vreinterpretq_s32_f32(g_Zero.v1), rawBiased);

            // Compute exponent and significand for normals.
            int32x4_t biased = vshrq_n_s32(rawBiased, 23);
            int32x4_t exponentNor = vsubq_s32(biased, g_ExponentBias.v1);
            int32x4_t trailingNor = trailing;

            // Compute exponent and significand for subnormals.
            int32x4_t leading = GetLeadingBit(trailing);
            int32x4_t shift = vsubq_s32(g_NumTrailing.v1, leading);
            int32x4_t exponentSub = vsubq_s32(g_SubnormalExponent.v1, shift);
            int32x4_t trailingSub = vshlq_s32(trailing, shift);
            trailingSub = vandq_s32(trailingSub, g_QNaNTest.v1);
            int32x4_t e = vbslq_s32(isExponentZero, exponentSub, exponentNor);
            int32x4_t t = vbslq_s32(isExponentZero, trailingSub, trailingNor);

            // Compute the approximation.
            int32x4_t tmp = vorrq_s32(vreinterpretq_s32_f32(g_One.v1), t);
            float32x4_t y = vsubq_f32(vreinterpretq_f32_s32(tmp), g_One.v1);

            float32x4_t log2 = vmlaq_f32(g_LogEst6.v1, g_LogEst7.v1, y);
            log2 = vmlaq_f32(g_LogEst5.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst4.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst3.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst2.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst1.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst0.v1, log2, y);
            log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

            log2 = vmulq_f32(g_InvLg10.v1, log2);

            uint32x4_t isInfinite = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_AbsMask.v1));
            isInfinite = vceqq_u32(isInfinite, vreinterpretq_u32_s32(g_Infinity.v1));

            uint32x4_t isGreaterZero = vcgtq_s32(vreinterpretq_s32_f32(v.v1), vreinterpretq_s32_f32(g_Zero.v1));
            uint32x4_t isNotFinite = vcgtq_s32(vreinterpretq_s32_f32(v.v1), g_Infinity.v1);
            uint32x4_t isPositive = vbicq_u32(isGreaterZero, isNotFinite);

            uint32x4_t isZero = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_AbsMask.v1));
            isZero = vceqq_u32(isZero, vreinterpretq_u32_f32(g_Zero.v1));

            uint32x4_t t0 = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_QNaNTest.v1));
            uint32x4_t t1 = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_Infinity.v1));
            t0 = vceqq_u32(t0, vreinterpretq_u32_f32(g_Zero.v1));
            t1 = vceqq_u32(t1, vreinterpretq_u32_s32(g_Infinity.v1));
            uint32x4_t isNaN = vbicq_u32(t1, t0);

            result.v1 = vbslq_f32(isInfinite, vreinterpretq_f32_s32(g_Infinity.v1), log2);
            float32x4_t tmp2 =
                vbslq_f32(isZero, vreinterpretq_f32_u32(g_NegInfinity.v1), vreinterpretq_f32_u32(g_NegQNaN.v1));
            result.v1 = vbslq_f32(isPositive, result.v1, tmp2);
            result.v1 = vbslq_f32(isNaN, vreinterpretq_f32_s32(g_QNaN.v1), result.v1);

#endif
            return result;
        }


        inline R128x1F __vectorcall VecLogE(R128x1F v)
        {
            R128x1F result;
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
            __m128i     rawBiased = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x7F800000));
            __m128i     trailing = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x007FFFFF));
            __m128i     isExponentZero = _mm_cmpeq_epi32(_mm_setzero_si128(), rawBiased);

            // Compute exponent and significand for normals.
            __m128i biased = _mm_srli_epi32(rawBiased, 23);
            __m128i exponentNor = _mm_sub_epi32(biased, _mm_set1_epi32(127));
            __m128i trailingNor = trailing;

            // Compute exponent and significand for subnormals.
            __m128i leading = VecGetLeadingBit(trailing);
            __m128i shift = _mm_sub_epi32(_mm_set1_epi32(23), leading);
            __m128i exponentSub = _mm_sub_epi32(_mm_set1_epi32(-126), shift);
            __m128i trailingSub = VecMultiSrlEpi32(trailing, shift);
            trailingSub = _mm_and_si128(trailingSub, _mm_set1_epi32(0x007FFFFF));

            __m128i select0 = _mm_and_si128(isExponentZero, exponentSub);
            __m128i select1 = _mm_andnot_si128(isExponentZero, exponentNor);
            __m128i e = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isExponentZero, trailingSub);
            select1 = _mm_andnot_si128(isExponentZero, trailingNor);
            __m128i t = _mm_or_si128(select0, select1);

            // Compute the approximation.
            __m128i tmp = _mm_or_si128(_mm_set1_epi32(0x3F800000), t);
            __m128  y = _mm_sub_ps(_mm_castsi128_ps(tmp), _mm_set1_ps(1.0f));

            __m128 log2 = SseFMAdd(_mm_set1_ps(-0.010578f), y, _mm_set1_ps(+0.057148f));
            log2 = SseFMAdd(log2, y, _mm_set1_ps(-0.145700f));
            log2 = SseFMAdd(log2, y, _mm_set1_ps(+0.248590f));
            log2 = SseFMAdd(log2, y, _mm_set1_ps(-0.350295f));
            log2 = SseFMAdd(log2, y, g_LogEst2.v1);
            log2 = SseFMAdd(log2, y, g_LogEst1.v1);
            log2 = SseFMAdd(log2, y, g_LogEst0.v1);
            log2 = SseFMAdd(log2, y, _mm_cvtepi32_ps(e));

            log2 = _mm_mul_ps(g_InvLgE.v1, log2);

            __m128i isInfinite = _mm_and_si128(_mm_castps_si128(v.v1), g_AbsMask.v1);
            isInfinite = _mm_cmpeq_epi32(isInfinite, _mm_set1_epi32(0x7F800000));

            __m128i isGreaterZero = _mm_cmpgt_epi32(_mm_castps_si128(v.v1), _mm_setzero_si128());
            __m128i isNotFinite = _mm_cmpgt_epi32(_mm_castps_si128(v.v1), _mm_set1_epi32(0x7F800000));
            __m128i isPositive = _mm_andnot_si128(isNotFinite, isGreaterZero);

            __m128i isZero = _mm_and_si128(_mm_castps_si128(v.v1), g_AbsMask.v1);
            isZero = _mm_cmpeq_epi32(isZero, _mm_setzero_si128());

            __m128i t0 = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x007FFFFF));
            __m128i t1 = _mm_and_si128(_mm_castps_si128(v.v1), _mm_set1_epi32(0x7F800000));
            t0 = _mm_cmpeq_epi32(t0, _mm_setzero_si128());
            t1 = _mm_cmpeq_epi32(t1, _mm_set1_epi32(0x7F800000));
            __m128i isNaN = _mm_andnot_si128(t0, t1);

            select0 = _mm_and_si128(isInfinite, _mm_set1_epi32(0x7F800000));
            select1 = _mm_andnot_si128(isInfinite, _mm_castps_si128(log2));
            __m128i res = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isZero, g_NegInfinity.v1);
            select1 = _mm_andnot_si128(isZero, g_NegQNaN.v1);
            tmp = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isPositive, res);
            select1 = _mm_andnot_si128(isPositive, tmp);
            res = _mm_or_si128(select0, select1);

            select0 = _mm_and_si128(isNaN, g_QNaN.v1);
            select1 = _mm_andnot_si128(isNaN, res);
            res = _mm_or_si128(select0, select1);

            result.v1 = _mm_castsi128_ps(res);
#elif defined SIMD_ARM_NEON
            int32x4_t rawBiased = vandq_s32(vreinterpretq_s32_f32(v.v1), g_Infinity.v1);
            int32x4_t trailing = vandq_s32(vreinterpretq_s32_f32(v.v1), g_QNaNTest.v1);
            uint32x4_t isExponentZero = vceqq_s32(vreinterpretq_s32_f32(g_Zero.v1), rawBiased);

            // Compute exponent and significand for normals.
            int32x4_t biased = vshrq_n_s32(rawBiased, 23);
            int32x4_t exponentNor = vsubq_s32(biased, g_ExponentBias.v1);
            int32x4_t trailingNor = trailing;

            // Compute exponent and significand for subnormals.
            int32x4_t leading = GetLeadingBit(trailing);
            int32x4_t shift = vsubq_s32(g_NumTrailing.v1, leading);
            int32x4_t exponentSub = vsubq_s32(g_SubnormalExponent.v1, shift);
            int32x4_t trailingSub = vshlq_s32(trailing, shift);
            trailingSub = vandq_s32(trailingSub, g_QNaNTest.v1);
            int32x4_t e = vbslq_s32(isExponentZero, exponentSub, exponentNor);
            int32x4_t t = vbslq_s32(isExponentZero, trailingSub, trailingNor);

            // Compute the approximation.
            int32x4_t tmp = vorrq_s32(vreinterpretq_s32_f32(g_One.v1), t);
            float32x4_t y = vsubq_f32(vreinterpretq_f32_s32(tmp), g_One.v1);

            float32x4_t log2 = vmlaq_f32(g_LogEst6.v1, g_LogEst7.v1, y);
            log2 = vmlaq_f32(g_LogEst5.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst4.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst3.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst2.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst1.v1, log2, y);
            log2 = vmlaq_f32(g_LogEst0.v1, log2, y);
            log2 = vmlaq_f32(vcvtq_f32_s32(e), log2, y);

            log2 = vmulq_f32(g_InvLgE.v1, log2);

            uint32x4_t isInfinite = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_AbsMask.v1));
            isInfinite = vceqq_u32(isInfinite, vreinterpretq_u32_s32(g_Infinity.v1));

            uint32x4_t isGreaterZero = vcgtq_s32(vreinterpretq_s32_f32(v.v1), vreinterpretq_s32_f32(g_Zero.v1));
            uint32x4_t isNotFinite = vcgtq_s32(vreinterpretq_s32_f32(v.v1), g_Infinity.v1);
            uint32x4_t isPositive = vbicq_u32(isGreaterZero, isNotFinite);

            uint32x4_t isZero = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_AbsMask.v1));
            isZero = vceqq_u32(isZero, vreinterpretq_u32_f32(g_Zero.v1));

            uint32x4_t t0 = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_QNaNTest.v1));
            uint32x4_t t1 = vandq_u32(vreinterpretq_u32_f32(v.v1), vreinterpretq_u32_s32(g_Infinity.v1));
            t0 = vceqq_u32(t0, vreinterpretq_u32_f32(g_Zero.v1));
            t1 = vceqq_u32(t1, vreinterpretq_u32_s32(g_Infinity.v1));
            uint32x4_t isNaN = vbicq_u32(t1, t0);

            result.v1 = vbslq_f32(isInfinite, vreinterpretq_f32_s32(g_Infinity.v1), log2);
            float32x4_t tmp2 =
                vbslq_f32(isZero, vreinterpretq_f32_u32(g_NegInfinity.v1), vreinterpretq_f32_u32(g_NegQNaN.v1));
            result.v1 = vbslq_f32(isPositive, result.v1, tmp2);
            result.v1 = vbslq_f32(isNaN, vreinterpretq_f32_s32(g_QNaN.v1), result.v1);
#endif
            return result;
        }
    } // namespace SIMDMath
} // namespace krystallic
