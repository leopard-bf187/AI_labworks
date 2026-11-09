/*
*  Authors:  Leonid (@LeoParD) Parmacli  &&  Victor (@RisovoePole) Anisimov
*
*  Description:
*
*  Date: 05.01.2026
*/


#pragma once


#include "declaration.h"
#include "global_defs.h"
#include "math_library.h"

#include "simd_registers_SSE42.h"
#include "simd_registers_AVX_AVX2.h"
#include "simd_registers_NEON.h"

#include "simd_helpers_SSE42.inl"
#include "simd_helpers_AVX_AVX2.inl"
#include "simd_helpers_NEON.inl"

namespace krystallic
{
    namespace SIMDMath
    {

#if defined(KRYSTALLIC_ARCH_X64) || defined(KRYSTALLIC_ARCH_ARM64)
    #define SIMDMATH_R128X1_BY_VALUE 1
#else
    #define SIMDMATH_R128X1_BY_VALUE 0
#endif

#if defined(KRYSTALLIC_ARCH_X64) && defined(SIMD_AVX)
    #define SIMDMATH_R256X1_BY_VALUE 1
#else
    #define SIMDMATH_R256X1_BY_VALUE 0
#endif

#if SIMDMATH_R128X1_BY_VALUE

        using R128x1F_Arg0 = R128x1F;
        using R128x1F_Arg1 = R128x1F;
        using R128x1F_Arg2 = R128x1F;
        using R128x1F_ArgN = const R128x1F&;

        using R128x1I_Arg0 = R128x1I;
        using R128x1I_Arg1 = R128x1I;
        using R128x1I_Arg2 = R128x1I;
        using R128x1I_ArgN = const R128x1I&;

        using R128x1D_Arg0 = R128x1D;
        using R128x1D_Arg1 = R128x1D;
        using R128x1D_Arg2 = R128x1D;
        using R128x1D_ArgN = const R128x1D&;

#else

        using R128x1F_Arg0 = const R128x1F&;
        using R128x1F_Arg1 = const R128x1F&;
        using R128x1F_Arg2 = const R128x1F&;
        using R128x1F_ArgN = const R128x1F&;

        using R128x1I_Arg0 = const R128x1I&;
        using R128x1I_Arg1 = const R128x1I&;
        using R128x1I_Arg2 = const R128x1I&;
        using R128x1I_ArgN = const R128x1I&;

        using R128x1D_Arg0 = const R128x1D&;
        using R128x1D_Arg1 = const R128x1D&;
        using R128x1D_Arg2 = const R128x1D&;
        using R128x1D_ArgN = const R128x1D&;

#endif

        using R128x2F_Arg0 = const R128x2F&;
        using R128x2F_Arg1 = const R128x2F&;
        using R128x2F_Arg2 = const R128x2F&;
        using R128x2F_ArgN = const R128x2F&;

        using R128x3F_Arg0 = const R128x3F&;
        using R128x3F_Arg1 = const R128x3F&;
        using R128x3F_Arg2 = const R128x3F&;
        using R128x3F_ArgN = const R128x3F&;

        using R128x4F_Arg0 = const R128x4F&;
        using R128x4F_Arg1 = const R128x4F&;
        using R128x4F_Arg2 = const R128x4F&;
        using R128x4F_ArgN = const R128x4F&;

        using R128x2I_Arg0 = const R128x2I&;
        using R128x2I_Arg1 = const R128x2I&;
        using R128x2I_Arg2 = const R128x2I&;
        using R128x2I_ArgN = const R128x2I&;

        using R128x3I_Arg0 = const R128x3I&;
        using R128x3I_Arg1 = const R128x3I&;
        using R128x3I_Arg2 = const R128x3I&;
        using R128x3I_ArgN = const R128x3I&;

        using R128x4I_Arg0 = const R128x4I&;
        using R128x4I_Arg1 = const R128x4I&;
        using R128x4I_Arg2 = const R128x4I&;
        using R128x4I_ArgN = const R128x4I&;

        using R128x2D_Arg0 = const R128x2D&;
        using R128x2D_Arg1 = const R128x2D&;
        using R128x2D_Arg2 = const R128x2D&;
        using R128x2D_ArgN = const R128x2D&;

        using R128x3D_Arg0 = const R128x3D&;
        using R128x3D_Arg1 = const R128x3D&;
        using R128x3D_Arg2 = const R128x3D&;
        using R128x3D_ArgN = const R128x3D&;

        using R128x4D_Arg0 = const R128x4D&;
        using R128x4D_Arg1 = const R128x4D&;
        using R128x4D_Arg2 = const R128x4D&;
        using R128x4D_ArgN = const R128x4D&;

#if SIMDMATH_R256X1_BY_VALUE

        using R256x1F_Arg0 = R256x1F;
        using R256x1F_Arg1 = R256x1F;
        using R256x1F_Arg2 = R256x1F;
        using R256x1F_ArgN = const R256x1F&;

        using R256x1I_Arg0 = R256x1I;
        using R256x1I_Arg1 = R256x1I;
        using R256x1I_Arg2 = R256x1I;
        using R256x1I_ArgN = const R256x1I&;

        using R256x1D_Arg0 = R256x1D;
        using R256x1D_Arg1 = R256x1D;
        using R256x1D_Arg2 = R256x1D;
        using R256x1D_ArgN = const R256x1D&;

#else

