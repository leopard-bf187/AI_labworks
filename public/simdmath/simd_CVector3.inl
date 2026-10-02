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
#include <cstdlib>
#include <type_traits>

namespace krystallic
{
	namespace SIMDMath
	{
		inline bool __vectorcall Vec3InBounds(R128x1F v, R128x1F bounds)
		{
			R128x1F mask = VecInBounds(v, bounds);

			return (VecMask(mask) & 0b0111) == 0b0111;
		}


		inline R128x1F __vectorcall Vec3Length(R128x1F v)
		{
			R128x1F result;

			result = VecDP<0x7F>(v, v);
			result = VecSqrt(result);

			return result;
		}


		inline R128x1F __vectorcall Vec3LengthSq(R128x1F v)
		{
			return VecDP<0x77>(v, v);
		}


		inline R128x1F __vectorcall Vec3Normalize(R128x1F v)
		{
			R128x1F zero = g_Zero;
			v = VecBlend<0b1000>(v, zero);
			R128x1F len = Vec3Length(v);
			return VecDiv(v, len);
		}


		inline R128x1F __vectorcall Vec3Dot(R128x1F a, R128x1F b)
		{
			return VecDP<0x77>(a, b);
		}


		inline R128x1F __vectorcall Vec3Cross(R128x1F a, R128x1F b)
		{
			R128x1F temp1, temp2, result;
			temp1 = VecPermute<2,0,1,3>(a); //a = z,x,y
			temp2 = VecPermute<1, 2, 0, 3>(b); //b = y,z,x

			a = VecPermute<1,2,0,3>(a); //a=y,z,x
			b = VecPermute<2,0,1,3>(b); //b=z,x,y

			result = VecMul(a, b);

			result = VecFNMAdd(temp1, temp2, result); // result - (temp1 * temp2)

			return result;
		}


		inline R128x1F __vectorcall Vec3Reflect(R128x1F v, R128x1F normal)
		{
    		R128x1F result;

            result = Vec3Dot(v, normal);
            result = VecAdd(result, result);
            result = VecFNMAdd(result, normal, v);

            return result;
		}


