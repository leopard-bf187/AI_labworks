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
        inline CVector2::CVector2() :
            CVector2(0.0f, 0.0f)
        {
        }


        inline CVector2::CVector2(float val) : 
            CVector2(val, val)
        {
        }


        inline CVector2::CVector2(const float* arr) : 
            CVector2(arr ? arr[0] : 0.0f, arr ? arr[1] : 0.0f)
        {
        }


        inline CVector2::CVector2(float _x, float _y) : 
            x(_x), y(_y)
        {
        }


        inline CVector2::CVector2(const CVector2& copy) :
            x(copy.x), y(copy.y)
        {
        }


        inline CVector2::operator float*()
        {
            return v;
        }


        inline CVector2::operator const float*() const
        {
            return v;
        }


        inline CVector2 CVector2::operator+() const
        {
            return *this;
        }


        inline CVector2 CVector2::operator-() const
        {
            return CVector2(-x, -y);
        }


        inline CVector2 CVector2::operator+(const CVector2& a) const
        {
            return CVector2(x + a.x, y + a.y);
        }


        inline CVector2 CVector2::operator+(float a) const
        {
            return CVector2(x + a, y + a);
        }


        inline CVector2 CVector2::operator-(const CVector2& a) const
        {
            return CVector2(x - a.x, y - a.y);
        }


        inline CVector2 CVector2::operator-(float a) const
        {
            return CVector2(x - a, y - a);
        }


        inline CVector2 CVector2::operator*(float a) const
        {
            return CVector2(x * a, y * a);
        }


        inline CVector2 CVector2::operator*(const CVector2& a) const
        {
            return CVector2(x * a.x, y * a.y);
        }


        inline CVector2 CVector2::operator/(float a) const
        {
            float val = 1.0f / a;
            return CVector2(x * val, y * val);
        }


        inline CVector2 CVector2::operator/(const CVector2& a) const
        {
            return CVector2(x / a.x, y / a.y);
        }


        inline CVector2& CVector2::operator+=(const CVector2& a)
        {
            x += a.x;
            y += a.y;
            return *this;
        }


        inline CVector2& CVector2::operator+=(float a)
        {
            x += a;
            y += a;
            return *this;
        }


        inline CVector2& CVector2::operator-=(const CVector2& a)
        {
            x -= a.x;
            y -= a.y;
            return *this;
        }


        inline CVector2& CVector2::operator-=(float a)
        {
            x -= a;
            y -= a;
            return *this;
        }


        inline CVector2& CVector2::operator*=(float a)
        {
            x *= a;
            y *= a;
            return *this;
        }


        inline CVector2& CVector2::operator*=(const CVector2& a)
        {
            x *= a.x;
            y *= a.y;
            return *this;
        }


        inline CVector2& CVector2::operator/=(float a)
        {
            float val = 1.0f / a;
            x *= val;
            y *= val;
            return *this;
        }


        inline CVector2& CVector2::operator/=(const CVector2& a)
        {
            x /= a.x;
            y /= a.y;
            return *this;
        }


        inline CVector2& CVector2::operator=(const float* arr)
        {
            x = arr ? arr[0] : 0.0f;
            y = arr ? arr[1] : 0.0f;
            return *this;
        }


        inline CVector2& CVector2::operator=(const CVector2& a)
        {
            x = a.x;
            y = a.y;
            return *this;
        }


        inline CVector2& CVector2::operator=(float a)
        {
            x = a;
            y = a;
            return *this;
        }


        inline float CVector2::Length() const
        {
            return sqrtf(x * x + y * y);
        }


        inline float CVector2::LengthSq() const
        {
            return x * x + y * y;
        }


        inline float CVector2::InvLength() const
        {
            return 1.0f / sqrtf(x * x + y * y);
        }


        inline float CVector2::Dot(const CVector2& other) const
        {
            return x * other.x + y * other.y;
        }


        inline float CVector2::Cross(const CVector2& other) const
        {
            return x * other.y - y * other.x;
        }


        inline CVector2 CVector2::CrossCW(const CVector2& other) const
        {
            float d = Cross(other);
            return CVector2(y * d, -x * d);
        }


        inline CVector2 CVector2::CrossCCW(const CVector2& other) const
        {
            float d = Cross(other);
            return CVector2(-y * d, x * d);
        }


        inline CVector2 CVector2::Normalize() const
        {
            float len = InvLength();
            return CVector2(x * len, y * len);
        }


        inline CVector2 CVector2::Lerp(const CVector2& b, float s)
        {
            return *this + s * (b - *this);
        }


        inline bool CVector2::IsNaN() const
        {
            return HasFloatsOneNaN(v, 2);
        }


        inline bool CVector2::IsInf() const
        {
            return HasFloatsOneInf(v, 2);
        }


        inline bool operator==(const CVector2& a, const CVector2& b)
        {
            return (a.x == b.x) && (a.y == b.y);
        }


        inline bool operator!=(const CVector2& a, const CVector2& b)
        {
            return !(a == b);
        }


        inline CVector2 operator*(float a, const CVector2& b)
        {
            return b * a;
        }


        inline CVector2 operator/(float a, const CVector2& b)
        {
            return b / a;
        }


        inline CVector2 operator+(float a, const CVector2& b)
        {
            return b + a;
        }


        inline CVector2 operator-(float a, const CVector2& b)
        {
            return b - a;
        }
    } // namespace SIMDMath
} // namespace krystallic