        using R256x1F_Arg0 = const R256x1F&;
        using R256x1F_Arg1 = const R256x1F&;
        using R256x1F_Arg2 = const R256x1F&;
        using R256x1F_ArgN = const R256x1F&;

        using R256x1I_Arg0 = const R256x1I&;
        using R256x1I_Arg1 = const R256x1I&;
        using R256x1I_Arg2 = const R256x1I&;
        using R256x1I_ArgN = const R256x1I&;

        using R256x1D_Arg0 = const R256x1D&;
        using R256x1D_Arg1 = const R256x1D&;
        using R256x1D_Arg2 = const R256x1D&;
        using R256x1D_ArgN = const R256x1D&;

#endif

        using R256x2F_Arg0 = const R256x2F&;
        using R256x2F_Arg1 = const R256x2F&;
        using R256x2F_Arg2 = const R256x2F&;
        using R256x2F_ArgN = const R256x2F&;

        using R256x2I_Arg0 = const R256x2I&;
        using R256x2I_Arg1 = const R256x2I&;
        using R256x2I_Arg2 = const R256x2I&;
        using R256x2I_ArgN = const R256x2I&;

        using R256x2D_Arg0 = const R256x2D&;
        using R256x2D_Arg1 = const R256x2D&;
        using R256x2D_Arg2 = const R256x2D&;
        using R256x2D_ArgN = const R256x2D&;

#undef SIMDMATH_R128X1_BY_VALUE
#undef SIMDMATH_R256X1_BY_VALUE

        // --------------------------- 128bit (SSE4 size) registers --------------------------- //
        R128x1F R128x1F_Zero() noexcept;
        R128x1F R128x1F_One(float v) noexcept;
        R128x1F R128x1F_Set(float v1, float v2, float v3, float v4) noexcept;

        R128x2F R128x2F_Zero() noexcept;
        R128x2F R128x2F_One(float v) noexcept;
        R128x2F R128x2F_Set(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8) noexcept;

        R128x3F R128x3F_Zero() noexcept;
        R128x3F R128x3F_One(float v) noexcept;
        R128x3F R128x3F_Set(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12) noexcept;

        R128x4F R128x4F_Zero() noexcept;
        R128x4F R128x4F_One(float v) noexcept;
        R128x4F R128x4F_Set(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15, float v16) noexcept;


        R128x1I R128x1I_Zero() noexcept;
        R128x1I R128x1I_One(int v) noexcept;
        R128x1I R128x1I_Set(int v1, int v2, int v3, int v4) noexcept;

        R128x2I R128x2I_Zero() noexcept;
        R128x2I R128x2I_One(int v) noexcept;
        R128x2I R128x2I_Set(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8) noexcept;

        R128x3I R128x3I_Zero() noexcept;
        R128x3I R128x3I_One(int v) noexcept;
        R128x3I R128x3I_Set(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9, int v10, int v11, int v12) noexcept;

        R128x4I R128x4I_Zero() noexcept;
        R128x4I R128x4I_One(int v) noexcept;
        R128x4I R128x4I_Set(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9, int v10, int v11, int v12, int v13, int v14, int v15, int v16) noexcept;


        R128x1D R128x1D_Zero() noexcept;
        R128x1D R128x1D_One(double v) noexcept;
        R128x1D R128x1D_Set(double v1, double v2) noexcept;

        R128x2D R128x2D_Zero() noexcept;
        R128x2D R128x2D_One(double v) noexcept;
        R128x2D R128x2D_Set(double v1, double v2, double v3, double v4) noexcept;

        R128x3D R128x3D_Zero() noexcept;
        R128x3D R128x3D_One(double v) noexcept;
        R128x3D R128x3D_Set(double v1, double v2, double v3, double v4, double v5, double v6) noexcept;

        R128x4D R128x4D_Zero() noexcept;
        R128x4D R128x4D_One(double v) noexcept;
        R128x4D R128x4D_Set(double v1, double v2, double v3, double v4, double v5, double v6, double v7, double v8) noexcept;



        // --------------------------- 256 bit (AVX size) registers --------------------------- //

        R256x1F R256x1F_Zero() noexcept;
        R256x1F R256x1F_One(float v) noexcept;
        R256x1F R256x1F_Set(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8) noexcept;

        R256x2F R256x2F_Zero() noexcept;
        R256x2F R256x2F_One(float v) noexcept;
        R256x2F R256x2F_Set(float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15, float v16) noexcept;


        R256x1I R256x1I_Zero() noexcept;
        R256x1I R256x1I_One(int v) noexcept;
        R256x1I R256x1I_Set(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8) noexcept;

        R256x2I R256x2I_Zero() noexcept;
        R256x2I R256x2I_One(int v) noexcept;
        R256x2I R256x2I_Set(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9, int v10, int v11, int v12, int v13, int v14, int v15, int v16) noexcept;


        R256x1D R256x1D_Zero() noexcept;
        R256x1D R256x1D_One(double v) noexcept;
        R256x1D R256x1D_Set(double v1, double v2, double v3, double v4) noexcept;

        R256x2D R256x2D_Zero() noexcept;
        R256x2D R256x2D_One(double v) noexcept;
        R256x2D R256x2D_Set(double v1, double v2, double v3, double v4, double v5, double v6, double v7, double v8) noexcept;


