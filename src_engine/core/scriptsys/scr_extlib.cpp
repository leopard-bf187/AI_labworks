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


int ext_GetType(lua_State* s, int pos)
{
	int type = lua_type(s, pos);

	if (!(type == LUA_TTABLE || type == LUA_TUSERDATA || type == LUA_TLIGHTUSERDATA))
		return 0;

	lua_pushstring(s, "innerType");
	lua_gettable(s, pos);

	type = 0;
	if (lua_isnumber(s, -1))
		type = (int)lua_tonumber(s, -1);

	lua_settop(s, -2);
	return type;
}


int ext_CheckType(lua_State* s, int pos, ELuaExtType type)
{
	return ext_GetType(s, pos) == type;
}


int ext_KrystallicStd(lua_State* s)
{
	luaL_Reg r[]
	{
		//{"EXECUTE_SCRIPT", lua_EXECUTE_SCRIPT},
		//{"LogPrint", lua_LogPrint},
		//{"DebugPrint", lua_DebugPrint},
		{0, 0},
	};

	luaL_checkversion(s);
	lua_createtable(s, 0, sizeof(r) / sizeof(luaL_Reg) - 1);
	luaL_setfuncs(s, r, 0);

	lua_pushstring(s, KRYSTALLIC_BUILD_STR);
	lua_setfield(s, -2, "BuildStr");

	lua_pushstring(s, KRYSTALLIC_BUILD_STR_EXT);
	lua_setfield(s, -2, "BuildStrExt");

	lua_pushstring(s, KRYSTALLIC_BUILD_STR_FULL);
	lua_setfield(s, -2, "BuildStrFull");

	lua_pushstring(s, KRYSTALLIC_BUILD_CONFIG);
	lua_setfield(s, -2, "BuildConfig");

	lua_pushstring(s, KRYSTALLIC_BUILD_TIMESTAMP);
	lua_setfield(s, -2, "BuildTimeStamp");

	lua_pushstring(s, KRYSTALLIC_CODENAME);
	lua_setfield(s, -2, "Codename");

	lua_pushstring(s, KRYSTALLIC_PLATFORM);
	lua_setfield(s, -2, "Platform");

	lua_pushstring(s, KRYSTALLIC_COMPILER);
	lua_setfield(s, -2, "Compiler");

	lua_pushstring(s, KRYSTALLIC_VERSION_MAJ);
	lua_setfield(s, -2, "Major");

	lua_pushstring(s, KRYSTALLIC_VERSION_MIN);
	lua_setfield(s, -2, "Minor");

	lua_pushstring(s, KRYSTALLIC_VERSION_REV);
	lua_setfield(s, -2, "Revision");

	lua_pushstring(s, KRYSTALLIC_STAGE);
	lua_setfield(s, -2, "Stage");

	lua_pushstring(s, KRYSTALLIC_VERSION);
	lua_setfield(s, -2, "Version");

//	lua_pop(s, 1);

	return 1;
}


int ext_RegVector2(lua_State* s)
{
	luaL_newmetatable(s, "CVector2_mtbl");

	lua_newtable(s);
	luaL_setfuncs(s, ext_Vec2_Methods, 0);
	lua_setfield(s, -2, "__methods");

	lua_pushcfunction(s, ext_Vec2_Get);
	lua_setfield(s, -2, "__index");
	lua_pushcfunction(s, ext_Vec2_Set);
	lua_setfield(s, -2, "__newindex");

	lua_pushcfunction(s, ext_Vec2_Op_Add);
	lua_setfield(s, -2, "__add");
	lua_pushcfunction(s, ext_Vec2_Op_Sub);
	lua_setfield(s, -2, "__sub");
	lua_pushcfunction(s, ext_Vec2_Op_Mul);
	lua_setfield(s, -2, "__mul");
	lua_pushcfunction(s, ext_Vec2_Op_Div);
	lua_setfield(s, -2, "__div");
	lua_pushcfunction(s, ext_Vec2_Op_Neg);
	lua_setfield(s, -2, "__unm");
	lua_pushcfunction(s, ext_Vec2_Op_Eq);
	lua_setfield(s, -2, "__eq");

	lua_pushcfunction(s, ext_Vec2_Dtor);
	lua_setfield(s, -2, "__gc");

	lua_pop(s, 1);

	lua_pushcfunction(s, ext_Vec2_Ctor);
	lua_setglobal(s, "CVector2");

	return 1;
}


int ext_RegVector3(lua_State* s)
{
	luaL_newmetatable(s, "CVector3_mtbl");

	lua_newtable(s);
	luaL_setfuncs(s, ext_Vec3_Methods, 0);
	lua_setfield(s, -2, "__methods");

	lua_pushcfunction(s, ext_Vec3_Get);
	lua_setfield(s, -2, "__index");
	lua_pushcfunction(s, ext_Vec3_Set);
	lua_setfield(s, -2, "__newindex");

	lua_pushcfunction(s, ext_Vec3_Op_Add);
	lua_setfield(s, -2, "__add");
	lua_pushcfunction(s, ext_Vec3_Op_Sub);
	lua_setfield(s, -2, "__sub");
	lua_pushcfunction(s, ext_Vec3_Op_Mul);
	lua_setfield(s, -2, "__mul");
	lua_pushcfunction(s, ext_Vec3_Op_Div);
	lua_setfield(s, -2, "__div");
	lua_pushcfunction(s, ext_Vec3_Op_Neg);
	lua_setfield(s, -2, "__unm");
	lua_pushcfunction(s, ext_Vec3_Op_Eq);
	lua_setfield(s, -2, "__eq");

	lua_pushcfunction(s, ext_Vec3_Dtor);
	lua_setfield(s, -2, "__gc");

	lua_pop(s, 1);

	lua_pushcfunction(s, ext_Vec3_Ctor);
	lua_setglobal(s, "CVector3");

	return 1;
}


int ext_RegVector4(lua_State* s)
{
	luaL_newmetatable(s, "CVector4_mtbl");

	lua_newtable(s);
	luaL_setfuncs(s, ext_Vec4_Methods, 0);
	lua_setfield(s, -2, "__methods");

	lua_pushcfunction(s, ext_Vec4_Get);
	lua_setfield(s, -2, "__index");
	lua_pushcfunction(s, ext_Vec4_Set);
	lua_setfield(s, -2, "__newindex");

	lua_pushcfunction(s, ext_Vec4_Op_Add);
	lua_setfield(s, -2, "__add");
	lua_pushcfunction(s, ext_Vec4_Op_Sub);
	lua_setfield(s, -2, "__sub");
	lua_pushcfunction(s, ext_Vec4_Op_Mul);
	lua_setfield(s, -2, "__mul");
	lua_pushcfunction(s, ext_Vec4_Op_Div);
	lua_setfield(s, -2, "__div");
	lua_pushcfunction(s, ext_Vec4_Op_Neg);
	lua_setfield(s, -2, "__unm");
	lua_pushcfunction(s, ext_Vec4_Op_Eq);
	lua_setfield(s, -2, "__eq");

	lua_pushcfunction(s, ext_Vec4_Dtor);
	lua_setfield(s, -2, "__gc");

	lua_pop(s, 1);

	lua_pushcfunction(s, ext_Vec4_Ctor);
	lua_setglobal(s, "CVector4");

	return 1;
}


int ext_RegQuaternion(lua_State* s)
{
	return 1;
}


int ext_RegPlane(lua_State* s)
{
	return 1;
}


int ext_RegRGBA(lua_State* s)
{
	return 1;
}