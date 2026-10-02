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


#include "scr_extlib.h"


luaL_Reg ext_Vec3_Methods[] =
{
	{"Length", ext_Vec3_Len},
	{"LengthSq", ext_Vec3_LenSq},
	{"InvLength", ext_Vec3_InvLen},
	{"Dot", ext_Vec3_Dot},
	{"Cross", ext_Vec3_Cross},
	{"Normalize", ext_Vec3_Norm},
	{"IsNaN", ext_Vec3_IsNan},
	{"IsInf", ext_Vec3_IsInf},
	{0, 0},
};


CVector3* ext_Vec3Create(lua_State* s)
{
	void* vec = lua_newuserdata(s, sizeof(CVector3));
	new (vec) CVector3(0.0f);

	luaL_getmetatable(s, "CVector3_mtbl");
	lua_setmetatable(s, -2);

	return static_cast<CVector3*>(vec);
}


int ext_Vec3_Ctor(lua_State* s)
{
	float x = luaL_optnumber(s, 1, 0.0f);
	float y = luaL_optnumber(s, 2, 0.0f);
	float z = luaL_optnumber(s, 3, 0.0f);

	void* vec = lua_newuserdata(s, sizeof(CVector3));
	new (vec) CVector3(x, y, z);

	luaL_getmetatable(s, "CVector3_mtbl");
	lua_setmetatable(s, -2);

//	lua_pushstring(s, "innerType");
//	lua_pushinteger(s, LUA_EXT_TVECTOR3);
//	lua_setmetatable(s, -3);

	return 1;
}


int ext_Vec3_Dtor(lua_State* s)
{
	return 1;
}


int ext_Vec3_Get(lua_State* s)
{
	CVector3* vec = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	const char* key = luaL_checkstring(s, 2);

	if (Stdlib::StrCmp(key, "x") == 0) { lua_pushnumber(s, vec->x); return 1; }
	if (Stdlib::StrCmp(key, "y") == 0) { lua_pushnumber(s, vec->y); return 1; }
	if (Stdlib::StrCmp(key, "z") == 0) { lua_pushnumber(s, vec->z); return 1; }

	luaL_getmetatable(s, "CVector3_mtbl");
	lua_getfield(s, -1, "__methods");
	lua_pushvalue(s, 2);
	lua_gettable(s, -2);
	return 1;
}


int ext_Vec3_Set(lua_State* s)
{
	CVector3* vec = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	const char* key = luaL_checkstring(s, 2);
	float value = luaL_checknumber(s, 3);

	if (Stdlib::StrCmp(key, "x") == 0) vec->x = value;
	if (Stdlib::StrCmp(key, "y") == 0) vec->y = value;
	if (Stdlib::StrCmp(key, "z") == 0) vec->z = value;

	return 1;
}


int ext_Vec3_Len(lua_State* s)
{
	CVector3* vec = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	lua_pushnumber(s, vec->Length());
	return 1;
}


int ext_Vec3_LenSq(lua_State* s)
{
	CVector3* vec = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	lua_pushnumber(s, vec->LengthSq());
	return 1;
}


int ext_Vec3_InvLen(lua_State* s)
{
	CVector3* vec = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	lua_pushnumber(s, vec->InvLength());
	return 1;
}


int ext_Vec3_Norm(lua_State* s)
{
	CVector3* a = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	CVector3* res = ext_Vec3Create(s);
	*res = a->Normalize();
	return 1;
}


int ext_Vec3_Dot(lua_State* s)
{
	CVector3* a = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	CVector3* b = (CVector3*)luaL_checkudata(s, 2, "CVector3_mtbl");
	lua_pushnumber(s, a->Dot(*b));
	return 1;
}


int ext_Vec3_Cross(lua_State* s)
{
	CVector3* a = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	CVector3* b = (CVector3*)luaL_checkudata(s, 2, "CVector3_mtbl");
	CVector3* res = ext_Vec3Create(s);
	*res = a->Cross(*b);
	return 1;
}


int ext_Vec3_IsNan(lua_State* s)
{
	CVector3* vec = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	lua_pushboolean(s, vec->IsNaN());
	return 1;
}


int ext_Vec3_IsInf(lua_State* s)
{
	CVector3* vec = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	lua_pushboolean(s, vec->IsInf());
	return 1;
}


int ext_Vec3_Op_Add(lua_State* s)
{
	CVector3* a = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	CVector3* b = (CVector3*)luaL_checkudata(s, 2, "CVector3_mtbl");
	CVector3* res = ext_Vec3Create(s);
	*res = *a + *b;
	return 1;
}


int ext_Vec3_Op_Sub(lua_State* s)
{
	CVector3* a = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	CVector3* b = (CVector3*)luaL_checkudata(s, 2, "CVector3_mtbl");
	CVector3* res = ext_Vec3Create(s);
	*res = *a - *b;
	return 1;
}


int ext_Vec3_Op_Mul(lua_State* s)
{
	CVector3* a = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	float b = lua_tonumber(s, 2);
	CVector3* res = ext_Vec3Create(s);
	*res = *a * b;
	return 1;
}


int ext_Vec3_Op_Div(lua_State* s)
{
	CVector3* a = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	float b = lua_tonumber(s, 2);
	CVector3* res = ext_Vec3Create(s);
	*res = *a / b;
	return 1;
}


int ext_Vec3_Op_Neg(lua_State* s)
{
	CVector3* a = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	CVector3* res = ext_Vec3Create(s);
	*res = -*a;
	return 1;
}


int ext_Vec3_Op_Eq(lua_State* s)
{
	CVector3* a = (CVector3*)luaL_checkudata(s, 1, "CVector3_mtbl");
	CVector3* b = (CVector3*)luaL_checkudata(s, 2, "CVector3_mtbl");
	lua_pushboolean(s, *a == *b);
	return 1;
}


