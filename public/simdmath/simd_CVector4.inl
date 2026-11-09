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
#include "simd_registers_SSE42.h"

namespace krystallic
{
    namespace SIMDMath
    {
        inline bool __vectorcall Vec4InBounds(R128x1F v, R128x1F bounds)
        {
            return false;
        }


        inline R128x1F __vectorcall Vec4Length(R128x1F v)
        {
            return R128x1F_Zero();
        }


        inline R128x1F __vectorcall Vec4LengthSq(R128x1F v)
        {
            return VecDP<0xFF>(v,  v);
        }


        inline R128x1F __vectorcall Vec4Normalize(R128x1F v)
        {
            if(VecMask(VecEqual(v, g_Zero)))
            {
               return R128x1F_Zero();
            }

            return VecDiv(v, Vec4Length(v));
        }


        inline R128x1F __vectorcall Vec4Dot(R128x1F a, R128x1F b)
        {
            return VecDP<0xFF>(a, b);
        }


        inline R128x1F __vectorcall Vec4Cross(R128x1F a, R128x1F b, R128x1F c)
        {
            R128x1F result, temp1, temp2, temp3;

            // result = b(yxxx)*c(z,z,y,y)
            result = VecPermute<1, 0, 0, 0>(b);
            temp1 = VecPermute<2, 2, 1, 1>(c);
            result = VecMul(result, temp1);

            // result -= b(z,z,y,y)*c(y,x,x,x)
            temp1 = VecPermute<2, 2, 1, 1>(b);
            temp2 = VecPermute<1, 0, 0, 0>(c);
            result = VecFNMAdd(temp1, temp2, result);

            // result *= a(w,w,w,z)
            temp1 = VecPermute<3,3,3,2>(a);
            result = VecMul(result, temp1);
            // получили кофакторы C02 для каждой матрицы 3 на 3.

            //temp3 = b(y,x,x,x)*c(w,w,w,z);
            temp1 = VecPermute<1, 0, 0, 0>(b);
            temp2 = VecPermute<3, 3, 3, 2>(c);
            temp3 = VecMul(temp1, temp2);

            // temp3 -= b(w,w,w,z)*c(y,x,x,x)
            temp1 = VecPermute<3, 3, 3, 2>(b);
            temp2 = VecPermute<1, 0, 0, 0>(c);
            temp3 = VecFNMAdd(temp1, temp2, temp3);

            // result -= a(z,z,y,y)*temp3
            temp1 = VecPermute<2, 2, 1, 1>(a);
            result = VecFNMAdd(temp1, temp3, result);
            //получили кофакторы C01 для каждой матрицы 3 на 3 и вычели из С02

            // temp3 = b(z,z,y,y)*c(w,w,w,z)
            temp1 = VecPermute<2, 2, 1, 1>(b);
            temp2 = VecPermute<3, 3, 3, 2>(c);
            temp3 = VecMul(temp1, temp2);

            // temp3 -= b(w,w,w,z)*c(z,z,y,y)
            temp1 = VecPermute<3, 3, 3, 2>(b);
            temp2 = VecPermute<2, 2, 1, 1>(c);
            temp3 = VecFNMAdd(temp1, temp2, temp3);

            // result += a(y,x,y,y)*temp3
            temp1 = VecPermute<1, 0, 1, 1>(a);
            result = VecFMAdd(temp1, temp3, result);
            //получили кофакторы C00 для каждой матрицы 3 на 3 и сложили с (С02-C01)

            return result;
        }


        inline R128x1F __vectorcall Vec4Reflect(R128x1F v, R128x1F normal)
        {
      		R128x1F result;

            result = Vec4Dot(v, normal);
            result = VecAdd(result, result);
            result = VecFNMAdd(result, normal, v);

            return result;
        }


        inline R128x1F __vectorcall Vec4Refract(R128x1F v, R128x1F normal, float refrCoeff)
        {
            R128x1F result;
            R128x1F refractCoefVec = R128x1F_One(refrCoeff);

            R128x1F temp1 = Vec4Dot(v, normal);
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


        inline R128x1F __vectorcall Vec4Transform(R128x1F v, R128x4F worldMatrix)
        {
            R128x1F row0, row1, row2, row3;

            row0.v1 = worldMatrix.v1;
            row1.v1 = worldMatrix.v2;
            row2.v1 = worldMatrix.v3;
            row3.v1 = worldMatrix.v4;

            R128x1F temp = VecMul(VecPermute<3,3,3,3>(v), row3);
            temp = VecFMAdd(VecPermute<2,2,2,2>(v), row2, temp);
            temp = VecFMAdd(VecPermute<1,1,1,1>(v), row1, temp);

            return VecFMAdd(VecPermute<0,0,0,0>(v),row0,temp);
        }


        inline bool __vectorcall Vec4Equal(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecEqual(a, b);

            return VecMask(mask) == 0b1111;
        }


        inline bool __vectorcall Vec4NotEqual(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecNotEqual(a, b);

            return VecMask(mask) == 0b1111;
        }


        inline bool __vectorcall Vec4Greater(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecGreater(a, b);

            return VecMask(mask) == 0b1111;
        }


        inline bool __vectorcall Vec4GreaterOrEqual(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecGreaterOrEqual(a, b);

            return VecMask(mask) == 0b1111;
        }


        inline bool __vectorcall Vec4Less(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecLess(a, b);

            return VecMask(mask) == 0b1111;
        }


        inline bool __vectorcall Vec4LessOrEqual(R128x1F a, R128x1F b)
        {
            R128x1F mask = VecLessOrEqual(a, b);

            return VecMask(mask) == 0b1111;
        }


        inline bool __vectorcall Vec4NearEqual(R128x1F a, R128x1F b, float epsilon)
        {
            R128x1F mask = VecNearEqual(a, b, epsilon);

            return VecMask(mask) == 0b1111;
        }


        inline bool __vectorcall Vec4IsInf(R128x1F val)
        {
            return VecMask(VecIsInf(val)) != 0x0;
        }


        inline bool __vectorcall Vec4IsNaN(R128x1F val)
        {
            return VecMask(VecIsNaN(val)) != 0x0;
        }

    } // namespace SIMDMath
} // namespace krystallic
