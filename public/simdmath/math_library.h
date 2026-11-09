/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
* 
*  Authors:  @leopard-bf187 && @RisovoePole
*
*  Description:
*
*  Date: 05.01.2026
*/


#pragma once


#include "declaration.h"


namespace krystallic
{
    namespace SIMDMath
    {
        struct CColorRGB;
        struct CColorRGBA;
        struct CColorHSLA;
        struct CColorHSVA;
        struct CColorYUVA;
        struct CVector2;
        struct CVector3;
        struct CVector4;
        struct CQuaternion;
        struct CMatrix2x2;
        struct CMatrix3x3;
        struct CMatrix4x3;
        struct CMatrix4x4;

	    typedef CVector4 CPlane;

        struct CSimd128x1F;
        struct CSimd128x2F;
        struct CSimd128x3F;
        struct CSimd128x4F;


        struct Float16;
        typedef float Float32;
        typedef double Float64;


        inline void F32ToF16(const Float32* src, int num, Float16* dst);
        inline void F16ToF32(const Float16* src, int num, Float32* dst);


        struct Float16
        {
            word bits;

            constexpr Float16() noexcept;
            explicit Float16(Float32 value) noexcept;
            constexpr Float16(const Float16&) noexcept = default;
            constexpr Float16& operator=(const Float16&) noexcept = default;
            Float16& operator=(Float32 value) noexcept;
            operator float() const noexcept;
            constexpr word ToBits() const noexcept;
            static constexpr Float16 FromBits(word value) noexcept;
        }; 
        

        static_assert(sizeof(word) == 2, "Float16 requires 16-bit word type");
        static_assert(sizeof(Float32) == 4, "Float16 requires 32-bit Float32");


        struct RGBA32F
        {
            Float32 r;
            Float32 g;
            Float32 b;
            Float32 a;
        };

        struct RGBA32S
        {
            int32 r;
            int32 g;
            int32 b;
            int32 a;
        };

        struct RGBA32U
        {
            uint32 r;
            uint32 g;
            uint32 b;
            uint32 a;
        };

        struct RGB32F
        {
            Float32 r;
            Float32 g;
            Float32 b;
        };

        struct RGB32S
        {
            int32 r;
            int32 g;
            int32 b;
        };

        struct RGB32U
        {
            uint32 r;
            uint32 g;
            uint32 b;
        };

        struct RGBA16F
        {
            Float16 r;
            Float16 g;
            Float16 b;
            Float16 a;
        };

        struct RGBA16S
        {
            int16 r;
            int16 g;
            int16 b;
            int16 a;
        };

        struct RGBA16U
        {
            uint16 r;
            uint16 g;
            uint16 b;
            uint16 a;
        };

        struct RGBA8S
        {
            int8 r;
            int8 g;
            int8 b;
            int8 a;
        };

        struct RGBA8U
        {
            uint8 r;
            uint8 g;
            uint8 b;
            uint8 a;
        };

        struct RGB8S
        {
            int8 r;
            int8 g;
            int8 b;
        };

        struct RGB8U
        {
            uint8 r;
            uint8 g;
            uint8 b;
        };

        struct BGRA8U
        {
            uint8 b;
            uint8 g;
            uint8 r;
            uint8 a;
        };

        struct BGR8U
        {
            uint8 b;
            uint8 g;
            uint8 r;
        };


        struct CColorRGB
        {
            union
            {
                struct
                {
                    float x, y, z;
                };
                struct
                {
                    float r, g, b;
                };
                float v[4];
            };

            CColorRGB();
            CColorRGB(float val);
            CColorRGB(float r, float g, float b);
            CColorRGB(const float* arr);
            CColorRGB(const CColorRGB& copy);

            operator float* ();
            operator const float* () const;

            CColorRGB operator + () const;
            CColorRGB operator - () const;

            CColorRGB operator + (const CColorRGB& a) const;
            CColorRGB operator + (float val) const;
            CColorRGB operator - (const CColorRGB& a) const;
            CColorRGB operator - (float val) const;
            CColorRGB operator * (const CColorRGB& a) const;
            CColorRGB operator * (float val) const;
            CColorRGB operator / (const CColorRGB& a) const;
            CColorRGB operator / (float val) const;