        // --------------------------- Load/Store functions for POD types --------------------------- //

        R128x1F __vectorcall R128x1F_LoadRGBA(const CColorRGBA& v) noexcept;
        R128x1F __vectorcall R128x1F_LoadHSLA(const CColorHSLA& v) noexcept;
        R128x1F __vectorcall R128x1F_LoadHSVA(const CColorHSVA& v) noexcept;
        R128x1F __vectorcall R128x1F_LoadYUVA(const CColorYUVA& v) noexcept;
        R128x1F __vectorcall R128x1F_LoadVec2(const CVector2& v) noexcept;
        R128x1F __vectorcall R128x1F_LoadVec3(const CVector3& v) noexcept;
        R128x1F __vectorcall R128x1F_LoadVec4(const CVector4& v) noexcept;
        R128x1F __vectorcall R128x1F_LoadQuat(const CQuaternion& v) noexcept;
        R128x2F __vectorcall R128x2F_LoadMat22(const CMatrix2x2& v) noexcept;
        R256x1F __vectorcall R256x1F_LoadMat22(const CMatrix2x2& v) noexcept;
        R128x3F __vectorcall R128x3F_LoadMat33(const CMatrix3x3& v) noexcept;
        R256x2F __vectorcall R256x2F_LoadMat33(const CMatrix3x3& v) noexcept;
        R128x4F __vectorcall R128x4F_LoadMat43(const CMatrix4x3& v) noexcept;
        R256x2F __vectorcall R256x2F_LoadMat43(const CMatrix4x3& v) noexcept;
        R128x4F __vectorcall R128x4F_LoadMat44(const CMatrix4x4& v) noexcept;
        R256x2F __vectorcall R256x2F_LoadMat44(const CMatrix4x4& v) noexcept;

        CColorRGBA  __vectorcall R128x1F_StoreRGBA(const R128x1F& simd) noexcept;
        CColorHSLA  __vectorcall R128x1F_StoreHSLA(const R128x1F& simd) noexcept;
        CColorHSVA  __vectorcall R128x1F_StoreHSVA(const R128x1F& simd) noexcept;
        CColorYUVA  __vectorcall R128x1F_StoreYUVA(const R128x1F& simd) noexcept;
        CVector2    __vectorcall R128x1F_StoreVec2(const R128x1F& simd) noexcept;
        CVector3    __vectorcall R128x1F_StoreVec3(const R128x1F& simd) noexcept;
        CVector4    __vectorcall R128x1F_StoreVec4(const R128x1F& simd) noexcept;
        CQuaternion __vectorcall R128x1F_StoreQuat(const R128x1F& simd) noexcept;
        CMatrix2x2  __vectorcall R128x2F_StoreMat22(const R128x2F& simd) noexcept;
        CMatrix2x2  __vectorcall R256x1F_StoreMat22(const R256x1F& simd) noexcept;
        CMatrix3x3  __vectorcall R128x3F_StoreMat33(const R128x3F& simd) noexcept;
        CMatrix3x3  __vectorcall R256x2F_StoreMat33(const R256x2F& simd) noexcept;
        CMatrix4x3  __vectorcall R128x4F_StoreMat43(const R128x4F& simd) noexcept;
        CMatrix4x3  __vectorcall R256x2F_StoreMat43(const R256x2F& simd) noexcept;
        CMatrix4x4  __vectorcall R128x4F_StoreMat44(const R128x4F& simd) noexcept;
        CMatrix4x4  __vectorcall R256x2F_StoreMat44(const R256x2F& simd) noexcept;


        // --------------------------- Cast functions --------------------------- //

        R128x1I __vectorcall Cast128x1FI(R128x1F_Arg0 v) noexcept;
        R128x1D __vectorcall Cast128x1FD(R128x1F_Arg0 v) noexcept;
        R128x1F __vectorcall Cast128x1IF(R128x1I_Arg0 v) noexcept;
        R128x1D __vectorcall Cast128x1ID(R128x1I_Arg0 v) noexcept;
        R128x1F __vectorcall Cast128x1DF(R128x1D_Arg0 v) noexcept;
        R128x1I __vectorcall Cast128x1DI(R128x1D_Arg0 v) noexcept;

        R128x2I __vectorcall Cast128x2FI(R128x2F_Arg0 v) noexcept;
        R128x2D __vectorcall Cast128x2FD(R128x2F_Arg0 v) noexcept;
        R128x2F __vectorcall Cast128x2IF(R128x2I_Arg0 v) noexcept;
        R128x2D __vectorcall Cast128x2ID(R128x2I_Arg0 v) noexcept;
        R128x2F __vectorcall Cast128x2DF(R128x2D_Arg0 v) noexcept;
        R128x2I __vectorcall Cast128x2DI(R128x2D_Arg0 v) noexcept;

        R128x3I __vectorcall Cast128x3FI(R128x3F_Arg0 v) noexcept;
        R128x3D __vectorcall Cast128x3FD(R128x3F_Arg0 v) noexcept;
        R128x3F __vectorcall Cast128x3IF(R128x3I_Arg0 v) noexcept;
        R128x3D __vectorcall Cast128x3ID(R128x3I_Arg0 v) noexcept;
        R128x3F __vectorcall Cast128x3DF(R128x3D_Arg0 v) noexcept;
        R128x3I __vectorcall Cast128x3DI(R128x3D_Arg0 v) noexcept;

