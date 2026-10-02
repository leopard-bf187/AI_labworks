/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD
*
*  Description:
*
*  Date: 17.07.2025
*/


#pragma once


#include "scripting.h"
#include "stdlib_dll.h"
#include "lua.hpp"


using namespace krystallic;
using namespace krystallic::SIMDMath;


enum ELuaExtType : int
{
	LUA_EXT_TVECTOR2 = 0x3e8,
	LUA_EXT_TVECTOR3,
	LUA_EXT_TVECTOR4,
	LUA_EXT_TQUATERNION,
	LUA_EXT_TRGB,
	LUA_EXT_TRGBA,
	LUA_EXT_TPLANE,
};


extern int ext_GetType(lua_State* s, int pos);
extern int ext_CheckType(lua_State* s, int pos, ELuaExtType type);


extern int ext_KrystallicStd(lua_State*s);
extern int lua_DebugPrint(lua_State* l);
extern int lua_callGlobalFunction(lua_State* l);
extern int lua_callClassMethod(lua_State* l);
extern int lua_callClassNativeMethod(lua_State* l);



extern int ext_RegVector2(lua_State*s);
extern int ext_RegVector3(lua_State*s);
extern int ext_RegVector4(lua_State*s);
extern int ext_RegQuaternion(lua_State*s);
extern int ext_RegPlane(lua_State*s);
extern int ext_RegRGBA(lua_State*s);



extern CVector2* ext_Vec2Create(lua_State* s);
extern int ext_Vec2_Ctor(lua_State* s);
extern int ext_Vec2_Dtor(lua_State* s);
extern int ext_Vec2_Get(lua_State* s);
extern int ext_Vec2_Set(lua_State* s);

extern luaL_Reg ext_Vec2_Methods[];
extern int ext_Vec2_Len(lua_State* s);
extern int ext_Vec2_LenSq(lua_State* s);
extern int ext_Vec2_InvLen(lua_State* s);
extern int ext_Vec2_IsNan(lua_State* s);
extern int ext_Vec2_IsInf(lua_State* s);
extern int ext_Vec2_Norm(lua_State* s);
extern int ext_Vec2_Dot(lua_State* s);
extern int ext_Vec2_Cross(lua_State* s);
extern int ext_Vec2_CrossCW(lua_State* s);
extern int ext_Vec2_CrossCCW(lua_State* s);

extern int ext_Vec2_Op_Add(lua_State* s);
extern int ext_Vec2_Op_Sub(lua_State* s);
extern int ext_Vec2_Op_Mul(lua_State* s);
extern int ext_Vec2_Op_Div(lua_State* s);
extern int ext_Vec2_Op_Neg(lua_State* s);
extern int ext_Vec2_Op_Eq(lua_State* s);



extern CVector3* ext_Vec3Create(lua_State* s);
extern int ext_Vec3_Ctor(lua_State* s);
extern int ext_Vec3_Dtor(lua_State* s);
extern int ext_Vec3_Get(lua_State* s);
extern int ext_Vec3_Set(lua_State* s);

extern luaL_Reg ext_Vec3_Methods[];
extern int ext_Vec3_Len(lua_State* s);
extern int ext_Vec3_LenSq(lua_State* s);
extern int ext_Vec3_InvLen(lua_State* s);
extern int ext_Vec3_Norm(lua_State* s);
extern int ext_Vec3_Dot(lua_State* s);
extern int ext_Vec3_Cross(lua_State* s);
extern int ext_Vec3_IsNan(lua_State* s);
extern int ext_Vec3_IsInf(lua_State* s);

extern int ext_Vec3_Op_Add(lua_State* s);
extern int ext_Vec3_Op_Sub(lua_State* s);
extern int ext_Vec3_Op_Mul(lua_State* s);
extern int ext_Vec3_Op_Div(lua_State* s);
extern int ext_Vec3_Op_Neg(lua_State* s);
extern int ext_Vec3_Op_Eq(lua_State* s);




extern CVector4* ext_Vec4Create(lua_State* s);
extern int ext_Vec4_Ctor(lua_State* s);
extern int ext_Vec4_Dtor(lua_State* s);
extern int ext_Vec4_Get(lua_State* s);
extern int ext_Vec4_Set(lua_State* s);

extern luaL_Reg ext_Vec4_Methods[];
extern int ext_Vec4_Len(lua_State* s);
extern int ext_Vec4_LenSq(lua_State* s);
extern int ext_Vec4_Norm(lua_State* s);
extern int ext_Vec4_Dot(lua_State* s);
extern int ext_Vec4_Cross(lua_State* s);
extern int ext_Vec4_InvLen(lua_State* s);
extern int ext_Vec4_IsNan(lua_State* s);
extern int ext_Vec4_IsInf(lua_State* s);

extern int ext_Vec4_Op_Add(lua_State* s);
extern int ext_Vec4_Op_Sub(lua_State* s);
extern int ext_Vec4_Op_Mul(lua_State* s);
extern int ext_Vec4_Op_Div(lua_State* s);
extern int ext_Vec4_Op_Neg(lua_State* s);
extern int ext_Vec4_Op_Eq(lua_State* s);
