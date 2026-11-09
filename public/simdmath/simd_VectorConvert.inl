#pragma once

#include "simd_library.h"


namespace krystallic
{
    namespace SIMDMath
    {
#if defined(SIMD_ARM_NEON)
        namespace inner_simd
        {
            inline R128D_Native ConvertNativeFD(R128F_Native v) noexcept
            {
                R128D_Native r = {};
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
                double temp[2] = {static_cast<double>(vgetq_lane_f32(v, 0)), static_cast<double>(vgetq_lane_f32(v, 1))};
                r = vld1q_f64(temp);
#else
                r.lo = static_cast<double>(vgetq_lane_f32(v, 0));
                r.hi = static_cast<double>(vgetq_lane_f32(v, 1));
#endif
                return r;
            }

            inline R128D_Native ConvertNativeID(R128I_Native v) noexcept
            {
                R128D_Native r = {};
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
                double temp[2] = {static_cast<double>(vgetq_lane_s32(v, 0)), static_cast<double>(vgetq_lane_s32(v, 1))};
                r = vld1q_f64(temp);
#else
                r.lo = static_cast<double>(vgetq_lane_s32(v, 0));
                r.hi = static_cast<double>(vgetq_lane_s32(v, 1));
#endif
                return r;
            }

            inline R128F_Native ConvertNativeDF(R128D_Native v) noexcept
            {
                R128F_Native r = vdupq_n_f32(0.0f);
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
                r = vsetq_lane_f32(static_cast<float>(vgetq_lane_f64(v, 0)), r, 0);
                r = vsetq_lane_f32(static_cast<float>(vgetq_lane_f64(v, 1)), r, 1);
#else
                r = vsetq_lane_f32(static_cast<float>(v.lo), r, 0);
                r = vsetq_lane_f32(static_cast<float>(v.hi), r, 1);
#endif
                return r;
            }

            inline R128I_Native ConvertNativeDI(R128D_Native v) noexcept
            {
                R128I_Native r = vdupq_n_s32(0);
#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
                r = vsetq_lane_s32(static_cast<int32_t>(vgetq_lane_f64(v, 0)), r, 0);
                r = vsetq_lane_s32(static_cast<int32_t>(vgetq_lane_f64(v, 1)), r, 1);
#else
                r = vsetq_lane_s32(static_cast<int32_t>(v.lo), r, 0);
                r = vsetq_lane_s32(static_cast<int32_t>(v.hi), r, 1);
#endif
                return r;
            }
        }
#endif

        inline R128x1I __vectorcall Convert128x1FI(R128x1F_Arg0 v) noexcept
        {
            R128x1I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvttps_epi32(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_s32_f32(v.v1);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 4; ++i) r.v[i] = static_cast<int>(v.v[i]);
#endif
            return r;
        }

