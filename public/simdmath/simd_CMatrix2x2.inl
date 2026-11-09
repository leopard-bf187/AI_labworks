/*
*  Authors:  Leonid (@LeoParD) Parmacli  &&  Victor (@RisovoePole) Anisimov
*
*  Description:
*
*  Date:  05.01.2026
*/


#pragma once

#include "simd_library.h"
#include "constants.h"
#include "declaration.h"
#include "simd_registers_SSE42.h"


namespace krystallic
{
    namespace SIMDMath
    {

        inline R128x2F __vectorcall Mat22Identity()
        {
            return R128x2F_Set(
                1.f,0.f,0.f,0.f,
                0.f,1.f,0.f,0.f
            );
        }

        inline R128x1F __vectorcall Mat22Determinant(R128x2F mat22)
        {
            R128x1F row1, row2;
            row1.v1 = mat22.v1;
            row2.v1 = mat22.v2;

            row2 = VecPermute<1,0,2,3>(row2);
            row2 = VecXor(row2, R128x1F_Set(0,g_negZero, 0, 0));

            return VecDP<0x3F>(row1, row2);
        }

        inline R128x2F __vectorcall Mat22Transpose(R128x2F mat22)
        {
            R128x1F row1, row2, temp;
            row1.v1 = mat22.v1;
            row2.v1 = mat22.v2;

            temp = VecShuffle<0, 1, 0, 1>(row1, row2);//1.x, 1.y,  2.x, 2.y

            row1 = VecShuffle<0,2,0,0>(row1, g_Zero);
            row2 = VecShuffle<1,3,0,0>(row1, g_Zero);

            R128x2F result;
            result.v1 = row1.v1;
            result.v2 = row2.v1;
            return result;
        }

        inline R128x2F __vectorcall Mat22Inverse(R128x2F mat22)
        {
            R128x1F mat22OneRow, row1, row2;
            row1.v1 = mat22.v1;
            row2.v1 = mat22.v2;
            R128x1F det = Mat22Determinant(mat22);
            if(VecMask(VecInBounds(det, R128x1F_One(1.5e-5f))) != 0xF)
            {
                return R128x2F_Zero();
            }

            mat22OneRow = VecShuffle<1, 0, 1, 0>(row2,row1);//2.y,2.x,1.y,1.x
            mat22OneRow = VecXor(mat22OneRow, R128x1F_Set(0.f, g_negZero,g_negZero, 0.f));
            mat22OneRow = VecDiv(mat22OneRow, det);

            R128x2F result;
            row1 = VecShuffle<0, 2, 0, 0>(mat22OneRow,g_Zero);
            row2 = VecShuffle<2, 3, 0, 0>(mat22OneRow, g_Zero);
            result.v1 = row1.v1;
            result.v2 = row2.v1;
            return result;
        }

        inline R128x2F __vectorcall Mat22Multiply(R128x2F a, R128x2F b)
        {
            R128x1F matA_1, matA_2, matB_1, matB_2;
            matA_1.v1 = a.v1;
            matA_2.v1 = a.v2;

            b = Mat22Transpose(b);
            matB_1.v1 = b.v1;
            matB_2.v1 = b.v2;

            R128x1F resMat_1, resMat_2;

            resMat_1 = VecDP<0x33>(matA_1, matB_1);
            resMat_1 = VecDP<0xFC>(matA_1, matB_2);

            resMat_2 = VecDP<0x33>(matA_2, matB_1);
            resMat_2 = VecDP<0xFC>(matA_2,matB_2);

           R128x2F result;

           result.v1 = resMat_1.v1;
           result.v2 = resMat_2.v1;

           return result;
        }

        inline R128x2F __vectorcall Mat22MultiplyTrasnpose(R128x2F a, R128x2F b)
        {
            return Mat22Transpose(Mat22Multiply(a,b));
        }

