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


luaL_Reg ext_Vec4_Methods[] =
{
	{"Length", ext_Vec4_Len},
	{"LengthSq", ext_Vec4_LenSq},
	{"InvLength", ext_Vec4_InvLen},
	{"Dot", ext_Vec4_Dot},
	{"Cross", ext_Vec4_Cross},
	{"Normalize", ext_Vec4_Norm},
	{"IsNaN", ext_Vec4_IsNan},
	{"IsInf", ext_Vec4_IsInf},
	{0, 0},
};


CVector4* ext_Vec4Create(lua_State* s)
{
	void* mem = lua_newuserdata(s, sizeof(CVector4));
	new (mem) CVector4(0.0f);

	luaL_getmetatable(s, "CVector4_mtbl");
	lua_setmetatable(s, -2);

	return static_cast<CVector4*>(mem);
}


int ext_Vec4_Ctor(lua_State* s)
{
	float x = luaL_optnumber(s, 1, 0.0f);
	float y = luaL_optnumber(s, 2, 0.0f);
	float z = luaL_optnumber(s, 3, 0.0f);
	float w = luaL_optnumber(s, 4, 0.0f);

	CVector4* mem = (CVector4*)lua_newuserdata(s, sizeof(CVector4));
	new (mem) CVector4(x, y, z, w);

	luaL_getmetatable(s, "CVector4_mtbl");
	lua_setmetatable(s, -2);

//	lua_pushstring(s, "innerType");
//	lua_pushinteger(s, LUA_EXT_TVECTOR4);
//	lua_setmetatable(s, -3);

	return 1;
}


int ext_Vec4_Dtor(lua_State* s)
{
	return 1;
}


int ext_Vec4_Get(lua_State* s)
{
	CVector4* vec = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	const char* key = luaL_checkstring(s, 2);

	if (Stdlib::StrCmp(key, "x") == 0) { lua_pushnumber(s, vec->x); return 1; }
	if (Stdlib::StrCmp(key, "y") == 0) { lua_pushnumber(s, vec->y); return 1; }
	if (Stdlib::StrCmp(key, "z") == 0) { lua_pushnumber(s, vec->z); return 1; }
	if (Stdlib::StrCmp(key, "w") == 0) { lua_pushnumber(s, vec->z); return 1; }

	luaL_getmetatable(s, "CVector4_mtbl");
	lua_getfield(s, -1, "__methods");
	lua_pushvalue(s, 2);
	lua_gettable(s, -2);

	return 1;
}


int ext_Vec4_Set(lua_State* s)
{
	CVector4* vec = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	const char* key = luaL_checkstring(s, 2);
	float value = luaL_checknumber(s, 3);

	if (Stdlib::StrCmp(key, "x") == 0) vec->x = value;
	if (Stdlib::StrCmp(key, "y") == 0) vec->y = value;
	if (Stdlib::StrCmp(key, "z") == 0) vec->z = value;
	if (Stdlib::StrCmp(key, "w") == 0) vec->z = value;

	return 1;
}


int ext_Vec4_Len(lua_State* s)
{
	CVector4* vec = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	lua_pushnumber(s, vec->Length());
	return 1;
}


int ext_Vec4_LenSq(lua_State* s)
{
	CVector4* vec = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	lua_pushnumber(s, vec->LengthSq());
	return 1;
}


int ext_Vec4_InvLen(lua_State* s)
{
	CVector4* vec = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	lua_pushnumber(s, vec->InvLength());
	return 1;
}


int ext_Vec4_Dot(lua_State* s)
{
	CVector4* a = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	CVector4* b = (CVector4*)luaL_checkudata(s, 2, "CVector4_mtbl");
	lua_pushnumber(s, a->Dot(*b));
	return 1;
}


int ext_Vec4_Cross(lua_State* s)
{
	CVector4* a = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	CVector4* b = (CVector4*)luaL_checkudata(s, 2, "CVector4_mtbl");
	CVector4* c = (CVector4*)luaL_checkudata(s, 3, "CVector4_mtbl");
	CVector4* res = ext_Vec4Create(s);
	*res = a->Cross(*b, *c);
	return 1;
}


int ext_Vec4_Norm(lua_State* s)
{
	CVector4* a = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	CVector4* res = ext_Vec4Create(s);
	*res = a->Normalize();
	return 1;
}


int ext_Vec4_IsNan(lua_State* s)
{
	CVector4* vec = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	lua_pushboolean(s, vec->IsNaN());
	return 1;
}


int ext_Vec4_IsInf(lua_State* s)
{
	CVector4* vec = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	lua_pushboolean(s, vec->IsInf());
	return 1;
}


int ext_Vec4_Op_Add(lua_State* s)
{
	CVector4* a = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	CVector4* b = (CVector4*)luaL_checkudata(s, 2, "CVector4_mtbl");
	CVector4* res = ext_Vec4Create(s);
	*res = *a + *b;
	return 1;
}


int ext_Vec4_Op_Sub(lua_State* s)
{
	CVector4* a = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	CVector4* b = (CVector4*)luaL_checkudata(s, 2, "CVector4_mtbl");
	CVector4* res = ext_Vec4Create(s);
	*res = *a - *b;
	return 1;
}


int ext_Vec4_Op_Mul(lua_State* s)
{
	CVector4* a = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	float b = lua_tonumber(s, 2);
	CVector4* res = ext_Vec4Create(s);
	*res = *a * b;
	return 1;
}


int ext_Vec4_Op_Div(lua_State* s)
{
	CVector4* a = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	float b = lua_tonumber(s, 2);
	CVector4* res = ext_Vec4Create(s);
	*res = *a / b;
	return 1;
}


int ext_Vec4_Op_Neg(lua_State* s)
{
	CVector4* a = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	CVector4* res = ext_Vec4Create(s);
	*res = -*a;
	return 1;
}


int ext_Vec4_Op_Eq(lua_State* s)
{
	CVector4* a = (CVector4*)luaL_checkudata(s, 1, "CVector4_mtbl");
	CVector4* b = (CVector4*)luaL_checkudata(s, 2, "CVector4_mtbl");
	lua_pushboolean(s, *a == *b);
	return 1;
}


