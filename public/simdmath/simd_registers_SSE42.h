#pragma once

#include "declaration.h"

namespace krystallic
{
	namespace SIMDMath
	{
#if defined(SIMD_SSE42) || defined(SIMD_AVX)
        struct alignas(16) R128x1F
        {
            union
            {
                struct
                {
                    __m128 v1;
                };
                __m128 v[1];
            };
        };


        struct alignas(16) R128x2F
        {
            union
            {
                struct
                {
                    __m128 v1, v2;
                };
                __m128 v[2];
            };
        };


        struct alignas(16) R128x3F
        {
            union
            {
                struct
                {
                    __m128 v1, v2, v3;
                };
                __m128 v[3];
            };
        };


        struct alignas(16) R128x4F
        {
            union
            {
                struct
                {
                    __m128 v1, v2, v3, v4;
                };
                __m128 v[4];
            };
        };


        struct alignas(16) R128x1I
        {
            union
            {
                struct
                {
                    __m128i v1;
                };
                __m128i v[1];
            };
        };


        struct alignas(16) R128x2I
        {
            union
            {
                struct
                {
                    __m128i v1, v2;
                };
                __m128i v[2];
            };
        };


        struct alignas(16) R128x3I
        {
            union
            {
                struct
                {
                    __m128i v1, v2, v3;
                };
                __m128i v[3];
            };
        };


        struct alignas(16) R128x4I
        {
            union
            {
                struct
                {
                    __m128i v1, v2, v3, v4;
                };
                __m128i v[4];
            };
        };


        struct alignas(16) R128x1D
        {
            union
            {
                struct
                {
                    __m128d v1;
                };
                __m128d v[1];
            };
        };


        struct alignas(16) R128x2D
        {
            union
            {
                struct
                {
                    __m128d v1, v2;
                };
                __m128d v[2];
            };
        };


        struct alignas(16) R128x3D
        {
            union
            {
                struct
                {
                    __m128d v1, v2, v3;
                };
                __m128d v[3];
            };
        };


        struct alignas(16) R128x4D
        {
            union
            {
                struct
                {
                    __m128d v1, v2, v3, v4;
                };
                __m128d v[4];
            };
        };

#if !(defined(SIMD_AVX) || defined(SIMD_AVX2))

        using R256x1F = R128x2F;
        using R256x2F = R128x4F;
        using R256x1I = R128x2I;
        using R256x2I = R128x4I;
        using R256x1D = R128x2D;
        using R256x2D = R128x4D;

#endif

#endif
	}
}