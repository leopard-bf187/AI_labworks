#pragma once

#include "declaration.h"

namespace krystallic
{
    namespace SIMDMath
    {
#if defined(SIMD_ARM_NEON)

        using R128F_Native = float32x4_t;
        using R128I_Native = int32x4_t;

#if defined(KRYSTALLIC_ARCH_ARM64) || defined(KRYSTALLIC_ARCH_ARMv8)
        using R128D_Native = float64x2_t;
#else
        struct alignas(16) R128D_Native
        {
            double lo;
            double hi;
        };
#endif

        struct alignas(16) R128x1F
        {
            union
            {
                struct { R128F_Native v1; };
                R128F_Native v[1];
            };
        };

        struct alignas(16) R128x2F
        {
            union
            {
                struct { R128F_Native v1, v2; };
                R128F_Native v[2];
            };
        };

        struct alignas(16) R128x3F
        {
            union
            {
                struct { R128F_Native v1, v2, v3; };
                R128F_Native v[3];
            };
        };

        struct alignas(16) R128x4F
        {
            union
            {
                struct { R128F_Native v1, v2, v3, v4; };
                R128F_Native v[4];
            };
        };

        struct alignas(16) R128x1I
        {
            union
            {
                struct { R128I_Native v1; };
                R128I_Native v[1];
            };
        };

        struct alignas(16) R128x2I
        {
            union
            {
                struct { R128I_Native v1, v2; };
                R128I_Native v[2];
            };
        };

        struct alignas(16) R128x3I
        {
            union
            {
                struct { R128I_Native v1, v2, v3; };
                R128I_Native v[3];
            };
        };

        struct alignas(16) R128x4I
        {
            union
            {
                struct { R128I_Native v1, v2, v3, v4; };
                R128I_Native v[4];
            };
        };

        struct alignas(16) R128x1D
        {
            union
            {
                struct { R128D_Native v1; };
                R128D_Native v[1];
            };
        };

        struct alignas(16) R128x2D
        {
            union
            {
                struct { R128D_Native v1, v2; };
                R128D_Native v[2];
            };
        };

        struct alignas(16) R128x3D
        {
            union
            {
                struct { R128D_Native v1, v2, v3; };
                R128D_Native v[3];
            };
        };

        struct alignas(16) R128x4D
        {
            union
            {
                struct { R128D_Native v1, v2, v3, v4; };
                R128D_Native v[4];
            };
        };

        struct alignas(16) R256x1F
        {
            union
            {
                struct { R128F_Native v1, v2; };
                R128F_Native v[2];
            };
        };

        struct alignas(16) R256x2F
        {
            union
            {
                struct { R128F_Native v1, v2, v3, v4; };
                R128F_Native v[4];
            };
        };

        struct alignas(16) R256x1I
        {
            union
            {
                struct { R128I_Native v1, v2; };
                R128I_Native v[2];
            };
        };

        struct alignas(16) R256x2I
        {
            union
            {
                struct { R128I_Native v1, v2, v3, v4; };
                R128I_Native v[4];
            };
        };

        struct alignas(16) R256x1D
        {
            union
            {
                struct { R128D_Native v1, v2; };
                R128D_Native v[2];
            };
        };

        struct alignas(16) R256x2D
        {
            union
            {
                struct { R128D_Native v1, v2, v3, v4; };
                R128D_Native v[4];
            };
        };

#endif
    }
}