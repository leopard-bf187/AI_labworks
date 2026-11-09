/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD
*
*  Description:
*
*  Date: 10.07.2025
*/


#pragma once
#ifndef __vertexformats_h__
#define __vertexformats_h__


#include "pch.h"

#include "simdmath.h"


// Vertex component description:
// XYZ - Position (float3)
// R - Radius - Vertex size (for particle system) (float)
// T - Tangent (float3)
// N - Normal (float3)
// B - Binormal (float3)
// C8 - RGBA each chanell is a 8 bit component (dword)
// C32 - RGBA each chanell is a 32 bit component (float4)
// UV[W]n - texture (2D/3D) coordinates (float2 | float3), n = [1..8]


namespace krystallic
{
    namespace Render
    {
		typedef enum _VERTEX_CLASSIFICATION : dword
        {
            VERTEX_PER_VERTEX_DATA,
            VERTEX_PER_INSTANCE_DATA
        } VERTEX_CLASSIFICATION;

        // 00000000 00000000 00000000 00000000

		typedef enum _VERTEX_COMPONENT : dword
        {
            VERTEX_POSITION2 = (1 << 0),
            VERTEX_POSITION3 = (1 << 1),
            VERTEX_POSITION4 = (1 << 2),
            VERTEX_RADIUS = (1 << 3),
            VERTEX_NORMAL = (1 << 4),
            VERTEX_TANGENT = (1 << 5),
            VERTEX_BINORMAL = (1 << 6),
            VERTEX_COLOR8 = (1 << 7),
            VERTEX_COLOR32 = (1 << 8),
            VERTEX_UV1 = (1 << 9),
            VERTEX_UV2 = (1 << 10),
            VERTEX_UV3 = (1 << 11),
            VERTEX_UV4 = (1 << 12),
            VERTEX_UV5 = (1 << 13),
            VERTEX_UV6 = (1 << 14),
            VERTEX_UV7 = (1 << 15),
            VERTEX_UV8 = (1 << 16),
            VERTEX_UVW1 = (1 << 17),
            VERTEX_UVW2 = (1 << 18),
            VERTEX_UVW3 = (1 << 19),
            VERTEX_UVW4 = (1 << 20),
            VERTEX_UVW5 = (1 << 21),
            VERTEX_UVW6 = (1 << 22),
            VERTEX_UVW7 = (1 << 23),
            VERTEX_UVW8 = (1 << 24),
        } VERTEX_COMPONENT;


