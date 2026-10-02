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


#define CORE_API_EXPORT
#include "scr_classes.h"


namespace krystallic
{
	namespace Runtime
	{
		CORE_API ERRCODE CreateScriptingSystem(Common::IAllocator* allocatorObject, IScriptSystem** scriptSys)
		{
			RefCounted<IAllocator> allocator(allocatorObject);
			RefCounted<CScriptSystem> scr;

			if (!scriptSys)
				return SCR_ERR_INVALID_ARGUMENT;

			if (!allocator.Get())
				allocator.Attach(Stdlib::g_stdlibDefaultAllocator);

			scr.Attach(CScriptSystem::_Create(allocator.Get()));

			if (!scr.Get())
				return SCR_ERR_OUT_OF_MEMORY;

			if (*scriptSys)
			{
				(*scriptSys)->Delete();
				*scriptSys = nullptr;
			}

			*scriptSys = static_cast<IScriptSystem*>(scr.Get());
			scr->IncRef();

			return SCR_ERR_OK;
		}
	}
}





