/*
*  Authors:  Leonid (@LeoParD) Parmacli  &&  Victor (@RisovoePole) Anisimov
*
*  Description:
*
*  Date: 05.01.2026
*/


#pragma once


#include "math_library.h"


namespace krystallic
{
    namespace SIMDMath
    {
        inline CMatrix4x3::CMatrix4x3() : 
            _11(1.0f), _12(0.0f), _13(0.0f), 
            _21(0.0f), _22(1.0f), _23(0.0f), 
            _31(0.0f), _32(0.0f), _33(1.0f), 
            _41(0.0f), _42(0.0f), _43(0.0f)
        {
        }


        inline CMatrix4x3::CMatrix4x3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33, float m41, float m42, float m43) : 
            _11(m11), _12(m12), _13(m13), 
            _21(m21), _22(m22), _23(m23), 
            _31(m31), _32(m32), _33(m33), 
            _41(m41), _42(m42), _43(m43) 
        {
        }


        inline CMatrix4x3::CMatrix4x3(const float* arr)
        {
            *this = arr;
        }


        inline CMatrix4x3::CMatrix4x3(const CMatrix4x3& copy)
        {
            *this = copy;
        }


        inline CMatrix4x3 CMatrix4x3::operator + () const
        {
            return *this;
        }


        inline CMatrix4x3 CMatrix4x3::operator - () const
        {
            return CMatrix4x3
            (
                -_11, -_12, -_13,
                -_21, -_22, -_23,
                -_31, -_32, -_33,
                -_41, -_42, -_43
            );
        }


        inline CMatrix4x3& CMatrix4x3::operator =(const CMatrix4x3& other)
        {
            _11 = other._11;
            _12 = other._12;
            _13 = other._13;

            _21 = other._21;
            _22 = other._22;
            _23 = other._23;

            _31 = other._31;
            _32 = other._32;
            _33 = other._33;

            _41 = other._41;
            _42 = other._42;
            _43 = other._43;

            return *this;
        }


        inline CMatrix4x3& CMatrix4x3::operator =(const float* arr)
        {
            _11 = arr[0];
            _12 = arr[1];
            _13 = arr[2];

            _21 = arr[3];
            _22 = arr[4];
            _23 = arr[5];

            _31 = arr[6];
            _32 = arr[7];
            _33 = arr[8];

            _41 = arr[9];
            _42 = arr[10];
            _43 = arr[11];

            return *this;
        }
    }
}