		typedef enum _VERTEX_FORMAT : dword
		{
			VERTEX_XYZ = VERTEX_POSITION3,
			VERTEX_XYZC8 = VERTEX_POSITION3 | VERTEX_COLOR8,
			VERTEX_XYZC32 = VERTEX_POSITION3 | VERTEX_COLOR32,
			VERTEX_XYZUV1 = VERTEX_POSITION3 | VERTEX_UV1,
			VERTEX_XYZUV2 = VERTEX_POSITION3 | VERTEX_UV2,
			VERTEX_XYZUV3 = VERTEX_POSITION3 | VERTEX_UV3,
			VERTEX_XYZC8UV1 = VERTEX_POSITION3 | VERTEX_COLOR8 | VERTEX_UV1,
			VERTEX_XYZC8UV2 = VERTEX_POSITION3 | VERTEX_COLOR8 | VERTEX_UV2,
			VERTEX_XYZC8UV3 = VERTEX_POSITION3 | VERTEX_COLOR8 | VERTEX_UV3,
			VERTEX_XYZC32UV1 = VERTEX_POSITION3 | VERTEX_COLOR32 | VERTEX_UV1,
			VERTEX_XYZC32UV2 = VERTEX_POSITION3 | VERTEX_COLOR32 | VERTEX_UV2,
			VERTEX_XYZC32UV3 = VERTEX_POSITION3 | VERTEX_COLOR32 | VERTEX_UV3,
			VERTEX_XYZN = VERTEX_POSITION3 | VERTEX_NORMAL,
			VERTEX_XYZNC8 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_COLOR8,
			VERTEX_XYZNC32 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_COLOR32,
			VERTEX_XYZNUV1 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_UV1,
			VERTEX_XYZNUV2 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_UV2,
			VERTEX_XYZNUV3 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_UV3,
			VERTEX_XYZNC8UV1 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_COLOR8 | VERTEX_UV1,
			VERTEX_XYZNC8UV2 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_COLOR8 | VERTEX_UV2,
			VERTEX_XYZNC8UV3 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_COLOR8 | VERTEX_UV3,
			VERTEX_XYZNC32UV1 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_COLOR32 | VERTEX_UV1,
			VERTEX_XYZNC32UV2 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_COLOR32 | VERTEX_UV2,
			VERTEX_XYZNC32UV3 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_COLOR32 | VERTEX_UV3,
			VERTEX_XYZNT = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT,
			VERTEX_XYZNTC8 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR8,
			VERTEX_XYZNTC32 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR32,
			VERTEX_XYZNTUV1 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_UV1,
			VERTEX_XYZNTUV2 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_UV2,
			VERTEX_XYZNTUV3 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_UV3,
			VERTEX_XYZNTC8UV1 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR8 | VERTEX_UV1,
			VERTEX_XYZNTC8UV2 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR8 | VERTEX_UV2,
			VERTEX_XYZNTC8UV3 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR8 | VERTEX_UV3,
			VERTEX_XYZNTC32UV1 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR32 | VERTEX_UV1,
			VERTEX_XYZNTC32UV2 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR32 | VERTEX_UV2,
			VERTEX_XYZNTC32UV3 = VERTEX_POSITION3 | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR32 | VERTEX_UV3,
			VERTEX_XYZR = VERTEX_POSITION3 | VERTEX_RADIUS,
			VERTEX_XYZRC8 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_COLOR8,
			VERTEX_XYZRC32 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_COLOR32,
			VERTEX_XYZRUV1 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_UV1,
			VERTEX_XYZRUV2 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_UV2,
			VERTEX_XYZRUV3 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_UV3,
			VERTEX_XYZRC8UV1 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_COLOR8 | VERTEX_UV1,
			VERTEX_XYZRC8UV2 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_COLOR8 | VERTEX_UV2,
			VERTEX_XYZRC8UV3 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_COLOR8 | VERTEX_UV3,
			VERTEX_XYZRC32UV1 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_COLOR32 | VERTEX_UV1,
			VERTEX_XYZRC32UV2 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_COLOR32 | VERTEX_UV2,
			VERTEX_XYZRC32UV3 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_COLOR32 | VERTEX_UV3,
			VERTEX_XYZRN = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL,
			VERTEX_XYZRNC8 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_COLOR8,
			VERTEX_XYZRNC32 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_COLOR32,
			VERTEX_XYZRNUV1 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_UV1,
			VERTEX_XYZRNUV2 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_UV2,
			VERTEX_XYZRNUV3 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_UV3,
			VERTEX_XYZRNC8UV1 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_COLOR8 | VERTEX_UV1,
			VERTEX_XYZRNC8UV2 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_COLOR8 | VERTEX_UV2,
			VERTEX_XYZRNC8UV3 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_COLOR8 | VERTEX_UV3,
			VERTEX_XYZRNC32UV1 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_COLOR32 | VERTEX_UV1,
			VERTEX_XYZRNC32UV2 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_COLOR32 | VERTEX_UV2,
			VERTEX_XYZRNC32UV3 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_COLOR32 | VERTEX_UV3,
			VERTEX_XYZRNT = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT,
			VERTEX_XYZRNTC8 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR8,
			VERTEX_XYZRNTC32 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR32,
			VERTEX_XYZRNTUV1 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_UV1,
			VERTEX_XYZRNTUV2 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_UV2,
			VERTEX_XYZRNTUV3 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_UV3,
			VERTEX_XYZRNTC8UV1 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR8 | VERTEX_UV1,
			VERTEX_XYZRNTC8UV2 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR8 | VERTEX_UV2,
			VERTEX_XYZRNTC8UV3 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR8 | VERTEX_UV3,
			VERTEX_XYZRNTC32UV1 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR32 | VERTEX_UV1,
			VERTEX_XYZRNTC32UV2 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR32 | VERTEX_UV2,
			VERTEX_XYZRNTC32UV3 = VERTEX_POSITION3 | VERTEX_RADIUS | VERTEX_NORMAL | VERTEX_TANGENT | VERTEX_COLOR32 | VERTEX_UV3,
			VERTEX_XYUV1C8 = VERTEX_POSITION2 | VERTEX_UV1 | VERTEX_COLOR8,
			VERTEX_NOFMT = 0xffffffffUL,
		} VERTEX_FORMAT, * PVERTEX_FORMAT;


		typedef struct _VTX_XYZ
        {
            SIMDMath::CVector3 pos;
        } XYZ;


		typedef struct _VTX_XYZC8
        {
            SIMDMath::CVector3 pos;
            dword          color;
        } XYZC8;


		typedef struct _VTX_XYZC32
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CColorRGBA color;
        } XYZC32;