        R128x4I __vectorcall Cast128x4FI(R128x4F_Arg0 v) noexcept;
        R128x4D __vectorcall Cast128x4FD(R128x4F_Arg0 v) noexcept;
        R128x4F __vectorcall Cast128x4IF(R128x4I_Arg0 v) noexcept;
        R128x4D __vectorcall Cast128x4ID(R128x4I_Arg0 v) noexcept;
        R128x4F __vectorcall Cast128x4DF(R128x4D_Arg0 v) noexcept;
        R128x4I __vectorcall Cast128x4DI(R128x4D_Arg0 v) noexcept;

        R256x1I __vectorcall Cast256x1FI(R256x1F_Arg0 v) noexcept;
        R256x1D __vectorcall Cast256x1FD(R256x1F_Arg0 v) noexcept;
        R256x1F __vectorcall Cast256x1IF(R256x1I_Arg0 v) noexcept;
        R256x1D __vectorcall Cast256x1ID(R256x1I_Arg0 v) noexcept;
        R256x1F __vectorcall Cast256x1DF(R256x1D_Arg0 v) noexcept;
        R256x1I __vectorcall Cast256x1DI(R256x1D_Arg0 v) noexcept;

        R256x2I __vectorcall Cast256x2FI(R256x2F_Arg0 v) noexcept;
        R256x2D __vectorcall Cast256x2FD(R256x2F_Arg0 v) noexcept;
        R256x2F __vectorcall Cast256x2IF(R256x2I_Arg0 v) noexcept;
        R256x2D __vectorcall Cast256x2ID(R256x2I_Arg0 v) noexcept;
        R256x2F __vectorcall Cast256x2DF(R256x2D_Arg0 v) noexcept;
        R256x2I __vectorcall Cast256x2DI(R256x2D_Arg0 v) noexcept;


        // --------------------------- Convert functions --------------------------- //

        R128x1I __vectorcall Convert128x1FI(R128x1F_Arg0 v) noexcept;
        R128x1D __vectorcall Convert128x1FD(R128x1F_Arg0 v) noexcept;
        R128x1F __vectorcall Convert128x1IF(R128x1I_Arg0 v) noexcept;
        R128x1D __vectorcall Convert128x1ID(R128x1I_Arg0 v) noexcept;
        R128x1F __vectorcall Convert128x1DF(R128x1D_Arg0 v) noexcept;
        R128x1I __vectorcall Convert128x1DI(R128x1D_Arg0 v) noexcept;

        R128x2I __vectorcall Convert128x2FI(R128x2F_Arg0 v) noexcept;
        R128x2D __vectorcall Convert128x2FD(R128x2F_Arg0 v) noexcept;
        R128x2F __vectorcall Convert128x2IF(R128x2I_Arg0 v) noexcept;
        R128x2D __vectorcall Convert128x2ID(R128x2I_Arg0 v) noexcept;
        R128x2F __vectorcall Convert128x2DF(R128x2D_Arg0 v) noexcept;
        R128x2I __vectorcall Convert128x2DI(R128x2D_Arg0 v) noexcept;

        R128x3I __vectorcall Convert128x3FI(R128x3F_Arg0 v) noexcept;
        R128x3D __vectorcall Convert128x3FD(R128x3F_Arg0 v) noexcept;
        R128x3F __vectorcall Convert128x3IF(R128x3I_Arg0 v) noexcept;
        R128x3D __vectorcall Convert128x3ID(R128x3I_Arg0 v) noexcept;
        R128x3F __vectorcall Convert128x3DF(R128x3D_Arg0 v) noexcept;
        R128x3I __vectorcall Convert128x3DI(R128x3D_Arg0 v) noexcept;

        R128x4I __vectorcall Convert128x4FI(R128x4F_Arg0 v) noexcept;
        R128x4D __vectorcall Convert128x4FD(R128x4F_Arg0 v) noexcept;
        R128x4F __vectorcall Convert128x4IF(R128x4I_Arg0 v) noexcept;
        R128x4D __vectorcall Convert128x4ID(R128x4I_Arg0 v) noexcept;
        R128x4F __vectorcall Convert128x4DF(R128x4D_Arg0 v) noexcept;
        R128x4I __vectorcall Convert128x4DI(R128x4D_Arg0 v) noexcept;

        R256x1I __vectorcall Convert256x1FI(R256x1F_Arg0 v) noexcept;
        R256x1D __vectorcall Convert256x1FD(R256x1F_Arg0 v) noexcept;
        R256x1F __vectorcall Convert256x1IF(R256x1I_Arg0 v) noexcept;
        R256x1D __vectorcall Convert256x1ID(R256x1I_Arg0 v) noexcept;
        R256x1F __vectorcall Convert256x1DF(R256x1D_Arg0 v) noexcept;
        R256x1I __vectorcall Convert256x1DI(R256x1D_Arg0 v) noexcept;

        R256x2I __vectorcall Convert256x2FI(R256x2F_Arg0 v) noexcept;
        R256x2D __vectorcall Convert256x2FD(R256x2F_Arg0 v) noexcept;
        R256x2F __vectorcall Convert256x2IF(R256x2I_Arg0 v) noexcept;
        R256x2D __vectorcall Convert256x2ID(R256x2I_Arg0 v) noexcept;
        R256x2F __vectorcall Convert256x2DF(R256x2D_Arg0 v) noexcept;
        R256x2I __vectorcall Convert256x2DI(R256x2D_Arg0 v) noexcept;