        inline R128x1D __vectorcall Convert128x1FD(R128x1F_Arg0 v) noexcept
        {
            R128x1D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtps_pd(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeFD(v.v1);
#elif defined(SIMD_NONE)
            r.v[0] = static_cast<double>(v.v[0]);
            r.v[1] = static_cast<double>(v.v[1]);
#endif
            return r;
        }

        inline R128x1F __vectorcall Convert128x1IF(R128x1I_Arg0 v) noexcept
        {
            R128x1F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtepi32_ps(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_f32_s32(v.v1);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 4; ++i) r.v[i] = static_cast<float>(v.v[i]);
#endif
            return r;
        }

        inline R128x1D __vectorcall Convert128x1ID(R128x1I_Arg0 v) noexcept
        {
            R128x1D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtepi32_pd(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeID(v.v1);
#elif defined(SIMD_NONE)
            r.v[0] = static_cast<double>(v.v[0]);
            r.v[1] = static_cast<double>(v.v[1]);
#endif
            return r;
        }

        inline R128x1F __vectorcall Convert128x1DF(R128x1D_Arg0 v) noexcept
        {
            R128x1F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtpd_ps(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeDF(v.v1);
#elif defined(SIMD_NONE)
            r.v[0] = static_cast<float>(v.v[0]);
            r.v[1] = static_cast<float>(v.v[1]);
            r.v[2] = 0.0f;
            r.v[3] = 0.0f;
#endif
            return r;
        }

        inline R128x1I __vectorcall Convert128x1DI(R128x1D_Arg0 v) noexcept
        {
            R128x1I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvttpd_epi32(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeDI(v.v1);
#elif defined(SIMD_NONE)
            r.v[0] = static_cast<int>(v.v[0]);
            r.v[1] = static_cast<int>(v.v[1]);
            r.v[2] = 0;
            r.v[3] = 0;
#endif
            return r;
        }

        inline R128x2I __vectorcall Convert128x2FI(R128x2F_Arg0 v) noexcept
        {
            R128x2I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvttps_epi32(v.v1);
            r.v2 = _mm_cvttps_epi32(v.v2);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_s32_f32(v.v1);
            r.v2 = vcvtq_s32_f32(v.v2);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 8; ++i) r.v[i] = static_cast<int>(v.v[i]);
#endif
            return r;
        }

        inline R128x2D __vectorcall Convert128x2FD(R128x2F_Arg0 v) noexcept
        {
            R128x2D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtps_pd(v.v1);
            r.v2 = _mm_cvtps_pd(v.v2);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeFD(v.v1);
            r.v2 = inner_simd::ConvertNativeFD(v.v2);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 2; ++i) { r.v[i * 2 + 0] = static_cast<double>(v.v[i * 4 + 0]); r.v[i * 2 + 1] = static_cast<double>(v.v[i * 4 + 1]); }
#endif
            return r;
        }

        inline R128x2F __vectorcall Convert128x2IF(R128x2I_Arg0 v) noexcept
        {
            R128x2F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtepi32_ps(v.v1);
            r.v2 = _mm_cvtepi32_ps(v.v2);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_f32_s32(v.v1);
            r.v2 = vcvtq_f32_s32(v.v2);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 8; ++i) r.v[i] = static_cast<float>(v.v[i]);
#endif
            return r;
        }

        inline R128x2D __vectorcall Convert128x2ID(R128x2I_Arg0 v) noexcept
        {
            R128x2D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtepi32_pd(v.v1);
            r.v2 = _mm_cvtepi32_pd(v.v2);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeID(v.v1);
            r.v2 = inner_simd::ConvertNativeID(v.v2);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 2; ++i) { r.v[i * 2 + 0] = static_cast<double>(v.v[i * 4 + 0]); r.v[i * 2 + 1] = static_cast<double>(v.v[i * 4 + 1]); }
#endif
            return r;
        }

        inline R128x2F __vectorcall Convert128x2DF(R128x2D_Arg0 v) noexcept
        {
            R128x2F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtpd_ps(v.v1);
            r.v2 = _mm_cvtpd_ps(v.v2);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeDF(v.v1);
            r.v2 = inner_simd::ConvertNativeDF(v.v2);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 2; ++i) { r.v[i * 4 + 0] = static_cast<float>(v.v[i * 2 + 0]); r.v[i * 4 + 1] = static_cast<float>(v.v[i * 2 + 1]); r.v[i * 4 + 2] = 0.0f; r.v[i * 4 + 3] = 0.0f; }
#endif
            return r;
        }

        inline R128x2I __vectorcall Convert128x2DI(R128x2D_Arg0 v) noexcept
        {
            R128x2I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvttpd_epi32(v.v1);
            r.v2 = _mm_cvttpd_epi32(v.v2);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeDI(v.v1);
            r.v2 = inner_simd::ConvertNativeDI(v.v2);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 2; ++i) { r.v[i * 4 + 0] = static_cast<int>(v.v[i * 2 + 0]); r.v[i * 4 + 1] = static_cast<int>(v.v[i * 2 + 1]); r.v[i * 4 + 2] = 0; r.v[i * 4 + 3] = 0; }
#endif
            return r;
        }

        inline R128x3I __vectorcall Convert128x3FI(R128x3F_Arg0 v) noexcept
        {
            R128x3I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvttps_epi32(v.v1);
            r.v2 = _mm_cvttps_epi32(v.v2);
            r.v3 = _mm_cvttps_epi32(v.v3);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_s32_f32(v.v1);
            r.v2 = vcvtq_s32_f32(v.v2);
            r.v3 = vcvtq_s32_f32(v.v3);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 12; ++i) r.v[i] = static_cast<int>(v.v[i]);
