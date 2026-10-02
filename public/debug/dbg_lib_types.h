/*
*  Copyright (c) BytesForge 2022-2025
*
*  Authors: @LeoParD
*
*  Description: Bug Report and debug library declarations
*
*  Date: 12.02.2023
*/


#pragma once


#include "../pch.h"
#include "../base.h"
#include "../core.h"


#include "dbg_lib_enums.h"


#define DEBUG_VERSION 0x10


#ifdef __cplusplus
extern "C" {
#endif


namespace krystallic
{
	namespace Debug
	{
		struct IDebugManager;
		struct ILog;
		struct IMiniDump;

		typedef ERRCODE(*CreateDebugManagerFn)(Common::IAllocator* allocatorObj, FileSystem::IFileSystem* gameFs, IDebugManager** outMgr);
		typedef ERRCODE(*CreateStdLogFn)(Common::IAllocator* allocatorObj, ILog** outLog);

		CORE_API ERRCODE CreateDebugManager(Common::IAllocator* allocatorObj, IDebugManager** outMgr);
		CORE_API ERRCODE CreateStdLog(const char* fileName, ILog** outLog, Common::IAllocator* optionalAllocatorObj = nullptr);
	}
}

#ifdef __cplusplus
}
#endif


