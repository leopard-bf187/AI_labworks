/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors:  @leopard-bf187 && @RisovoePole
*
*  Description:
*
*  Date: 11.08.2026
*/


#pragma once


#include "math_library.h"


namespace krystallic
{
    namespace SIMDMath
    {
        inline CColorRGBA::CColorRGBA() :
            CColorRGBA(0.0f, 0.0f, 0.0f, 1.0f)
        {
        }


        inline CColorRGBA::CColorRGBA(float val) :
            CColorRGBA(val, val, val, val)
        {
        }


        inline CColorRGBA::CColorRGBA(float _r, float _g, float _b, float _a) :
            r(_r), g(_g), b(_b), a(_a)
        {
        }


        inline CColorRGBA::CColorRGBA(const float* arr)
        {
            r = arr ? arr[0] : 0.0f;
            g = arr ? arr[0] : 0.0f;
            b = arr ? arr[0] : 0.0f;
            a = arr ? arr[0] : 1.0f;
        }


        inline CColorRGBA::CColorRGBA(const CColorRGBA& copy)
        {
            r = copy.r;
            g = copy.g;
            b = copy.b;
            a = copy.a;
        }


        inline CColorRGBA::operator float* ()
        {
            return v;
        }


        inline CColorRGBA::operator const float* () const
        {
            return v;
        }


        inline CColorRGBA CColorRGBA::operator + () const
        {
            return *this;
        }


        inline CColorRGBA CColorRGBA::operator - () const
        {
            return CColorRGBA(-r, -g, -b, -a);
        }


        inline CColorRGBA CColorRGBA::operator + (const CColorRGBA& other) const
        {
            return CColorRGBA(r + other.r, g + other.g, b + other.b, a + other.a);
        }


        inline CColorRGBA CColorRGBA::operator + (float val) const
        {
            return CColorRGBA(r + val, g + val, b + val, a + val);
        }


        inline CColorRGBA CColorRGBA::operator - (const CColorRGBA& other) const
        {
            return CColorRGBA(r - other.r, g - other.g, b - other.b, a - other.a);
        }


        inline CColorRGBA CColorRGBA::operator - (float val) const
        {
            return CColorRGBA(r - val, g - val, b - val, a - val);
        }


        inline CColorRGBA CColorRGBA::operator * (const CColorRGBA& other) const
        {
            return CColorRGBA(r * other.r, g * other.g, b * other.b, a * other.a);
        }


        inline CColorRGBA CColorRGBA::operator * (float val) const
        {
            return CColorRGBA(r * val, g * val, b * val, a * val);
        }


        inline CColorRGBA CColorRGBA::operator / (float val) const
        {
            float iv = 1.0f / val;
            return CColorRGBA(r * iv, g * iv, b * iv, a * iv);
        }