            CColorRGB& operator += (const CColorRGB& a);
            CColorRGB& operator += (float val);
            CColorRGB& operator -= (const CColorRGB& a);
            CColorRGB& operator -= (float val);
            CColorRGB& operator *= (const CColorRGB& a);
            CColorRGB& operator *= (float val);
            CColorRGB& operator /= (const CColorRGB& a);
            CColorRGB& operator /= (float val);

            CColorRGB& operator = (const float* arr);
            CColorRGB& operator = (const CColorRGB& a);
            CColorRGB& operator = (float val);

            bool IsNaN();
            bool IsInf();

            float GrayScale() const;
            CColorRGB Negative() const;
            CColorRGB Add(const CColorRGB& other) const;
            float Dot(const CColorRGB& other) const;
            CColorRGB Subtract(const CColorRGB& other) const;
            CColorRGB Modulate(const CColorRGB& other) const;
            CColorRGB Scale(float scale) const;
            CColorRGB Lerp(const CColorRGB& b, float s);
            CColorRGB& AdjustContrast(float contrast);
            CColorRGB& AdjustSaturation(float saturation);

            CColorRGB& LoadRGB32F(const RGB32F& val);
            CColorRGB& LoadRGB32S(const RGB32S& val);
            CColorRGB& LoadRGB32U(const RGB32U& val);
            CColorRGB& LoadRGBA32F(const RGBA32F& val);
            CColorRGB& LoadRGBA32S(const RGBA32S& val);
            CColorRGB& LoadRGBA32U(const RGBA32U& val);
            CColorRGB& LoadRGBA16F(const RGBA16F& val);
            CColorRGB& LoadRGBA16S(const RGBA16S& val);
            CColorRGB& LoadRGBA16U(const RGBA16U& val);
            CColorRGB& LoadRGBA8S(const RGBA8S& val);
            CColorRGB& LoadRGBA8U(const RGBA8U& val);
            CColorRGB& LoadRGB8S(const RGB8S& val);
            CColorRGB& LoadRGB8U(const RGB8U& val);
            CColorRGB& LoadBGRA8U(const BGRA8U& val);
            CColorRGB& LoadBGR8U(const BGR8U& val);

            RGB32F StoreRGB32F();
            RGB32S StoreRGB32S();
            RGB32U StoreRGB32U();
            RGBA32F StoreRGBA32F();
            RGBA32S StoreRGBA32S();
            RGBA32U StoreRGBA32U();
            RGBA16F StoreRGBA16F();
            RGBA16S StoreRGBA16S();
            RGBA16U StoreRGBA16U();
            RGBA8S StoreRGBA8S();
            RGBA8U StoreRGBA8U();
            RGB8S StoreRGB8S();
            RGB8U StoreRGB8U();
            BGRA8U StoreBGRA8U();
            BGR8U StoreBGR8U();
        };

        inline bool operator == (const CColorRGB& a, const CColorRGB& b);
        inline bool operator != (const CColorRGB& a, const CColorRGB& b);

        inline CColorRGB operator * (float a, const CColorRGB& b);
        inline CColorRGB operator / (float a, const CColorRGB& b);
        inline CColorRGB operator + (float a, const CColorRGB& b);
        inline CColorRGB operator - (float a, const CColorRGB& b);


        struct CColorRGBA
        {
            union
            {
                struct
                {
                    float x, y, z, w;
                };
                struct
                {
                    float r, g, b, a;
                };
                float v[4];
            };

            CColorRGBA();
            CColorRGBA(float val);
            CColorRGBA(float r, float g, float b, float a);
            CColorRGBA(const float* arr);
            CColorRGBA(const CColorRGBA& copy);

            operator float* ();
            operator const float* () const;

            CColorRGBA operator + () const;
            CColorRGBA operator - () const;

            CColorRGBA operator + (const CColorRGBA& other) const;
            CColorRGBA operator + (float val) const;
            CColorRGBA operator - (const CColorRGBA& other) const;
            CColorRGBA operator - (float val) const;
            CColorRGBA operator * (const CColorRGBA& other) const;
            CColorRGBA operator * (float val) const;
            CColorRGBA operator / (const CColorRGBA& other) const;
            CColorRGBA operator / (float val) const;

