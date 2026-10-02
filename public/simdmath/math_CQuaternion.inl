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
        inline CQuaternion::CQuaternion() : 
            x(0.0f), y(0.0f), z(0.0f), w(1.0f)
        {
        }

        inline CQuaternion::CQuaternion(const float* arr) : 
            x(arr ? arr[0] : 0.0f), y(arr ? arr[1] : 0.0f), z(arr ? arr[2] : 0.0f), w(arr ? arr[3] : 1.0f)
        {
        }

        inline CQuaternion::CQuaternion(float _x, float _y, float _z, float _w) : 
            x(_x), y(_y), z(_z), w(_w)
        {
        }

        inline CQuaternion::CQuaternion(const CQuaternion& copy) : 
            x(copy.x), y(copy.y), z(copy.z), w(copy.w)
        {
        }


        inline CQuaternion::operator float*()
        {
            return v;
        }


        inline CQuaternion::operator const float*() const
        {
            return v;
        }


        inline CQuaternion CQuaternion::operator+() const
        {
            return *this;
        }


        inline CQuaternion CQuaternion::operator-() const
        {
            return CQuaternion(-x, -y, -z, -w);
        }


        inline CQuaternion CQuaternion::operator+(const CQuaternion& q) const
        {
            return CQuaternion(x + q.x, y + q.y, z + q.z, w + q.w);
        }


        inline CQuaternion CQuaternion::operator-(const CQuaternion& q) const
        {
            return CQuaternion(x - q.x, y - q.y, z - q.z, w - q.w);
        }


        inline CQuaternion CQuaternion::operator*(const CQuaternion& q) const
        {
            return CQuaternion
            (
                w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z
            );
        }


        inline CQuaternion CQuaternion::operator+(float s) const
        {
            return CQuaternion(x + s, y + s, z + s, w + s);
        }


        inline CQuaternion CQuaternion::operator-(float s) const
        {
            return CQuaternion(x - s, y - s, z - s, w - s);
        }


        inline CQuaternion CQuaternion::operator*(float s) const
        {
            return CQuaternion(x * s, y * s, z * s, w * s);
        }


        inline CQuaternion CQuaternion::operator/(float s) const
        {
            const float inv = 1.0f / s;
            return CQuaternion(x * inv, y * inv, z * inv, w * inv);
        }


        inline CQuaternion& CQuaternion::operator+=(const CQuaternion& q)
        {
            x += q.x;
            y += q.y;
            z += q.z;
            w += q.w;
            return *this;
        }


        inline CQuaternion& CQuaternion::operator-=(const CQuaternion& q)
        {
            x -= q.x;
            y -= q.y;
            z -= q.z;
            w -= q.w;
            return *this;
        }


        inline CQuaternion& CQuaternion::operator*=(const CQuaternion& q)
        {
            float _x = x;
            float _y = y;
            float _z = z;
            float _w = w;

            x = _w * q.x + _x * q.w + _y * q.z - _z * q.y;
            y = _w * q.y - _x * q.z + _y * q.w + _z * q.x;
            z = _w * q.z + _x * q.y - _y * q.x + _z * q.w;
            w = _w * q.w - _x * q.x - _y * q.y - _z * q.z;
            
            return *this;
        }


        inline CQuaternion& CQuaternion::operator+=(float s)
        {
            x += s;
            y += s;
            z += s;
            w += s;
            return *this;
        }


        inline CQuaternion& CQuaternion::operator-=(float s)
        {
            x -= s;
            y -= s;
            z -= s;
            w -= s;
            return *this;
        }


        inline CQuaternion& CQuaternion::operator*=(float s)
        {
            x *= s;
            y *= s;
            z *= s;
            w *= s;
            return *this;
        }


        inline CQuaternion& CQuaternion::operator/=(float s)
        {
            const float inv = 1.0f / s;
            x *= inv;
            y *= inv;
            z *= inv;
            w *= inv;
            return *this;
        }

        inline CQuaternion& CQuaternion::operator=(const float* arr)
        {
            x = arr ? arr[0] : 0.0f;
            y = arr ? arr[1] : 0.0f;
            z = arr ? arr[2] : 0.0f;
            w = arr ? arr[3] : 0.0f;
            return *this;
        }


        inline CQuaternion& CQuaternion::operator=(const CQuaternion& q)
        {
            x = q.x;
            y = q.y;
            z = q.z;
            w = q.w;
            return *this;
        }


        inline CQuaternion& CQuaternion::operator=(float val)
        {
            x = val;
            y = val;
            z = val;
            w = val;
            return *this;
        }


        inline CQuaternion& CQuaternion::Multiply(const CQuaternion& q)
        {
            float _x = x;
            float _y = y;
            float _z = z;
            float _w = w;

            x = _w * q.x + _x * q.w + _y * q.z - _z * q.y;
            y = _w * q.y - _x * q.z + _y * q.w + _z * q.x;
            z = _w * q.z + _x * q.y - _y * q.x + _z * q.w;
            w = _w * q.w - _x * q.x - _y * q.y - _z * q.z;

            return *this;
        }


        inline float CQuaternion::LengthSq() const
        {
            return x * x + y * y + z * z + w * w;
        }


        inline float CQuaternion::Length() const
        {
            return sqrtf(LengthSq());
        }


        inline float CQuaternion::Dot(const CQuaternion& other) const
        {
            return x * other.x + y * other.y +  z * other.z + w * other.w;
        }


        inline CQuaternion CQuaternion::Conjugate() const
        {
            return CQuaternion(-x, -y, -z, w);
        }


        inline CQuaternion CQuaternion::Normalize() const
        {
            const float lengthSq = LengthSq();

            if (lengthSq == 0.0f)
            {
                const float nan = std::numeric_limits<float>::quiet_NaN();
                return CQuaternion(nan, nan, nan, nan);
            }

            const float invLength = 1.0f / sqrtf(lengthSq);

            return CQuaternion
            (
                x * invLength,
                y * invLength,
                z * invLength,
                w * invLength
            );
        }


        inline CQuaternion CQuaternion::Inverse() const
        {
            const float lengthSq = LengthSq();

            if (lengthSq == 0.0f)
            {
                const float nan = std::numeric_limits<float>::quiet_NaN();
                return CQuaternion(nan, nan, nan, nan);
            }

            const float invLengthSq = 1.0f / lengthSq;

            return CQuaternion
            (
                -x * invLengthSq,
                -y * invLengthSq,
                -z * invLengthSq,
                w * invLengthSq
            );
        }


        inline bool CQuaternion::IsIdentity() const
        {
            return x == 0.0f && y == 0.0f && z == 0.0f && w == 1.0f;
        }


        inline bool CQuaternion::IsInf() const
        {
            return HasFloatsOneInf(v, 4);
        }



        inline bool CQuaternion::IsNaN() const
        {
            return HasFloatsOneNaN(v, 4);
        }


        inline CQuaternion CQuaternion::Slerp(CQuaternion qa, CQuaternion qb, float t)
        {
            float dot = qa.Dot(qb);

            if (dot < 0.0f)
                qb = -qb, dot = -dot;

            if (dot > 1.0f)
                dot = 1.0f;

            if (dot > 0.9995f)
            {
                const CQuaternion result
                (
                    qa.x + (qb.x - qa.x) * t,
                    qa.y + (qb.y - qa.y) * t,
                    qa.z + (qb.z - qa.z) * t,
                    qa.w + (qb.w - qa.w) * t
                );

                return result.Normalize();
            }

            const float angle = acosf(dot);
            const float sine = sinf(angle);

            if (sine == 0.0f)
                return qa;

            const float invSine = 1.0f / sine;
            const float weightA = sinf((1.0f - t) * angle) * invSine;
            const float weightB = sinf(t * angle) * invSine;

            return CQuaternion
            (
                qa.x * weightA + qb.x * weightB,
                qa.y * weightA + qb.y * weightB,
                qa.z * weightA + qb.z * weightB,
                qa.w * weightA + qb.w * weightB
            );
        }


        inline CQuaternion CQuaternion::Squad(CQuaternion qa, CQuaternion qb, CQuaternion qc, CQuaternion qd, float t)
        {
            const CQuaternion first = Slerp(qa, qd, t);
            const CQuaternion second = Slerp(qb, qc, t);

            const float squadT = 2.0f * t * (1.0f - t);

            return Slerp(first, second, squadT);
        }


        inline CQuaternion CQuaternion::RotationAxis(CVector3 axis, float angle)
        {
            const float lengthSq =
                axis.x * axis.x +
                axis.y * axis.y +
                axis.z * axis.z;

            if (lengthSq == 0.0f)
            {
                const float nan = std::numeric_limits<float>::quiet_NaN();
                return CQuaternion(nan, nan, nan, nan);
            }

            const float invLength = 1.0f / sqrtf(lengthSq);

            const float normalizedX = axis.x * invLength;
            const float normalizedY = axis.y * invLength;
            const float normalizedZ = axis.z * invLength;

            const float halfAngle = angle * 0.5f;
            const float sine = sinf(halfAngle);
            const float cosine = cosf(halfAngle);

            return CQuaternion(normalizedX * sine, normalizedY * sine, normalizedZ * sine, cosine);
        }


        inline CQuaternion CQuaternion::RotationMatrix(CMatrix4x4 matrix)
        {
            const float trace = matrix._11 + matrix._22 + matrix._33;

            CQuaternion result;

            if (trace > 0.0f)
            {
                const float root = sqrtf(trace + 1.0f);
                const float scale = 0.5f / root;

                result.x = (matrix._23 - matrix._32) * scale;
                result.y = (matrix._31 - matrix._13) * scale;
                result.z = (matrix._12 - matrix._21) * scale;
                result.w = root * 0.5f;
            }
            else if (matrix._11 >= matrix._22 && matrix._11 >= matrix._33)
            {
                const float root = sqrtf(1.0f + matrix._11 - matrix._22 - matrix._33);
                const float scale = 0.5f / root;

                result.x = root * 0.5f;
                result.y = (matrix._12 + matrix._21) * scale;
                result.z = (matrix._13 + matrix._31) * scale;
                result.w = (matrix._23 - matrix._32) * scale;
            }
            else if (matrix._22 >= matrix._33)
            {
                const float root = sqrtf(1.0f - matrix._11 + matrix._22 - matrix._33);
                const float scale = 0.5f / root;

                result.x = (matrix._12 + matrix._21) * scale;
                result.y = root * 0.5f;
                result.z = (matrix._23 + matrix._32) * scale;
                result.w = (matrix._31 - matrix._13) * scale;
            }
            else
            {
                const float root = sqrtf(1.0f -  matrix._11 -  matrix._22 + matrix._33);
                const float scale = 0.5f / root;

                result.x = (matrix._13 + matrix._31) * scale;
                result.y = (matrix._23 + matrix._32) * scale;
                result.z = root * 0.5f;
                result.w = (matrix._12 - matrix._21) * scale;
            }

            return result.Normalize();
        }


        inline CQuaternion CQuaternion::PitchYawRoll(float pitch, float yaw, float roll)
        {
            const float halfPitch = pitch * 0.5f;
            const float halfYaw = yaw * 0.5f;
            const float halfRoll = roll * 0.5f;

            const float sinPitch = sinf(halfPitch);
            const float cosPitch = cosf(halfPitch);

            const float sinYaw = sinf(halfYaw);
            const float cosYaw = cosf(halfYaw);

            const float sinRoll = sinf(halfRoll);
            const float cosRoll = cosf(halfRoll);

			const CQuaternion pitchQuaternion(sinPitch, 0.0f, 0.0f, cosPitch);
            const CQuaternion yawQuaternion(0.0f, sinYaw, 0.0f, cosYaw);
            const CQuaternion rollQuaternion(0.0f, 0.0f, sinRoll, cosRoll);

            return (yawQuaternion * pitchQuaternion * rollQuaternion).Normalize();
        }


        inline CQuaternion CQuaternion::PitchYawRollVec(CVector3 pitchYawRall)
        {
            return PitchYawRoll(pitchYawRall.x, pitchYawRall.y, pitchYawRall.z);
        }


        inline CQuaternion operator+(float a, const CQuaternion& b)
        {
            return CQuaternion(a + b.x, a + b.y, a + b.z, a + b.w);
        }


        inline CQuaternion operator-(float a, const CQuaternion& b)
        {
            return CQuaternion(a - b.x, a - b.y, a - b.z, a - b.w);
        }


        inline CQuaternion operator*(float a, const CQuaternion& b)
        {
            return CQuaternion(a * b.x, a * b.y, a * b.z, a * b.w);
        }


        inline bool operator==(const CQuaternion& a, const CQuaternion& b)
        {
            return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
        }


        inline bool operator!=(const CQuaternion& a, const CQuaternion& b)
        {
            return !(a == b);
        }



    } // namespace SIMDMath
} // namespace krystallic