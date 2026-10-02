/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 11.07.2026
*/

#define PLATFORM_API_EXPORT

#if defined(KRYSTALLIC_OS_WINNT)
#include "win32/win32_classes.h"
#elif defined(KRYSTALLIC_OS_LINUX)
#include "linux/linux_classes.h"
#elif defined(KRYSTALLIC_OS_ANDROID)
#include "android/android_classes.h"
#endif


namespace krystallic
{
	namespace Platform
	{
		PLATFORM_API ERRCODE CreatePlatformManager(Common::IAllocator* allocatorObj, const SNativeHandle* applicationInstance, IPlatformManager** outManager)
		{
			RefCounted<IAllocator> allocator(allocatorObj);
			RefCounted<CPlatformManager> mgr;

			if (!outManager || !applicationInstance)
				return PLATFORM_ERR_INVALID_ARGUMENT;

			if (!allocator.Get())
				allocator.Attach(Stdlib::g_stdlibDefaultAllocator);

			mgr.Attach(CPlatformManager::_Create(allocator.Get(), applicationInstance));

			if (!mgr.Get())
				return PLATFORM_ERR_OUT_OF_MEMORY;

			if (*outManager)
				(*outManager)->Delete();

			*outManager = mgr.Cast<IPlatformManager*>();
			mgr->IncRef();

			return PLATFORM_ERR_OK;
		}
	}
}

