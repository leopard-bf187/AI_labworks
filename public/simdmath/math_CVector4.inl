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
        inline CVector4::CVector4() : 
            CVector4(0.0f, 0.0f, 0.0f, 0.0f)
        {
        }


        inline CVector4::CVector4(float value) : 
            CVector4(value, value, value, value)
        {
        }


        inline CVector4::CVector4(const float* arr) : 
            CVector4(arr ? arr[0] : 0.0f, arr ? arr[1] : 0.0f, arr ? arr[2] : 0.0f, arr ? arr[3] : 0.0f)
        {
        }


        inline CVector4::CVector4(float _x, float _y, float _z, float _w) : 
            x(_x), y(_y), z(_z), w(_w)
        {
        }


        inline CVector4::CVector4(const CVector4& copy) :
            x(copy.x), y(copy.y), z(copy.z), w(copy.w)
        {
        }


        inline CVector4 CVector4::operator+() const
        {
            return CVector4(*this);
        }


        inline CVector4 CVector4::operator-() const
        {
            return CVector4(-x, -y, -z, -w);
        }


        inline CVector4::operator float*()
        {
            return v;
        }


        inline CVector4::operator const float*() const
        {
            return v;
        }


        inline CVector4 CVector4::operator+(const CVector4& a) const
        {
            return CVector4(x + a.x, y + a.y, z + a.z, w + a.w);
        }


        inline CVector4 CVector4::operator+(float a) const
        {
            return CVector4(x + a, y + a, z + a, w + a);
        }


        inline CVector4 CVector4::operator-(const CVector4& a) const
        {
            return CVector4(x - a.x, y - a.y, z - a.z, w - a.w);
        }


        inline CVector4 CVector4::operator-(float a) const
        {
            return CVector4(x - a, y - a, z - a, w - a);
        }


        inline CVector4 CVector4::operator*(const CVector4& a) const
        {
            return CVector4(x * a.x, y * a.y, z / a.z, w / a.w);
        }


        inline CVector4 CVector4::operator*(float a) const
        {
            return CVector4(x * a, y * a, z * a, w * a);
        }


        inline CVector4 CVector4::operator/(const CVector4& a) const
        {
            return CVector4(x / a.x, y / a.y, z / a.z, w / a.w);
        }


        inline CVector4 CVector4::operator/(float a) const
        {
            float val = 1.0f / a;
            return CVector4(x * val, y * val, z * val, w * val);
        }


        inline CVector4& CVector4::operator+=(const CVector4& a)
        {
            x += a.x;
            y += a.y;
            z += a.z;
            w += a.w;
            return *this;
        }


        inline CVector4& CVector4::operator+=(float a)
        {
            x += a;
            y += a;
            z += a;
            w += a;
            return *this;
        }


        inline CVector4& CVector4::operator-=(const CVector4& a)
        {
            x -= a.x;
            y -= a.y;
            z -= a.z;
            w -= a.w;
            return *this;
        }


        inline CVector4& CVector4::operator-=(float a)
        {
            x -= a;
            y -= a;
            z -= a;
            w -= a;
            return *this;
        }


        inline CVector4& CVector4::operator*=(const CVector4& a)
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            w *= a.w;
            return *this;
        }


        inline CVector4& CVector4::operator*=(float a)
        {
            x *= a;
            y *= a;
            z *= a;
            w *= a;
            return *this;
        }


        inline CVector4& CVector4::operator/=(const CVector4& a)
        {
            x /= a.x;
            y /= a.y;
            z /= a.z;
            w /= a.w;
            return *this;
        }


        inline CVector4& CVector4::operator/=(float a)
        {
            float val = 1.0f / a;
            x *= val;
            y *= val;
            z *= val;
            w *= val;
            return *this;
        }


        inline CVector4& CVector4::operator=(const float* arr)
        {
            x = arr ? arr[0] : 0.0f;
            y = arr ? arr[1] : 0.0f;
            z = arr ? arr[2] : 0.0f;
            w = arr ? arr[3] : 0.0f;
            return *this;
        }


        inline CVector4& CVector4::operator=(const CVector4& a)
        {
            x = a.x;
            y = a.y;
            z = a.z;
            w = a.w;
            return *this;
        }


        inline CVector4& CVector4::operator=(float a)
        {
            x = a;
            y = a;
            z = a;
            w = a;
            return *this;
        }


        inline float CVector4::Length() const
        {
            return sqrtf(x * x + y * y + z * z + w * w);
        }


        inline float CVector4::LengthSq() const
        {
            return x * x + y * y + z * z + w * w;
        }


        inline float CVector4::InvLength() const
        {
            return 1.0f / Length();
        }


        inline float CVector4::Dot(const CVector4& other) const
        {
            return x * other.x + y * other.y + z * other.z + w * other.w;
        }


        inline CVector4 CVector4::Cross(const CVector4& b, const CVector4& c) const
        {
            return CVector4
            (
				((b.z * c.w - b.w * c.z) * y) - ((b.y * c.w - b.w * c.y) * z) + ((b.y * c.z - b.z * c.y) * w),
				((b.w * c.z - b.z * c.w) * x) - ((b.w * c.x - b.x * c.w) * z) + ((b.z * c.x - b.x * c.z) * w),
				((b.y * c.w - b.w * c.y) * x) - ((b.x * c.w - b.w * c.x) * y) + ((b.x * c.y - b.y * c.x) * w),
				((b.z * c.y - b.y * c.z) * x) - ((b.z * c.x - b.x * c.z) * y) + ((b.y * c.x - b.x * c.y) * z)
            );
        }


        inline CVector4 CVector4::Normalize() const
        {
            float len = InvLength();
            return CVector4(x * len, y * len, z * len, w * len);
        }


        inline CVector4 CVector4::Lerp(const CVector4& b, float s)
        {
            return *this + s * (b - *this);
        }


        inline bool CVector4::IsNaN() const
        {
            return HasFloatsOneNaN(v, 4);
        }


        inline bool CVector4::IsInf() const
        {
            return HasFloatsOneInf(v, 4);
        }


        inline bool operator==(const CVector4& a, const CVector4& b)
        {
            return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
        }


        inline bool operator!=(const CVector4& a, const CVector4& b)
        {
            return !(a == b);
        }


        inline CVector4 operator*(float a, const CVector4& b)
        {
            return b * a;
        }


        inline CVector4 operator/(float a, const CVector4& b)
        {
            return b / a;
        }


        inline CVector4 operator+(float a, const CVector4& b)
        {
            return b + a;
        }


        inline CVector4 operator-(float a, const CVector4& b)
        {
            return b - a;
        }
    } // namespace SIMDMath
} // namespace krystallic