/*
*  Authors:  Leonid (@LeoParD) Parmacli  &&  Victor (@RisovoePole) Anisimov
*
*  Description:
*
*  Date: 05.01.2026
*/



#pragma once

#include <cstdint>
#include "declaration.h"

#include "simd_registers_SSE42.h"
#include "simd_registers_AVX_AVX2.h"
#include "simd_registers_NEON.h"

namespace krystallic
{
	namespace SIMDMath
	{

		constexpr float g_PI = 3.141592654f;
		constexpr float g_2PI = 6.283185307f;
		constexpr float g_1DIVPI = 0.318309886f;
		constexpr float g_1DIV2PI = 0.159154943f;
		constexpr float g_PIDIV2 = 1.570796327f;
		constexpr float g_PIDIV4 = 0.785398163f;

		constexpr uint32_t g_SELECT_0 = 0x00000000;
		constexpr uint32_t g_negZero = 0x80000000;
		constexpr uint32_t g_SELECT_1 = 0xFFFFFFFF;


		constexpr uint32_t g_PERMUTE_0X = 0;
		constexpr uint32_t g_PERMUTE_0Y = 1;
		constexpr uint32_t g_PERMUTE_0Z = 2;
		constexpr uint32_t g_PERMUTE_0W = 3;
		constexpr uint32_t g_PERMUTE_1X = 4;
		constexpr uint32_t g_PERMUTE_1Y = 5;
		constexpr uint32_t g_PERMUTE_1Z = 6;
		constexpr uint32_t g_PERMUTE_1W = 7;

		constexpr uint32_t g_SWIZZLE_X = 0;
		constexpr uint32_t g_SWIZZLE_Y = 1;
		constexpr uint32_t g_SWIZZLE_Z = 2;
		constexpr uint32_t g_SWIZZLE_W = 3;

		constexpr uint32_t g_CRMASK_CR6 = 0x000000F0;
		constexpr uint32_t g_CRMASK_CR6TRUE = 0x00000080;
		constexpr uint32_t g_CRMASK_CR6FALSE = 0x00000020;
		constexpr uint32_t g_CRMASK_CR6BOUNDS = g_CRMASK_CR6FALSE;

