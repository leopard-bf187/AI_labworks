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
		inline CColorRGB::CColorRGB() :
			CColorRGB(0.0f, 0.0f, 0.0f)
		{
		}


		inline CColorRGB::CColorRGB(float val) :
			CColorRGB(val, val, val)
		{
		}


		inline CColorRGB::CColorRGB(float _r, float _g, float _b) :
			r(_r), g(_g), b(_b)
		{
		}


		inline CColorRGB::CColorRGB(const float* arr)
		{
			r = arr ? arr[0] : 0.0f;
			g = arr ? arr[0] : 0.0f;
			b = arr ? arr[0] : 0.0f;
		}


		inline CColorRGB::CColorRGB(const CColorRGB& copy)
		{
			r = copy.r;
			g = copy.g;
			b = copy.b;
		}


		inline CColorRGB::operator float* ()
		{
			return v;
		}


		inline CColorRGB::operator const float* () const
		{
			return v;
		}


		inline CColorRGB CColorRGB::operator + () const
		{
			return *this;
		}


		inline CColorRGB CColorRGB::operator - () const
		{
			return CColorRGB(-r, -g, -b);
		}


		inline CColorRGB CColorRGB::operator + (const CColorRGB& other) const
		{
			return CColorRGB(r + other.r, g + other.g, b + other.b);
		}


		inline CColorRGB CColorRGB::operator + (float val) const
		{
			return CColorRGB(r + val, g + val, b + val);
		}


		inline CColorRGB CColorRGB::operator - (const CColorRGB& other) const
		{
			return CColorRGB(r - other.r, g - other.g, b - other.b);
		}


		inline CColorRGB CColorRGB::operator - (float val) const
		{
			return CColorRGB(r - val, g - val, b - val);
		}


		inline CColorRGB CColorRGB::operator * (const CColorRGB& other) const
		{
			return CColorRGB(r * other.r, g * other.g, b * other.b);
		}


		inline CColorRGB CColorRGB::operator * (float val) const
		{
			return CColorRGB(r * val, g * val, b * val);
		}


		inline CColorRGB CColorRGB::operator / (float val) const
		{
			float iv = 1.0f / val;
			return CColorRGB(r * iv, g * iv, b * iv);
		}


		inline CColorRGB& CColorRGB::operator +=(const CColorRGB& other)
		{
			r += other.r;
			g += other.g;
			b += other.b;
			return *this;
		}


		inline CColorRGB& CColorRGB::operator += (float val)
		{
			r += val;
			g += val;
			b += val;
			return *this;
		}


		inline CColorRGB& CColorRGB::operator -=(const CColorRGB& other)
		{
			r -= other.r;
			g -= other.g;
			b -= other.b;
			return *this;
		}


		inline CColorRGB& CColorRGB::operator -= (float val)
		{
			r -= val;
			g -= val;
			b -= val;
			return *this;
		}


		inline CColorRGB& CColorRGB::operator *= (float val)
		{
			r *= val;
			g *= val;
			b *= val;
			return *this;
		}


		inline CColorRGB& CColorRGB::operator /= (float val)
		{
			float iv = 1.0f / val;
			r *= iv;
			g *= iv;
			b *= iv;
			return *this;
		}


		inline CColorRGB& CColorRGB::operator = (const float* arr)
		{
			r = arr ? arr[0] : 0.0f;
			g = arr ? arr[0] : 0.0f;
			b = arr ? arr[0] : 0.0f;
			return *this;
		}


		inline CColorRGB& CColorRGB::operator = (const CColorRGB& other)
		{
			r = other.r;
			g = other.g;
			b = other.b;
			return *this;
		}


		inline CColorRGB& CColorRGB::operator = (float val)
		{
			r = val;
			g = val;
			b = val;
			return *this;
		}


		inline bool CColorRGB::IsNaN()
		{
			return HasFloatsOneNaN(v, sizeof(CColorRGB));
		}


		inline bool CColorRGB::IsInf()
		{
			return HasFloatsOneInf(v, sizeof(CColorRGB));
		}


		inline float CColorRGB::GrayScale() const
		{
			return Dot(CColorRGB(0.2125f, 0.7154f, 0.0721f));
		}


		inline CColorRGB CColorRGB::Negative() const
		{
			return -*this;
		}


		inline CColorRGB CColorRGB::Add(const CColorRGB& other) const
		{
			return *this + other;
		}


		inline float CColorRGB::Dot(const CColorRGB& other) const
		{
			return r * other.r + g * other.g + b * other.b;
		}


		inline CColorRGB CColorRGB::Subtract(const CColorRGB& other) const
		{
			return *this - other;
		}


		inline CColorRGB CColorRGB::Modulate(const CColorRGB& other) const
		{
			return *this * other;
		}


		inline CColorRGB CColorRGB::Scale(float scale) const
		{
			return *this * scale;
		}


		inline CColorRGB CColorRGB::Lerp(const CColorRGB& other, float s)
		{
			return *this + s * (other - *this);
		}


		inline CColorRGB& CColorRGB::AdjustContrast(float contrast)
		{
			*this = Lerp(0.5f, contrast);
			return *this;
		}


		inline CColorRGB& CColorRGB::AdjustSaturation(float saturation)
		{
			*this = Lerp(GrayScale(), saturation);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGB32F(const RGB32F& val)
		{
			r = val.r;
			g = val.g;
			b = val.b;
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGB32S(const RGB32S& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGB32U(const RGB32U& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGBA32F(const RGBA32F& val)
		{
			r = val.r;
			g = val.g;
			b = val.b;
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGBA32S(const RGBA32S& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGBA32U(const RGBA32U& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGBA16F(const RGBA16F& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGBA16S(const RGBA16S& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGBA16U(const RGBA16U& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGBA8S(const RGBA8S& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGBA8U(const RGBA8U& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGB8S(const RGB8S& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadRGB8U(const RGB8U& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadBGRA8U(const BGRA8U& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline CColorRGB& CColorRGB::LoadBGR8U(const BGR8U& val)
		{
			r = static_cast<float>(val.r);
			g = static_cast<float>(val.g);
			b = static_cast<float>(val.b);
			return *this;
		}


		inline RGB32F CColorRGB::StoreRGB32F()
		{
			return { r, g, b };
		}


		inline RGB32S CColorRGB::StoreRGB32S()
		{
			return { static_cast<int32>(r), static_cast<int32>(g), static_cast<int32>(b) };
		}


		inline RGB32U CColorRGB::StoreRGB32U()
		{
			return { static_cast<uint32>(r), static_cast<uint32>(g), static_cast<uint32>(b) };
		}


		inline RGBA32F CColorRGB::StoreRGBA32F()
		{
			return { r, g, b, 1.0f };
		}


		inline RGBA32S CColorRGB::StoreRGBA32S()
		{
			return { static_cast<int32>(r), static_cast<int32>(g), static_cast<int32>(b), static_cast<int32>(1) };
		}


		inline RGBA32U CColorRGB::StoreRGBA32U()
		{
			return { static_cast<uint32>(r), static_cast<uint32>(g), static_cast<uint32>(b), static_cast<uint32>(1) };
		}


		inline RGBA16F CColorRGB::StoreRGBA16F()
		{
			return { Float16(r), Float16(g), Float16(b), Float16(1.0f) };
		}


		inline RGBA16S CColorRGB::StoreRGBA16S()
		{
			return { static_cast<int16>(r), static_cast<int16>(g), static_cast<int16>(b), static_cast<int16>(1) };
		}


		inline RGBA16U CColorRGB::StoreRGBA16U()
		{
			return { static_cast<uint16>(r), static_cast<uint16>(g), static_cast<uint16>(b), static_cast<uint16>(1) };
		}


		inline RGBA8S CColorRGB::StoreRGBA8S()
		{
			return { static_cast<int8>(r), static_cast<int8>(g), static_cast<int8>(b), static_cast<int8>(1) };
		}


		inline RGBA8U CColorRGB::StoreRGBA8U()
		{
			return { static_cast<uint8>(r), static_cast<uint8>(g), static_cast<uint8>(b), static_cast<uint8>(1) };
		}


		inline RGB8S CColorRGB::StoreRGB8S()
		{
			return { static_cast<int8>(r), static_cast<int8>(g), static_cast<int8>(b) };
		}


		inline RGB8U CColorRGB::StoreRGB8U()
		{
			return { static_cast<uint8>(r), static_cast<uint8>(g), static_cast<uint8>(b) };
		}


		inline BGRA8U CColorRGB::StoreBGRA8U()
		{
			return { static_cast<uint8>(b), static_cast<uint8>(g), static_cast<uint8>(r), static_cast<uint8>(1) };
		}


		inline BGR8U CColorRGB::StoreBGR8U()
		{
			return { static_cast<uint8>(b), static_cast<uint8>(g), static_cast<uint8>(r) };
		}


		inline bool operator == (const CColorRGB& a, const CColorRGB& b)
		{
			return a.r == b.r && a.g == b.g && a.b == b.b;
		}


		inline bool operator != (const CColorRGB& a, const CColorRGB& b)
		{
			return !(a == b);
		}


		inline CColorRGB operator * (float a, const CColorRGB& b)
		{
			return CColorRGB(a * b.r, a * b.g, a * b.b);
		}


		inline CColorRGB operator / (float a, const CColorRGB& b)
		{
			return CColorRGB(a / b.r, a / b.g, a / b.b);
		}


		inline CColorRGB operator + (float a, const CColorRGB& b)
		{
			return CColorRGB(a + b.r, a + b.g, a + b.b);
		}


		inline CColorRGB operator - (float a, const CColorRGB& b)
		{
			return CColorRGB(a - b.r, a - b.g, a - b.b);
		}
	}
}