        inline CColorRGBA& CColorRGBA::operator +=(const CColorRGBA& other)
        {
            r += other.r;
            g += other.g;
            b += other.b;
            a += other.a;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::operator += (float val)
        {
            r += val;
            g += val;
            b += val;
            a += val;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::operator -=(const CColorRGBA& other)
        {
            r -= other.r;
            g -= other.g;
            b -= other.b;
            a -= other.a;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::operator -= (float val)
        {
            r -= val;
            g -= val;
            b -= val;
            a -= val;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::operator *= (float val)
        {
            r *= val;
            g *= val;
            b *= val;
            a *= val;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::operator /= (float val)
        {
            float iv = 1.0f / val;
            r *= iv;
            g *= iv;
            b *= iv;
            a *= iv;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::operator = (const float* arr)
        {
            r = arr ? arr[0] : 0.0f;
            g = arr ? arr[0] : 0.0f;
            b = arr ? arr[0] : 0.0f;
            a = arr ? arr[0] : 1.0f;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::operator = (const CColorRGBA& other)
        {
            r = other.r;
            g = other.g;
            b = other.b;
            a = other.a;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::operator = (float val)
        {
            r = val;
            g = val;
            b = val;
            a = val;
            return *this;
        }


        inline bool CColorRGBA::IsNaN()
        {
            return HasFloatsOneNaN(v, sizeof(CColorRGBA));
        }


        inline bool CColorRGBA::IsInf()
        {
            return HasFloatsOneInf(v, sizeof(CColorRGBA));
        }


        inline float CColorRGBA::GrayScale() const
        {
            return Dot(CColorRGBA(0.2125f, 0.7154f, 0.0721f, 0.0f));
        }


        inline CColorRGBA CColorRGBA::Negative() const
        {
            return -*this;
        }


        inline CColorRGBA CColorRGBA::Add(const CColorRGBA& other) const
        {
            return *this + other;
        }


        inline float CColorRGBA::Dot(const CColorRGBA& other) const
        {
            return r * other.r + g * other.g + b * other.b + a * other.a;
        }


        inline CColorRGBA CColorRGBA::Subtract(const CColorRGBA& other) const
        {
            return *this - other;
        }


        inline CColorRGBA CColorRGBA::Modulate(const CColorRGBA& other) const
        {
            return *this * other;
        }


        inline CColorRGBA CColorRGBA::Scale(float scale) const
        {
            return *this * scale;
        }


        inline CColorRGBA CColorRGBA::Lerp(const CColorRGBA& other, float s)
        {
            return *this + s * (other - *this);
        }


        inline CColorRGBA& CColorRGBA::AdjustContrast(float contrast)
        {
            *this = Lerp(0.5f, contrast);
            return *this;
        }


        inline CColorRGBA& CColorRGBA::AdjustSaturation(float saturation)
        {
            *this = Lerp(GrayScale(), saturation);
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGB32F(const RGB32F& val)
        {
            r = val.r;
            g = val.g;
            b = val.b;
            a = 1.0f;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGB32S(const RGB32S& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = 1.0f;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGB32U(const RGB32U& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = 1.0f;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGBA32F(const RGBA32F& val)
        {
            r = val.r;
            g = val.g;
            b = val.b;
            a = val.a;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGBA32S(const RGBA32S& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = static_cast<float>(val.a);
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGBA32U(const RGBA32U& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = static_cast<float>(val.a);
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGBA16F(const RGBA16F& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = static_cast<float>(val.a);
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGBA16S(const RGBA16S& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = static_cast<float>(val.a);
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGBA16U(const RGBA16U& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = static_cast<float>(val.a);
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGBA8S(const RGBA8S& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = static_cast<float>(val.a);
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGBA8U(const RGBA8U& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = static_cast<float>(val.a);
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGB8S(const RGB8S& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = 1.0f;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadRGB8U(const RGB8U& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = 1.0f;
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadBGRA8U(const BGRA8U& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = static_cast<float>(val.a);
            return *this;
        }


        inline CColorRGBA& CColorRGBA::LoadBGR8U(const BGR8U& val)
        {
            r = static_cast<float>(val.r);
            g = static_cast<float>(val.g);
            b = static_cast<float>(val.b);
            a = 1.0f;
            return *this;
        }


        inline RGB32F CColorRGBA::StoreRGB32F()
        {
            return { r, g, b };
        }


        inline RGB32S CColorRGBA::StoreRGB32S()
        {
            return { static_cast<int32>(r), static_cast<int32>(g), static_cast<int32>(b) };
        }


        inline RGB32U CColorRGBA::StoreRGB32U()
        {
            return { static_cast<uint32>(r), static_cast<uint32>(g), static_cast<uint32>(b) };
        }


        inline RGBA32F CColorRGBA::StoreRGBA32F()
        {
            return { r, g, b, a };
        }


        inline RGBA32S CColorRGBA::StoreRGBA32S()
        {
            return { static_cast<int32>(r), static_cast<int32>(g), static_cast<int32>(b), static_cast<int32>(a) };
        }


        inline RGBA32U CColorRGBA::StoreRGBA32U()
        {
            return { static_cast<uint32>(r), static_cast<uint32>(g), static_cast<uint32>(b), static_cast<uint32>(a) };
        }


        inline RGBA16F CColorRGBA::StoreRGBA16F()
        {
            return { Float16(r), Float16(g), Float16(b), Float16(a) };
        }


        inline RGBA16S CColorRGBA::StoreRGBA16S()
        {
            return { static_cast<int16>(r), static_cast<int16>(g), static_cast<int16>(b), static_cast<int16>(a) };
        }


        inline RGBA16U CColorRGBA::StoreRGBA16U()
        {
            return { static_cast<uint16>(r), static_cast<uint16>(g), static_cast<uint16>(b), static_cast<uint16>(a) };
        }


        inline RGBA8S CColorRGBA::StoreRGBA8S()
        {
            return { static_cast<int8>(r), static_cast<int8>(g), static_cast<int8>(b), static_cast<int8>(a) };
        }


        inline RGBA8U CColorRGBA::StoreRGBA8U()
        {
            return { static_cast<uint8>(r), static_cast<uint8>(g), static_cast<uint8>(b), static_cast<uint8>(a) };
        }


        inline RGB8S CColorRGBA::StoreRGB8S()
        {
            return { static_cast<int8>(r), static_cast<int8>(g), static_cast<int8>(b) };
        }


        inline RGB8U CColorRGBA::StoreRGB8U()
        {
            return { static_cast<uint8>(r), static_cast<uint8>(g), static_cast<uint8>(b) };
        }


        inline BGRA8U CColorRGBA::StoreBGRA8U()
        {
            return { static_cast<uint8>(b), static_cast<uint8>(g), static_cast<uint8>(r), static_cast<uint8>(a) };
        }


        inline BGR8U CColorRGBA::StoreBGR8U()
        {
            return { static_cast<uint8>(b), static_cast<uint8>(g), static_cast<uint8>(r) };
        }


        inline bool operator == (const CColorRGBA& a, const CColorRGBA& b)
        {
            return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
        }


        inline bool operator != (const CColorRGBA& a, const CColorRGBA& b)
        {
            return !(a == b);
        }


        inline CColorRGBA operator * (float a, const CColorRGBA& b)
        {
            return CColorRGBA(a * b.r, a * b.g, a * b.b, a * b.a);
        }


        inline CColorRGBA operator / (float a, const CColorRGBA& b)
        {
            return CColorRGBA(a / b.r, a / b.g, a / b.b, a / b.a);
        }


        inline CColorRGBA operator + (float a, const CColorRGBA& b)
        {
            return CColorRGBA(a + b.r, a + b.g, a + b.b, a + b.a);
        }


        inline CColorRGBA operator - (float a, const CColorRGBA& b)
        {
            return CColorRGBA(a - b.r, a - b.g, a - b.b, a - b.a);
        }
    }
} 