        // загрузка-сохранение и инициализация
        // R128x1F LoadSIMD() noexcept;
        // R128x1F LoadSIMD(float value) noexcept;
        // R128x1F LoadSIMD(float x, float y, float z, float w) noexcept;
        // R128x1F LoadSIMD(const CColorRGBA& v) noexcept;
        // R128x1F LoadSIMD(const CColorHSLA& v) noexcept;
        // R128x1F LoadSIMD(const CColorHSVA& v) noexcept;
        // R128x1F LoadSIMD(const CColorYUVA& v) noexcept;
        // R128x1F LoadSIMD(const CVector2& v) noexcept;
        // R128x1F LoadSIMD(const CVector3& v) noexcept;
        // R128x1F LoadSIMD(const CVector4& v) noexcept;
        // R128x1F LoadSIMD(const CQuaternion& v) noexcept;
        // R128x2F LoadSIMD(const CMatrix2x2& v) noexcept;
        // R128x3F LoadSIMD(const CMatrix3x3& v) noexcept;
        // R128x4F LoadSIMD(const CMatrix4x3& v) noexcept;
        // R128x4F LoadSIMD(const CMatrix4x4& v) noexcept;

        // float       StoreSIMD_Float(const R128x1F& simd) noexcept;
        // CColorRGBA  StoreSIMD_RGBA(const R128x1F& simd) noexcept;
        // CColorHSLA  StoreSIMD_HSLA(const R128x1F& simd) noexcept;
        // CColorHSVA  StoreSIMD_HSVA(const R128x1F& simd) noexcept;
        // CColorYUVA  StoreSIMD_YUVA(const R128x1F& simd) noexcept;
        // CVector2    StoreSIMD_Vec2(const R128x1F& simd) noexcept;
        // CVector3    StoreSIMD_Vec3(const R128x1F& simd) noexcept;
        // CVector4    StoreSIMD_Vec4(const R128x1F& simd) noexcept;
        // CQuaternion StoreSIMD_Quat(const R128x1F& simd) noexcept;
        // CMatrix2x2  StoreSIMD_Mx22(const R128x2F& simd) noexcept;
        // CMatrix3x3  StoreSIMD_Mx33(const R128x3F& simd) noexcept;
        // CMatrix4x3  StoreSIMD_Mx43(const R128x4F& simd) noexcept;
        // CMatrix4x4  StoreSIMD_Mx44(const R128x4F& simd) noexcept;


        // доступ
        R128x1F VecSetX(R128x1F v, float x) noexcept;
        R128x1F VecSetY(R128x1F v, float y) noexcept;
        R128x1F VecSetZ(R128x1F v, float z) noexcept;
        R128x1F VecSetW(R128x1F v, float w) noexcept;

        float VecGetX(R128x1F v) noexcept;
        float VecGetY(R128x1F v) noexcept;
        float VecGetZ(R128x1F v) noexcept;
        float VecGetW(R128x1F v) noexcept;


        //битовые операции
        template <int X, int Y, int Z, int W> R128x1F VecPermute(R128x1F v) noexcept;
        int VecMask(R128x1F v) noexcept;
        template <int X, int Y, int Z, int W> R128x1F VecShuffle(R128x1F a, R128x1F b);
        template <int MASK> R128x1F VecBlend(R128x1F a, R128x1F b);
        R128x1F VecMergeLow(R128x1F a, R128x1F b);
        R128x1F VecMergeHigh(R128x1F a, R128x1F b);
        R128x1F __vectorcall VecAnd(R128x1F a, R128x1F b);
        R128x1F __vectorcall VecXor(R128x1F a, R128x1F b);
        R128x1F __vectorcall VecOr(R128x1F a, R128x1F b);
        R128x1F __vectorcall VecAndNot(R128x1F a, R128x1F b);

        // арифметика
        R128x1F __vectorcall                     VecAbs(R128x1F a);
        R128x1F __vectorcall                     VecNeg(R128x1F a);
        R128x1F __vectorcall                     VecAdd(R128x1F a, R128x1F b);
        R128x1F __vectorcall                     VecSub(R128x1F a, R128x1F b);
        R128x1F __vectorcall                     VecMul(R128x1F a, R128x1F b);
        R128x1F __vectorcall                     VecDiv(R128x1F a, R128x1F b);
        R128x1F __vectorcall                     VecScale(R128x1F a, float scale);
        R128x1F __vectorcall                     VecMin(R128x1F a, R128x1F b);
        R128x1F __vectorcall                     VecMax(R128x1F a, R128x1F b);
        R128x1F __vectorcall                     VecClamp(R128x1F value, R128x1F min, R128x1F max);
        R128x1F __vectorcall                     VecSaturate(R128x1F value);
        R128x1F __vectorcall                     VecSqrt(R128x1F value);
        R128x1F __vectorcall                     VecPow(R128x1F vec, R128x1F power);
        R128x1F __vectorcall                     VecRsqrt(R128x1F value);
        R128x1F __vectorcall                     VecIsInf(R128x1F a);
        R128x1F __vectorcall                     VecIsNaN(R128x1F a);
        R128x1F __vectorcall                     VecFMAdd(R128x1F a, R128x1F b, R128x1F c);
        R128x1F __vectorcall                     VecFNMAdd(R128x1F a, R128x1F b, R128x1F c);
        template <int MASK> R128x1F __vectorcall VecDP(R128x1F a, R128x1F b);
        R128x1F __vectorcall                     VecRcp(R128x1F a);


