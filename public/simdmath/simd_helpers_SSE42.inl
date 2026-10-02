#pragma once

#include "declaration.h"


namespace krystallic
{
	namespace SIMDMath
	{
#if defined(SIMD_SSE42)
		inline __m128i VecMultiSrlEpi32(__m128i value, __m128i count) noexcept
        {
            __m128i v = _mm_shuffle_epi32(value, _MM_SHUFFLE(0, 0, 0, 0));
            __m128i c = _mm_shuffle_epi32(count, _MM_SHUFFLE(0, 0, 0, 0));
            c = _mm_and_si128(c, _mm_setr_epi32(0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000));
            __m128i r0 = _mm_srl_epi32(v, c);

            v = _mm_shuffle_epi32(value, _MM_SHUFFLE(1, 1, 1, 1));
            c = _mm_shuffle_epi32(count, _MM_SHUFFLE(1, 1, 1, 1));
            c = _mm_and_si128(c, _mm_setr_epi32(0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000));
            __m128i r1 = _mm_srl_epi32(v, c);

            v = _mm_shuffle_epi32(value, _MM_SHUFFLE(2, 2, 2, 2));
            c = _mm_shuffle_epi32(count, _MM_SHUFFLE(2, 2, 2, 2));
            c = _mm_and_si128(c, _mm_setr_epi32(0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000));
            __m128i r2 = _mm_srl_epi32(v, c);

            v = _mm_shuffle_epi32(value, _MM_SHUFFLE(3, 3, 3, 3));
            c = _mm_shuffle_epi32(count, _MM_SHUFFLE(3, 3, 3, 3));
            c = _mm_and_si128(c, _mm_setr_epi32(0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000));
            __m128i r3 = _mm_srl_epi32(v, c);

            // (r0,r0,r1,r1)
            __m128 r01 = _mm_shuffle_ps(_mm_castsi128_ps(r0), _mm_castsi128_ps(r1), _MM_SHUFFLE(0, 0, 0, 0));
            // (r2,r2,r3,r3)
            __m128 r23 = _mm_shuffle_ps(_mm_castsi128_ps(r2), _mm_castsi128_ps(r3), _MM_SHUFFLE(0, 0, 0, 0));
            // (r0,r1,r2,r3)
            __m128 result = _mm_shuffle_ps(r01, r23, _MM_SHUFFLE(2, 0, 2, 0));
            return _mm_castps_si128(result);
        }

        inline __m128i __vectorcall VecGetLeadingBit(const __m128i value) noexcept
        {
            static const __m128i g_0000FFFF = _mm_castps_si128(_mm_setr_ps(0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF));
            static const __m128i g_000000FF = _mm_castps_si128(_mm_setr_ps(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF));
            static const __m128i g_0000000F = _mm_castps_si128(_mm_setr_ps(0x0000000F, 0x0000000F, 0x0000000F, 0x0000000F));
            static const __m128i g_00000003 = _mm_castps_si128(_mm_setr_ps(0x00000003, 0x00000003, 0x00000003, 0x00000003));

            __m128i v = value, r, c, b, s;

            c = _mm_cmpgt_epi32(v, g_0000FFFF); // c = (v > 0xFFFF)
            b = _mm_srli_epi32(c, 31);          // b = (c ? 1 : 0)
            r = _mm_slli_epi32(b, 4);           // r = (b << 4)
            v = VecMultiSrlEpi32(v, r);         // v = (v >> r)

            c = _mm_cmpgt_epi32(v, g_000000FF); // c = (v > 0xFF)
            b = _mm_srli_epi32(c, 31);          // b = (c ? 1 : 0)
            s = _mm_slli_epi32(b, 3);           // s = (b << 3)
            v = VecMultiSrlEpi32(v, s);         // v = (v >> s)
            r = _mm_or_si128(r, s);             // r = (r | s)

            c = _mm_cmpgt_epi32(v, g_0000000F); // c = (v > 0xF)
            b = _mm_srli_epi32(c, 31);          // b = (c ? 1 : 0)
            s = _mm_slli_epi32(b, 2);           // s = (b << 2)
            v = VecMultiSrlEpi32(v, s);         // v = (v >> s)
            r = _mm_or_si128(r, s);             // r = (r | s)

            c = _mm_cmpgt_epi32(v, g_00000003); // c = (v > 0x3)
            b = _mm_srli_epi32(c, 31);          // b = (c ? 1 : 0)
            s = _mm_slli_epi32(b, 1);           // s = (b << 1)
            v = VecMultiSrlEpi32(v, s);         // v = (v >> s)
            r = _mm_or_si128(r, s);             // r = (r | s)

            s = _mm_srli_epi32(v, 1);
            r = _mm_or_si128(r, s);
            return r;
        }

        inline __m128 SseFMAdd(__m128 a, __m128 b, __m128 c) noexcept
        {
#if defined(__FMA__)
            return _mm_fmadd_ps(a, b, c);
#else
            return _mm_add_ps(_mm_mul_ps(a, b), c);
#endif
        }

        inline __m128 SseFNMAdd(__m128 a, __m128 b, __m128 c) noexcept
        {
#if defined(__FMA__)
            return _mm_fnmadd_ps(a, b, c);
#else
            return _mm_sub_ps(c, _mm_mul_ps(a, b));
#endif
        }

#endif
	}
}
