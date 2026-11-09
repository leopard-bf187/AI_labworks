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
        inline CVector3::CVector3() : 
            CVector3(0.0f, 0.0f, 0.0f)
        {
        }


        inline CVector3::CVector3(float value) : 
            CVector3(value, value, value)
        {
        }


        inline CVector3::CVector3(const float* arr)
            : CVector3(arr ? arr[0] : 0.0f, arr ? arr[1] : 0.0f, arr ? arr[2] : 0.0f)
        {
        }


        inline CVector3::CVector3(float _x, float _y, float _z) : 
            x(_x), y(_y), z(_z)
        {
        }


        inline CVector3::CVector3(const CVector3& copy) :
            x(copy.x), y(copy.y), z(copy.z)
        {
        }


        inline CVector3 CVector3::operator+() const
        {
            return CVector3(*this);
        }


        inline CVector3 CVector3::operator-() const
        {
            return CVector3(-x, -y, -z);
        }


        inline CVector3::operator float*()
        {
            return v;
        }


        inline CVector3::operator const float*() const
        {
            return v;
        }


        inline CVector3 CVector3::operator+(const CVector3& a) const
        {
            return CVector3(x + a.x, y + a.y, z + a.z);
        }


        inline CVector3 CVector3::operator+(float a) const
        {
            return CVector3(x + a, y + a, z + a);
        }


        inline CVector3 CVector3::operator-(const CVector3& a) const
        {
            return CVector3(x - a.x, y - a.y, z - a.z);
        }


        inline CVector3 CVector3::operator-(float a) const
        {
            return CVector3(x - a, y - a, z - a);
        }


        inline CVector3 CVector3::operator*(const CVector3& a) const
        {
            return CVector3(x * a.x, y * a.y, z / a.z);
        }


        inline CVector3 CVector3::operator*(float a) const
        {
            return CVector3(x * a, y * a, z * a);
        }


        inline CVector3 CVector3::operator/(const CVector3& a) const
        {
            return CVector3(x / a.x, y / a.y, z / a.z);
        }


        inline CVector3 CVector3::operator/(float a) const
        {
            float val = 1.0f / a;
            return CVector3(x * val, y * val, z * val);
        }


        inline CVector3& CVector3::operator+=(const CVector3& a)
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }


        inline CVector3& CVector3::operator+=(float a)
        {
            x += a;
            y += a;
            z += a;
            return *this;
        }


        inline CVector3& CVector3::operator-=(const CVector3& a)
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            return *this;
        }


        inline CVector3& CVector3::operator-=(float a)
        {
            x -= a;
            y -= a;
            z -= a;
            return *this;
        }


        inline CVector3& CVector3::operator*=(const CVector3& a)
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            return *this;
        }


        inline CVector3& CVector3::operator*=(float a)
        {
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }


        inline CVector3& CVector3::operator/=(const CVector3& a)
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            return *this;
        }


        inline CVector3& CVector3::operator/=(float a)
        {
            float val = 1.0f / a;
            x *= val;
            y *= val;
            z *= val;
            return *this;
        }


        inline CVector3& CVector3::operator=(const float* arr)
        {
            x = arr ? arr[0] : 0.0f;
            y = arr ? arr[1] : 0.0f;
            z = arr ? arr[2] : 0.0f;
            return *this;
        }


        inline CVector3& CVector3::operator=(const CVector3& a)
        {
            x = a.x;
            y = a.y;
            z = a.z;
            return *this;
        }


        inline CVector3& CVector3::operator=(float a)
        {
            x = a;
            y = a;
            z = a;
            return *this;
        }


        inline float CVector3::Length() const
        {
            return sqrtf(x * x + y * y + z * z);
        }


        inline float CVector3::LengthSq() const
        {
            return x * x + y * y + z * z;
        }


        inline float CVector3::InvLength() const
        {
            return 1.0f / Length();
        }


        inline float CVector3::Dot(const CVector3& other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }


        inline CVector3 CVector3::Cross(const CVector3& other) const
        {
            float _x = y * other.z - z * other.y;
            float _y = z * other.x - x * other.z;
            float _z = x * other.y - y * other.x;
            return CVector3(_x, _y, _z);
        }


        inline CVector3 CVector3::Normalize() const
        {
            float len = InvLength();
            return CVector3(x * len, y * len, z * len);
        }


        inline CVector3 CVector3::Lerp(const CVector3& b, float s)
        {
            return *this + s * (b - *this);
        }


        inline bool CVector3::IsNaN() const
        {
            return HasFloatsOneNaN(v, 3);
        }


        inline bool CVector3::IsInf() const
        {
            return HasFloatsOneInf(v, 3);
        }


        inline bool operator==(const CVector3& a, const CVector3& b)
        {
            return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
        }


        inline bool operator!=(const CVector3& a, const CVector3& b)
        {
            return !(a == b);
        }


        inline CVector3 operator*(float a, const CVector3& b)
        {
            return b * a;
        }


        inline CVector3 operator/(float a, const CVector3& b)
        {
            return b / a;
        }


        inline CVector3 operator+(float a, const CVector3& b)
        {
            return b + a;
        }


        inline CVector3 operator-(float a, const CVector3& b)
        {
            return b - a;
        }
    } // namespace SIMDMath
} // namespace krystallic