		inline R128x1F __vectorcall Vec3Refract(R128x1F v, R128x1F normal, float refrCoeff)
		{
    		R128x1F result;
            R128x1F refractCoefVec = R128x1F_One(refrCoeff);

            R128x1F temp1 = Vec3Dot(v, normal);
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


		inline R128x1F __vectorcall Vec3Rotate(R128x1F v, R128x3F rotationMatrix)
		{
    		R128x1F temp, matRow1, matRow2, matRow3;
            matRow1.v1 = rotationMatrix.v1;
            matRow2.v1 = rotationMatrix.v2;
            matRow3.v1 = rotationMatrix.v3;

            v = VecBlend<0b1000>(v, g_Zero);
            temp = VecMul(v, matRow1);
            temp = VecFMAdd(v, matRow2, temp);
            return VecFMAdd(v, matRow3, temp);
		}


		inline R128x1F __vectorcall Vec3Rotate(R128x1F v, R128x1F rotationQuaternion)
		{
            R128x1F temp1, temp2, temp3, xyzQuat;
            xyzQuat = VecBlend<0b1000>(rotationQuaternion, g_Zero);
            temp1 = VecMul(g_Two, Vec3Cross(xyzQuat, v));

            temp2 = VecPermute<3, 3, 3, 3>(v);
            temp2 = VecMul(temp2, temp1);
            temp3 = VecAdd(v, temp2);

            temp2 = Vec3Cross(xyzQuat, temp1);
            return VecAdd(temp3, temp2);
		}


		inline R128x1F __vectorcall Vec3InvRotate(R128x1F v, R128x1F rotationQuaternion)
		{
    		R128x1F temp1, temp2, temp3, xyzQuat;
            xyzQuat = VecBlend<0b1000>(rotationQuaternion, g_Zero);
            temp1 = VecMul(g_Two, Vec3Cross(xyzQuat, v));
            temp1 = VecNeg(temp1);

            temp2 = VecPermute<3, 3, 3, 3>(v);
            temp2 = VecMul(temp2, temp1);
            temp3 = VecAdd(v, temp2);

            temp2 = Vec3Cross(xyzQuat, temp1);
            return VecSub(temp3, temp2);
		}


		inline R128x1F __vectorcall Vec3Transform(R128x1F v, R128x4F worldMatrix)
		{
      		R128x1F x = VecPermute<0,0,0,0>(v);
            R128x1F y = VecPermute<1,1,1,1>(v);
            R128x1F z = VecPermute<2,2,2,2>(v);

            R128x1F row0, row1, row2, row3;
            row0.v1 = worldMatrix.v1;
            row1.v1 = worldMatrix.v2;
            row2.v1 = worldMatrix.v3;
            row3.v1 = worldMatrix.v4;

            R128x1F temp = VecFMAdd(y, row1, row3);
            temp = VecFMAdd(z, row2, temp);

            return VecFMAdd(x, row0, temp);
		}


		inline R128x1F __vectorcall Vec3Project(R128x1F v, R128x4F world, R128x4F view, R128x4F proj, float  ViewportX, float ViewportY,float ViewportWidth,float ViewportHeight,float ViewportMinZ,float ViewportMaxZ)
		{
    		const float HalfViewportWidth = ViewportWidth * 0.5f;
            const float HalfViewportHeight = ViewportHeight * 0.5f;

            R128x1F Scale = R128x1F_Set(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 0.0f);
            R128x1F Offset = R128x1F_Set(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

            R128x4F Transform = Mat44Multiply(world, view);
            Transform = Mat44Multiply(Transform, proj);

            // R128x1F Result = XMVector3TransformCoord(v, Transform);
            R128x1F Result, temp, w, transformRow0, transformRow1, transformRow2, transformRow3;
            transformRow0.v1 = Transform.v1;
            transformRow1.v1 = Transform.v2;
            transformRow2.v1 = Transform.v3;
            transformRow3.v1 = Transform.v4;

            Result = VecPermute<2, 2, 2, 2>(v);
            Result = VecFMAdd(Result, transformRow2, transformRow3);
            temp = VecPermute<1, 1, 1, 1>(v);
            Result = VecFMAdd(temp, transformRow1, Result);
            temp = VecPermute<0, 0, 0, 0>(v);
            Result = VecFMAdd(temp, transformRow0, Result);
            w = VecPermute<3, 3, 3, 3>(Result);
            Result = VecDiv(Result, w);

            Result = VecFMAdd(Result, Scale, Offset);

            return Result;
		}


		inline R128x1F __vectorcall Vec3UnProject(R128x1F v, R128x4F world, R128x4F view, R128x4F proj, float  ViewportX, float ViewportY,float ViewportWidth,float ViewportHeight,float ViewportMinZ,float ViewportMaxZ)
		{
		    R128x1F D = R128x1F_Set(-1.0f, 1.0f, 0.0f, 0.0f);

            R128x1F Scale = R128x1F_Set(ViewportWidth * 0.5f, -ViewportHeight * 0.5f, ViewportMaxZ - ViewportMinZ, 1.0f);
            Scale = VecDiv(g_One, Scale);

            R128x1F Offset = R128x1F_Set(-ViewportX, -ViewportY, -ViewportMinZ, 0.0f);
            Offset = VecFMAdd(Scale, Offset, D);

            R128x4F Transform = Mat44Multiply(world, view);
            Transform = Mat44Multiply(Transform, proj);
            Transform = Mat44Inverse(Transform);

            R128x1F Result = VecFMAdd(v, Scale, Offset);

            R128x1F temp, w, transformRow0, transformRow1, transformRow2, transformRow3;
            transformRow0.v1 = Transform.v1;
            transformRow1.v1 = Transform.v2;
            transformRow2.v1 = Transform.v3;
            transformRow3.v1 = Transform.v4;

            Result = VecPermute<2, 2, 2, 2>(v);
            Result = VecFMAdd(Result, transformRow2, transformRow3);
            temp = VecPermute<1, 1, 1, 1>(v);
            Result = VecFMAdd(temp, transformRow1, Result);
            temp = VecPermute<0, 0, 0, 0>(v);
            Result = VecFMAdd(temp, transformRow0, Result);
            w = VecPermute<3, 3, 3, 3>(Result);
            Result = VecDiv(Result, w);
            return Result;
		}


		inline bool __vectorcall Vec3Equal(R128x1F a, R128x1F b)
		{
			R128x1F mask = VecEqual(a, b);
			return (VecMask(mask) & 0b0111) == 0b0111;
		}


		inline bool __vectorcall Vec3NotEqual(R128x1F a, R128x1F b)
		{
			R128x1F mask = VecNotEqual(a, b);
			return (VecMask(mask) & 0b0111) == 0b0111;
		}


		inline bool __vectorcall Vec3Greater(R128x1F a, R128x1F b)
		{
			R128x1F mask = VecGreater(a, b);
			return (VecMask(mask) & 0b0111) == 0b0111;
		}


		inline bool __vectorcall Vec3GreaterOrEqual(R128x1F a, R128x1F b)
		{
			R128x1F mask = VecGreaterOrEqual(a, b);
			return (VecMask(mask) & 0b0111) == 0b0111;
		}


		inline bool __vectorcall Vec3Less(R128x1F a, R128x1F b)
		{
			R128x1F mask = VecLess(a, b);
			return (VecMask(mask) & 0b0111) == 0b0111;
		}


		inline bool __vectorcall Vec3LessOrEqual(R128x1F a, R128x1F b)
		{
			R128x1F mask = VecLessOrEqual(a, b);
			return (VecMask(mask) & 0b0111) == 0b0111;
		}


		inline bool __vectorcall Vec3NearEqual(R128x1F a, R128x1F b, float epsilon)
		{
			R128x1F mask = VecNearEqual(a, b, epsilon);
			return (VecMask(mask) & 0b0111) == 0b0111;
		}


		inline bool __vectorcall Vec3IsInf(R128x1F val)
		{
			return (VecMask(VecIsInf(val)) & 0x7) != 0;
		}


		inline bool __vectorcall Vec3IsNaN(R128x1F val)
		{
			return VecMask(VecIsNaN(val)) != 0x0;
		}

	} // namespace SIMDMath
} // namespace krystallic
