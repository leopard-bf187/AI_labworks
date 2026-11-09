/*
*  Authors:  Leonid (@LeoParD) Parmacli  &&  Victor (@RisovoePole) Anisimov
*
*  Description:
*
*  Date: 05.01.2026
*/


#pragma once

#include "simd_library.h"
#include "constants.h"
#include <functional>


namespace krystallic
{
    namespace SIMDMath
    {
        inline bool __vectorcall Vec2InBounds(R128x1F v, R128x1F bounds)
        {
            R128x1F mask = VecInBounds(v, bounds);

            // movemask reads from right to left and writes sign bits in the same order
            return (VecMask(mask) & 0x3) == 0x3; //checks two lower bits to (0011b)
        }


        inline R128x1F __vectorcall Vec2Length(R128x1F v)
        {
            R128x1F result;

            //0011 1111: high - sum the 2 lower; low - where to store in dst vec
            result = VecDP<0x3F>(v, v);
            result = VecSqrt(result);

            return result;
        }


        inline R128x1F __vectorcall Vec2LengthSq(R128x1F v)
        {
            return VecDP<0x33>(v, v);
        }


        inline R128x1F __vectorcall Vec2Normalize(R128x1F v)
        {
            R128x1F result;
            R128x1F zeroMask = g_Zero;
            R128x1F infMask = Cast128x1IF(g_Infinity);
            R128x1F QNaN = Cast128x1IF(g_QNaN);

            R128x1F len = Vec2Length(v);
            zeroMask = VecNotEqual(zeroMask, len);
            infMask = VecNotEqual(infMask, len);
            result = VecDiv(v, len);
            result = VecAnd(result, zeroMask);

            R128x1F temp1, temp2;
            temp1 = VecAndNot(infMask, QNaN);
            temp2 = VecAnd(result, infMask);
            result = VecOr(temp1, temp2);
            result = VecBlend<0b1100>(result, zeroMask);
            return result;
        }


        inline R128x1F __vectorcall Vec2Dot(R128x1F a, R128x1F b)
        {
            return VecDP<0x33>(a, b);
        }


        inline R128x1F __vectorcall Vec2Cross(R128x1F a, R128x1F b)
        {
            R128x1F result;

            R128x1F temp1, temp2;

            temp1 = VecXor(a, R128x1F_Set(0, g_negZero, 0, 0));

            temp2 = VecPermute<1,0,0,0>(b);
            result = VecDP<0x3F>(temp1, temp2);
            return result;
        }


        inline R128x1F __vectorcall Vec2CrossCW(R128x1F a, R128x1F b)
        {
            R128x1F d = Vec2Cross(a, b);
            R128x1F temp;
            temp = VecXor(a, R128x1F_Set(g_negZero, 0, 0, 0));
            temp = VecPermute<1, 0, 0, 0>(temp);
            temp = VecMul(temp, d);
            return VecBlend<0b0011>(R128x1F_Zero(), temp);
        }


        inline R128x1F __vectorcall Vec2CrossCCW(R128x1F a, R128x1F b)
        {
            R128x1F d = Vec2Cross(a, b);
            R128x1F temp;
            temp = VecPermute<1, 0, 0, 0>(temp);
            temp = VecXor(a, R128x1F_Set(g_negZero, 0, 0, 0));
            temp = VecMul(temp, d);
            return VecBlend<0b0011>(R128x1F_Zero(), temp);
        }


        inline R128x1F __vectorcall Vec2Reflect(R128x1F v, R128x1F normal)
        {
            R128x1F result;

            result = Vec2Dot(v, normal);
            result = VecAdd(result, result);
            result = VecFNMAdd(result, normal, v);

            return result;
        }


        inline R128x1F __vectorcall Vec2Refract(R128x1F v, R128x1F normal, float refrCoeff)
        {
            R128x1F result;
            R128x1F refractCoefVec = R128x1F_One(refrCoeff);

            R128x1F temp1 = Vec2Dot(v, normal);
            R128x1F temp2 = VecFNMAdd(temp1, temp1, g_One);
            temp2 = VecMul(temp2, refractCoefVec);
            temp2 = VecFNMAdd(temp2, refractCoefVec, g_One);

            R128x1F mask = VecGreater(temp2, g_Zero);
            temp2 = VecSqrt(temp2);
            temp2 = VecFMAdd(refractCoefVec, temp1, temp2);

            result = VecMul(refractCoefVec, v);
            result = VecFNMAdd(temp2, normal, result);
            result = VecAnd(result, mask);

            return result;
        }


