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


#include "fs_dll_types.h"


namespace krystallic
{
	namespace FileSystem
	{
        struct IFileIO : IBase
        {
            virtual ~IFileIO() = default;

            virtual ERRCODE Open(const char* filePath, dword openMode) = 0;
            virtual ERRCODE Close() = 0;

            virtual EFSFileType GetFileType() const = 0;
            virtual dword GetCapabilities() const = 0;
            virtual dword GetOpenMode() const = 0;
            virtual uint64 GetFileSize() const = 0;

            virtual ERRCODE PrintF(const char* format, ...) = 0;
            virtual ERRCODE ReadF(const char* format, ...) = 0;
            virtual ERRCODE PutLine(const char* line, uint64 lineSize) = 0;
            virtual ERRCODE GetLine(char* line, uint64 lineSize) = 0;

            virtual ERRCODE ReadData(void* buffer, uint64 bufferSize, uint64* outBytesRead = nullptr) = 0;
            virtual ERRCODE WriteData(const void* buffer, uint64 bufferSize, uint64* outBytesWritten = nullptr) = 0;

            virtual ERRCODE SetFilePointer(int64 fp, EFSSeekOrigin seekOrg) = 0;
            virtual ERRCODE GetFilePointer(int64* outFp) const = 0;

            virtual ERRCODE Flush() = 0;

            virtual bool Eof() const = 0;

            inline static SGuid GUID()
            {
                return { 0x9e7cc801, 0x80b1, 0x414e, { 0x81, 0xe8, 0x1e, 0x5b, 0x48, 0x71, 0x7b, 0xa5 } };
            }
		};
	}
}