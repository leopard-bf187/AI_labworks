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

        inline CMatrix4x4::CMatrix4x4()
            : _11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f)
            , _21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f)
            , _31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f)
            , _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
        {
        }


        inline CMatrix4x4::CMatrix4x4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
            : _11(m11), _12(m12), _13(m13), _14(m14)
            , _21(m21), _22(m22), _23(m23), _24(m24)
            , _31(m31), _32(m32), _33(m33), _34(m34)
            , _41(m41), _42(m42), _43(m43), _44(m44)
        {
        }


        inline CMatrix4x4::CMatrix4x4(const float* arr)
        {
            *this = arr;
        }


        inline CMatrix4x4::CMatrix4x4(const CMatrix4x4& copy)
        {
            *this = copy;
        }


        inline CMatrix4x4 CMatrix4x4::operator + () const
        {
            return *this;
        }


        inline CMatrix4x4 CMatrix4x4::operator - () const
        {
            return CMatrix4x4
            (
                -_11, -_12, -_13, -_14,
                -_21, -_22, -_23, -_24,
                -_31, -_32, -_33, -_34,
                -_41, -_42, -_43, -_44
            );
        }


        inline CMatrix4x4 CMatrix4x4::operator +(const CMatrix4x4& other) const
        {
            return CMatrix4x4
            (
                _11 + other._11, _12 + other._12, _13 + other._13, _14 + other._14,
                _21 + other._21, _22 + other._22, _23 + other._23, _24 + other._24,
                _31 + other._31, _32 + other._32, _33 + other._33, _34 + other._34,
                _41 + other._41, _42 + other._42, _43 + other._43, _44 + other._44
            );
        }


        inline CMatrix4x4 CMatrix4x4::operator -(const CMatrix4x4& other) const
        {
            return CMatrix4x4
            (
                _11 - other._11, _12 - other._12, _13 - other._13, _14 - other._14,
                _21 - other._21, _22 - other._22, _23 - other._23, _24 - other._24,
                _31 - other._31, _32 - other._32, _33 - other._33, _34 - other._34,
                _41 - other._41, _42 - other._42, _43 - other._43, _44 - other._44
            );
        }


        inline CMatrix4x4 CMatrix4x4::operator *(const CMatrix4x4& other) const
        {
            return CMatrix4x4
            (
                _11 * other._11 + _12 * other._21 + _13 * other._31 + _14 * other._41,
                _11 * other._12 + _12 * other._22 + _13 * other._32 + _14 * other._42,
                _11 * other._13 + _12 * other._23 + _13 * other._33 + _14 * other._43,
                _11 * other._14 + _12 * other._24 + _13 * other._34 + _14 * other._44,

                _21 * other._11 + _22 * other._21 + _23 * other._31 + _24 * other._41,
                _21 * other._12 + _22 * other._22 + _23 * other._32 + _24 * other._42,
                _21 * other._13 + _22 * other._23 + _23 * other._33 + _24 * other._43,
                _21 * other._14 + _22 * other._24 + _23 * other._34 + _24 * other._44,

                _31 * other._11 + _32 * other._21 + _33 * other._31 + _34 * other._41,
                _31 * other._12 + _32 * other._22 + _33 * other._32 + _34 * other._42,
                _31 * other._13 + _32 * other._23 + _33 * other._33 + _34 * other._43,
                _31 * other._14 + _32 * other._24 + _33 * other._34 + _34 * other._44,

                _41 * other._11 + _42 * other._21 + _43 * other._31 + _44 * other._41,
                _41 * other._12 + _42 * other._22 + _43 * other._32 + _44 * other._42,
                _41 * other._13 + _42 * other._23 + _43 * other._33 + _44 * other._43,
                _41 * other._14 + _42 * other._24 + _43 * other._34 + _44 * other._44
            );
        }


        inline CMatrix4x4 CMatrix4x4::operator *(float val) const
        {
            return CMatrix4x4
            (
                _11 * val, _12 * val, _13 * val, _14 * val,
                _21 * val, _22 * val, _23 * val, _24 * val,
                _31 * val, _32 * val, _33 * val, _34 * val,
                _41 * val, _42 * val, _43 * val, _44 * val
            );
        }


        inline CMatrix4x4 CMatrix4x4::operator /(float val) const
        {
            return *this * (1.0f / val);
        }


        inline CMatrix4x4& CMatrix4x4::operator +=(const CMatrix4x4& other)
        {
            _11 += other._11;
            _12 += other._12;
            _13 += other._13;
            _14 += other._14;

            _21 += other._21;
            _22 += other._22;
            _23 += other._23;
            _24 += other._24;

            _31 += other._31;
            _32 += other._32;
            _33 += other._33;
            _34 += other._34;

            _41 += other._41;
            _42 += other._42;
            _43 += other._43;
            _44 += other._44;

            return *this;
        }


        inline CMatrix4x4& CMatrix4x4::operator -=(const CMatrix4x4& other)
        {
            _11 -= other._11;
            _12 -= other._12;
            _13 -= other._13;
            _14 -= other._14;

            _21 -= other._21;
            _22 -= other._22;
            _23 -= other._23;
            _24 -= other._24;

            _31 -= other._31;
            _32 -= other._32;
            _33 -= other._33;
            _34 -= other._34;

            _41 -= other._41;
            _42 -= other._42;
            _43 -= other._43;
            _44 -= other._44;

            return *this;
        }


        inline CMatrix4x4& CMatrix4x4::operator *=(const CMatrix4x4& other)
        {
            return Multiply(other);
        }


        inline CMatrix4x4& CMatrix4x4::operator *=(float val)
        {
            return Multiply(val);
        }


        inline CMatrix4x4& CMatrix4x4::operator /=(float val)
        {
            return Multiply(1.0f / val);
        }


        inline CMatrix4x4& CMatrix4x4::operator =(const CMatrix4x4& other)
        {
            _11 = other._11;
            _12 = other._12;
            _13 = other._13;
            _14 = other._14;

            _21 = other._21;
            _22 = other._22;
            _23 = other._23;
            _24 = other._24;

            _31 = other._31;
            _32 = other._32;
            _33 = other._33;
            _34 = other._34;

            _41 = other._41;
            _42 = other._42;
            _43 = other._43;
            _44 = other._44;

            return *this;
        }


        inline CMatrix4x4& CMatrix4x4::operator =(const float* arr)
        {
            _11 = arr[0];
            _12 = arr[1];
            _13 = arr[2];
            _14 = arr[3];

            _21 = arr[4];
            _22 = arr[5];
            _23 = arr[6];
            _24 = arr[7];

            _31 = arr[8];
            _32 = arr[9];
            _33 = arr[10];
            _34 = arr[11];

            _41 = arr[12];
            _42 = arr[13];
            _43 = arr[14];
            _44 = arr[15];

            return *this;
        }


        inline float CMatrix4x4::Determinant() const
        {
            const float minor00 = _33 * _44 - _34 * _43;
            const float minor01 = _32 * _44 - _34 * _42;
            const float minor02 = _32 * _43 - _33 * _42;
            const float minor03 = _31 * _44 - _34 * _41;
            const float minor04 = _31 * _43 - _33 * _41;
            const float minor05 = _31 * _42 - _32 * _41;

            return
                _11 * (_22 * minor00 - _23 * minor01 + _24 * minor02) -
                _12 * (_21 * minor00 - _23 * minor03 + _24 * minor04) +
                _13 * (_21 * minor01 - _22 * minor03 + _24 * minor05) -
                _14 * (_21 * minor02 - _22 * minor04 + _23 * minor05);
        }


        inline CMatrix4x4 CMatrix4x4::Inverse() const
        {
            float augmented[4][8] =
            {
                { _11, _12, _13, _14, 1.0f, 0.0f, 0.0f, 0.0f },
                { _21, _22, _23, _24, 0.0f, 1.0f, 0.0f, 0.0f },
                { _31, _32, _33, _34, 0.0f, 0.0f, 1.0f, 0.0f },
                { _41, _42, _43, _44, 0.0f, 0.0f, 0.0f, 1.0f }
            };

            for (int column = 0; column < 4; ++column)
            {
                int pivotRow = column;
                float pivotMagnitude = fabsf(augmented[column][column]);

                for (int row = column + 1; row < 4; ++row)
                {
                    const float magnitude = fabsf(augmented[row][column]);

                    if (magnitude > pivotMagnitude)
                    {
                        pivotMagnitude = magnitude;
                        pivotRow = row;
                    }
                }

                if (pivotMagnitude == 0.0f)
                {
                    const float nan = std::numeric_limits<float>::quiet_NaN();

                    return CMatrix4x4
                    (
                        nan, nan, nan, nan,
                        nan, nan, nan, nan,
                        nan, nan, nan, nan,
                        nan, nan, nan, nan
                    );
                }

                if (pivotRow != column)
                {
                    for (int element = 0; element < 8; ++element)
                    {
                        const float temp = augmented[column][element];
                        augmented[column][element] = augmented[pivotRow][element];
                        augmented[pivotRow][element] = temp;
                    }
                }

                const float pivot = augmented[column][column];
                const float invPivot = 1.0f / pivot;

                for (int element = 0; element < 8; ++element)
                    augmented[column][element] *= invPivot;

                for (int row = 0; row < 4; ++row)
                {
                    if (row == column)
                        continue;

                    const float factor = augmented[row][column];

                    for (int element = 0; element < 8; ++element)
                        augmented[row][element] -= factor * augmented[column][element];
                }
            }

            return CMatrix4x4
            (
                augmented[0][4], augmented[0][5], augmented[0][6], augmented[0][7],
                augmented[1][4], augmented[1][5], augmented[1][6], augmented[1][7],
                augmented[2][4], augmented[2][5], augmented[2][6], augmented[2][7],
                augmented[3][4], augmented[3][5], augmented[3][6], augmented[3][7]
            );
        }


        inline CMatrix4x4 CMatrix4x4::Transpose() const
        {
            return CMatrix4x4
            (
                _11, _21, _31, _41,
                _12, _22, _32, _42,
                _13, _23, _33, _43,
                _14, _24, _34, _44
            );
        }


        inline CMatrix4x4& CMatrix4x4::Multiply(float val)
        {
            _11 *= val;
            _12 *= val;
            _13 *= val;
            _14 *= val;

            _21 *= val;
            _22 *= val;
            _23 *= val;
            _24 *= val;

            _31 *= val;
            _32 *= val;
            _33 *= val;
            _34 *= val;

            _41 *= val;
            _42 *= val;
            _43 *= val;
            _44 *= val;

            return *this;
        }


        inline CMatrix4x4& CMatrix4x4::Multiply(const CMatrix4x4& other)
        {
            *this = *this * other;
            return *this;
        }


        inline CMatrix4x4& CMatrix4x4::MultiplyTranspose(const CMatrix4x4& other)
        {
            const CMatrix4x4 result
            (
                _11 * other._11 + _12 * other._12 + _13 * other._13 + _14 * other._14,
                _11 * other._21 + _12 * other._22 + _13 * other._23 + _14 * other._24,
                _11 * other._31 + _12 * other._32 + _13 * other._33 + _14 * other._34,
                _11 * other._41 + _12 * other._42 + _13 * other._43 + _14 * other._44,

                _21 * other._11 + _22 * other._12 + _23 * other._13 + _24 * other._14,
                _21 * other._21 + _22 * other._22 + _23 * other._23 + _24 * other._24,
                _21 * other._31 + _22 * other._32 + _23 * other._33 + _24 * other._34,
                _21 * other._41 + _22 * other._42 + _23 * other._43 + _24 * other._44,

                _31 * other._11 + _32 * other._12 + _33 * other._13 + _34 * other._14,
                _31 * other._21 + _32 * other._22 + _33 * other._23 + _34 * other._24,
                _31 * other._31 + _32 * other._32 + _33 * other._33 + _34 * other._34,
                _31 * other._41 + _32 * other._42 + _33 * other._43 + _34 * other._44,

                _41 * other._11 + _42 * other._12 + _43 * other._13 + _44 * other._14,
                _41 * other._21 + _42 * other._22 + _43 * other._23 + _44 * other._24,
                _41 * other._31 + _42 * other._32 + _43 * other._33 + _44 * other._34,
                _41 * other._41 + _42 * other._42 + _43 * other._43 + _44 * other._44
            );

            *this = result;
            return *this;
        }


        inline bool CMatrix4x4::IsIdentity() const
        {
            return
                _11 == 1.0f && _12 == 0.0f && _13 == 0.0f && _14 == 0.0f &&
                _21 == 0.0f && _22 == 1.0f && _23 == 0.0f && _24 == 0.0f &&
                _31 == 0.0f && _32 == 0.0f && _33 == 1.0f && _34 == 0.0f &&
                _41 == 0.0f && _42 == 0.0f && _43 == 0.0f && _44 == 1.0f;
        }


        inline bool CMatrix4x4::IsInf() const
        {
            return HasFloatsOneInf(v, 16);
        }


        inline bool CMatrix4x4::IsNaN() const
        {
            return HasFloatsOneNaN(v, 16);
        }


        inline CMatrix4x4 CMatrix4x4::Identity()
        {
            return CMatrix4x4();
        }


        inline CMatrix4x4 CMatrix4x4::LookAtLH(CVector3 eyePos, CVector3 focusPos, CVector3 upDir)
        {
            return LookToLH (eyePos, focusPos - eyePos, upDir);
        }


        inline CMatrix4x4 CMatrix4x4::LookAtRH(CVector3 eyePos, CVector3 focusPos, CVector3 upDir)
        {
            return LookToRH(eyePos, focusPos - eyePos, upDir);
        }


        inline CMatrix4x4 CMatrix4x4::LookToLH(CVector3 eyePos, CVector3 eyeDir, CVector3 upDir)
        {
            const CVector3 zAxis = eyeDir.Normalize();

            const CVector3 xAxis = upDir.Cross(zAxis).Normalize();

            const CVector3 yAxis = zAxis.Cross(xAxis);

            return CMatrix4x4
            (
                xAxis.x, yAxis.x, zAxis.x, 0.0f,
                xAxis.y, yAxis.y, zAxis.y, 0.0f,
                xAxis.z, yAxis.z, zAxis.z, 0.0f,

                -xAxis.Dot(eyePos),
                -yAxis.Dot(eyePos),
                -zAxis.Dot(eyePos),
                1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::LookToRH(CVector3 eyePos, CVector3 eyeDir, CVector3 upDir)
        {
            return LookToLH(eyePos, -eyeDir, upDir);
        }


        inline CMatrix4x4 CMatrix4x4::OrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ)
        {
            const float range = 1.0f / (farZ - nearZ);

            return CMatrix4x4
            (
                2.0f / viewWidth, 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f / viewHeight, 0.0f, 0.0f,
                0.0f, 0.0f, range, 0.0f,
                0.0f, 0.0f, -nearZ * range, 1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::OrthographicRH(float viewWidth, float viewHeight, float nearZ, float farZ)
        {
            const float range = 1.0f / (nearZ - farZ);

            return CMatrix4x4
            (
                2.0f / viewWidth, 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f / viewHeight, 0.0f, 0.0f,
                0.0f, 0.0f, range, 0.0f,
                0.0f, 0.0f, nearZ * range, 1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::OrthographicOffCenterLH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ)
        {
            const float invWidth = 1.0f / (viewRight - viewLeft);
            const float invHeight = 1.0f / (viewTop - viewBottom);
            const float invDepth = 1.0f / (farZ - nearZ);

            return CMatrix4x4
            (
                2.0f * invWidth, 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f * invHeight, 0.0f, 0.0f,
                0.0f, 0.0f, invDepth, 0.0f,

                -(viewLeft + viewRight) * invWidth,
                -(viewTop + viewBottom) * invHeight,
                -nearZ * invDepth,
                1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::OrthographicOffCenterRH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ)
        {
            const float invWidth = 1.0f / (viewRight - viewLeft);
            const float invHeight = 1.0f / (viewTop - viewBottom);
            const float invDepth = 1.0f / (nearZ - farZ);

            return CMatrix4x4
            (
                2.0f * invWidth, 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f * invHeight, 0.0f, 0.0f,
                0.0f, 0.0f, invDepth, 0.0f,

                -(viewLeft + viewRight) * invWidth,
                -(viewTop + viewBottom) * invHeight,
                nearZ * invDepth,
                1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::PerspectiveLH(float viewWidth, float viewHeight, float nearZ, float farZ)
        {
            const float range = farZ / (farZ - nearZ);

            return CMatrix4x4
            (
                2.0f * nearZ / viewWidth, 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f * nearZ / viewHeight, 0.0f, 0.0f,
                0.0f, 0.0f, range, 1.0f,
                0.0f, 0.0f, -nearZ * range, 0.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::PerspectiveRH(float viewWidth, float viewHeight, float nearZ, float farZ)
        {
            const float range = farZ / (nearZ - farZ);

            return CMatrix4x4
            (
                2.0f * nearZ / viewWidth, 0.0f, 0.0f, 0.0f,
                0.0f, 2.0f * nearZ / viewHeight, 0.0f, 0.0f,
                0.0f, 0.0f, range, -1.0f,
                0.0f, 0.0f, nearZ * range, 0.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::PerspectiveFovXLH(float fovAngleX, float aspectRatio, float nearZ, float farZ)
        {
            const float scaleX = 1.0f / tanf(fovAngleX * 0.5f);
            const float scaleY = scaleX * aspectRatio;
            const float range = farZ / (farZ - nearZ);

            return CMatrix4x4
            (
                scaleX, 0.0f, 0.0f, 0.0f,
                0.0f, scaleY, 0.0f, 0.0f,
                0.0f, 0.0f, range, 1.0f,
                0.0f, 0.0f, -nearZ * range, 0.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::PerspectiveFovXRH(float fovAngleX, float aspectRatio, float nearZ, float farZ)
        {
            const float scaleX = 1.0f / tanf(fovAngleX * 0.5f);
            const float scaleY = scaleX * aspectRatio;
            const float range = farZ / (nearZ - farZ);

            return CMatrix4x4
            (
                scaleX, 0.0f, 0.0f, 0.0f,
                0.0f, scaleY, 0.0f, 0.0f,
                0.0f, 0.0f, range, -1.0f,
                0.0f, 0.0f, nearZ * range, 0.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::PerspectiveFovYLH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
        {
            const float scaleY = 1.0f / tanf(fovAngleY * 0.5f);
            const float scaleX = scaleY / aspectRatio;
            const float range = farZ / (farZ - nearZ);

            return CMatrix4x4
            (
                scaleX, 0.0f, 0.0f, 0.0f,
                0.0f, scaleY, 0.0f, 0.0f,
                0.0f, 0.0f, range, 1.0f,
                0.0f, 0.0f, -nearZ * range, 0.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::PerspectiveFovYRH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
        {
            const float scaleY = 1.0f / tanf(fovAngleY * 0.5f);
            const float scaleX = scaleY / aspectRatio;
            const float range = farZ / (nearZ - farZ);

            return CMatrix4x4
            (
                scaleX, 0.0f, 0.0f, 0.0f,
                0.0f, scaleY, 0.0f, 0.0f,
                0.0f, 0.0f, range, -1.0f,
                0.0f, 0.0f, nearZ * range, 0.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::PerspectiveOffCenterLH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ)
        {
            const float invWidth = 1.0f / (viewRight - viewLeft);
            const float invHeight = 1.0f / (viewTop - viewBottom);
            const float range = farZ / (farZ - nearZ);

            return CMatrix4x4
            (
                2.0f * nearZ * invWidth,
                0.0f,
                0.0f,
                0.0f,

                0.0f,
                2.0f * nearZ * invHeight,
                0.0f,
                0.0f,

                -(viewLeft + viewRight) * invWidth,
                -(viewTop + viewBottom) * invHeight,
                range,
                1.0f,

                0.0f,
                0.0f,
                -nearZ * range,
                0.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::PerspectiveOffCenterRH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ)
        {
            const float invWidth = 1.0f / (viewRight - viewLeft);
            const float invHeight = 1.0f / (viewTop - viewBottom);
            const float range = farZ / (nearZ - farZ);

            return CMatrix4x4
            (
                2.0f * nearZ * invWidth,
                0.0f,
                0.0f,
                0.0f,

                0.0f,
                2.0f * nearZ * invHeight,
                0.0f,
                0.0f,

                (viewLeft + viewRight) * invWidth,
                (viewTop + viewBottom) * invHeight,
                range,
                -1.0f,

                0.0f,
                0.0f,
                nearZ * range,
                0.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::RotationX(float angle)
        {
            const float sine = sinf(angle);
            const float cosine = cosf(angle);

            return CMatrix4x4
            (
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, cosine, sine, 0.0f,
                0.0f, -sine, cosine, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::RotationY(float angle)
        {
            const float sine = sinf(angle);
            const float cosine = cosf(angle);

            return CMatrix4x4
            (
                cosine, 0.0f, -sine, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                sine, 0.0f, cosine, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::RotationZ(float angle)
        {
            const float sine = sinf(angle);
            const float cosine = cosf(angle);

            return CMatrix4x4
            (
                cosine, sine, 0.0f, 0.0f,
                -sine, cosine, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::RotationAxis(CVector3 axis, float angle)
        {
            axis = axis.Normalize();

            const float x = axis.x;
            const float y = axis.y;
            const float z = axis.z;

            const float sine = sinf(angle);
            const float cosine = cosf(angle);
            const float oneMinusCosine = 1.0f - cosine;

            return CMatrix4x4
            (
                cosine + x * x * oneMinusCosine,
                x * y * oneMinusCosine + z * sine,
                x * z * oneMinusCosine - y * sine,
                0.0f,

                x * y * oneMinusCosine - z * sine,
                cosine + y * y * oneMinusCosine,
                y * z * oneMinusCosine + x * sine,
                0.0f,

                x * z * oneMinusCosine + y * sine,
                y * z * oneMinusCosine - x * sine,
                cosine + z * z * oneMinusCosine,
                0.0f,

                0.0f,
                0.0f,
                0.0f,
                1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::RotationQuaternion(CQuaternion quat)
        {
            const float lengthSquared = quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w;

            if (lengthSquared == 0.0f)
            {
                const float nan = std::numeric_limits<float>::quiet_NaN();

                return CMatrix4x4
                (
                    nan, nan, nan, nan,
                    nan, nan, nan, nan,
                    nan, nan, nan, nan,
                    nan, nan, nan, nan
                );
            }

            const float invLength = 1.0f / sqrtf(lengthSquared);

            const float x = quat.x * invLength;
            const float y = quat.y * invLength;
            const float z = quat.z * invLength;
            const float w = quat.w * invLength;

            const float xx = x * x;
            const float yy = y * y;
            const float zz = z * z;

            const float xy = x * y;
            const float xz = x * z;
            const float yz = y * z;

            const float xw = x * w;
            const float yw = y * w;
            const float zw = z * w;

            return CMatrix4x4
            (
                1.0f - 2.0f * (yy + zz), 2.0f * (xy + zw),        2.0f * (xz - yw),        0.0f,
                2.0f * (xy - zw),        1.0f - 2.0f * (xx + zz), 2.0f * (yz + xw),        0.0f,
                2.0f * (xz + yw),        2.0f * (yz - xw),        1.0f - 2.0f * (xx + yy), 0.0f,
                0.0f,                    0.0f,                    0.0f,                    1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::RotationPitchYawRoll(float pitch, float yaw, float roll)
        {
            return
                RotationZ(roll) *
                RotationX(pitch) *
                RotationY(yaw);
        }


        inline CMatrix4x4 CMatrix4x4::RotationPitchYawRollFromVector(CVector3 pitchYawRoll)
        {
            return RotationPitchYawRoll
            (
                pitchYawRoll.x,
                pitchYawRoll.y,
                pitchYawRoll.z
            );
        }


        inline CMatrix4x4 CMatrix4x4::Scale(float scaleX, float scaleY, float scaleZ)
        {
            return CMatrix4x4
            (
                scaleX, 0.0f,   0.0f,   0.0f,
                0.0f,   scaleY, 0.0f,   0.0f,
                0.0f,   0.0f,   scaleZ, 0.0f,
                0.0f,   0.0f,   0.0f,   1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::ScaleFromVector(CVector3 scale)
        {
            return Scale(scale.x, scale.y, scale.z);
        }


        inline CMatrix4x4 CMatrix4x4::Skew(float skewXY, float skewXZ, float skewYX, float skewYZ, float skewZX, float skewZY)
        {
            return CMatrix4x4
            (
                1.0f,   skewXY, skewXZ, 0.0f,
                skewYX, 1.0f,   skewYZ, 0.0f,
                skewZX, skewZY, 1.0f,   0.0f,
                0.0f,   0.0f,   0.0f,   1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::SkewFromVector(CVector2 skewX, CVector2 skewY, CVector2 skewZ)
        {
            return Skew
            (
                skewX.x,
                skewX.y,
                skewY.x,
                skewY.y,
                skewZ.x,
                skewZ.y
            );
        }


        inline CMatrix4x4 CMatrix4x4::Translation(float translateX, float translateY, float translateZ)
        {
            return CMatrix4x4
            (
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                translateX, translateY, translateZ, 1.0f
            );
        }


        inline CMatrix4x4 CMatrix4x4::TranslationFromVector(CVector3 translationVec3)
        {
            return Translation
            (
                translationVec3.x,
                translationVec3.y,
                translationVec3.z
            );
        }


        inline CMatrix4x4 CMatrix4x4::AffineTransformation(CVector3 scale, CVector3 rotationOrigin, CQuaternion rotationQuaternion, CVector3 translation)
        {
            return
                ScaleFromVector(scale) *
                Translation(-rotationOrigin.x, -rotationOrigin.y, -rotationOrigin.z) *
                RotationQuaternion(rotationQuaternion) *
                Translation(rotationOrigin.x, rotationOrigin.y, rotationOrigin.z) *
                TranslationFromVector(translation);
        }


        inline CMatrix4x4 CMatrix4x4::Transformation(CVector3 scaleOrigin, CQuaternion scaleOrientationQuaternion, CVector3 scale, CVector3 rotationOrigin, CQuaternion rotationQuaternion, CVector3 translation)
        {
            const CQuaternion inverseScaleOrientation = scaleOrientationQuaternion.Conjugate();

            return
                Translation(-scaleOrigin.x, -scaleOrigin.y, -scaleOrigin.z) *
                RotationQuaternion(inverseScaleOrientation) *
                ScaleFromVector(scale) *
                RotationQuaternion(scaleOrientationQuaternion) *
                Translation(scaleOrigin.x, scaleOrigin.y, scaleOrigin.z) *
                Translation(-rotationOrigin.x, -rotationOrigin.y, -rotationOrigin.z) *
                RotationQuaternion(rotationQuaternion) *
                Translation(rotationOrigin.x, rotationOrigin.y, rotationOrigin.z) *
                TranslationFromVector(translation);
        }


        inline CMatrix4x4 CMatrix4x4::AffineTransformation2D(CVector3 scale, CVector3 rotationOrigin, float rotationAngle, CVector2 translation)
        {
            return
                ScaleFromVector(scale) *
                Translation(-rotationOrigin.x, -rotationOrigin.y, -rotationOrigin.z) *
                RotationZ(rotationAngle) *
                Translation(rotationOrigin.x, rotationOrigin.y, rotationOrigin.z) *
                Translation(translation.x, translation.y, 0.0f);
        }


        inline CMatrix4x4 CMatrix4x4::Transformation2D(CVector3 scaleOrigin, float scaleOrientation, CVector3 scale, CVector3 rotationOrigin, float rotationAngle, CVector3 translation)
        {
            return
                Translation(-scaleOrigin.x, -scaleOrigin.y, -scaleOrigin.z) *
                RotationZ(-scaleOrientation) *
                ScaleFromVector(scale) *
                RotationZ(scaleOrientation) *
                Translation(scaleOrigin.x, scaleOrigin.y, scaleOrigin.z) *
                Translation(-rotationOrigin.x, -rotationOrigin.y, -rotationOrigin.z) *
                RotationZ(rotationAngle) *
                Translation(rotationOrigin.x, rotationOrigin.y, rotationOrigin.z) *
                TranslationFromVector(translation);
        }


        inline bool operator == (const CMatrix4x4& a, const CMatrix4x4& b)
        {
            return
                a._11 == b._11 && a._12 == b._12 && a._13 == b._13 && a._14 == b._14 &&
                a._21 == b._21 && a._22 == b._22 && a._23 == b._23 && a._24 == b._24 &&
                a._31 == b._31 && a._32 == b._32 && a._33 == b._33 && a._34 == b._34 &&
                a._41 == b._41 && a._42 == b._42 && a._43 == b._43 && a._44 == b._44;
        }


        inline bool operator != (const CMatrix4x4& a, const CMatrix4x4& b)
        {
            return !(a == b);
        }


        inline CMatrix4x4 operator * (float a, const CMatrix4x4& b)
        {
            return b * a;
        }
    }
} // namespace krystallic