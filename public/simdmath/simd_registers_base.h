#pragma once

#include "declaration.h"

namespace krystallic
{
    namespace SIMDMath
    {
#if defined(SIMD_NONE)
        struct alignas(16) R128x1F
        {
            union
            {
                struct
                {
                    float v1, v2, v3, v4;
                };
                float v[4];
            };
        };


        struct alignas(16) R128x2F
        {
            union
            {
                struct
                {
                    float v1, v2, v3, v4, v5, v6, v7, v8;
                };
                int v[8];
            };
        };


        struct alignas(16) R128x3F
        {
            union
            {
                struct
                {
                    float v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12;
                };
                int v[12];
            };
        };


        struct alignas(16) R128x4F
        {
            union
            {
                struct
                {
                    float v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16;
                };
                float v[16];
            };
        };


        struct alignas(16) R128x1I
        {
            union
            {
                struct
                {
                    int v1, v2, v3, v4;
                };
                int v[4];
            };
        };


        struct alignas(16) R128x2I
        {
            union
            {
                struct
                {
                    int v1, v2, v3, v4, v5, v6, v7, v8;
                };
                int v[8];
            };
        };


        struct alignas(16) R128x3I
        {
            union
            {
                struct
                {
                    int v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12;
                };
                int v[12];
            };
        };


        struct alignas(16) R128x4I
        {
            union
            {
                struct
                {
                    int v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16;
                };
                int v[16];
            };
        };


        struct alignas(16) R128x1D
        {
            union
            {
                struct
                {
                    double v1, v2;
                };
                double v[2];
            };
        };


        struct alignas(16) R128x2D
        {
            union
            {
                struct
                {
                    double v1, v2, v3, v4;
                };
                double v[4];
            };
        };


        struct alignas(16) R128x3D
        {
            union
            {
                struct
                {
                    double v1, v2, v3, v4, v5, v6;
                };
                double v[6];
            };
        };


        struct alignas(16) R128x4D
        {
            union
            {
                struct
                {
                    double v1, v2, v3, v4, v5, v6, v7, v8;
                };
                double v[8];
            };
        };


        using R256x1F = R128x2F;
        using R256x2F = R128x4F;
        using R256x1I = R128x2I;
        using R256x2I = R128x4I;
        using R256x1D = R128x2D;
        using R256x2D = R128x4D;
#endif
    }
}