#endif
            return r;
        }

        inline R128x3D __vectorcall Convert128x3FD(R128x3F_Arg0 v) noexcept
        {
            R128x3D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtps_pd(v.v1);
            r.v2 = _mm_cvtps_pd(v.v2);
            r.v3 = _mm_cvtps_pd(v.v3);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeFD(v.v1);
            r.v2 = inner_simd::ConvertNativeFD(v.v2);
            r.v3 = inner_simd::ConvertNativeFD(v.v3);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 3; ++i) { r.v[i * 2 + 0] = static_cast<double>(v.v[i * 4 + 0]); r.v[i * 2 + 1] = static_cast<double>(v.v[i * 4 + 1]); }
#endif
            return r;
        }

        inline R128x3F __vectorcall Convert128x3IF(R128x3I_Arg0 v) noexcept
        {
            R128x3F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtepi32_ps(v.v1);
            r.v2 = _mm_cvtepi32_ps(v.v2);
            r.v3 = _mm_cvtepi32_ps(v.v3);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_f32_s32(v.v1);
            r.v2 = vcvtq_f32_s32(v.v2);
            r.v3 = vcvtq_f32_s32(v.v3);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 12; ++i) r.v[i] = static_cast<float>(v.v[i]);
#endif
            return r;
        }

        inline R128x3D __vectorcall Convert128x3ID(R128x3I_Arg0 v) noexcept
        {
            R128x3D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtepi32_pd(v.v1);
            r.v2 = _mm_cvtepi32_pd(v.v2);
            r.v3 = _mm_cvtepi32_pd(v.v3);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeID(v.v1);
            r.v2 = inner_simd::ConvertNativeID(v.v2);
            r.v3 = inner_simd::ConvertNativeID(v.v3);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 3; ++i) { r.v[i * 2 + 0] = static_cast<double>(v.v[i * 4 + 0]); r.v[i * 2 + 1] = static_cast<double>(v.v[i * 4 + 1]); }
#endif
            return r;
        }

        inline R128x3F __vectorcall Convert128x3DF(R128x3D_Arg0 v) noexcept
        {
            R128x3F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtpd_ps(v.v1);
            r.v2 = _mm_cvtpd_ps(v.v2);
            r.v3 = _mm_cvtpd_ps(v.v3);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeDF(v.v1);
            r.v2 = inner_simd::ConvertNativeDF(v.v2);
            r.v3 = inner_simd::ConvertNativeDF(v.v3);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 3; ++i) { r.v[i * 4 + 0] = static_cast<float>(v.v[i * 2 + 0]); r.v[i * 4 + 1] = static_cast<float>(v.v[i * 2 + 1]); r.v[i * 4 + 2] = 0.0f; r.v[i * 4 + 3] = 0.0f; }
#endif
            return r;
        }

        inline R128x3I __vectorcall Convert128x3DI(R128x3D_Arg0 v) noexcept
        {
            R128x3I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvttpd_epi32(v.v1);
            r.v2 = _mm_cvttpd_epi32(v.v2);
            r.v3 = _mm_cvttpd_epi32(v.v3);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeDI(v.v1);
            r.v2 = inner_simd::ConvertNativeDI(v.v2);
            r.v3 = inner_simd::ConvertNativeDI(v.v3);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 3; ++i) { r.v[i * 4 + 0] = static_cast<int>(v.v[i * 2 + 0]); r.v[i * 4 + 1] = static_cast<int>(v.v[i * 2 + 1]); r.v[i * 4 + 2] = 0; r.v[i * 4 + 3] = 0; }
#endif
            return r;
        }

        inline R128x4I __vectorcall Convert128x4FI(R128x4F_Arg0 v) noexcept
        {
            R128x4I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvttps_epi32(v.v1);
            r.v2 = _mm_cvttps_epi32(v.v2);
            r.v3 = _mm_cvttps_epi32(v.v3);
            r.v4 = _mm_cvttps_epi32(v.v4);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_s32_f32(v.v1);
            r.v2 = vcvtq_s32_f32(v.v2);
            r.v3 = vcvtq_s32_f32(v.v3);
            r.v4 = vcvtq_s32_f32(v.v4);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 16; ++i) r.v[i] = static_cast<int>(v.v[i]);
