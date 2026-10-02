#pragma once

#include "declaration.h"

namespace krystallic
{
	namespace SIMDMath
	{

#if defined(SIMD_ARM_NEON)

#if defined(KRYSTALLIC_ARCH_ARMv8) || defined(KRYSTALLIC_ARCH_ARM64)

    #define neon_vfmaq_f32(a, b, c) vfmaq_f32((a), (b), (c))
    #define neon_vfmsq_f32(a, b, c) vfmsq_f32((a), (b), (c))
    #define neon_vdivq_f32(a, b)    vdivq_f32((a), (b))
    #define neon_vsqrtq_f32(a)      vsqrtq_f32((a))
    #define neon_vrndnq_f32(a)      vrndnq_f32((a))

#else

    #define neon_vfmaq_f32(a, b, c) vaddq_f32((a), vmulq_f32((b), (c)))
    #define neon_vfmsq_f32(a, b, c) vsubq_f32((a), vmulq_f32((b), (c)))

    inline float32x4_t neon_vdivq_f32(float32x4_t a, float32x4_t b)
    {
        float32x4_t r = vrecpeq_f32(b);
        r = vmulq_f32(vrecpsq_f32(b, r), r);
        r = vmulq_f32(vrecpsq_f32(b, r), r);
        return vmulq_f32(a, r);
    }

    inline float32x4_t neon_vsqrtq_f32(float32x4_t v)
    {
        return 
		{
            sqrtf(vgetq_lane_f32(v, 0)),
            sqrtf(vgetq_lane_f32(v, 1)),
            sqrtf(vgetq_lane_f32(v, 2)),
            sqrtf(vgetq_lane_f32(v, 3))
        };
    }

    inline float32x4_t neon_vrndnq_f32(float32x4_t v)
    {
        return 
		{
            nearbyintf(vgetq_lane_f32(v, 0)),
            nearbyintf(vgetq_lane_f32(v, 1)),
            nearbyintf(vgetq_lane_f32(v, 2)),
            nearbyintf(vgetq_lane_f32(v, 3))
        };
    }

#endif

    inline int32x4_t GetLeadingBit(const int32x4_t value) noexcept
        {
            static const vec4i g_0000FFFF = _set_vec4i(0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF);
            static const vec4i g_000000FF = _set_vec4i(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF);
            static const vec4i g_0000000F = _set_vec4i(0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F);
            static const vec4i g_00000003 = _set_vec4i(0x00000003, 0x00000003, 0x00000003, 0x00000003);

            uint32x4_t c = vcgtq_s32(value, g_0000FFFF);              // c = (v > 0xFFFF)
            int32x4_t  b = vshrq_n_s32(vreinterpretq_s32_u32(c), 31); // b = (c ? 1 : 0)
            int32x4_t  r = vshlq_n_s32(b, 4);                         // r = (b << 4)
            r = vnegq_s32(r);
            int32x4_t v = vshlq_s32(value, r); // v = (v >> r)

            c = vcgtq_s32(v, g_000000FF);                  // c = (v > 0xFF)
            b = vshrq_n_s32(vreinterpretq_s32_u32(c), 31); // b = (c ? 1 : 0)
            int32x4_t s = vshlq_n_s32(b, 3);               // s = (b << 3)
            s = vnegq_s32(s);
            v = vshlq_s32(v, s); // v = (v >> s)
            r = vorrq_s32(r, s); // r = (r | s)

            c = vcgtq_s32(v, g_0000000F);                  // c = (v > 0xF)
            b = vshrq_n_s32(vreinterpretq_s32_u32(c), 31); // b = (c ? 1 : 0)
            s = vshlq_n_s32(b, 2);                         // s = (b << 2)
            s = vnegq_s32(s);
            v = vshlq_s32(v, s); // v = (v >> s)
            r = vorrq_s32(r, s); // r = (r | s)

            c = vcgtq_s32(v, g_00000003);                  // c = (v > 0x3)
            b = vshrq_n_s32(vreinterpretq_s32_u32(c), 31); // b = (c ? 1 : 0)
            s = vshlq_n_s32(b, 1);                         // s = (b << 1)
            s = vnegq_s32(s);
            v = vshlq_s32(v, s); // v = (v >> s)
            r = vorrq_s32(r, s); // r = (r | s)

            s = vshrq_n_s32(v, 1);
            r = vorrq_s32(r, s);
            return r;
        }

#endif
	}
}