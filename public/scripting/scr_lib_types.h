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

#include "../core.h"

#include "scr_lib_enums.h"


#ifdef __cplusplus
extern"C" {
#endif


namespace krystallic
{
	namespace Runtime
	{
		class CArg;
		class CArgStack;
		struct IScriptSystem;
		struct IScriptCompiler;
		struct IScriptContext;
		struct IScriptExecutionContext;
		struct IScriptErrorBuffer;
		struct IScript;


		struct SScriptFunctionDesc
		{
			const char* m_fnName;
			EScriptArgumentType m_returnType;
			EScriptArgumentType m_args[16];
			uint m_numArgs;
			const char* desc;
		};


		typedef int (*GlobalScriptFn)(CArgStack* stack);


		CORE_API ERRCODE CreateScriptingSystem(Common::IAllocator* allocatorObject, IScriptSystem** scriptSys);
	}
}


#ifdef __cplusplus
}
#endif


