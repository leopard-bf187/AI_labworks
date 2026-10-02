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


luaL_Reg ext_Vec2_Methods[]
{
	{"Length", ext_Vec2_Len},
	{"LengthSq", ext_Vec2_LenSq},
	{"InvLength", ext_Vec2_InvLen},
	{"Dot", ext_Vec2_Dot},
	{"Cross", ext_Vec2_Cross},
	{"CrossCW", ext_Vec2_CrossCW},
	{"CrossCCW", ext_Vec2_CrossCCW},
	{"Normalize", ext_Vec2_Norm},
	{"IsNaN", ext_Vec2_IsNan},
	{"IsInf", ext_Vec2_IsInf},
	{0, 0},
};


CVector2* ext_Vec2Create(lua_State* s)
{
	void* mem = lua_newuserdata(s, sizeof(CVector2));
	new (mem) CVector2(0.0f);

	luaL_getmetatable(s, "CVector2_mtbl");
	lua_setmetatable(s, -2);

	return static_cast<CVector2*>(mem);
}


int ext_Vec2_Ctor(lua_State* s)
{
	float x = luaL_optnumber(s, 1, 0.0f);
	float y = luaL_optnumber(s, 2, 0.0f);

	void* mem = lua_newuserdata(s, sizeof(CVector2));
	new (mem) CVector2(x, y);

	luaL_getmetatable(s, "CVector2_mtbl");
	lua_setmetatable(s, -2);

	return 1;
}


int ext_Vec2_Dtor(lua_State* s)
{
	return 1;
}


int ext_Vec2_Get(lua_State* s)
{
	CVector2* vec = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	const char* key = luaL_checkstring(s, 2);

	if (Stdlib::StrCmp(key, "x") == 0) { lua_pushnumber(s, vec->x); return 1; }
	if (Stdlib::StrCmp(key, "y") == 0) { lua_pushnumber(s, vec->y); return 1; }

	luaL_getmetatable(s, "CVector2_mtbl");
	lua_getfield(s, -1, "__methods");
	lua_pushvalue(s, 2);
	lua_gettable(s, -2);

//	lua_pushstring(s, "innerType");
//	lua_pushinteger(s, LUA_EXT_TVECTOR2);
//	lua_setmetatable(s, -3);

	return 1;
}


int ext_Vec2_Set(lua_State* s)
{
	CVector2* vec = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	const char* key = luaL_checkstring(s, 2);
	float value = luaL_checknumber(s, 3);

	if (Stdlib::StrCmp(key, "x") == 0) vec->x = value;
	if (Stdlib::StrCmp(key, "y") == 0) vec->y = value;

	return 1;
}


int ext_Vec2_Len(lua_State* s)
{
	CVector2* vec = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	lua_pushnumber(s, vec->Length());
	return 1;
}


int ext_Vec2_LenSq(lua_State* s)
{
	CVector2* vec = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	lua_pushnumber(s, vec->LengthSq());
	return 1;
}


int ext_Vec2_InvLen(lua_State* s)
{
	CVector2* vec = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	lua_pushnumber(s, vec->InvLength());
	return 1;
}


int ext_Vec2_Dot(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	CVector2* b = (CVector2*)luaL_checkudata(s, 2, "CVector2_mtbl");
	lua_pushnumber(s, a->Dot(*b));
	return 1;
}


int ext_Vec2_Cross(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	CVector2* b = (CVector2*)luaL_checkudata(s, 2, "CVector2_mtbl");
	lua_pushnumber(s, a->Cross(*b));
	return 1;
}


int ext_Vec2_CrossCW(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	CVector2* b = (CVector2*)luaL_checkudata(s, 2, "CVector2_mtbl");
	CVector2* res = ext_Vec2Create(s);
	*res = a->CrossCW(*b);
	return 1;
}


int ext_Vec2_CrossCCW(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	CVector2* b = (CVector2*)luaL_checkudata(s, 2, "CVector2_mtbl");
	CVector2* res = ext_Vec2Create(s);
	*res = a->CrossCCW(*b);
	return 1;
}


int ext_Vec2_Norm(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	CVector2* res = ext_Vec2Create(s);
	*res = a->Normalize();
	return 1;
}


int ext_Vec2_IsNan(lua_State* s)
{
	CVector2* vec = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	lua_pushboolean(s, vec->IsNaN());
	return 1;
}


int ext_Vec2_IsInf(lua_State* s)
{
	CVector2* vec = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	lua_pushboolean(s, vec->IsInf());
	return 1;
}


int ext_Vec2_Op_Add(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	CVector2* b = (CVector2*)luaL_checkudata(s, 2, "CVector2_mtbl");
	CVector2* res = ext_Vec2Create(s);
	*res = *a + *b;
	return 1;
}


int ext_Vec2_Op_Sub(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	CVector2* b = (CVector2*)luaL_checkudata(s, 2, "CVector2_mtbl");
	CVector2* res = ext_Vec2Create(s);
	*res = *a - *b;
	return 1;
}


int ext_Vec2_Op_Mul(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	float b = lua_tonumber(s, 2);
	CVector2* res = ext_Vec2Create(s);
	*res = *a * b;
	return 1;
}


int ext_Vec2_Op_Div(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	float b = lua_tonumber(s, 2);
	CVector2* res = ext_Vec2Create(s);
	*res = *a / b;
	return 1;
}


int ext_Vec2_Op_Neg(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	CVector2* res = ext_Vec2Create(s);
	*res = -*a;
	return 1;
}


int ext_Vec2_Op_Eq(lua_State* s)
{
	CVector2* a = (CVector2*)luaL_checkudata(s, 1, "CVector2_mtbl");
	CVector2* b = (CVector2*)luaL_checkudata(s, 2, "CVector2_mtbl");
	lua_pushboolean(s, *a == *b);
	return 1;
}


