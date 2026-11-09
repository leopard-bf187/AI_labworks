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
        inline CMatrix3x3::CMatrix3x3() : 
            _11(1.0f), _12(0.0f), _13(0.0f), 
            _21(0.0f), _22(1.0f), _23(0.0f), 
            _31(0.0f), _32(0.0f), _33(1.0f)
        {
        }


        inline CMatrix3x3::CMatrix3x3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33) : 
            _11(m11), _12(m12), _13(m13), 
            _21(m21), _22(m22), _23(m23), 
            _31(m31), _32(m32), _33(m33)
        {
        }


        inline CMatrix3x3::CMatrix3x3(const float* arr) :
            _11(arr ? arr[0] : 1.0f), _12(arr ? arr[1] : 0.0f), _13(arr ? arr[2] : 0.0f),
            _21(arr ? arr[3] : 0.0f), _22(arr ? arr[4] : 1.0f), _23(arr ? arr[5] : 0.0f),
            _31(arr ? arr[6] : 0.0f), _32(arr ? arr[7] : 0.0f), _33(arr ? arr[8] : 1.0f)
        {
        }


        inline CMatrix3x3::CMatrix3x3(const CMatrix3x3& copy) :
            _11(copy._11), _12(copy._12), _13(copy._13),
            _21(copy._21), _22(copy._22), _23(copy._23),
            _31(copy._31), _32(copy._32), _33(copy._33)
        {
        }


        inline CMatrix3x3::operator float* ()
        {
            return v;
        }


        inline CMatrix3x3::operator const float* () const
        {
            return v;
        }


        inline CMatrix3x3 CMatrix3x3::operator + () const
        {
            return *this;
        }


        inline CMatrix3x3 CMatrix3x3::operator - () const
        {
            return CMatrix3x3
            (
                -_11, -_12, -_13,
                -_21, -_22, -_23,
                -_31, -_32, -_33
            );
        }


        inline CMatrix3x3 CMatrix3x3::operator +(const CMatrix3x3& other) const
        {
            return CMatrix3x3
            (
                _11 + other._11, _12 + other._12, _13 + other._13,
                _21 + other._21, _22 + other._22, _23 + other._23,
                _31 + other._31, _32 + other._32, _33 + other._33
            );
        }


        inline CMatrix3x3 CMatrix3x3::operator -(const CMatrix3x3& other) const
        {
            return CMatrix3x3
            (
                _11 - other._11, _12 - other._12, _13 - other._13,
                _21 - other._21, _22 - other._22, _23 - other._23,
                _31 - other._31, _32 - other._32, _33 - other._33
            );
        }


        inline CMatrix3x3 CMatrix3x3::operator *(const CMatrix3x3& other) const
        {
            return CMatrix3x3
            (
                _11 * other._11 + _12 * other._21 + _13 * other._31,
                _11 * other._12 + _12 * other._22 + _13 * other._32,
                _11 * other._13 + _12 * other._23 + _13 * other._33,

                _21 * other._11 + _22 * other._21 + _23 * other._31,
                _21 * other._12 + _22 * other._22 + _23 * other._32,
                _21 * other._13 + _22 * other._23 + _23 * other._33,

                _31 * other._11 + _32 * other._21 + _33 * other._31,
                _31 * other._12 + _32 * other._22 + _33 * other._32,
                _31 * other._13 + _32 * other._23 + _33 * other._33
            );
        }


        inline CMatrix3x3 CMatrix3x3::operator *(float val) const
        {
            return CMatrix3x3
            (
                _11 * val, _12 * val, _13 * val,
                _21 * val, _22 * val, _23 * val,
                _31 * val, _32 * val, _33 * val
            );
        }


        inline CMatrix3x3 CMatrix3x3::operator /(float val) const
        {
            const float inv = 1.0f / val;
            return *this * inv;
        }


        inline CMatrix3x3& CMatrix3x3::operator +=(const CMatrix3x3& other)
        {
            _11 += other._11; _12 += other._12; _13 += other._13;
            _21 += other._21; _22 += other._22; _23 += other._23;
            _31 += other._31; _32 += other._32; _33 += other._33;
            return *this;
        }


        inline CMatrix3x3& CMatrix3x3::operator -=(const CMatrix3x3& other)
        {
            _11 -= other._11; _12 -= other._12; _13 -= other._13;
            _21 -= other._21; _22 -= other._22; _23 -= other._23;
            _31 -= other._31; _32 -= other._32; _33 -= other._33;
            return *this;
        }


        inline CMatrix3x3& CMatrix3x3::operator *=(const CMatrix3x3& other)
        {
            return Multiply(other);
        }


        inline CMatrix3x3& CMatrix3x3::operator *=(float val)
        {
            return Multiply(val);
        }


        inline CMatrix3x3& CMatrix3x3::operator /=(float val)
        {
            return Multiply(1.0f / val);
        }


        inline CMatrix3x3& CMatrix3x3::operator =(const CMatrix3x3& other)
        {
            _11 = other._11; _12 = other._12; _13 = other._13;
            _21 = other._21; _22 = other._22; _23 = other._23;
            _31 = other._31; _32 = other._32; _33 = other._33;
            return *this;
        }


        inline CMatrix3x3& CMatrix3x3::operator =(const float* arr)
        {
            _11 = arr ? arr[0] : 1.0f; _12 = arr ? arr[1] : 0.0f; _13 = arr ? arr[2] : 0.0f;
            _21 = arr ? arr[3] : 0.0f; _22 = arr ? arr[4] : 1.0f; _23 = arr ? arr[5] : 0.0f;
            _31 = arr ? arr[6] : 0.0f; _32 = arr ? arr[7] : 0.0f; _33 = arr ? arr[8] : 1.0f;
            return *this;
        }


        inline float CMatrix3x3::Determinant() const
        {
            return
                _11 * (_22 * _33 - _23 * _32) -
                _12 * (_21 * _33 - _23 * _31) +
                _13 * (_21 * _32 - _22 * _31);
        }


        inline CMatrix3x3 CMatrix3x3::Inverse() const
        {
            const float det = Determinant();

            if (det == 0.0f)
            {
                const float nan = std::numeric_limits<float>::quiet_NaN();

                return CMatrix3x3
                (
                    nan, nan, nan,
                    nan, nan, nan,
                    nan, nan, nan
                );
            }

            const float invDet = 1.0f / det;

            return CMatrix3x3
            (
                (_22 * _33 - _23 * _32) * invDet,
                (_13 * _32 - _12 * _33) * invDet,
                (_12 * _23 - _13 * _22) * invDet,

                (_23 * _31 - _21 * _33) * invDet,
                (_11 * _33 - _13 * _31) * invDet,
                (_13 * _21 - _11 * _23) * invDet,

                (_21 * _32 - _22 * _31) * invDet,
                (_12 * _31 - _11 * _32) * invDet,
                (_11 * _22 - _12 * _21) * invDet
            );
        }


        inline CMatrix3x3 CMatrix3x3::Transpose() const
        {
            return CMatrix3x3
            (
                _11, _21, _31,
                _12, _22, _32,
                _13, _23, _33
            );
        }


        inline CMatrix3x3& CMatrix3x3::Multiply(float val)
        {
            _11 *= val; _12 *= val; _13 *= val;
            _21 *= val; _22 *= val; _23 *= val;
            _31 *= val; _32 *= val; _33 *= val;
            return *this;
        }


        inline CMatrix3x3& CMatrix3x3::Multiply(const CMatrix3x3& other)
        {
            *this = *this * other;
            return *this;
        }


        inline CMatrix3x3& CMatrix3x3::MultiplyTranspose(const CMatrix3x3& other)
        {
            *this = *this * other.Transpose();
            return *this;
        }


        inline bool CMatrix3x3::IsIdentity()
        {
            return
                _11 == 1.0f && _12 == 0.0f && _13 == 0.0f &&
                _21 == 0.0f && _22 == 1.0f && _23 == 0.0f &&
                _31 == 0.0f && _32 == 0.0f && _33 == 1.0f;
        }


        inline bool CMatrix3x3::IsInf()
        {
            return HasFloatsOneInf(v, 9);
        }


        inline bool CMatrix3x3::IsNaN()
        {
            return HasFloatsOneNaN(v, 9);
        }


        inline CMatrix3x3 CMatrix3x3::Identity()
        {
            return CMatrix3x3();
        }


        inline CMatrix3x3 CMatrix3x3::RotationX(float angle)
        {
            const float s = sinf(angle);
            const float c = cosf(angle);

            return CMatrix3x3(
                1.0f, 0.0f, 0.0f,
                0.0f, c, s,
                0.0f, -s, c
            );
        }


        inline CMatrix3x3 CMatrix3x3::RotationY(float angle)
        {
            const float s = sinf(angle);
            const float c = cosf(angle);

            return CMatrix3x3(
                c, 0.0f, -s,
                0.0f, 1.0f, 0.0f,
                s, 0.0f, c
            );
        }


        inline CMatrix3x3 CMatrix3x3::RotationZ(float angle)
        {
            const float s = sinf(angle);
            const float c = cosf(angle);

            return CMatrix3x3
            (
                c, s, 0.0f,
                -s, c, 0.0f,
                0.0f, 0.0f, 1.0f
            );
        }


        inline CMatrix3x3 CMatrix3x3::Scale(float scaleX, float scaleY, float scaleZ)
        {
            return CMatrix3x3
            (
                scaleX, 0.0f, 0.0f,
                0.0f, scaleY, 0.0f,
                0.0f, 0.0f, scaleZ
            );
        }


        inline CMatrix3x3 CMatrix3x3::ScaleFromVector(CVector3 scale)
        {
            return Scale(scale.x, scale.y, scale.z);
        }


        inline CMatrix3x3 CMatrix3x3::Skew(float skewXY, float skewXZ, float skewYX, float skewYZ, float skewZX, float skewZY)
        {
            return CMatrix3x3
            (
                1.0f, skewXY, skewXZ,
                skewYX, 1.0f, skewYZ,
                skewZX, skewZY, 1.0f
            );
        }


        inline CMatrix3x3 CMatrix3x3::SkewFromVector(CVector2 skewX, CVector2 skewY, CVector2 skewZ)
        {
            return Skew(skewX.x, skewX.y, skewY.x, skewY.y, skewZ.x, skewZ.y);
        }


        inline CMatrix3x3 CMatrix3x3::Translation(float translateX, float translateY)
        {
            return CMatrix3x3
            (
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                translateX, translateY, 1.0f
            );
        }


        inline CMatrix3x3 CMatrix3x3::TranslationFromVector(CVector2 translationVec2)
        {
            return Translation(translationVec2.x, translationVec2.y);
        }


        inline CMatrix3x3 CMatrix3x3::AffineTransformation(CVector2 scale, CVector2 rotationOrigin, float rotationAngle, CVector2 translation)
        {
            return
                Scale(scale.x, scale.y, 1.0f) *
                Translation(-rotationOrigin.x, -rotationOrigin.y) *
                RotationZ(rotationAngle) *
                Translation(rotationOrigin.x, rotationOrigin.y) *
                Translation(translation.x, translation.y);
        }


        inline CMatrix3x3 CMatrix3x3::Transformation(CVector2 scaleOrigin, float scaleOrientation, CVector2 scale, CVector2 rotationOrigin, float rotationAngle, CVector2 translation)
        {
            return
                Translation(-scaleOrigin.x, -scaleOrigin.y) *
                RotationZ(-scaleOrientation) *
                Scale(scale.x, scale.y, 1.0f) *
                RotationZ(scaleOrientation) *
                Translation(scaleOrigin.x, scaleOrigin.y) *
                Translation(-rotationOrigin.x, -rotationOrigin.y) *
                RotationZ(rotationAngle) *
                Translation(rotationOrigin.x, rotationOrigin.y) *
                Translation(translation.x, translation.y);
        }


        inline bool operator == (const CMatrix3x3& a, const CMatrix3x3& b)
        {
            return
                a._11 == b._11 && a._12 == b._12 && a._13 == b._13 &&
                a._21 == b._21 && a._22 == b._22 && a._23 == b._23 &&
                a._31 == b._31 && a._32 == b._32 && a._33 == b._33;
        }


        inline bool operator != (const CMatrix3x3& a, const CMatrix3x3& b)
        {
            return !(a == b);
        }


        inline CMatrix3x3 operator * (float a, CMatrix3x3& b)
        {
            return b * a;
        }
    }
} // namespace krystallic