		typedef struct _VTX_XYZUV1
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector2 uv1;
        } XYZUV1;


		typedef struct _VTX_XYZUV2
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZUV2;


		typedef struct _VTX_XYZUV3
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
            SIMDMath::CVector2 uv3;
        } XYZUV3;


		typedef struct _VTX_XYZC8UV1
        {
            SIMDMath::CVector3 pos;
            dword          color;
            SIMDMath::CVector2 uv1;
        } XYZC8UV1;


		typedef struct _VTX_XYZC8UV2
        {
            SIMDMath::CVector3 pos;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZC8UV2;


		typedef struct _VTX_XYZC8UV3
        {
            SIMDMath::CVector3 pos;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
            SIMDMath::CVector2 uv3;
        } XYZC8UV3;


		typedef struct _VTX_XYZC32UV1
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
        } XYZC32UV1;


		typedef struct _VTX_XYZC32UV2
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
        } XYZC32UV2;


		typedef struct _VTX_XYZC32UV3
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
            SIMDMath::CVector2   uv3;
        } XYZC32UV3;


		typedef struct _VTX_XYZN
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
        } XYZN;


		typedef struct _VTX_XYZNC8
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            dword          color;
        } XYZNC8;


		typedef struct _VTX_XYZNC32
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CVector3   normal;
            SIMDMath::CColorRGBA color;
        } XYZNC32;


		typedef struct _VTX_XYZNUV1
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector2 uv1;
        } XYZNUV1;


		typedef struct _VTX_XYZNUV2
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZNUV2;


		typedef struct _VTX_XYZNUV3
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
        } XYZNUV3;


		typedef struct _VTX_XYZNC8UV1
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            dword          color;
            SIMDMath::CVector2 uv1;
        } XYZNC8UV1;


		typedef struct _VTX_XYZNC8UV2
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZNC8UV2;


		typedef struct _VTX_XYZNC8UV3
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
            SIMDMath::CVector2 uv3;
        } XYZNC8UV3;


		typedef struct _VTX_XYZNC32UV1
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CVector3   normal;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
        } XYZNC32UV1;


		typedef struct _VTX_XYZNC32UV2
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CVector3   normal;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
        } XYZNC32UV2;


		typedef struct _VTX_XYZNC32UV3
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CVector3   normal;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
            SIMDMath::CVector2   uv3;
        } XYZNC32UV3;


		typedef struct _VTX_XYZNT
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
        } XYZNT;


		typedef struct _VTX_XYZNTC8
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            dword          color;
        } XYZNTC8;


		typedef struct _VTX_XYZNTC32
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CVector3   normal;
            SIMDMath::CVector3   tangent;
            SIMDMath::CColorRGBA color;
        } XYZNTC32;


		typedef struct _VTX_XYZNTUV1
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            SIMDMath::CVector2 uv1;
        } XYZNTUV1;


		typedef struct _VTX_XYZNTUV2
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZNTUV2;


		typedef struct _VTX_XYZNTUV3
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
            SIMDMath::CVector2 uv3;
        } XYZNTUV3;


		typedef struct _VTX_XYZNTC8UV1
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            dword          color;
            SIMDMath::CVector2 uv1;
        } XYZNTC8UV1;


		typedef struct _VTX_XYZNTC8UV2
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZNTC8UV2;


		typedef struct _VTX_XYZNTC8UV3
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
            SIMDMath::CVector2 uv3;
        } XYZNTC8UV3;


		typedef struct _VTX_XYZNTC32UV1
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CVector3   normal;
            SIMDMath::CVector3   tangent;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
        } XYZNTC32UV1;


		typedef struct _VTX_XYZNTC32UV2
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CVector3   normal;
            SIMDMath::CVector3   tangent;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
        } XYZNTC32UV2;


		typedef struct _VTX_XYZNTC32UV3
        {
            SIMDMath::CVector3   pos;
            SIMDMath::CVector3   normal;
            SIMDMath::CVector3   tangent;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
            SIMDMath::CVector2   uv3;
        } XYZNTC32UV3;


		typedef struct _VTX_XYZR
        {
            SIMDMath::CVector3 pos;
            float          radius;
        } XYZR;


		typedef struct _VTX_XYZRC8
        {
            SIMDMath::CVector3 pos;
            float          radius;
            dword          color;
        } XYZRC8;


		typedef struct _VTX_XYZRC32
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CColorRGBA color;
        } XYZRC32;


		typedef struct _VTX_XYZRUV1
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector2 uv1;
        } XYZRUV1;


		typedef struct _VTX_XYZRUV2
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZRUV2;


		typedef struct _VTX_XYZRUV3
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
            SIMDMath::CVector2 uv3;
        } XYZRUV3;


		typedef struct _VTX_XYZRC8UV1
        {
            SIMDMath::CVector3 pos;
            float          radius;
            dword          color;
            SIMDMath::CVector2 uv1;
        } XYZRC8UV1;


		typedef struct _VTX_XYZRC8UV2
        {
            SIMDMath::CVector3 pos;
            float          radius;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZRC8UV2;


		typedef struct _VTX_XYZRC8UV3
        {
            SIMDMath::CVector3 pos;
            float          radius;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
            SIMDMath::CVector2 uv3;
        } XYZRC8UV3;


		typedef struct _VTX_XYZRC32UV1
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
        } XYZRC32UV1;


		typedef struct _VTX_XYZRC32UV2
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
        } XYZRC32UV2;


		typedef struct _VTX_XYZRC32UV3
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
            SIMDMath::CVector2   uv3;
        } XYZRC32UV3;


		typedef struct _VTX_XYZRN
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
        } XYZRN;


		typedef struct _VTX_XYZRNC8
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            dword          color;
        } XYZRNC8;


		typedef struct _VTX_XYZRNC32
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector4 color;
        } XYZRNC32;


		typedef struct _VTX_XYZRNUV1
        {
            SIMDMath::CVector3 pos;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector2 uv1;
        } XYZRNUV1;


		typedef struct _VTX_XYZRNUV2
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZRNUV2;


		typedef struct _VTX_XYZRNUV3
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
        } XYZRNUV3;


		typedef struct _VTX_XYZRNC8UV1
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            dword          color;
            SIMDMath::CVector2 uv1;
        } XYZRNC8UV1;


		typedef struct _VTX_XYZRNC8UV2
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZRNC8UV2;


		typedef struct _VTX_XYZRNC8UV3
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
            SIMDMath::CVector2 uv3;
        } XYZRNC8UV3;


		typedef struct _VTX_XYZRNC32UV1
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CVector3   normal;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
        } XYZRNC32UV1;


		typedef struct _VTX_XYZRNC32UV2
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CVector3   normal;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
        } XYZRNC32UV2;


		typedef struct _VTX_XYZRNC32UV3
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CVector3   normal;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
            SIMDMath::CVector2   uv3;
        } XYZRNC32UV3;


		typedef struct _VTX_XYZRNT
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
        } XYZRNT;


		typedef struct _VTX_XYZRNTC8
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            dword          color;
        } XYZRNTC8;


		typedef struct _VTX_XYZRNTC32
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CVector3   normal;
            SIMDMath::CVector3   tangent;
            SIMDMath::CColorRGBA color;
        } XYZRNTC32;


		typedef struct _VTX_XYZRNTUV1
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            SIMDMath::CVector2 uv1;
        } XYZRNTUV1;


		typedef struct _VTX_XYZRNTUV2
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZRNTUV2;


		typedef struct _VTX_XYZRNTUV3
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
            SIMDMath::CVector2 uv3;
        } XYZRNTUV3;


		typedef struct _VTX_XYZRNTC8UV1
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            dword          color;
            SIMDMath::CVector2 uv1;
        } XYZRNTC8UV1;


		typedef struct _VTX_XYZRNTC8UV2
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
        } XYZRNTC8UV2;


		typedef struct _VTX_XYZRNTC8UV3
        {
            SIMDMath::CVector3 pos;
            float          radius;
            SIMDMath::CVector3 normal;
            SIMDMath::CVector3 tangent;
            dword          color;
            SIMDMath::CVector2 uv1;
            SIMDMath::CVector2 uv2;
            SIMDMath::CVector2 uv3;
        } XYZRNTC8UV3;


		typedef struct _VTX_XYZRNTC32UV1
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CVector3   normal;
            SIMDMath::CVector3   tangent;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
        } XYZRNTC32UV1;


		typedef struct _VTX_XYZRNTC32UV2
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CVector3   normal;
            SIMDMath::CVector3   tangent;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
        } XYZRNTC32UV2;


		typedef struct _VTX_XYZRNTC32UV3
        {
            SIMDMath::CVector3   pos;
            float            radius;
            SIMDMath::CVector3   normal;
            SIMDMath::CVector3   tangent;
            SIMDMath::CColorRGBA color;
            SIMDMath::CVector2   uv1;
            SIMDMath::CVector2   uv2;
            SIMDMath::CVector2   uv3;
        } XYZRNTC32UV3;


		typedef struct _VTX_XYUV1C8
        {
            SIMDMath::CVector2 pos;
            SIMDMath::CVector2 uv1;
            dword          color;
        } XYUV1C8;


		typedef struct _VTX_XYUV1C32DW2
        {
            SIMDMath::CVector2   pos;
            SIMDMath::CVector2   uv;
            SIMDMath::CColorRGBA color;
            uint             textureSlot;
            uint             textureArrayIndex;
        } XYUV1C32DW2;
    } // namespace Render
} // namespace krystallic


#endif