        // сравнение
        // можно чекать результат с помощью _mm_movemask_ps
        R128x1F __vectorcall VecEqual(R128x1F a, R128x1F b);
        R128x1F __vectorcall VecNotEqual(R128x1F a, R128x1F b);
        R128x1F __vectorcall VecGreater(R128x1F a, R128x1F b);
        R128x1F __vectorcall VecGreaterOrEqual(R128x1F a, R128x1F b);
        R128x1F __vectorcall VecLess(R128x1F a, R128x1F b);
        R128x1F __vectorcall VecLessOrEqual(R128x1F a, R128x1F b);
        R128x1F __vectorcall VecNearEqual(R128x1F a, R128x1F b, float epsilon);


        // геометрия
        R128x1F __vectorcall VecLerp(R128x1F a, R128x1F b, float t);
        R128x1F __vectorcall VecInBounds(R128x1F value, R128x1F bounds);

        // тригонометрия и экспоненты
        R128x1F __vectorcall VecRound(R128x1F vec);
        R128x1F __vectorcall VecCos(R128x1F v);
        R128x1F __vectorcall VecACos(R128x1F v);
        R128x1F __vectorcall VecSin(R128x1F v);
        R128x1F __vectorcall VecASin(R128x1F v);
        R128x1F __vectorcall VecTan(R128x1F v);
        R128x1F __vectorcall VecATan(R128x1F v);
        void __vectorcall    VecSinCos(R128x1F v, R128x1F* outSin, R128x1F* outCos);
        void __vectorcall    VecASinACos(R128x1F v, R128x1F* outASin, R128x1F* outACos);
        R128x1F __vectorcall VecExp2(R128x1F v);
        R128x1F __vectorcall VecExp10(R128x1F v);
        R128x1F __vectorcall VecExpE(R128x1F v);
        R128x1F __vectorcall VecLog2(R128x1F v);
        R128x1F __vectorcall VecLog10(R128x1F v);
        R128x1F __vectorcall VecLogE(R128x1F v);


        // Vec2
        bool __vectorcall    Vec2InBounds(R128x1F v, R128x1F bounds);
        R128x1F __vectorcall Vec2Length(R128x1F v);
        R128x1F __vectorcall Vec2LengthSq(R128x1F v);
        R128x1F __vectorcall Vec2Normalize(R128x1F v);
        R128x1F __vectorcall Vec2Dot(R128x1F a, R128x1F b);
        R128x1F __vectorcall Vec2Cross(R128x1F a, R128x1F b);
        R128x1F __vectorcall Vec2CrossCW(R128x1F a, R128x1F b);
        R128x1F __vectorcall Vec2CrossCCW(R128x1F a, R128x1F b);
        R128x1F __vectorcall Vec2Reflect(R128x1F v, R128x1F normal);
        R128x1F __vectorcall Vec2Refract(R128x1F v, R128x1F normal, float refrCoeff);

        R128x1F __vectorcall Vec2Rotate(R128x1F v, R128x2F rotationMatrix); // rotation это матрица 2х2 либо 1 либо 2 m128
        R128x1F __vectorcall Vec2Rotate(R128x1F v, R128x1F rotationQuaternion);
        R128x1F __vectorcall Vec2InvRotate(R128x1F v, R128x1F rotationQuaternion);
        R128x1F __vectorcall Vec2Transform(R128x1F v, R128x4F worldMatrix);

        bool __vectorcall Vec2Equal(R128x1F a, R128x1F b);
        bool __vectorcall Vec2NotEqual(R128x1F a, R128x1F b);
        bool __vectorcall Vec2Greater(R128x1F a, R128x1F b);
        bool __vectorcall Vec2GreaterOrEqual(R128x1F a, R128x1F b);
        bool __vectorcall Vec2Less(R128x1F a, R128x1F b);
        bool __vectorcall Vec2LessOrEqual(R128x1F a, R128x1F b);
        bool __vectorcall Vec2NearEqual(R128x1F a, R128x1F b, float epsilon);
        bool __vectorcall Vec2IsInf(R128x1F val);
        bool __vectorcall Vec2IsNaN(R128x1F val);

        // Vec3
        bool __vectorcall    Vec3InBounds(R128x1F v, R128x1F bounds);
        R128x1F __vectorcall Vec3Length(R128x1F v);
        R128x1F __vectorcall Vec3LengthSq(R128x1F v);
        R128x1F __vectorcall Vec3Normalize(R128x1F v);
        R128x1F __vectorcall Vec3Dot(R128x1F a, R128x1F b);
        R128x1F __vectorcall Vec3Cross(R128x1F a, R128x1F b);
        R128x1F __vectorcall Vec3Reflect(R128x1F v, R128x1F normal);
        R128x1F __vectorcall Vec3Refract(R128x1F v, R128x1F normal, float refrCoeff);

