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


#include "pch.h"


namespace krystallic
{
	namespace Runtime
	{
		enum EScriptErrorCode : dword
		{
			SCR_ERR_OK,
			SCR_ERR_INVALID_ARGUMENT,
			SCR_ERR_NOT_SUPPORTED,
			SCR_ERR_NOT_IMPLEMENTED,
			SCR_ERR_NOT_INIT,
			SCR_ERR_OUT_OF_MEMORY,
			SCR_ERR_FILE_NOT_FOUND,
			SCR_ERR_NOT_EXISTS,
			SCR_ERR_RUNTIME,
			SCR_ERR_SYNTAX,
			SCR_ERR_MEMORY,
			SCR_ERR_OTHER,
			SCR_ERR_NO_SUCH_FUNCTION,
			SCR_ERR_ALREADY_REGISTERED,
		};

		enum EScriptArgumentType : dword
		{
			SCR_ARG_VOID,
			SCR_ARG_BOOL,
			SCR_ARG_INT,
			SCR_ARG_FLOAT,
			SCR_ARG_STR,
			SCR_ARG_VEC2,
			SCR_ARG_VEC3,
			SCR_ARG_VEC4,
			SCR_ARG_QUAT,
			SCR_ARG_RGBA,
			SCR_ARG_PLANE,
			SCR_ARG_OBJECT,
			SCR_ARG_IBASE,
		};

		enum ELuaStdlibLoadFlags : dword
		{
			LUA_STD_BASE = (1 << 0),
			LUA_STD_PACKAGE = (1 << 1),
			LUA_STD_COROUTINE = (1 << 2),
			LUA_STD_TABLE = (1 << 3),
			LUA_STD_IO = (1 << 4),
			LUA_STD_OS = (1 << 5),
			LUA_STD_STRING = (1 << 6),
			LUA_STD_MATH = (1 << 7),
			LUA_STD_UTF8 = (1 << 8),
			LUA_STD_DEBUG = (1 << 9),
			LUA_STD_KRYSTALLIC = (1 << 10),
			LUA_STD_EVERYTHING = 0xffffffff,
		};
	}
}