#endif
            return r;
        }

        inline R128x4D __vectorcall Convert128x4FD(R128x4F_Arg0 v) noexcept
        {
            R128x4D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtps_pd(v.v1);
            r.v2 = _mm_cvtps_pd(v.v2);
            r.v3 = _mm_cvtps_pd(v.v3);
            r.v4 = _mm_cvtps_pd(v.v4);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeFD(v.v1);
            r.v2 = inner_simd::ConvertNativeFD(v.v2);
            r.v3 = inner_simd::ConvertNativeFD(v.v3);
            r.v4 = inner_simd::ConvertNativeFD(v.v4);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 4; ++i) { r.v[i * 2 + 0] = static_cast<double>(v.v[i * 4 + 0]); r.v[i * 2 + 1] = static_cast<double>(v.v[i * 4 + 1]); }
#endif
            return r;
        }

        inline R128x4F __vectorcall Convert128x4IF(R128x4I_Arg0 v) noexcept
        {
            R128x4F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtepi32_ps(v.v1);
            r.v2 = _mm_cvtepi32_ps(v.v2);
            r.v3 = _mm_cvtepi32_ps(v.v3);
            r.v4 = _mm_cvtepi32_ps(v.v4);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_f32_s32(v.v1);
            r.v2 = vcvtq_f32_s32(v.v2);
            r.v3 = vcvtq_f32_s32(v.v3);
            r.v4 = vcvtq_f32_s32(v.v4);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 16; ++i) r.v[i] = static_cast<float>(v.v[i]);
#endif
            return r;
        }

        inline R128x4D __vectorcall Convert128x4ID(R128x4I_Arg0 v) noexcept
        {
            R128x4D r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtepi32_pd(v.v1);
            r.v2 = _mm_cvtepi32_pd(v.v2);
            r.v3 = _mm_cvtepi32_pd(v.v3);
            r.v4 = _mm_cvtepi32_pd(v.v4);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeID(v.v1);
            r.v2 = inner_simd::ConvertNativeID(v.v2);
            r.v3 = inner_simd::ConvertNativeID(v.v3);
            r.v4 = inner_simd::ConvertNativeID(v.v4);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 4; ++i) { r.v[i * 2 + 0] = static_cast<double>(v.v[i * 4 + 0]); r.v[i * 2 + 1] = static_cast<double>(v.v[i * 4 + 1]); }
#endif
            return r;
        }

        inline R128x4F __vectorcall Convert128x4DF(R128x4D_Arg0 v) noexcept
        {
            R128x4F r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvtpd_ps(v.v1);
            r.v2 = _mm_cvtpd_ps(v.v2);
            r.v3 = _mm_cvtpd_ps(v.v3);
            r.v4 = _mm_cvtpd_ps(v.v4);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeDF(v.v1);
            r.v2 = inner_simd::ConvertNativeDF(v.v2);
            r.v3 = inner_simd::ConvertNativeDF(v.v3);
            r.v4 = inner_simd::ConvertNativeDF(v.v4);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 4; ++i) { r.v[i * 4 + 0] = static_cast<float>(v.v[i * 2 + 0]); r.v[i * 4 + 1] = static_cast<float>(v.v[i * 2 + 1]); r.v[i * 4 + 2] = 0.0f; r.v[i * 4 + 3] = 0.0f; }
#endif
            return r;
        }

        inline R128x4I __vectorcall Convert128x4DI(R128x4D_Arg0 v) noexcept
        {
            R128x4I r = {};
#if defined(SIMD_SSE42) || defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm_cvttpd_epi32(v.v1);
            r.v2 = _mm_cvttpd_epi32(v.v2);
            r.v3 = _mm_cvttpd_epi32(v.v3);
            r.v4 = _mm_cvttpd_epi32(v.v4);
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeDI(v.v1);
            r.v2 = inner_simd::ConvertNativeDI(v.v2);
            r.v3 = inner_simd::ConvertNativeDI(v.v3);
            r.v4 = inner_simd::ConvertNativeDI(v.v4);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 4; ++i) { r.v[i * 4 + 0] = static_cast<int>(v.v[i * 2 + 0]); r.v[i * 4 + 1] = static_cast<int>(v.v[i * 2 + 1]); r.v[i * 4 + 2] = 0; r.v[i * 4 + 3] = 0; }