        R128x1F __vectorcall Vec3Rotate(R128x1F v, R128x3F rotationMatrix);
        R128x1F __vectorcall Vec3Rotate(R128x1F v, R128x1F rotationQuaternion);
        R128x1F __vectorcall Vec3InvRotate(R128x1F v, R128x1F rotationQuaternion);
        R128x1F __vectorcall Vec3Transform(R128x1F v, R128x4F worldMatrix);
        R128x1F __vectorcall Vec3Project(R128x1F v, R128x4F world, R128x4F view, R128x4F proj, R128x4F viewport); //error
        R128x1F __vectorcall Vec3UnProject(R128x1F v, R128x4F world, R128x4F view, R128x4F proj, R128x4F viewport); //error

        bool __vectorcall Vec3Equal(R128x1F a, R128x1F b);
        bool __vectorcall Vec3NotEqual(R128x1F a, R128x1F b);
        bool __vectorcall Vec3Greater(R128x1F a, R128x1F b);
        bool __vectorcall Vec3GreaterOrEqual(R128x1F a, R128x1F b);
        bool __vectorcall Vec3Less(R128x1F a, R128x1F b);
        bool __vectorcall Vec3LessOrEqual(R128x1F a, R128x1F b);
        bool __vectorcall Vec3NearEqual(R128x1F a, R128x1F b, float epsilon);
        bool __vectorcall Vec3IsInf(R128x1F val);
        bool __vectorcall Vec3IsNaN(R128x1F val);

        // Vec4
        bool __vectorcall    Vec4InBounds(R128x1F v, R128x1F bounds);
        R128x1F __vectorcall Vec4Length(R128x1F v);
        R128x1F __vectorcall Vec4LengthSq(R128x1F v);
        R128x1F __vectorcall Vec4Normalize(R128x1F v);
        R128x1F __vectorcall Vec4Dot(R128x1F a, R128x1F b);
        R128x1F __vectorcall Vec4Cross(R128x1F a, R128x1F b, R128x1F c);
        R128x1F __vectorcall Vec4Reflect(R128x1F v, R128x1F normal);
        R128x1F __vectorcall Vec4Refract(R128x1F v, R128x1F normal, float refrCoeff);

        R128x1F __vectorcall Vec4Transform(R128x1F v, R128x4F worldMatrix);

        bool __vectorcall Vec4Equal(R128x1F a, R128x1F b);
        bool __vectorcall Vec4NotEqual(R128x1F a, R128x1F b);
        bool __vectorcall Vec4Greater(R128x1F a, R128x1F b);
        bool __vectorcall Vec4GreaterOrEqual(R128x1F a, R128x1F b);
        bool __vectorcall Vec4Less(R128x1F a, R128x1F b);
        bool __vectorcall Vec4LessOrEqual(R128x1F a, R128x1F b);
		bool __vectorcall Vec4NearEqual(R128x1F a, R128x1F b, float epsilon);
		bool __vectorcall Vec4IsInf(R128x1F val);
		bool __vectorcall Vec4IsNaN(R128x1F val);


		//#####################################################################
		//--------------------------------MATRICES
		//#####################################################################

		R128x2F __vectorcall Mat22Identity();
		R128x1F __vectorcall Mat22Determinant(R128x2F mat22);
		R128x2F __vectorcall Mat22Transpose(R128x2F mat22);
		R128x2F __vectorcall Mat22Inverse(R128x2F mat22);
		R128x2F __vectorcall Mat22Multiply(R128x2F a, R128x2F b);
		R128x2F __vectorcall Mat22MultiplyTrasnpose(R128x2F a, R128x2F b);
		R128x2F __vectorcall Mat22Rotation(float angle);
		R128x2F __vectorcall Mat22Scale(float scaleX, float scaleY);
		R128x2F __vectorcall Mat22ScaleFromVector(R128x1F scale);
		R128x2F __vectorcall Mat22Set(float m11, float m12, float m21, float m22);
		R128x2F __vectorcall Mat22Skew(float skewXY, float skewYX);
		R128x2F __vectorcall Mat22SkewFromVector(R128x1F skewVec2);

		bool __vectorcall Mat22IsIdentity(R128x2F mat22);
		bool __vectorcall Mat22IsNaN(R128x2F mat22);
		bool __vectorcall Mat22IsInf(R128x2F mat22);

		bool __vectorcall Mat22Equal(R128x2F a, R128x2F b);
		bool __vectorcall Mat22NotEqual(R128x2F a, R128x2F b);

		R128x3F __vectorcall Mat33Identity();
		R128x1F __vectorcall Mat33Determinant(R128x3F mat33);
		R128x3F __vectorcall Mat33Transpose(R128x3F mat33);
		R128x3F __vectorcall Mat33Inverse(R128x3F mat33);
		R128x3F __vectorcall Mat33Multiply(R128x3F a, R128x3F b);
		R128x3F __vectorcall Mat33MultiplyTrasnpose(R128x3F a, R128x3F b);
		R128x3F __vectorcall Mat33RotationX(float angle);
		R128x3F __vectorcall Mat33RotationY(float angle);
		R128x3F __vectorcall Mat33RotationZ(float angle);
		R128x3F __vectorcall Mat33Scale(float scaleX, float scaleY, float scaleZ);
		R128x3F __vectorcall Mat33ScaleFromVector(R128x1F scaleVec3);
		R128x3F __vectorcall Mat33Set(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);
		R128x3F __vectorcall Mat33Skew(float skewXY, float skewXZ, float skewYX, float skewYZ, float skewZX, float skewZY);
		R128x3F __vectorcall Mat33SkewFromVector(R128x1F skewX, R128x1F skewY, R128x1F skewZ);
		R128x3F __vectorcall Mat33Translation(float translateX, float translateY);
		R128x3F __vectorcall Mat33TranslationFromVector(R128x1F translationVec2);