        inline R128x2F __vectorcall Mat22Rotation(float angle)
        {
            R128x1F reg_angle, sin, cos,mat22OneRow, row1, row2;
            reg_angle = R128x1F_One(angle);
            VecSinCos(reg_angle,&sin,&cos);
            mat22OneRow = VecBlend<0b1001>(sin, cos);
            mat22OneRow = VecXor(mat22OneRow, R128x1F_Set(0.f, 0.f, g_negZero, 0.f));
            row1 = VecShuffle<0,1,0,0>(mat22OneRow, g_Zero);
            row2 = VecShuffle<2, 3, 0, 0>(mat22OneRow, g_Zero);

            R128x2F result;

            result.v1 = row1.v1;
            result.v2 = row2.v1;

            return result;
        }

        inline R128x2F __vectorcall Mat22Scale(float scaleX, float scaleY)
        {
            return Mat22Set(scaleX, 0.f, 0.f, scaleY);
        }

        inline R128x2F __vectorcall Mat22ScaleFromVector(R128x1F scale)
        {
            R128x1F row1, row2;
            row1 = VecBlend<0b0111>(scale, g_Zero);
            row2 = VecBlend<0b1011>(scale, g_Zero);

            R128x2F result;

            result.v1 = row1.v1;
            result.v2 = row2.v1;

            return result;
        }

        inline R128x2F __vectorcall Mat22Set(float m11, float m12, float m21, float m22)
        {
           return R128x2F_Set(m11, m12, 0.f, 0.f, m21, m22, 0.f, 0.f);
        }

        inline R128x2F __vectorcall Mat22Skew(float skewXY, float skewYX)
        {
            return Mat22Set(0.f, skewXY, skewYX, 0.f);
        }

        inline R128x2F __vectorcall Mat22SkewFromVector(R128x1F skewVec2)
        {
            R128x1F row1, row2;
            skewVec2 = VecPermute<1, 0, 2, 3>(skewVec2);
            row1 = VecBlend<0b1011>(skewVec2, g_Zero);
            row2 = VecBlend<0b0111>(skewVec2, g_Zero);

            R128x2F result;

            result.v1 = row1.v1;
            result.v2 = row2.v1;

            return result;
        }
        inline bool __vectorcall Mat22IsIdentity(R128x2F mat22)
        {
           return Mat22Equal(mat22,Mat22Identity());
        }
        inline bool __vectorcall Mat22IsNaN(R128x2F mat22)
        {
            R128x1F row1, row2;
            row1.v1 = mat22.v1;
            row2.v1 = mat22.v2;

            return Vec2IsNaN(row1) or Vec2IsNaN(row2);
        }
        inline bool __vectorcall Mat22IsInf(R128x2F mat22)
        {
            R128x1F row1, row2;
            row1.v1 = mat22.v1;
            row2.v1 = mat22.v2;

            return Vec2IsInf(row1) or Vec2IsInf(row2);
        }
        inline bool __vectorcall Mat22Equal(R128x2F a, R128x2F b)
        {
            R128x1F aRow1, aRow2, bRow1, bRow2;
            aRow1.v1 = a.v1;
            aRow2.v1 = a.v2;
            bRow1.v1 = b.v1;
            bRow2.v1 = b.v2;

            aRow1 = VecEqual(aRow1, bRow1);
            aRow2 = VecEqual(aRow2, bRow2);

            return VecMask(VecAnd(aRow1, aRow2)) == 0xF;

        }
        inline bool __vectorcall Mat22NotEqual(R128x2F a, R128x2F b)
        {
            R128x1F aRow1, aRow2, bRow1, bRow2;
            aRow1.v1 = a.v1;
            aRow2.v1 = a.v2;
            bRow1.v1 = b.v1;
            bRow2.v1 = b.v2;

            aRow1 = VecNotEqual(aRow1, bRow1);
            aRow2 = VecNotEqual(aRow2, bRow2);

            return VecMask(VecOr(aRow1, aRow2)) != 0.f;
        }


    } // namespace SIMDMath
} // namespace krystallic