#endif
            return r;
        }

        inline R256x1I __vectorcall Convert256x1FI(R256x1F_Arg0 v) noexcept
        {
            R256x1I r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_cvttps_epi32(v.v1);
            r.v2 = _mm_cvttps_epi32(v.v2);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_cvttps_epi32(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_s32_f32(v.v1);
            r.v2 = vcvtq_s32_f32(v.v2);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 8; ++i) r.v[i] = static_cast<int>(v.v[i]);
#endif
            return r;
        }

        inline R256x1D __vectorcall Convert256x1FD(R256x1F_Arg0 v) noexcept
        {
            R256x1D r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_cvtps_pd(v.v1);
            r.v2 = _mm_cvtps_pd(_mm_movehl_ps(v.v1, v.v1));
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_cvtps_pd(_mm256_castps256_ps128(v.v1));
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeFD(v.v1);
            r.v2 = inner_simd::ConvertNativeFD(vextq_f32(v.v1, v.v1, 2));
#elif defined(SIMD_NONE)
            for (int i = 0; i < 4; ++i) r.v[i] = static_cast<double>(v.v[i]);
#endif
            return r;
        }

        inline R256x1F __vectorcall Convert256x1IF(R256x1I_Arg0 v) noexcept
        {
            R256x1F r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_cvtepi32_ps(v.v1);
            r.v2 = _mm_cvtepi32_ps(v.v2);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_cvtepi32_ps(v.v1);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_f32_s32(v.v1);
            r.v2 = vcvtq_f32_s32(v.v2);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 8; ++i) r.v[i] = static_cast<float>(v.v[i]);
#endif
            return r;
        }

        inline R256x1D __vectorcall Convert256x1ID(R256x1I_Arg0 v) noexcept
        {
            R256x1D r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_cvtepi32_pd(v.v1);
            r.v2 = _mm_cvtepi32_pd(_mm_srli_si128(v.v1, 8));
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_cvtepi32_pd(_mm256_castsi256_si128(v.v1));
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeID(v.v1);
            r.v2 = inner_simd::ConvertNativeID(vextq_s32(v.v1, v.v1, 2));
#elif defined(SIMD_NONE)
            for (int i = 0; i < 4; ++i) r.v[i] = static_cast<double>(v.v[i]);
#endif
            return r;
        }

        inline R256x1F __vectorcall Convert256x1DF(R256x1D_Arg0 v) noexcept
        {
            R256x1F r = {};
#if defined(SIMD_SSE42)
            __m128 low = _mm_cvtpd_ps(v.v1);
            __m128 high = _mm_cvtpd_ps(v.v2);
            r.v1 = _mm_movelh_ps(low, high);
            r.v2 = _mm_setzero_ps();
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            __m128 low = _mm256_cvtpd_ps(v.v1);
            r.v1 = _mm256_insertf128_ps(_mm256_setzero_ps(), low, 0);
#elif defined(SIMD_ARM_NEON)
            R128F_Native low = inner_simd::ConvertNativeDF(v.v1);
            R128F_Native high = inner_simd::ConvertNativeDF(v.v2);
            r.v1 = vcombine_f32(vget_low_f32(low), vget_low_f32(high));
            r.v2 = vdupq_n_f32(0.0f);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 4; ++i) r.v[i] = static_cast<float>(v.v[i]);
            for (int i = 4; i < 8; ++i) r.v[i] = 0.0f;
#endif
            return r;
        }

        inline R256x1I __vectorcall Convert256x1DI(R256x1D_Arg0 v) noexcept
        {
            R256x1I r = {};
#if defined(SIMD_SSE42)
            __m128i low = _mm_cvttpd_epi32(v.v1);
            __m128i high = _mm_cvttpd_epi32(v.v2);
            r.v1 = _mm_unpacklo_epi64(low, high);
            r.v2 = _mm_setzero_si128();
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            __m128i low = _mm256_cvttpd_epi32(v.v1);
            __m256 packed = _mm256_insertf128_ps(_mm256_setzero_ps(), _mm_castsi128_ps(low), 0);
            r.v1 = _mm256_castps_si256(packed);
#elif defined(SIMD_ARM_NEON)
            R128I_Native low = inner_simd::ConvertNativeDI(v.v1);
            R128I_Native high = inner_simd::ConvertNativeDI(v.v2);
            r.v1 = vcombine_s32(vget_low_s32(low), vget_low_s32(high));
            r.v2 = vdupq_n_s32(0);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 4; ++i) r.v[i] = static_cast<int>(v.v[i]);
            for (int i = 4; i < 8; ++i) r.v[i] = 0;