		R128x3F __vectorcall Mat33AffineTransformation(R128x1F scale, R128x1F rotationOrigin, float rotationAngle, R128x1F translation);
		R128x3F __vectorcall Mat33Transformation(R128x1F scaleOrigin, R128x1F scaleOrientation, R128x1F scale, R128x1F rotationOrigin, float rotationAngle, R128x1F translation);

		bool __vectorcall Mat33IsIdentity(R128x3F mat33);
		bool __vectorcall Mat33IsNaN(R128x3F mat33);
		bool __vectorcall Mat33IsInf(R128x3F mat33);

		bool __vectorcall Mat33Equal(R128x3F a, R128x3F b);
		bool __vectorcall Mat33NotEqual(R128x3F a, R128x3F b);


		R128x4F  __vectorcall Mat44Identity();
		R128x1F __vectorcall Mat33Determinant(R128x3F mat33);
		R128x4F __vectorcall Mat44Transpose(R128x4F mat33);
		R128x4F __vectorcall Mat44Inverse(R128x4F mat33);
		R128x4F __vectorcall Mat44Multiply(R128x4F a, R128x4F b);
		R128x4F __vectorcall Mat44MultiplyTrasnpose(R128x4F a, R128x4F b);

		R128x4F  __vectorcall Mat44LookAtLH(R128x1F eyePos, R128x1F focusPos, R128x1F upDir);
		R128x4F  __vectorcall Mat44LookAtRH(R128x1F eyePos, R128x1F focusPos, R128x1F upDir);
		R128x4F  __vectorcall Mat44LookToLH(R128x1F eyePos, R128x1F eyeDir, R128x1F upDir);
		R128x4F  __vectorcall Mat44LookToRH(R128x1F eyePos, R128x1F eyeDir, R128x1F upDir);
		R128x4F  __vectorcall Mat44OrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44OrthographicRH(float viewWidth, float viewHeight, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44OrthographicOffCenterLH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44OrthographicOffCenterRH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44PerspectiveLH(float viewWidth, float viewHeight, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44PerspectiveRH(float viewWidth, float viewHeight, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44PerspectiveFovXLH(float fovAngleX, float aspectRatio, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44PerspectiveFovXRH(float fovAngleX, float aspectRatio, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44PerspectiveFovYLH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44PerspectiveFovYRH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44PerspectiveOffCenterLH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44PerspectiveOffCenterRH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ);
		R128x4F  __vectorcall Mat44RotationX(float angle);
		R128x4F  __vectorcall Mat44RotationY(float angle);
		R128x4F  __vectorcall Mat44RotationZ(float angle);
		R128x4F  __vectorcall Mat44RotationAxis(R128x1F axis, float angle);
		R128x4F  __vectorcall Mat44RotationQuaternion(R128x1F quat);
		R128x4F  __vectorcall Mat44RotationPitchYawRoll(float pitch, float yaw, float roll);
		R128x4F  __vectorcall Mat44RotationPitchYawRollFromVector(R128x1F pitchYawRoll);
		R128x4F  __vectorcall Mat44Scale(float scaleX, float scaleY, float scaleZ);
		R128x4F  __vectorcall Mat44ScaleFromVector(R128x1F scale);
		R128x4F  __vectorcall Mat44Skew(float skewXY, float skewXZ, float skewYX, float skewYZ, float skewZX, float skewZY);
		R128x4F  __vectorcall Mat44SkewFromVector(CVector2 skewX, CVector2 skewY, CVector2 skewZ);
		R128x4F  __vectorcall Mat44Translation(float translateX, float translateY, float translateZ);
		R128x4F  __vectorcall Mat44TranslationFromVector(R128x1F translationVec3);

		R128x4F  __vectorcall Mat44AffineTransformation(R128x1F scale, R128x1F rotationOrigin, R128x1F rotationQuaternion, R128x1F translation);
		R128x4F  __vectorcall Mat44Transformation(R128x1F scaleOrigin, R128x1F scaleOrientationQuaternion, R128x1F scale, R128x1F rotationOrigin, R128x1F rotationQuaternion, R128x1F translation);
		R128x4F  __vectorcall Mat44AffineTransformation2D(R128x1F scale, R128x1F rotationOrigin, float rotationAngle, CVector2 translation);
		R128x4F  __vectorcall Mat44Transformation2D(R128x1F scaleOrigin, float scaleOrientation, R128x1F scale, R128x1F rotationOrigin, float rotationAngle, R128x1F translation);

		bool __vectorcall Mat44IsIdentity(R128x4F mat44);
		bool __vectorcall Mat44IsNaN(R128x4F mat44);
		bool __vectorcall Mat44IsInf(R128x4F mat44);

		bool __vectorcall Mat44Equal(R128x4F a, R128x4F b);
		bool __vectorcall Mat44NotEqual(R128x4F a, R128x4F b);


    } // namespace SIMDMath

} // namespace krystallic
