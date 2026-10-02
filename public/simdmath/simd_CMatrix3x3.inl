/*
*  Authors:  Leonid (@LeoParD) Parmacli  &&  Victor (@RisovoePole) Anisimov
*
*  Description:
*
*  Date: 05.01.2026
*/


#pragma once


#include "declaration.h"
#include "constants.h"
#include "simd_library.h"
#include "simd_registers_SSE42.h"


namespace krystallic
{
    namespace SIMDMath
    {
        inline R128x3F __vectorcall Mat33Identity()
        {
            return R128x3F_Set(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
        }
        inline R128x1F __vectorcall Mat33Determinant(R128x3F mat33)
        {
            R128x1F row1, row2, row3, temp1, temp2;
            row1.v1 = mat33.v1;
            row2.v1 = mat33.v2;
            row3.v1 = mat33.v3;

            // 11 12 13
            // 22 23 21
            // 33 31 32
            row2 = VecPermute<1, 2, 0, 3>(row2);
            row3 = VecPermute<2, 0, 1, 3>(row3);
            temp1 = VecMul(row1, row2);
            temp1 = VecDP<0x7F>(temp1, row3);

            //reset
            row1.v1 = mat33.v1;
            row2.v1 = mat33.v2;
            row3.v1 = mat33.v3;

            // 11 12 13
            // 23 21 22
            // 32 33 31
            row2 = VecPermute<2, 0, 1, 3>(row2);
            row3 = VecPermute<1, 2, 0, 3>(row3);
            temp2 = VecMul(row1, row2);
            temp2 = VecDP<0x7F>(temp2, row3);

            return VecSub(temp1,temp2);
        }
        inline R128x3F __vectorcall Mat33Transpose(R128x3F mat33)
        {
            R128x1F row1, row2, row3, temp1, temp2, temp3;
            row1.v1 = mat33.v1;
            row2.v1 = mat33.v2;
            row3.v1 = mat33.v3;

            temp1 = VecMergeLow(row1, row2);//1.x, 2.x, 1.y, 2.y
            temp2 = VecMergeHigh(row1, row2);//1.z, 2.z, 1.w, 2.w
            temp3 = VecMergeLow(row3, g_Zero);//3.x, 0, 3.y, 0

            R128x1F ResMatRow1, ResMatRow2,ResMatRow3;

            ResMatRow1 = VecShuffle<0,1,0,1>(temp1, temp3);
            ResMatRow2 = VecShuffle<2,3,2,3>(temp1, temp3);
            ResMatRow3 = VecShuffle<0,2,1,1>(temp2, temp3);

            ResMatRow3 = VecBlend<0b0100>(ResMatRow3, row3);

            R128x3F result;
            result.v1 = ResMatRow1.v1;
            result.v2 = ResMatRow2.v1;
            result.v3 = ResMatRow3.v1;

            return result;
        }
        inline R128x3F __vectorcall Mat33Inverse(R128x3F mat33)
        {
            R128x1F row1, row2, row3;
            row1.v1 = mat33.v1;
            row2.v1 = mat33.v2;
            row3.v1 = mat33.v3;


            R128x1F c0, c1, c2, det;

            c0 = Vec3Cross(row2, row3);
            c1 = Vec3Cross(row3, row1);
            c2 = Vec3Cross(row1, row2);

            det = VecDP<0x7F>(row1, c0);

            if(VecMask(VecInBounds(det, R128x1F_One(1.5e-5f))) != 0xF)
            {
                return R128x3F_Zero();
            }

            R128x3F result;

            result.v1 = VecDiv(c0, det).v1;
            result.v2 = VecDiv(c1, det).v1;
            result.v3 = VecDiv(c2, det).v1;

            return Mat33Transpose(result);
        }
        inline R128x3F __vectorcall Mat33Multiply(R128x3F a, R128x3F b)
        {
            R128x1F ARow1, ARow2, ARow3, BRow1, BRow2, BRow3, rowElem1, rowElem2, rowElem3;

            ARow1.v1 = a.v1;
            ARow2.v1 = a.v2;
            ARow3.v1 = a.v3;

            BRow1.v1 = a.v1;
            BRow2.v1 = a.v2;
            BRow3.v1 = a.v3;

            R128x1F resMatRow1, resMatRow2, resMatRow3;

            rowElem1 = VecPermute<0,0,0,0>(ARow1);
            rowElem2 = VecPermute<1,1,1,1>(ARow1);
            rowElem3 = VecPermute<2,2,2,2>(ARow1);

            resMatRow1 = VecMul(rowElem1, BRow1);
            resMatRow1 = VecFMAdd(rowElem2, BRow2, resMatRow1);
            resMatRow1 = VecFMAdd(rowElem3, BRow3, resMatRow1);

            rowElem1 = VecPermute<0,0,0,0>(ARow2);
            rowElem2 = VecPermute<1,1,1,1>(ARow2);
            rowElem3 = VecPermute<2,2,2,2>(ARow2);

            resMatRow2 = VecMul(rowElem1, BRow1);
            resMatRow2 = VecFMAdd(rowElem2, BRow2, resMatRow2);
            resMatRow2 = VecFMAdd(rowElem3, BRow3, resMatRow2);

            rowElem1 = VecPermute<0,0,0,0>(ARow3);
            rowElem2 = VecPermute<1,1,1,1>(ARow3);
            rowElem3 = VecPermute<2,2,2,2>(ARow3);

            resMatRow3 = VecMul(rowElem1, BRow1);
            resMatRow3 = VecFMAdd(rowElem2, BRow2, resMatRow3);
            resMatRow3 = VecFMAdd(rowElem3, BRow3, resMatRow3);

            resMatRow1 = VecBlend<0b1000>(resMatRow1, g_Zero);
            resMatRow2 = VecBlend<0b1000>(resMatRow2, g_Zero);
            resMatRow3 = VecBlend<0b1000>(resMatRow3, g_Zero);

            R128x3F result;

            result.v1 = resMatRow1.v1;
            result.v2 = resMatRow2.v1;
            result.v3 = resMatRow3.v1;

            return result;
        }
        inline R128x3F __vectorcall Mat33MultiplyTrasnpose(R128x3F a, R128x3F b)
        {
            return Mat33Transpose(Mat33Multiply(a, b));
        }
        inline R128x3F __vectorcall Mat33RotationX(float angle)
        {
            R128x1F sin, cos, temp1;
            VecSinCos(R128x1F_One(angle), &sin, &cos);

            temp1 = VecShuffle<0, 0, 0, 0>(sin, cos);
            R128x1F resMatRow1, resMatRow2, resMatRow3;

            resMatRow1 = g_IdentityR0;
            resMatRow3 = VecBlend<0b1001>(temp1, g_Zero);
            resMatRow2 = VecPermute<0, 2, 1, 3>(resMatRow3);
            resMatRow2 = VecXor(resMatRow2, Cast128x1IF(g_FlipZ));

            R128x3F result;

            result.v1 = resMatRow1.v1;
            result.v2 = resMatRow2.v1;
            result.v3 = resMatRow3.v1;

            return result;
        }
        inline R128x3F __vectorcall Mat33RotationY(float angle)
        {
            R128x1F sin, cos, temp1;
            VecSinCos(R128x1F_One(angle), &sin, &cos);

            temp1 = VecShuffle<0, 0, 0, 0>(cos, sin);
            R128x1F resMatRow1, resMatRow2, resMatRow3;

            resMatRow2 = g_IdentityR1;
            resMatRow1 = VecBlend<0b0101>(temp1, g_Zero);
            resMatRow3 = VecPermute<2, 1, 0, 3>(resMatRow1);
            resMatRow3 = VecXor(resMatRow3, Cast128x1IF(g_FlipX));

            R128x3F result;

            result.v1 = resMatRow1.v1;
            result.v2 = resMatRow2.v1;
            result.v3 = resMatRow3.v1;

            return result;
        }
        inline R128x3F __vectorcall Mat33RotationZ(float angle)
        {
            R128x1F sin, cos, temp1;
            VecSinCos(R128x1F_One(angle), &sin, &cos);

            temp1 = VecMergeHigh(sin, cos);
            R128x1F resMatRow1, resMatRow2, resMatRow3;

            resMatRow3 = g_IdentityR2;
            resMatRow2 = VecShuffle<0,1,0,0>(temp1, g_Zero);
            resMatRow1 = VecPermute<1, 0, 2, 3>(resMatRow2);
            resMatRow1 = VecXor(resMatRow1, Cast128x1IF(g_FlipY));

            R128x3F result;

            result.v1 = resMatRow1.v1;
            result.v2 = resMatRow2.v1;
            result.v3 = resMatRow3.v1;

            return result;
        }
        inline R128x3F __vectorcall Mat33Scale(float scaleX, float scaleY, float scaleZ)
        {
            return Mat33ScaleFromVector(R128x1F_Set(scaleX, scaleY, scaleZ, 0.f));
        }
        inline R128x3F __vectorcall Mat33ScaleFromVector(R128x1F scaleVec3)
        {
            R128x1F resMatRow1, resMatRow2, resMatRow3;

            resMatRow1 = VecPermute<0, 3, 3, 3>(scaleVec3);
            resMatRow2 = VecPermute<3, 1, 3, 3>(scaleVec3);
            resMatRow3 = VecPermute<3, 3, 2, 3>(scaleVec3);

            R128x3F result;

            result.v1 = resMatRow1.v1;
            result.v2 = resMatRow2.v1;
            result.v3 = resMatRow3.v1;

            return result;
        }
        inline R128x3F __vectorcall Mat33Set(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
        {
            R128x1F resMatRow1, resMatRow2, resMatRow3;
            resMatRow1 = R128x1F_Set(m11, m12, m13, 0.f);
            resMatRow2 = R128x1F_Set(m21, m22, m23, 0.f);
            resMatRow3 = R128x1F_Set(m31, m32, m33, 0.f);

            R128x3F result;

            result.v1 = resMatRow1.v1;
            result.v2 = resMatRow2.v1;
            result.v3 = resMatRow3.v1;

            return result;
        }
        inline R128x3F __vectorcall Mat33Skew(float skewXY, float skewXZ, float skewYX, float skewYZ, float skewZX, float skewZY)
        {
            R128x1F resMatRow1, resMatRow2, resMatRow3;
            resMatRow1 = R128x1F_Set(1.f, skewXY, skewXZ, 0.f);
            resMatRow2 = R128x1F_Set(skewYX, 1.f, skewYZ, 0.f);
            resMatRow3 = R128x1F_Set(skewZX, skewZY, 1.f, 0.f);

            R128x3F result;

            result.v1 = resMatRow1.v1;
            result.v2 = resMatRow2.v1;
            result.v3 = resMatRow3.v1;

            return result;
        }
        inline R128x3F __vectorcall Mat33SkewFromVector(R128x1F skewX, R128x1F skewY, R128x1F skewZ)
        {
            R128x1F resMatRow1, resMatRow2, resMatRow3;
            resMatRow1 = VecBlend<0b1001>(skewX, g_IdentityR0);
            resMatRow2 = VecBlend<0b0101>(skewY, g_IdentityR1);
            resMatRow3 = VecShuffle<0, 1, 0, 1>(skewZ, g_IdentityR0);

            R128x3F result;

            result.v1 = resMatRow1.v1;
            result.v2 = resMatRow2.v1;
            result.v3 = resMatRow3.v1;

            return result;
        }
        inline R128x3F __vectorcall Mat33Translation(float translateX, float translateY)
        {
            R128x1F resMatRow1, resMatRow2, resMatRow3;
            resMatRow1 = R128x1F_Set(1.f, 0.f, translateX, 0.f);
            resMatRow2 = R128x1F_Set(0.f, 1.f, translateY, 0.f);
            resMatRow3 = g_IdentityR2;

            R128x3F result;

            result.v1 = resMatRow1.v1;
            result.v2 = resMatRow2.v1;
            result.v3 = resMatRow3.v1;

            return result;
        }
        inline R128x3F __vectorcall Mat33TranslationFromVector(R128x1F translationVec2)
        {
            R128x1F resMatRow1, resMatRow2, resMatRow3;
            resMatRow1 = VecShuffle<0, 1, 0, 3>(g_IdentityR0, translationVec2);
            resMatRow2 = VecShuffle<0, 1, 1, 3>(g_IdentityR1, translationVec2);
            resMatRow3 = g_IdentityR2;

            R128x3F result;

            result.v1 = resMatRow1.v1;
            result.v2 = resMatRow2.v1;
            result.v3 = resMatRow3.v1;

            return result;
        }

        inline R128x3F __vectorcall Mat33AffineTransformation2D(R128x1F scale, R128x1F rotationOrigin, float rotationAngle, R128x1F translation)
        {
            R128x1F temp1, temp2, temp3;
            R128x3F resMatrix, rotationMatrix;
            temp1 = VecBlend<0b1100>(scale, g_One);
            resMatrix = Mat33ScaleFromVector(temp1);
            temp2 = VecBlend<0b1100>(rotationOrigin, g_One);

            rotationMatrix = Mat33RotationZ(rotationAngle);
            temp3 = VecBlend<0b1100>(translation, g_One);
            R128x1F matRow3;

            matRow3.v1 = resMatrix.v3;

            matRow3 = VecSub(matRow3, temp2);
            resMatrix.v3 = matRow3.v1;

            resMatrix = Mat33Multiply(resMatrix, rotationMatrix);
            matRow3.v1 = resMatrix.v3;
            matRow3 = VecAdd(matRow3, temp2);
            matRow3 = VecAdd(matRow3, temp3);
            resMatrix.v3 = matRow3.v1;

            return resMatrix;
        }
        inline R128x3F __vectorcall Mat33Transformation2D(R128x1F scaleOrigin, R128x1F scaleOrientation, R128x1F scale, R128x1F rotationOrigin, float rotationAngle, R128x1F translation)
        {
            // M = Inverse(MScalingOrigin) * Transpose(MScalingOrientation) * MScaling * MScalingOrientation *
            //MScalingOrigin * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

            // M= T(-SO) * R(-SOAngle) * S * R(SOAngle)
            // * T(SO) * T(-RO) * R(Rotation) * T(RO) * T(T)

            // M = T(-SO) * R(-SOAngle);   // 1
            // M = M * S;                  // 2
            // M = M * R(SOAngle);         // 3

            // M.r[3] += SO - RO;

            // M = M * R(Rotation);        // 4

            // M.r[3] += RO + Translation;

            R128x1F temp1;
            R128x3F resMat;

            resMat = Mat33Multiply(Mat33TranslationFromVector(VecNeg(scaleOrigin)), Mat33RotationZ(-VecGetX(scaleOrientation)));
            resMat = Mat33Multiply(resMat, Mat33ScaleFromVector(scale));
            resMat = Mat33Multiply(resMat, Mat33RotationZ(VecGetX(scaleOrientation)));

            temp1.v1 = resMat.v3;
            temp1 = VecAdd(temp1, VecSub(scaleOrigin, rotationOrigin));
            resMat.v3 = temp1.v1;

            resMat = Mat33Multiply(resMat, Mat33RotationZ(rotationAngle));

            temp1.v1 = resMat.v3;
            temp1 = VecAdd(temp1, VecAdd(rotationOrigin, translation));
            resMat.v3 = temp1.v1;

            return resMat;
        }

        inline bool __vectorcall Mat33IsIdentity(R128x3F mat33)
        {
            return Mat33Equal(mat33, Mat33Identity());
        }
        inline bool __vectorcall Mat33IsNaN(R128x3F mat33)
        {
            R128x1F row1, row2, row3;
            row1.v1 = mat33.v1;
            row2.v1 = mat33.v2;
            row3.v1 = mat33.v3;

           return Vec3IsNaN(row1) or Vec3IsNaN(row2) or Vec3IsNaN(row3);
        }
        inline bool __vectorcall Mat33IsInf(R128x3F mat33)
        {
            R128x1F row1, row2, row3;
            row1.v1 = mat33.v1;
            row2.v1 = mat33.v2;
            row3.v1 = mat33.v3;

           return Vec3IsInf(row1) or Vec3IsInf(row2) or Vec3IsInf(row3);
        }


        inline bool __vectorcall Mat33Equal(R128x3F a, R128x3F b)
        {
            R128x1F aRow1, aRow2, aRow3, bRow1, bRow2, bRow3;
            aRow1.v1 = a.v1;
            aRow2.v1 = a.v2;
            aRow3.v1 = a.v3;
            bRow1.v1 = b.v1;
            bRow2.v1 = b.v2;
            bRow3.v1 = b.v3;

            aRow1 = VecEqual(aRow1, bRow1);
            aRow2 = VecEqual(aRow2, bRow2);
            aRow3 = VecEqual(aRow3, bRow3);

            aRow1 = VecAnd(aRow1, aRow2);
            aRow1 = VecAnd(aRow1, aRow3);

            return VecMask(aRow1) == 0xF;
        }
        inline bool __vectorcall Mat33NotEqual(R128x3F a, R128x3F b)
        {
            R128x1F aRow1, aRow2, aRow3, bRow1, bRow2, bRow3;
            aRow1.v1 = a.v1;
            aRow2.v1 = a.v2;
            aRow3.v1 = a.v3;
            bRow1.v1 = b.v1;
            bRow2.v1 = b.v2;
            bRow3.v1 = b.v3;

            aRow1 = VecNotEqual(aRow1, bRow1);
            aRow2 = VecNotEqual(aRow2, bRow2);
            aRow3 = VecNotEqual(aRow3, bRow3);

            aRow1 = VecOr(aRow1, aRow2);
            aRow1 = VecOr(aRow1, aRow3);

            return VecMask(aRow1) != 0.f;
        }
    }
} // namespace krystallic
