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
#include "dbg_classes.h"

const char* g_LogInfoType[7] =
{
	"[MSG]",
	"[INF]",
	"[DBG]",
	"[WRN]",
	"[ERR]",
	"[FTL]",
	"[EXC]",
};

namespace krystallic
{
	namespace Debug
	{
		CORE_API ERRCODE CreateDebugManager(Common::IAllocator* allocatorObj, IDebugManager** outMgr)
		{
			RefCounted<IAllocator> allocator(allocatorObj);
			RefCounted<CDebugManager> mgr;

			if (!outMgr)
				return DBG_ERR_INVALID_ARGUMENT;

			if (!allocator.Get())
				allocator.Attach(Stdlib::g_stdlibDefaultAllocator);

			mgr.Attach(CDebugManager::_Create(allocator.Get()));

			if (!mgr.Get())
				return DBG_ERR_OUT_OF_MEMORY;

			if (*outMgr)
			{
				(*outMgr)->Delete();
				*outMgr = nullptr;
			}

			*outMgr = static_cast<IDebugManager*>(mgr.Get());
			mgr->IncRef();

			return DBG_ERR_OK;
		}


		CORE_API ERRCODE CreateStdLog(const char* fileName, ILog** outLog, Common::IAllocator* optionalAllocatorObj)
		{
			RefCounted<IAllocator> allocator(optionalAllocatorObj);
			RefCounted<CLog> log;

			if (!outLog)
				return DBG_ERR_INVALID_ARGUMENT;

			if (!allocator.Get())
				allocator.Attach(Stdlib::g_stdlibDefaultAllocator);

			log.Attach(CLog::_Create(allocator.Get(), fileName));

			if (!log.Get())
				return DBG_ERR_OUT_OF_MEMORY;

			if (*outLog)
			{
				(*outLog)->Delete();
				*outLog = nullptr;
			}

			*outLog = static_cast<ILog*>(log.Get());
			log->IncRef();

			return DBG_ERR_OK;
		}
	}
}