            CColorRGBA& operator += (const CColorRGBA& other);
            CColorRGBA& operator += (float val);
            CColorRGBA& operator -= (const CColorRGBA& other);
            CColorRGBA& operator -= (float val);
            CColorRGBA& operator *= (const CColorRGBA& other);
            CColorRGBA& operator *= (float val);
            CColorRGBA& operator /= (const CColorRGBA& other);
            CColorRGBA& operator /= (float val);

            CColorRGBA& operator = (const float* arr);
            CColorRGBA& operator = (const CColorRGBA& other);
            CColorRGBA& operator = (float val);

            bool IsNaN();
            bool IsInf();

            float GrayScale() const;
            CColorRGBA Negative() const;
            CColorRGBA Add(const CColorRGBA& other) const;
            float Dot(const CColorRGBA& other) const;
            CColorRGBA Subtract(const CColorRGBA& other) const;
            CColorRGBA Modulate(const CColorRGBA& other) const;
            CColorRGBA Scale(float scale) const;
            CColorRGBA Lerp(const CColorRGBA& other, float s);
            CColorRGBA& AdjustContrast(float contrast);
            CColorRGBA& AdjustSaturation(float saturation);

            CColorRGBA& LoadRGB32F (const RGB32F& val);
            CColorRGBA& LoadRGB32S(const RGB32S& val);
            CColorRGBA& LoadRGB32U(const RGB32U& val);
            CColorRGBA& LoadRGBA32F(const RGBA32F& val);
            CColorRGBA& LoadRGBA32S(const RGBA32S& val);
            CColorRGBA& LoadRGBA32U(const RGBA32U& val);
            CColorRGBA& LoadRGBA16F(const RGBA16F& val);
            CColorRGBA& LoadRGBA16S(const RGBA16S& val);
            CColorRGBA& LoadRGBA16U(const RGBA16U& val);
            CColorRGBA& LoadRGBA8S(const RGBA8S& val);
            CColorRGBA& LoadRGBA8U(const RGBA8U& val);
            CColorRGBA& LoadRGB8S(const RGB8S& val);
            CColorRGBA& LoadRGB8U(const RGB8U& val);
            CColorRGBA& LoadBGRA8U(const BGRA8U& val);
            CColorRGBA& LoadBGR8U(const BGR8U& val);

            RGB32F StoreRGB32F();
            RGB32S StoreRGB32S();
            RGB32U StoreRGB32U();
            RGBA32F StoreRGBA32F();
            RGBA32S StoreRGBA32S();
            RGBA32U StoreRGBA32U();
            RGBA16F StoreRGBA16F();
            RGBA16S StoreRGBA16S();
            RGBA16U StoreRGBA16U();
            RGBA8S StoreRGBA8S();
            RGBA8U StoreRGBA8U();
            RGB8S StoreRGB8S();
            RGB8U StoreRGB8U();
            BGRA8U StoreBGRA8U();
            BGR8U StoreBGR8U();
        };

        inline bool operator == (const CColorRGBA& a, const CColorRGBA& b);
        inline bool operator != (const CColorRGBA& a, const CColorRGBA& b);

        inline CColorRGBA operator * (float a, const CColorRGBA& b);
        inline CColorRGBA operator / (float a, const CColorRGBA& b);
        inline CColorRGBA operator + (float a, const CColorRGBA& b);
        inline CColorRGBA operator - (float a, const CColorRGBA& b);


        struct CColorHSLA
        {
            float h, s, l, a;

            CColorHSLA() = default;
        };


        struct CColorHSVA
        {
            float h, s, v, a;

            CColorHSVA() = default;
        };


        struct CColorYUVA
        {
            float y, u, v, a;

            CColorYUVA() = default;
        };



        struct CVector2
        {
            union
            {
                struct
                {
                    float x, y;
                };
                float v[2];
            };

            CVector2();
            CVector2(float val);
            CVector2(const float* arr);
            CVector2(float _x, float _y);
            CVector2(const CVector2& copy);

