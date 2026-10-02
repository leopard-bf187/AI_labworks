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


#include "scr_classes.h"


#if defined(KRYSTALLIC_OS_WINNT)

#include <Windows.h>

#endif


inline void PrepareInputArgs(lua_State* l, CArgStack* stack, int argc)
{
	stack->Clear();

	for (int i = 2; i <= argc; i++)
	{
		int type = lua_type(l, i);

		if (type == LUA_TNIL)
			stack->PushInputArg()->SetBool(false);

		if (type == LUA_TBOOLEAN)
			stack->PushInputArg()->SetBool(lua_toboolean(l, i) != 0);

		if (type == LUA_TNUMBER)
		{
			if (lua_isinteger(l, i))
				stack->PushInputArg()->SetInt(lua_tointeger(l, i));
			else
				stack->PushInputArg()->SetFloat(lua_tonumber(l, i));
		}

		if (type == LUA_TSTRING)
			stack->PushInputArg()->SetString((char*)lua_tostring(l, i));

		if (type == LUA_TLIGHTUSERDATA || type == LUA_TUSERDATA)
		{
			void* userData = 0;

			if (userData = luaL_testudata(l, i, "CVector2_mtbl"))
				stack->PushInputArg()->SetVector2(*(CVector2*)userData);

			if (userData = luaL_testudata(l, i, "CVector3_mtbl"))
				stack->PushInputArg()->SetVector3(*(CVector3*)userData);

			if (userData = luaL_testudata(l, i, "CVector4_mtbl"))
				stack->PushInputArg()->SetVector4(*(CVector4*)userData);
		}
	}
}


inline void ProcessOutputArgs(lua_State* l, CArgStack* stack, int argc)
{
	for (int i = 0; i < argc; i++)
	{
		CArg* arg = stack->PopOutputArg();
		EScriptArgumentType type = arg->GetArgumentDataType();

		assert(stack->GetCurrOutArg() < argc);

		if (type == SCR_ARG_VOID)
			lua_pushnil(l);

		if (type == SCR_ARG_BOOL)
			lua_pushboolean(l, arg->AsBool());

		if (type == SCR_ARG_INT)
			lua_pushinteger(l, arg->AsInt());

		if (type == SCR_ARG_FLOAT)
			lua_pushnumber(l, arg->AsFloat());

		if (type == SCR_ARG_STR)
			lua_pushstring(l, arg->AsString());

		if (type == SCR_ARG_VEC2)
		{
			CVector2* v = ext_Vec2Create(l);
			*v = arg->AsVector2();
		}

		if (type == SCR_ARG_VEC3)
		{
			CVector3* v = ext_Vec3Create(l);
			*v = arg->AsVector2();
		}

		if (type == SCR_ARG_VEC4)
		{
			CVector4* v = ext_Vec4Create(l);
			*v = arg->AsVector2();
		}

		//if (type == SCR_ARG_QUAT);

		//if (type == SCR_ARG_RGBA);

		//if (type == SCR_ARG_PLANE);

		//if (type == SCR_ARG_OBJECT);

		//if (type == SCR_ARG_IBASE);
	}
}


int lua_DebugPrint(lua_State* l)
{
	char* str = 0;
	
	if (lua_type(l, 1) == LUA_TSTRING)
	{
		str = (char*)lua_tostring(l, 1);
	}

#if defined(KRYSTALLIC_OS_WINNT)

	OutputDebugString("SCRIPT SYSTEM: ");
	OutputDebugString(str);
	OutputDebugString("\n");

#endif

	return 1;
}


int lua_callGlobalFunction(lua_State* l)
{
	GlobalScriptFn func = 0;
	CArgStack stack;
	int numOutArgs = 0;

	func = *(GlobalScriptFn*)lua_touserdata(l, 1);

	int luaArgc = lua_gettop(l);
	int argc = (luaArgc > 18) ? 18 : luaArgc;

	PrepareInputArgs(l, &stack, argc);

	if (!func(&stack))
		return 0;

	numOutArgs = stack.GetNumOutputArgs();

	ProcessOutputArgs(l, &stack, numOutArgs);

	return numOutArgs;
}


int lua_callClassMethod(lua_State* l)
{
	return 1;
}


int lua_callClassNativeMethod(lua_State* l)
{
	return 1;
}


