#pragma once


#include "../pch.h"
#include "../base.h"
#include "../common.h"
#include "../filesys.h"
#include "../simdmath.h"
#include "../rhi.h"


#define		CORE_VERSION		0x0003


#if defined(KRYSTALLIC_OS_WINNT)
#if defined(CORE_API_EXPORT)
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#elif defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)
#define CORE_API __attribute__((visibility("default")))
#endif


#ifdef __cplusplus
extern"C" {
#endif


namespace krystallic
{
	namespace Core
	{

	}

	namespace Scene {}
	namespace Content {}
	namespace Runtime {}
	namespace Render {}
}


#ifdef __cplusplus
}
#endif