            operator float*();
            operator const float*() const;

            CVector2 operator+() const;
            CVector2 operator-() const;

            CVector2  operator+(const CVector2& a) const;
            CVector2  operator+(float a) const;
            CVector2  operator-(const CVector2& a) const;
            CVector2  operator-(float a) const;
            CVector2  operator*(const CVector2& a) const;
            CVector2  operator*(float a) const;
            CVector2  operator/(const CVector2& a) const;
            CVector2  operator/(float a) const;

            CVector2& operator+=(const CVector2& a);
            CVector2& operator+=(float a);
            CVector2& operator-=(const CVector2& a);
            CVector2& operator-=(float a);
            CVector2& operator*=(const CVector2& a);
            CVector2& operator*=(float a);
            CVector2& operator/=(const CVector2& a);
            CVector2& operator/=(float a);

            CVector2& operator=(const float* arr);
            CVector2& operator=(const CVector2& a);
            CVector2& operator=(float a);

            float    Length() const;
            float    LengthSq() const;
            float    InvLength() const;
            float    Dot(const CVector2& other) const;
            float    Cross(const CVector2& other) const;
            CVector2 CrossCW(const CVector2& other) const;
            CVector2 CrossCCW(const CVector2& other) const;
            CVector2 Normalize() const;
            CVector2 Lerp(const CVector2& b, float s);

            bool IsNaN() const;
            bool IsInf() const;
        };

        bool operator==(const CVector2& a, const CVector2& b);
        bool operator!=(const CVector2& a, const CVector2& b);

        CVector2 operator*(float a, const CVector2& b);
        CVector2 operator/(float a, const CVector2& b);
        CVector2 operator+(float a, const CVector2& b);
        CVector2 operator-(float a, const CVector2& b);



        struct CVector3
        {
            union
            {
                struct
                {
                    float x, y, z;
                };
                float v[3];
            };

            CVector3();
            CVector3(float value);
            CVector3(const float* arr);
            CVector3(float _x, float _y, float _z);
            CVector3(const CVector3& copy);

            CVector3 operator+() const;
            CVector3 operator-() const;

            operator float*();
            operator const float*() const;

            CVector3  operator+(const CVector3& a) const;
            CVector3  operator+(float a) const;
            CVector3  operator-(const CVector3& a) const;
            CVector3  operator-(float a) const;
            CVector3  operator*(const CVector3& a) const;
            CVector3  operator*(float a) const;
            CVector3  operator/(const CVector3& a) const;
            CVector3  operator/(float a) const;

            CVector3& operator+=(const CVector3& a);
            CVector3& operator+=(float a);
            CVector3& operator-=(const CVector3& a);
            CVector3& operator-=(float a);
            CVector3& operator*=(const CVector3& a);
            CVector3& operator*=(float a);
            CVector3& operator/=(const CVector3& a);
            CVector3& operator/=(float a);

            CVector3& operator=(const float* arr);
            CVector3& operator=(const CVector3& a);
            CVector3& operator=(float a);

            float    Length() const;
            float    LengthSq() const;
            float    InvLength() const;
            float    Dot(const CVector3& other) const;
            CVector3 Cross(const CVector3& other) const;
            CVector3 Normalize() const;
            CVector3 Lerp(const CVector3& b, float s);

            bool IsNaN() const;
            bool IsInf() const;
        };

        bool operator==(const CVector3& a, const CVector3& b);
        bool operator!=(const CVector3& a, const CVector3& b);

        CVector3 operator*(float a, const CVector3& b);
        CVector3 operator/(float a, const CVector3& b);
        CVector3 operator+(float a, const CVector3& b);
        CVector3 operator-(float a, const CVector3& b);



        struct CVector4
        {
            union
            {
                struct
                {
                    float x, y, z, w;
                };
                float v[4];
            };

            CVector4();
            CVector4(float value);
            CVector4(float _x, float _y, float _z, float _w);
            CVector4(const float* arr);
            CVector4(const CVector4& copy);

            operator float*();
            operator const float*() const;