#endif
            return r;
        }

        inline R256x2I __vectorcall Convert256x2FI(R256x2F_Arg0 v) noexcept
        {
            R256x2I r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_cvttps_epi32(v.v1);
            r.v2 = _mm_cvttps_epi32(v.v2);
            r.v3 = _mm_cvttps_epi32(v.v3);
            r.v4 = _mm_cvttps_epi32(v.v4);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_cvttps_epi32(v.v1);
            r.v2 = _mm256_cvttps_epi32(v.v2);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_s32_f32(v.v1);
            r.v2 = vcvtq_s32_f32(v.v2);
            r.v3 = vcvtq_s32_f32(v.v3);
            r.v4 = vcvtq_s32_f32(v.v4);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 16; ++i) r.v[i] = static_cast<int>(v.v[i]);
#endif
            return r;
        }

        inline R256x2D __vectorcall Convert256x2FD(R256x2F_Arg0 v) noexcept
        {
            R256x2D r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_cvtps_pd(v.v1);
            r.v2 = _mm_cvtps_pd(_mm_movehl_ps(v.v1, v.v1));
            r.v3 = _mm_cvtps_pd(v.v3);
            r.v4 = _mm_cvtps_pd(_mm_movehl_ps(v.v3, v.v3));
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_cvtps_pd(_mm256_castps256_ps128(v.v1));
            r.v2 = _mm256_cvtps_pd(_mm256_castps256_ps128(v.v2));
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeFD(v.v1);
            r.v2 = inner_simd::ConvertNativeFD(vextq_f32(v.v1, v.v1, 2));
            r.v3 = inner_simd::ConvertNativeFD(v.v3);
            r.v4 = inner_simd::ConvertNativeFD(vextq_f32(v.v3, v.v3, 2));
#elif defined(SIMD_NONE)
            for (int block = 0; block < 2; ++block) for (int i = 0; i < 4; ++i) r.v[block * 4 + i] = static_cast<double>(v.v[block * 8 + i]);
#endif
            return r;
        }

        inline R256x2F __vectorcall Convert256x2IF(R256x2I_Arg0 v) noexcept
        {
            R256x2F r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_cvtepi32_ps(v.v1);
            r.v2 = _mm_cvtepi32_ps(v.v2);
            r.v3 = _mm_cvtepi32_ps(v.v3);
            r.v4 = _mm_cvtepi32_ps(v.v4);
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_cvtepi32_ps(v.v1);
            r.v2 = _mm256_cvtepi32_ps(v.v2);
#elif defined(SIMD_ARM_NEON)
            r.v1 = vcvtq_f32_s32(v.v1);
            r.v2 = vcvtq_f32_s32(v.v2);
            r.v3 = vcvtq_f32_s32(v.v3);
            r.v4 = vcvtq_f32_s32(v.v4);
#elif defined(SIMD_NONE)
            for (int i = 0; i < 16; ++i) r.v[i] = static_cast<float>(v.v[i]);
#endif
            return r;
        }

        inline R256x2D __vectorcall Convert256x2ID(R256x2I_Arg0 v) noexcept
        {
            R256x2D r = {};
#if defined(SIMD_SSE42)
            r.v1 = _mm_cvtepi32_pd(v.v1);
            r.v2 = _mm_cvtepi32_pd(_mm_srli_si128(v.v1, 8));
            r.v3 = _mm_cvtepi32_pd(v.v3);
            r.v4 = _mm_cvtepi32_pd(_mm_srli_si128(v.v3, 8));
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            r.v1 = _mm256_cvtepi32_pd(_mm256_castsi256_si128(v.v1));
            r.v2 = _mm256_cvtepi32_pd(_mm256_castsi256_si128(v.v2));
#elif defined(SIMD_ARM_NEON)
            r.v1 = inner_simd::ConvertNativeID(v.v1);
            r.v2 = inner_simd::ConvertNativeID(vextq_s32(v.v1, v.v1, 2));
            r.v3 = inner_simd::ConvertNativeID(v.v3);
            r.v4 = inner_simd::ConvertNativeID(vextq_s32(v.v3, v.v3, 2));
#elif defined(SIMD_NONE)
            for (int block = 0; block < 2; ++block) for (int i = 0; i < 4; ++i) r.v[block * 4 + i] = static_cast<double>(v.v[block * 8 + i]);
#endif
            return r;
        }

        inline R256x2F __vectorcall Convert256x2DF(R256x2D_Arg0 v) noexcept
        {
            R256x2F r = {};
#if defined(SIMD_SSE42)
            __m128 low1 = _mm_cvtpd_ps(v.v1);
            __m128 high1 = _mm_cvtpd_ps(v.v2);
            __m128 low2 = _mm_cvtpd_ps(v.v3);
            __m128 high2 = _mm_cvtpd_ps(v.v4);
            r.v1 = _mm_movelh_ps(low1, high1);
            r.v2 = _mm_setzero_ps();
            r.v3 = _mm_movelh_ps(low2, high2);
            r.v4 = _mm_setzero_ps();
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            __m128 low1 = _mm256_cvtpd_ps(v.v1);
            __m128 low2 = _mm256_cvtpd_ps(v.v2);
            r.v1 = _mm256_insertf128_ps(_mm256_setzero_ps(), low1, 0);
            r.v2 = _mm256_insertf128_ps(_mm256_setzero_ps(), low2, 0);
#elif defined(SIMD_ARM_NEON)
            R128F_Native low1 = inner_simd::ConvertNativeDF(v.v1);
            R128F_Native high1 = inner_simd::ConvertNativeDF(v.v2);
            R128F_Native low2 = inner_simd::ConvertNativeDF(v.v3);
            R128F_Native high2 = inner_simd::ConvertNativeDF(v.v4);
            r.v1 = vcombine_f32(vget_low_f32(low1), vget_low_f32(high1));
            r.v2 = vdupq_n_f32(0.0f);
            r.v3 = vcombine_f32(vget_low_f32(low2), vget_low_f32(high2));
            r.v4 = vdupq_n_f32(0.0f);
#elif defined(SIMD_NONE)
            for (int block = 0; block < 2; ++block) { for (int i = 0; i < 4; ++i) r.v[block * 8 + i] = static_cast<float>(v.v[block * 4 + i]); for (int i = 4; i < 8; ++i) r.v[block * 8 + i] = 0.0f; }
#endif
            return r;
        }

        inline R256x2I __vectorcall Convert256x2DI(R256x2D_Arg0 v) noexcept
        {
            R256x2I r = {};
#if defined(SIMD_SSE42)
            __m128i low1 = _mm_cvttpd_epi32(v.v1);
            __m128i high1 = _mm_cvttpd_epi32(v.v2);
            __m128i low2 = _mm_cvttpd_epi32(v.v3);
            __m128i high2 = _mm_cvttpd_epi32(v.v4);
            r.v1 = _mm_unpacklo_epi64(low1, high1);
            r.v2 = _mm_setzero_si128();
            r.v3 = _mm_unpacklo_epi64(low2, high2);
            r.v4 = _mm_setzero_si128();
#elif defined(SIMD_AVX) || defined(SIMD_AVX2)
            __m128i low1 = _mm256_cvttpd_epi32(v.v1);
            __m128i low2 = _mm256_cvttpd_epi32(v.v2);
            __m256 packed1 = _mm256_insertf128_ps(_mm256_setzero_ps(), _mm_castsi128_ps(low1), 0);
            __m256 packed2 = _mm256_insertf128_ps(_mm256_setzero_ps(), _mm_castsi128_ps(low2), 0);
            r.v1 = _mm256_castps_si256(packed1);
            r.v2 = _mm256_castps_si256(packed2);
#elif defined(SIMD_ARM_NEON)
            R128I_Native low1 = inner_simd::ConvertNativeDI(v.v1);
            R128I_Native high1 = inner_simd::ConvertNativeDI(v.v2);
            R128I_Native low2 = inner_simd::ConvertNativeDI(v.v3);
            R128I_Native high2 = inner_simd::ConvertNativeDI(v.v4);
            r.v1 = vcombine_s32(vget_low_s32(low1), vget_low_s32(high1));
            r.v2 = vdupq_n_s32(0);
            r.v3 = vcombine_s32(vget_low_s32(low2), vget_low_s32(high2));
            r.v4 = vdupq_n_s32(0);
#elif defined(SIMD_NONE)
            for (int block = 0; block < 2; ++block) { for (int i = 0; i < 4; ++i) r.v[block * 8 + i] = static_cast<int>(v.v[block * 4 + i]); for (int i = 4; i < 8; ++i) r.v[block * 8 + i] = 0; }
#endif
            return r;
        }
    }
}
