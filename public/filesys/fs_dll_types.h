/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 14.07.2026
*/


#pragma once


#include "../pch.h"
#include "../base.h"
#include "../common.h"


#include "fs_dll_enums.h"


#define		FILESYS_VERSION_MAJOR	0x01
#define		FILESYS_VERSION_MINOR	0x05
#define		FILESYS_VERSION_PATCH	0x00


#if defined(KRYSTALLIC_OS_WINNT)
#if defined(FILESYS_API_EXPORT)
#define FILESYS_API __declspec(dllexport)
#else
#define FILESYS_API __declspec(dllimport)
#endif
#elif defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)
#define FILESYS_API __attribute__((visibility("default")))
#endif


#ifdef __cplusplus
extern "C" {
#endif


namespace krystallic
{
	namespace FileSystem
	{
		struct IFileSystem;
		struct IFileIO;
		struct IPack;

		FILESYS_API ERRCODE CreateFileSystem(Common::IAllocator* allocator, IFileSystem** outFileSystem);
		FILESYS_API ERRCODE StdFileOpen(const char* path, const char* mode, IFileIO** outFile, Common::IAllocator* optionalAllocator = nullptr);
	}
}


#ifdef __cplusplus
}
#endif


