/*
*  Copyright (c) BytesForge 2022-2025
*
*  Authors: @LeoParD
*
*  Description: Bug Report and debug library enumerations
*
*  Date: 12.02.2023
*/


#pragma once


#include "../pch.h"


#if defined(KRYSTALLIC_OS_WINNT)

#include <windows.h>
#define MBOX_INF(msg)	MessageBoxA(0, msg, "Application Info",    MB_OK|MB_ICONINFORMATION)
#define MBOX_WRN(msg)	MessageBoxA(0, msg, "Application Warning", MB_OK|MB_ICONEXCLAMATION)
#define MBOX_ERR(msg)	MessageBoxA(0, msg, "Application Error",   MB_OK|MB_ICONERROR)

#elif defined(KRYSTALLIC_OS_LINUX)

#elif defined(KRYSTALLIC_OS_ANDROID)

#endif


namespace krystallic
{
	namespace Debug
	{
		enum EDebugErrorCode : ERRCODE
		{
			DBG_ERR_OK,
			DBG_ERR_INVALID_ARGUMENT,
			DBG_ERR_NOT_IMLEMENTED,
			DBG_ERR_NOT_SUPPORTED,
			DBG_ERR_NOT_FOUND,
			DBG_ERR_FAILED_TO_CREATE,
			DBG_ERR_OUT_OF_MEMORY,
			DBG_ERR_INVALID_FILE,
			DBG_ERR_LAST_VALUE,
		};

		enum EDebugLogMsgType : uint
		{
			DBG_LOG_MESSGE = 0,
			DBG_LOG_INFO,
			DBG_LOG_DEBUG,
			DBG_LOG_WARN,
			DBG_LOG_ERROR,
			DBG_LOG_FATAL_ERROR,
			DBG_LOG_EXCEPT,
		};
	}
}


