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


namespace krystallic
{
	namespace FileSystem
	{
        enum EFileSystemError : ERRCODE
        {
            FS_ERR_OK,
            FS_ERR_INVALID_ARGUMENT,
            FS_ERR_NOT_IMPLEMENTED,
            FS_ERR_NOT_SUPPORTED,
            FS_ERR_NOT_FOUND,
            FS_ERR_ALREADY_EXISTS,
            FS_ERR_ACCESS_DENIED,
            FS_ERR_FAILED_TO_CREATE,
            FS_ERR_OUT_OF_MEMORY,
            FS_ERR_INVALID_FILE,
            FS_ERR_INVALID_PATH,
            FS_ERR_INVALID_OPEN_MODE,
            FS_ERR_READ,
            FS_ERR_WRITE,
            FS_ERR_SEEK,
            FS_ERR_TELL,
            FS_ERR_EOF,
            FS_ERR_BUFFER_TOO_SMALL,
            FS_ERR_FILE_TOO_LARGE,
            FS_ERR_FLUSH,
            FS_ERR_MAPPING,
            FS_ERR_BACKEND_ERROR,
            FS_ERR_BACKEND_FAILED_TO_CREATE,
            FS_ERR_BACKEND_OUT_OF_MEMORY,
            FS_ERR_LAST_VALUE,
        };

        enum EFSFileType
        {
            FS_FILE_STD,
            FS_FILE_PHYSICAL,
            FS_FILE_MAPPED,
            FS_FILE_IN_MEMORY_READ,
            FS_FILE_IN_MEMORY_WRITE,
        };

        enum EFSFileCapability : dword
        {
            FS_CAP_NONE = 0,
            FS_CAP_READ = (1 << 0),
            FS_CAP_WRITE = (1 << 1),
            FS_CAP_SEEK = (1 << 2),
            FS_CAP_FLUSH = (1 << 3),
            FS_CAP_TEXT = (1 << 4),
            FS_CAP_MEMORY_MAPPED = (1 << 5),
        };

        enum EFSSeekOrigin : int
        {
            FS_SEEK_BEGIN = 0,
            FS_SEEK_CURRENT = 1,
            FS_SEEK_END = 2,
        };

        enum EFSFileOpenMode : dword
        {
            FS_OPEN_READ = (1 << 0),
            FS_OPEN_WRITE = (1 << 1),
            FS_OPEN_APPEND = (1 << 2),
            FS_OPEN_TEXT = (1 << 3),
            FS_OPEN_CREATE = (1 << 4),
            FS_OPEN_TRUNCATE = (1 << 5),
        };

		enum EFileSystemDataSourceType
		{
			FS_DATA_SOURCE_DIRECTORY,
			FS_DATA_SOURCE_PACK,
		};
	}
}