            CVector4 operator+() const;
            CVector4 operator-() const;

            CVector4  operator+(const CVector4& a) const;
            CVector4  operator+(float a) const;
            CVector4  operator-(const CVector4& a) const;
            CVector4  operator-(float a) const;
            CVector4  operator*(const CVector4& a) const;
            CVector4  operator*(float a) const;
            CVector4  operator/(const CVector4& a) const;
            CVector4  operator/(float a) const;

            CVector4& operator+=(const CVector4& a);
            CVector4& operator+=(float a);
            CVector4& operator-=(const CVector4& a);
            CVector4& operator-=(float a);
            CVector4& operator*=(const CVector4& a);
            CVector4& operator*=(float a);
            CVector4& operator/=(const CVector4& a);
            CVector4& operator/=(float a);

            CVector4& operator=(const float* arr);
            CVector4& operator=(const CVector4& a);
            CVector4& operator=(float a);

            float    Length() const;
            float    LengthSq() const;
            float    InvLength() const;
            float    Dot(const CVector4& other) const;
            CVector4 Cross(const CVector4& v2, const CVector4& v3) const;
            CVector4 Normalize() const;
            CVector4 Lerp(const CVector4& b, float s);

            bool IsNaN() const;
            bool IsInf() const;
        };

        bool operator==(const CVector4& a, const CVector4& b);
        bool operator!=(const CVector4& a, const CVector4& b);

        CVector4 operator*(float a, const CVector4& b);
        CVector4 operator/(float a, const CVector4& b);
        CVector4 operator+(float a, const CVector4& b);
        CVector4 operator-(float a, const CVector4& b);



        struct CQuaternion
        {
            union
            {
                struct
                {
                    float b, c, d, a;
                };
                struct
                {
                    float x, y, z, w;
                };
                float v[4];
            };

            CQuaternion();
            CQuaternion(const float* arr);
            CQuaternion(float x, float y, float z, float w);
            CQuaternion(const CQuaternion& copy);

            operator float*();
            operator const float*() const;

            CQuaternion operator+() const;
            CQuaternion operator-() const;

            CQuaternion operator+(const CQuaternion& v) const;
            CQuaternion operator-(const CQuaternion& v) const;
            CQuaternion operator*(const CQuaternion& v) const;
            CQuaternion operator+(float v) const;
            CQuaternion operator-(float v) const;
            CQuaternion operator*(float v) const;
            CQuaternion operator/(float v) const;

            CQuaternion& operator+=(const CQuaternion& v);
            CQuaternion& operator-=(const CQuaternion& v);
            CQuaternion& operator*=(const CQuaternion& v);
            CQuaternion& operator+=(float v);
            CQuaternion& operator-=(float v);
            CQuaternion& operator*=(float v);
            CQuaternion& operator/=(float v);

            CQuaternion& operator=(const float* arr);
            CQuaternion& operator=(const CQuaternion& a);
            CQuaternion& operator=(float a);

            CQuaternion& Multiply(const CQuaternion& other);

            float Length() const;
            float LengthSq() const;
            float Dot(const CQuaternion& other) const;
            CQuaternion Conjugate() const;
            CQuaternion Normalize() const;
            CQuaternion Inverse() const;

            bool IsIdentity() const;
            bool IsInf() const;
            bool IsNaN() const;

            inline static CQuaternion Slerp(CQuaternion qa, CQuaternion qb, float t);
            inline static CQuaternion Squad(CQuaternion qa, CQuaternion qb, CQuaternion qc, CQuaternion qd, float t);
            inline static CQuaternion RotationAxis(CVector3 axis, float angle);
            inline static CQuaternion RotationMatrix(CMatrix4x4 matrix);
            inline static CQuaternion PitchYawRoll(float pitch, float yaw, float roll);
            inline static CQuaternion PitchYawRollVec(CVector3 pitchYawRall);
        };

        CQuaternion operator+(float a, const CQuaternion& b);
        CQuaternion operator-(float a, const CQuaternion& b);
        CQuaternion operator*(float a, const CQuaternion& b);

        bool operator==(const CQuaternion& a, const CQuaternion& b);
        bool operator!=(const CQuaternion& a, const CQuaternion& b);


