/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors:  @leopard-bf187 && @RisovoePole
*
*  Description:
*
*  Date: 09.07.2026
*/


#pragma once


#include "math_library.h"


namespace krystallic
{
    namespace SIMDMath
    {
        inline CMatrix2x2::CMatrix2x2() : 
            CMatrix2x2(1.0f, 0.0f, 0.0f, 1.0f)
        {
        }


        inline CMatrix2x2::CMatrix2x2(const float* arr) :
            CMatrix2x2(arr ? arr[0] : 1.0f, arr ? arr[1] : 0.0f, arr ? arr[2] : 0.0f, arr ? arr[3] : 1.0f)
        {
        }


        inline CMatrix2x2::CMatrix2x2(float m11, float m12, float m21, float m22) :
            _11(m11), _12(m12), _21(m21), _22(m22)
        {
        }


        inline CMatrix2x2::CMatrix2x2(const CMatrix2x2& copy) :
            _11(copy._11), _12(copy._12), _21(copy._21), _22(copy._22)
        {
        }


        inline CMatrix2x2::operator float* ()
        {
            return v;
        }


        inline CMatrix2x2::operator const float* () const
        {
            return v;
        }


        inline CMatrix2x2 CMatrix2x2::operator + () const
        {
            return *this;
        }


        inline CMatrix2x2 CMatrix2x2::operator - () const
        {
            return CMatrix2x2(-_11, -_12, -_21, -_22);
        }


        inline CMatrix2x2 CMatrix2x2::operator +(const CMatrix2x2& other) const
        {
            return CMatrix2x2
            (
                _11 + other._11, _12 + other._12,
                _21 + other._21, _22 + other._22
            );
        }


        inline CMatrix2x2 CMatrix2x2::operator -(const CMatrix2x2& other) const
        {
            return CMatrix2x2
            (
                _11 - other._11, _12 - other._12,
                _21 - other._21, _22 - other._22
            );
        }


        inline CMatrix2x2 CMatrix2x2::operator *(const CMatrix2x2& other) const
        {
            return CMatrix2x2
            (
                _11 * other._11 + _12 * other._21,
                _11 * other._12 + _12 * other._22,

                _21 * other._11 + _22 * other._21,
                _21 * other._12 + _22 * other._22
            );
        }


        inline CMatrix2x2 CMatrix2x2::operator *(float val) const
        {
            return CMatrix2x2
            (
                _11 * val, _12 * val,
                _21 * val, _22 * val
            );
        }


        inline CMatrix2x2 CMatrix2x2::operator /(float val) const
        {
            const float inv = 1.0f / val;
            return *this * inv;
        }


        inline CMatrix2x2& CMatrix2x2::operator +=(const CMatrix2x2& other)
        {
            _11 += other._11; _12 += other._12;
            _21 += other._21; _22 += other._22;
            return *this;
        }


        inline CMatrix2x2& CMatrix2x2::operator -=(const CMatrix2x2& other)
        {
            _11 -= other._11; _12 -= other._12;
            _21 -= other._21; _22 -= other._22;
            return *this;
        }


        inline CMatrix2x2& CMatrix2x2::operator *=(const CMatrix2x2& other)
        {
            return Multiply(other);
        }


        inline CMatrix2x2& CMatrix2x2::operator *=(float val)
        {
            return Multiply(val);
        }


        inline CMatrix2x2& CMatrix2x2::operator /=(float val)
        {
            return Multiply(1.0f / val);
        }


        inline CMatrix2x2& CMatrix2x2::operator =(const CMatrix2x2& other)
        {
            _11 = other._11; _12 = other._12;
            _21 = other._21; _22 = other._22;
            return *this;
        }


        inline CMatrix2x2& CMatrix2x2::operator =(const float* arr)
        {
            _11 = arr ? arr[0] : 1.0f; _12 = arr ? arr[1] : 0.0f;
            _21 = arr ? arr[2] : 0.0f; _22 = arr ? arr[3] : 1.0f;
            return *this;
        }


        inline float CMatrix2x2::Determinant() const
        {
            return _11 * _22 - _12 * _21;
        }


        inline CMatrix2x2 CMatrix2x2::Inverse() const
        {
            const float det = Determinant();

            if (det == 0.0f)
            {
                const float nan = std::numeric_limits<float>::quiet_NaN();

                return CMatrix2x2
                (
                    nan, nan,
                    nan, nan
                );
            }

            const float invDet = 1.0f / det;

            return CMatrix2x2
            (
                _22 * invDet, -_12 * invDet,
                -_21 * invDet, _11 * invDet
            );
        }


        inline CMatrix2x2 CMatrix2x2::Transpose() const
        {
            return CMatrix2x2
            (
                _11, _21, 
                _12, _22
            );
        }


        inline CMatrix2x2& CMatrix2x2::Multiply(float val)
        {
            _11 = _11 * val; _12 = _12 * val;
            _21 = _21 * val; _22 = _22 * val;
            return *this;
        }


        inline CMatrix2x2& CMatrix2x2::Multiply(const CMatrix2x2& other)
        {
            *this = *this * other;
            return *this;
        }


        inline CMatrix2x2 CMatrix2x2::MultiplyTranspose(const CMatrix2x2& other) const
        {
            return (*this * other).Transpose();
        }


        inline bool CMatrix2x2::IsIdentity() const
        {
            bool units = (_11 == 1.0f) && (_22 == 1.0f);
            bool zeros = (_12 == 0.0f) && (_21 == 0.0f);
            return units && zeros;
        }


        inline bool CMatrix2x2::IsInf() const
        {
            return HasFloatsOneInf(v, 4);
        }


        inline bool CMatrix2x2::IsNaN() const
        {
            return HasFloatsOneNaN(v, 4);
        }


        inline CMatrix2x2 CMatrix2x2::Identity()
        {
            return CMatrix2x2();
        }


        inline CMatrix2x2 CMatrix2x2::Rotation(float angle)
        {
            const float s = sinf(angle);
            const float c = cosf(angle);

            return CMatrix2x2
            ( 
                c, s,
                -s, c
            );
        }


        inline CMatrix2x2 CMatrix2x2::Scale(float scaleX, float scaleY)
        {
            return CMatrix2x2
            (
                scaleX, 0.0f, 
                0.0f, scaleY
            );
        }


        inline CMatrix2x2 CMatrix2x2::ScaleFromVector(CVector2 scale)
        {
            return CMatrix2x2
            (
                scale.x, 0.0f, 
                0.0f, scale.y
            );
        }


        inline CMatrix2x2 CMatrix2x2::Skew(float skewXY, float skewYX)
        {
            return CMatrix2x2
            (
                0.0f, skewXY,
                skewYX, 0.0f
            );
        }


        inline CMatrix2x2 CMatrix2x2::SkewFromVector(CVector2 skewVec2)
        {
            return CMatrix2x2
            (
                0.0f, skewVec2.x,
                skewVec2.y, 0.0f
            );
        }


        inline bool operator == (const CMatrix2x2& a, const CMatrix2x2& b)
        {
            return 
                (a._11 == b._11) && (a._12 == b._12) && 
                (a._21 == b._21) && (a._22 == b._22);
        }


        inline bool operator != (const CMatrix2x2& a, const CMatrix2x2& b)
        {
            return !(a == b);
        }


        inline CMatrix2x2 operator * (float a, CMatrix2x2& b)
        {
            return b * a;
        }
    }
} // namespace krystallic