        inline R128x1F __vectorcall Vec2Rotate(R128x1F v, R128x2F rotationMatrix)
        {
            R128x1F temp, matRow1, matRow2;
            matRow1.v1 = rotationMatrix.v1;
            matRow2.v1 = rotationMatrix.v2;

            v = VecBlend<0b1000>(v, g_Zero);
            temp = VecMul(v, matRow1);
            return VecFMAdd(v, matRow2, temp);
        }


        inline R128x1F __vectorcall Vec2Rotate(R128x1F v, R128x1F rotationQuaternion)
        {
            R128x1F cosSin, temp;
            cosSin = VecMul(g_Two, VecPermute<2, 2, 2, 2>(rotationQuaternion));
            temp = VecPermute<2, 3, 2, 3>(rotationQuaternion);
            temp = VecXor(temp, R128x1F_Set(-0.f, 0.f, -0.f, -0.f));
            cosSin = VecMul(cosSin, temp);
            cosSin = VecAdd(cosSin, R128x1F_Set(1.f, 0.f, 1.f, 0.f));
            v = VecPermute<1, 1, 0, 0>(v);
            cosSin = VecMul(cosSin, v);
            temp = VecPermute<2, 3, 0, 0>(cosSin);
            cosSin = VecAdd(cosSin, temp);
            return VecBlend<0b1100>(cosSin, g_Zero);
        }


        inline R128x1F __vectorcall Vec2InvRotate(R128x1F v, R128x1F rotationQuaternion)
        {
            R128x1F cosSin, temp;
            cosSin = VecMul(g_Two, VecPermute<2, 2, 2, 2>(rotationQuaternion));
            temp = VecPermute<2, 3, 2, 3>(rotationQuaternion);
            temp = VecXor(temp, R128x1F_Set(-0.f, -0.f, -0.f, 0.f));
            cosSin = VecMul(cosSin, temp);
            cosSin = VecAdd(cosSin, R128x1F_Set(1.f, 0.f, 1.f, 0.f));
            v = VecPermute<1, 1, 0, 0>(v);
            cosSin = VecMul(cosSin, v);
            temp = VecPermute<2, 3, 0, 0>(cosSin);
            cosSin = VecAdd(cosSin, temp);
            return VecBlend<0b1100>(cosSin, g_Zero);
        }


        inline R128x1F __vectorcall Vec2Transform(R128x1F v, R128x4F worldMatrix)
        {
            R128x1F row0, row1, row3;

            row0.v1 = worldMatrix.v1;
            row1.v1 = worldMatrix.v2;
            row3.v1 = worldMatrix.v4;

            R128x1F temp = VecMul(VecPermute<1,1,1,1>(v), row1);
            temp = VecAdd(temp, row3);

            return VecFMAdd(VecPermute<0,0,0,0>(v), row0, temp);
        }


        inline bool __vectorcall Vec2Equal(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecEqual(a, b);
            return (VecMask(mask) & 0x3) == 0x3;
        }


        inline bool __vectorcall Vec2NotEqual(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecNotEqual(a, b);
            return (VecMask(mask) & 0x3) == 0x3;
        }


        inline bool __vectorcall Vec2Greater(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecGreater(a, b);
            return (VecMask(mask) & 0x3) == 0x3;
        }


        inline bool __vectorcall Vec2GreaterOrEqual(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecGreaterOrEqual(a, b);
            return (VecMask(mask) & 0x3) == 0x3;
        }


        inline bool __vectorcall Vec2Less(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecLess(a, b);
            return (VecMask(mask) & 0x3) == 0x3;
        }


        inline bool __vectorcall Vec2LessOrEqual(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecLessOrEqual(a, b);
            return (VecMask(mask) & 0x3) == 0x3;
        }


        inline bool __vectorcall Vec2NearEqual(R128x1F a, R128x1F b, float epsilon)
        {
            R128x1F mask = VecNearEqual(a, b, epsilon);
            return (VecMask(mask) & 0x3) == 0x3;
        }


        inline bool __vectorcall Vec2IsInf(R128x1F val)
        {
            return (VecMask(VecIsInf(val)) & 0x3) != 0;
        }


        inline bool __vectorcall Vec2IsNaN(R128x1F val)
        {
            return (VecMask(VecIsNaN(val)) & 0x3) != 0;
        }

    } // namespace SIMDMath
} // namespace krystallic