        struct CMatrix2x2
        {
            union
            {
                struct
                {
                    float _11, _12;
                    float _21, _22;
                };
                // struct
                // {
                // 	CVector2 x;
                // 	CVector2 y;
                // };
                CVector2 r[2];
                float    v[4];
                float    m[2][2];
            };

            CMatrix2x2();
            CMatrix2x2(float m11, float m12, float m21, float m22);
            CMatrix2x2(const float* arr);
            CMatrix2x2(const CMatrix2x2& copy);

            operator float* ();
            operator const float* () const;

            CMatrix2x2 operator + () const;
            CMatrix2x2 operator - () const;

            CMatrix2x2 operator +(const CMatrix2x2& m) const;
            CMatrix2x2 operator -(const CMatrix2x2& m) const;
            CMatrix2x2 operator *(const CMatrix2x2& m) const;
            CMatrix2x2 operator *(float v) const;
            CMatrix2x2 operator /(float v) const;

            CMatrix2x2& operator +=(const CMatrix2x2& m);
            CMatrix2x2& operator -=(const CMatrix2x2& m);
            CMatrix2x2& operator *=(const CMatrix2x2& m);
            CMatrix2x2& operator *=(float v);
            CMatrix2x2& operator /=(float v);

            CMatrix2x2& operator =(const CMatrix2x2& m);
            CMatrix2x2& operator =(const float* arr);

            float Determinant() const;
            CMatrix2x2 Inverse() const;
            CMatrix2x2 Transpose() const;
            CMatrix2x2& Multiply(float v);
            CMatrix2x2& Multiply(const CMatrix2x2& other);
            CMatrix2x2 MultiplyTranspose(const CMatrix2x2& other) const;

            bool IsIdentity() const;
            bool IsInf() const;
            bool IsNaN() const;

            inline static CMatrix2x2 Identity();
            inline static CMatrix2x2 Rotation(float angle);
            inline static CMatrix2x2 Scale(float scaleX, float scaleY);
            inline static CMatrix2x2 ScaleFromVector(CVector2 scale);
            inline static CMatrix2x2 Skew(float skewXY, float skewYX);
            inline static CMatrix2x2 SkewFromVector(CVector2 skewVec2);
        };

        bool operator == (const CMatrix2x2& a, const CMatrix2x2& b);
        bool operator != (const CMatrix2x2& a, const CMatrix2x2& b);

        CMatrix2x2 operator * (float a, CMatrix2x2& b);



        struct CMatrix3x3
        {
            union
            {
                struct
                {
                    float _11, _12, _13;
                    float _21, _22, _23;
                    float _31, _32, _33;
                };
                //struct
                //{
                //    CVector3 x;
                //    CVector3 y;
                //    CVector3 z;
                //};
                CVector3 r[3];
                float v[9];
                float m[3][3];
            };

            CMatrix3x3();
            CMatrix3x3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);
            CMatrix3x3(const float* arr);
            CMatrix3x3(const CMatrix3x3& copy);

            operator float* ();
            operator const float* () const;

            CMatrix3x3 operator + () const;
            CMatrix3x3 operator - () const;

            CMatrix3x3 operator +(const CMatrix3x3& other) const;
            CMatrix3x3 operator -(const CMatrix3x3& other) const;
            CMatrix3x3 operator *(const CMatrix3x3& other) const;
            CMatrix3x3 operator *(float val) const;
            CMatrix3x3 operator /(float val) const;

            CMatrix3x3& operator +=(const CMatrix3x3& other);
            CMatrix3x3& operator -=(const CMatrix3x3& other);
            CMatrix3x3& operator *=(const CMatrix3x3& other);
            CMatrix3x3& operator *=(float val);
            CMatrix3x3& operator /=(float val);

            CMatrix3x3& operator =(const CMatrix3x3& other);
            CMatrix3x3& operator =(const float* arr);

            float Determinant() const;
            CMatrix3x3 Inverse() const;
            CMatrix3x3 Transpose() const;
            CMatrix3x3& Multiply(float val);
            CMatrix3x3& Multiply(const CMatrix3x3& other);
            CMatrix3x3& MultiplyTranspose(const CMatrix3x3& other);