		inline const R128x1F g_SinCoefficients0 = { { _set_vec4f(-0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f) } };
		inline const R128x1F g_SinCoefficients1 = { { _set_vec4f(-2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/) } };
		inline const R128x1F g_CosCoefficients0 = { { _set_vec4f(-0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f) } };
		inline const R128x1F g_CosCoefficients1 = { { _set_vec4f(-2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/) } };
		inline const R128x1F g_TanCoefficients0 = { { _set_vec4f(1.0f, 0.333333333f, 0.133333333f, 5.396825397e-2f) } };
		inline const R128x1F g_TanCoefficients1 = { { _set_vec4f(2.186948854e-2f, 8.863235530e-3f, 3.592128167e-3f, 1.455834485e-3f) } };
		inline const R128x1F g_TanCoefficients2 = { { _set_vec4f(5.900274264e-4f, 2.391290764e-4f, 9.691537707e-5f, 3.927832950e-5f) } };
		inline const R128x1F g_ArcCoefficients0 = { { _set_vec4f(+1.5707963050f, -0.2145988016f, +0.0889789874f, -0.0501743046f) } };
		inline const R128x1F g_ArcCoefficients1 = { { _set_vec4f(+0.0308918810f, -0.0170881256f, +0.0066700901f, -0.0012624911f) } };
		inline const R128x1F g_ATanCoefficients0 = { { _set_vec4f(-0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f) } };
		inline const R128x1F g_ATanCoefficients1 = { { _set_vec4f(-0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f) } };
		inline const R128x1F g_ATanEstCoefficients0 = { { _set_vec4f(+0.999866f, +0.999866f, +0.999866f, +0.999866f) } };
		inline const R128x1F g_ATanEstCoefficients1 = { { _set_vec4f(-0.3302995f, +0.180141f, -0.085133f, +0.0208351f) } };
		inline const R128x1F g_TanEstCoefficients = { { _set_vec4f(2.484f, -1.954923183e-1f, 2.467401101f, g_1DIVPI) } };
		inline const R128x1F g_ArcEstCoefficients = { { _set_vec4f(+1.5707288f, -0.2121144f, +0.0742610f, -0.0187293f) } };
		inline const R128x1F g_PiConstants0 = { { _set_vec4f(g_PI, g_2PI, g_1DIVPI, g_1DIV2PI) } };
		inline const R128x1F g_IdentityR0 = { { _set_vec4f(1.0f, 0.0f, 0.0f, 0.0f) } };
		inline const R128x1F g_IdentityR1 = { { _set_vec4f(0.0f, 1.0f, 0.0f, 0.0f) } };
		inline const R128x1F g_IdentityR2 = { { _set_vec4f(0.0f, 0.0f, 1.0f, 0.0f) } };
		inline const R128x1F g_IdentityR3 = { { _set_vec4f(0.0f, 0.0f, 0.0f, 1.0f) } };
		inline const R128x1F g_NegIdentityR0 = { { _set_vec4f(-1.0f, 0.0f, 0.0f, 0.0f) } };
		inline const R128x1F g_NegIdentityR1 = { { _set_vec4f(0.0f, -1.0f, 0.0f, 0.0f) } };
		inline const R128x1F g_NegIdentityR2 = { { _set_vec4f(0.0f, 0.0f, -1.0f, 0.0f) } };
		inline const R128x1F g_NegIdentityR3 = { { _set_vec4f(0.0f, 0.0f, 0.0f, -1.0f) } };
		inline const R128x1I g_NegativeZero = {  { _set_vec4ui(0x80000000, 0x80000000, 0x80000000, 0x80000000) } };
		inline const R128x1I g_Negate3 = {  { _set_vec4ui(0x80000000, 0x80000000, 0x80000000, 0x00000000) } };
		inline const R128x1I g_MaskXY = {  { _set_vec4ui(0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000) } };
		inline const R128x1I g_Mask3 = {  { _set_vec4ui(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000) } };
		inline const R128x1I g_MaskX = {  { _set_vec4ui(0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000) } };
		inline const R128x1I g_MaskY = {  { _set_vec4ui(0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000) } };
		inline const R128x1I g_MaskZ = {  { _set_vec4ui(0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000) } };
		inline const R128x1I g_MaskW = {  { _set_vec4ui(0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF) } };
		inline const R128x1F g_One = { { _set_vec4f(1.0f, 1.0f, 1.0f, 1.0f) } };
		inline const R128x1F g_One3 = { { _set_vec4f(1.0f, 1.0f, 1.0f, 0.0f) } };
		inline const R128x1F g_Zero = { { _set_vec4f(0.0f, 0.0f, 0.0f, 0.0f) } };
		inline const R128x1F g_Two = { { _set_vec4f(2.f, 2.f, 2.f, 2.f) } };
		inline const R128x1F g_Four = { { _set_vec4f(4.f, 4.f, 4.f, 4.f) } };
		inline const R128x1F g_Six = { { _set_vec4f(6.f, 6.f, 6.f, 6.f) } };
		inline const R128x1F g_NegativeOne = { { _set_vec4f(-1.0f, -1.0f, -1.0f, -1.0f) } };
		inline const R128x1F g_OneHalf = { { _set_vec4f(0.5f, 0.5f, 0.5f, 0.5f) } };
		inline const R128x1F g_NegativeOneHalf = { { _set_vec4f(-0.5f, -0.5f, -0.5f, -0.5f) } };
		inline const R128x1F g_NegativeTwoPi = { { _set_vec4f(-g_2PI, -g_2PI, -g_2PI, -g_2PI) } };
		inline const R128x1F g_NegativePi = { { _set_vec4f(-g_PI, -g_PI, -g_PI, -g_PI) } };
		inline const R128x1F g_HalfPi = { { _set_vec4f(g_PIDIV2, g_PIDIV2, g_PIDIV2, g_PIDIV2) } };
		inline const R128x1F g_Pi = { { _set_vec4f(g_PI, g_PI, g_PI, g_PI) } };
		inline const R128x1F g_ReciprocalPi = { { _set_vec4f(g_1DIVPI, g_1DIVPI, g_1DIVPI, g_1DIVPI) } };
		inline const R128x1F g_TwoPi = { { _set_vec4f(g_2PI, g_2PI, g_2PI, g_2PI) } };
		inline const R128x1F g_ReciprocalTwoPi = { { _set_vec4f(g_1DIV2PI, g_1DIV2PI, g_1DIV2PI, g_1DIV2PI) } };
		inline const R128x1F g_Epsilon = { { _set_vec4f(1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f) } };
		inline const R128x1I g_Infinity = { { _set_vec4i(0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000) } };
		inline const R128x1I g_QNaN = { { _set_vec4i(0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000) } };
		inline const R128x1I g_QNaNTest = { { _set_vec4i(0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF) } };
		inline const R128x1I g_AbsMask = { { _set_vec4i(0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF) } };
		inline const R128x1I g_FltMin = { { _set_vec4i(0x00800000, 0x00800000, 0x00800000, 0x00800000) } };
		inline const R128x1I g_FltMax = { { _set_vec4i(0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF) } };
		inline const R128x1I g_NegOneMask = {  { _set_vec4ui(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF) } };
		inline const R128x1I g_MaskA8R8G8B8 = {  { _set_vec4ui(0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000) } };
		inline const R128x1I g_FlipA8R8G8B8 = {  { _set_vec4ui(0x00000000, 0x00000000, 0x00000000, 0x80000000) } };
		inline const R128x1F g_FixAA8R8G8B8 = { { _set_vec4f(0.0f, 0.0f, 0.0f, float(0x80000000U)) } };
		inline const R128x1F g_NormalizeA8R8G8B8 = { { _set_vec4f(1.0f / (255.0f * float(0x10000)), 1.0f / (255.0f * float(0x100)), 1.0f / 255.0f, 1.0f / (255.0f * float(0x1000000))) } };
		inline const R128x1I g_MaskA2B10G10R10 = {  { _set_vec4ui(0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000) } };
		inline const R128x1I g_FlipA2B10G10R10 = {  { _set_vec4ui(0x00000200, 0x00080000, 0x20000000, 0x80000000) } };
		inline const R128x1F g_FixAA2B10G10R10 = { { _set_vec4f(-512.0f, -512.0f * float(0x400), -512.0f * float(0x100000), float(0x80000000U)) } };
		inline const R128x1F g_NormalizeA2B10G10R10 = { { _set_vec4f(1.0f / 511.0f, 1.0f / (511.0f * float(0x400)), 1.0f / (511.0f * float(0x100000)), 1.0f / (3.0f * float(0x40000000))) } };
		inline const R128x1I g_MaskX16Y16 = {  { _set_vec4ui(0x0000FFFF, 0xFFFF0000, 0x00000000, 0x00000000) } };
		inline const R128x1I g_FlipX16Y16 = { { _set_vec4i(0x00008000, 0x00000000, 0x00000000, 0x00000000) } };
		inline const R128x1F g_FixX16Y16 = { { _set_vec4f(-32768.0f, 0.0f, 0.0f, 0.0f) } };
		inline const R128x1F g_NormalizeX16Y16 = { { _set_vec4f(1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 0.0f, 0.0f) } };
		inline const R128x1I g_MaskX16Y16Z16W16 = {  { _set_vec4ui(0x0000FFFF, 0x0000FFFF, 0xFFFF0000, 0xFFFF0000) } };
		inline const R128x1I g_FlipX16Y16Z16W16 = { { _set_vec4i(0x00008000, 0x00008000, 0x00000000, 0x00000000) } };
		inline const R128x1F g_FixX16Y16Z16W16 = { { _set_vec4f(-32768.0f, -32768.0f, 0.0f, 0.0f) } };
		inline const R128x1F g_NormalizeX16Y16Z16W16 = { { _set_vec4f(1.0f / 32767.0f, 1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 1.0f / (32767.0f * 65536.0f)) } };
		inline const R128x1F g_NoFraction = { { _set_vec4f(8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f) } };
		inline const R128x1I g_MaskByte = { { _set_vec4i(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF) } };
		inline const R128x1F g_NegateX = { { _set_vec4f(-1.0f, 1.0f, 1.0f, 1.0f) } };
		inline const R128x1F g_NegateY = { { _set_vec4f(1.0f, -1.0f, 1.0f, 1.0f) } };
		inline const R128x1F g_NegateZ = { { _set_vec4f(1.0f, 1.0f, -1.0f, 1.0f) } };
		inline const R128x1F g_NegateW = { { _set_vec4f(1.0f, 1.0f, 1.0f, -1.0f) } };
		inline const R128x1I g_Select0101 = {  { _set_vec4ui(g_SELECT_0, g_SELECT_1, g_SELECT_0, g_SELECT_1) } };
		inline const R128x1I g_Select1010 = {  { _set_vec4ui(g_SELECT_1, g_SELECT_0, g_SELECT_1, g_SELECT_0) } };
		inline const R128x1I g_OneHalfMinusEpsilon = { { _set_vec4i(0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD) } };
		inline const R128x1I g_Select1000 = {  { _set_vec4ui(g_SELECT_1, g_SELECT_0, g_SELECT_0, g_SELECT_0) } };
		inline const R128x1I g_Select1100 = {  { _set_vec4ui(g_SELECT_1, g_SELECT_1, g_SELECT_0, g_SELECT_0) } };
		inline const R128x1I g_Select1110 = {  { _set_vec4ui(g_SELECT_1, g_SELECT_1, g_SELECT_1, g_SELECT_0) } };
		inline const R128x1I g_Select1011 = {  { _set_vec4ui(g_SELECT_1, g_SELECT_0, g_SELECT_1, g_SELECT_1) } };
		inline const R128x1F g_FixupY16 = { { _set_vec4f(1.0f, 1.0f / 65536.0f, 0.0f, 0.0f) } };
		inline const R128x1F g_FixupY16W16 = { { _set_vec4f(1.0f, 1.0f, 1.0f / 65536.0f, 1.0f / 65536.0f) } };
		inline const R128x1I g_FlipX = {  { _set_vec4ui(0x80000000, 0, 0, 0) } };
		inline const R128x1I g_FlipY = {  { _set_vec4ui(0, 0x80000000, 0, 0) } };
		inline const R128x1I g_FlipZ = {  { _set_vec4ui(0, 0, 0x80000000, 0) } };
		inline const R128x1I g_FlipW = {  { _set_vec4ui(0, 0, 0, 0x80000000) } };
		inline const R128x1I g_FlipYZ = {  { _set_vec4ui(0, 0x80000000, 0x80000000, 0) } };
		inline const R128x1I g_FlipZW = {  { _set_vec4ui(0, 0, 0x80000000, 0x80000000) } };
		inline const R128x1I g_FlipYW = {  { _set_vec4ui(0, 0x80000000, 0, 0x80000000) } };
		inline const R128x1I g_FlipXZ = {  { _set_vec4ui(0x80000000, 0, 0x80000000, 0) } };
		inline const R128x1I g_FlipXY = {  { _set_vec4ui(0x80000000, 0x80000000, 0, 0) } };
		inline const R128x1I g_MaskDec4 = { { _set_vec4i(0x3FF, 0x3FF << 10, 0x3FF << 20, static_cast<int>(0xC0000000)) } };
		inline const R128x1I g_XorDec4 = { { _set_vec4i(0x200, 0x200 << 10, 0x200 << 20, 0) } };
		inline const R128x1F g_AddUDec4 = { { _set_vec4f(0, 0, 0, 32768.0f * 65536.0f) } };
		inline const R128x1F g_AddDec4 = { { _set_vec4f(-512.0f, -512.0f * 1024.0f, -512.0f * 1024.0f * 1024.0f, 0) } };
		inline const R128x1F g_MulDec4 = { { _set_vec4f(1.0f, 1.0f / 1024.0f, 1.0f / (1024.0f * 1024.0f), 1.0f / (1024.0f * 1024.0f * 1024.0f)) } };
		inline const R128x1I g_MaskByte4 = {  { _set_vec4ui(0xFF, 0xFF00, 0xFF0000, 0xFF000000) } };
		inline const R128x1I g_XorByte4 = { { _set_vec4i(0x80, 0x8000, 0x800000, 0x00000000) } };
		inline const R128x1F g_AddByte4 = { { _set_vec4f(-128.0f, -128.0f * 256.0f, -128.0f * 65536.0f, 0) } };
		inline const R128x1F g_FixUnsigned = { { _set_vec4f(32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f) } };
		inline const R128x1F g_MaxInt = { { _set_vec4f(65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f) } };
		inline const R128x1F g_MaxUInt = { { _set_vec4f(65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f) } };
		inline const R128x1F g_UnsignedFix = { { _set_vec4f(32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f) } };
		inline const R128x1F g_srgbScale = { { _set_vec4f(12.92f, 12.92f, 12.92f, 1.0f) } };
		inline const R128x1F g_srgbA = { { _set_vec4f(0.055f, 0.055f, 0.055f, 0.0f) } };
		inline const R128x1F g_srgbA1 = { { _set_vec4f(1.055f, 1.055f, 1.055f, 1.0f) } };
		inline const R128x1I g_ExponentBias = { { _set_vec4i(127, 127, 127, 127) } };
		inline const R128x1I g_SubnormalExponent = { { _set_vec4i(-126, -126, -126, -126) } };
		inline const R128x1I g_NumTrailing = { { _set_vec4i(23, 23, 23, 23) } };
		inline const R128x1I g_MinNormal = { { _set_vec4i(0x00800000, 0x00800000, 0x00800000, 0x00800000) } };
		inline const R128x1I g_NegInfinity = {  { _set_vec4ui(0xFF800000, 0xFF800000, 0xFF800000, 0xFF800000) } };
		inline const R128x1I g_NegQNaN = {  { _set_vec4ui(0xFFC00000, 0xFFC00000, 0xFFC00000, 0xFFC00000) } };
		inline const R128x1I g_Bin128 = { { _set_vec4i(0x43000000, 0x43000000, 0x43000000, 0x43000000) } };
		inline const R128x1I g_BinNeg150 = {  { _set_vec4ui(0xC3160000, 0xC3160000, 0xC3160000, 0xC3160000) } };
		inline const R128x1I g_253 = { { _set_vec4i(253, 253, 253, 253) } };
		inline const R128x1F g_ExpEst1 = { { _set_vec4f(-6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f, -6.93147182e-1f) } };
		inline const R128x1F g_ExpEst2 = { { _set_vec4f(+2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f, +2.40226462e-1f) } };
		inline const R128x1F g_ExpEst3 = { { _set_vec4f(-5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f, -5.55036440e-2f) } };
		inline const R128x1F g_ExpEst4 = { { _set_vec4f(+9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f, +9.61597636e-3f) } };
		inline const R128x1F g_ExpEst5 = { { _set_vec4f(-1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f, -1.32823968e-3f) } };
		inline const R128x1F g_ExpEst6 = { { _set_vec4f(+1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f, +1.47491097e-4f) } };
		inline const R128x1F g_ExpEst7 = { { _set_vec4f(-1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f, -1.08635004e-5f) } };
		inline const R128x1F g_LogEst0 = { { _set_vec4f(+1.442693f, +1.442693f, +1.442693f, +1.442693f) } };
		inline const R128x1F g_LogEst1 = { { _set_vec4f(-0.721242f, -0.721242f, -0.721242f, -0.721242f) } };
		inline const R128x1F g_LogEst2 = { { _set_vec4f(+0.479384f, +0.479384f, +0.479384f, +0.479384f) } };
		inline const R128x1F g_LogEst3 = { { _set_vec4f(-0.350295f, -0.350295f, -0.350295f, -0.350295f) } };
		inline const R128x1F g_LogEst4 = { { _set_vec4f(+0.248590f, +0.248590f, +0.248590f, +0.248590f) } };
		inline const R128x1F g_LogEst5 = { { _set_vec4f(-0.145700f, -0.145700f, -0.145700f, -0.145700f) } };
		inline const R128x1F g_LogEst6 = { { _set_vec4f(+0.057148f, +0.057148f, +0.057148f, +0.057148f) } };
		inline const R128x1F g_LogEst7 = { { _set_vec4f(-0.010578f, -0.010578f, -0.010578f, -0.010578f) } };
		inline const R128x1F g_LgE = { { _set_vec4f(+1.442695f, +1.442695f, +1.442695f, +1.442695f) } };
		inline const R128x1F g_InvLgE = { { _set_vec4f(+6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f, +6.93147182e-1f) } };
		inline const R128x1F g_Lg10 = { { _set_vec4f(+3.321928f, +3.321928f, +3.321928f, +3.321928f) } };
		inline const R128x1F g_InvLg10 = { { _set_vec4f(+3.010299956e-1f, +3.010299956e-1f, +3.010299956e-1f, +3.010299956e-1f) } };
		inline const R128x1F g_UByteMax = { { _set_vec4f(255.0f, 255.0f, 255.0f, 255.0f) } };
		inline const R128x1F g_ByteMin = { { _set_vec4f(-127.0f, -127.0f, -127.0f, -127.0f) } };
		inline const R128x1F g_ByteMax = { { _set_vec4f(127.0f, 127.0f, 127.0f, 127.0f) } };
		inline const R128x1F g_ShortMin = { { _set_vec4f(-32767.0f, -32767.0f, -32767.0f, -32767.0f) } };
		inline const R128x1F g_ShortMax = { { _set_vec4f(32767.0f, 32767.0f, 32767.0f, 32767.0f) } };
		inline const R128x1F g_UShortMax = { { _set_vec4f(65535.0f, 65535.0f, 65535.0f, 65535.0f) } };

	} // namespace SIMDMath
} // namespace krystallic