            bool IsIdentity();
            bool IsInf();
            bool IsNaN();

            inline static CMatrix3x3 Identity();
            inline static CMatrix3x3 RotationX(float angle);
            inline static CMatrix3x3 RotationY(float angle);
            inline static CMatrix3x3 RotationZ(float angle);
            inline static CMatrix3x3 Scale(float scaleX, float scaleY, float scaleZ);
            inline static CMatrix3x3 ScaleFromVector(CVector3 scale);
            inline static CMatrix3x3 Skew(float skewXY, float skewXZ, float skewYX, float skewYZ, float skewZX, float skewZY);
            inline static CMatrix3x3 SkewFromVector(CVector2 skewX, CVector2 skewY, CVector2 skewZ);
            inline static CMatrix3x3 Translation(float translateX, float translateY);
            inline static CMatrix3x3 TranslationFromVector(CVector2 translationVec2);

            inline static CMatrix3x3 AffineTransformation(CVector2 scale, CVector2 rotationOrigin, float rotationAngle, CVector2 translation);
            inline static CMatrix3x3 Transformation(CVector2 scaleOrigin, float scaleOrientation, CVector2 scale, CVector2 rotationOrigin, float rotationAngle, CVector2 translation);
        };

        bool operator == (const CMatrix3x3& a, const CMatrix3x3& b);
        bool operator != (const CMatrix3x3& a, const CMatrix3x3& b);

        CMatrix3x3 operator * (float a, CMatrix3x3& b);



        struct CMatrix4x3
        {
            union
            {
                struct
                {
                    float _11, _12, _13;
                    float _21, _22, _23;
                    float _31, _32, _33;
                    float _41, _42, _43;
                };
                // struct
                // {
                // 	CVector3 x;
                // 	CVector3 y;
                // 	CVector3 z;
                // 	CVector3 w;
                // };
                CVector3 r[4];
                float    v[12];
                float    m[4][3];
            };

            CMatrix4x3();
            CMatrix4x3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33, float m41, float m42, float m43);
            CMatrix4x3(const float* arr);
            CMatrix4x3(const CMatrix4x3& copy);

            CMatrix4x3 operator - () const;
            CMatrix4x3 operator + () const;

            CMatrix4x3& operator =(const CMatrix4x3& other);
            CMatrix4x3& operator =(const float* arr);
        };



        struct CMatrix4x4
        {
            union
            {
                struct
                {
                    float _11, _12, _13, _14;
                    float _21, _22, _23, _24;
                    float _31, _32, _33, _34;
                    float _41, _42, _43, _44;
                };
                //struct
                //{
                //    CVector4 x;
                //    CVector4 y;
                //    CVector4 z;
                //    CVector4 w;
                //};
                CVector4 r[4];
                float v[16];
                float m[4][4];
            };

            CMatrix4x4();
            CMatrix4x4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
            CMatrix4x4(const float* arr);
            CMatrix4x4(const CMatrix4x4& copy);

            CMatrix4x4 operator + () const;
            CMatrix4x4 operator - () const;

            CMatrix4x4 operator +(const CMatrix4x4& other) const;
            CMatrix4x4 operator -(const CMatrix4x4& other) const;
            CMatrix4x4 operator *(const CMatrix4x4& other) const;
            CMatrix4x4 operator *(float val) const;
            CMatrix4x4 operator /(float val) const;

            CMatrix4x4& operator +=(const CMatrix4x4& other);
            CMatrix4x4& operator -=(const CMatrix4x4& other);
            CMatrix4x4& operator *=(const CMatrix4x4& other);
            CMatrix4x4& operator *=(float val);
            CMatrix4x4& operator /=(float val);

            CMatrix4x4& operator =(const CMatrix4x4& other);
            CMatrix4x4& operator =(const float* arr);

            float Determinant() const;
            CMatrix4x4 Inverse() const;
            CMatrix4x4 Transpose() const;
            CMatrix4x4& Multiply(float val);
            CMatrix4x4& Multiply(const CMatrix4x4& other);
            CMatrix4x4& MultiplyTranspose(const CMatrix4x4& other);

            bool IsIdentity() const;
            bool IsInf() const;
            bool IsNaN() const;

            inline static CMatrix4x4 Identity();
            inline static CMatrix4x4 LookAtLH(CVector3 eyePos, CVector3 focusPos, CVector3 upDir);
            inline static CMatrix4x4 LookAtRH(CVector3 eyePos, CVector3 focusPos, CVector3 upDir);
            inline static CMatrix4x4 LookToLH(CVector3 eyePos, CVector3 eyeDir, CVector3 upDir);
            inline static CMatrix4x4 LookToRH(CVector3 eyePos, CVector3 eyeDir, CVector3 upDir);
            inline static CMatrix4x4 OrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ);
            inline static CMatrix4x4 OrthographicRH(float viewWidth, float viewHeight, float nearZ, float farZ);
            inline static CMatrix4x4 OrthographicOffCenterLH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ);
            inline static CMatrix4x4 OrthographicOffCenterRH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ);
            inline static CMatrix4x4 PerspectiveLH(float viewWidth, float viewHeight, float nearZ, float farZ);
            inline static CMatrix4x4 PerspectiveRH(float viewWidth, float viewHeight, float nearZ, float farZ);
            inline static CMatrix4x4 PerspectiveFovXLH(float fovAngleX, float aspectRatio, float nearZ, float farZ);
            inline static CMatrix4x4 PerspectiveFovXRH(float fovAngleX, float aspectRatio, float nearZ, float farZ);
            inline static CMatrix4x4 PerspectiveFovYLH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
            inline static CMatrix4x4 PerspectiveFovYRH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
            inline static CMatrix4x4 PerspectiveOffCenterLH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ);
            inline static CMatrix4x4 PerspectiveOffCenterRH(float viewLeft, float viewTop, float viewRight, float viewBottom, float nearZ, float farZ);
            inline static CMatrix4x4 RotationX(float angle);
            inline static CMatrix4x4 RotationY(float angle);
            inline static CMatrix4x4 RotationZ(float angle);
            inline static CMatrix4x4 RotationAxis(CVector3 axis, float angle);
            inline static CMatrix4x4 RotationQuaternion(CQuaternion quat);
            inline static CMatrix4x4 RotationPitchYawRoll(float pitch, float yaw, float roll);
            inline static CMatrix4x4 RotationPitchYawRollFromVector(CVector3 pitchYawRoll);
            inline static CMatrix4x4 Scale(float scaleX, float scaleY, float scaleZ);
            inline static CMatrix4x4 ScaleFromVector(CVector3 scale);
            inline static CMatrix4x4 Skew(float skewXY, float skewXZ, float skewYX, float skewYZ, float skewZX, float skewZY);
            inline static CMatrix4x4 SkewFromVector(CVector2 skewX, CVector2 skewY, CVector2 skewZ);
            inline static CMatrix4x4 Translation(float translateX, float translateY, float translateZ);
            inline static CMatrix4x4 TranslationFromVector(CVector3 translationVec3);

            inline static CMatrix4x4 AffineTransformation(CVector3 scale, CVector3 rotationOrigin, CQuaternion rotationQuaternion, CVector3 translation);
            inline static CMatrix4x4 Transformation(CVector3 scaleOrigin, CQuaternion scaleOrientationQuaternion, CVector3 scale, CVector3 rotationOrigin, CQuaternion rotationQuaternion, CVector3 translation);
            inline static CMatrix4x4 AffineTransformation2D(CVector3 scale, CVector3 rotationOrigin, float rotationAngle, CVector2 translation);
            inline static CMatrix4x4 Transformation2D(CVector3 scaleOrigin, float scaleOrientation, CVector3 scale, CVector3 rotationOrigin, float rotationAngle, CVector3 translation);
        };

        bool operator == (const CMatrix4x4& a, const CMatrix4x4& b);
        bool operator != (const CMatrix4x4& a, const CMatrix4x4& b);

        CMatrix4x4 operator * (float a, const CMatrix4x4& b);
    } // namespace SIMDMath
} // namespace krystallic