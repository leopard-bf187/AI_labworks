/*
*  Copyright (c) BytesForge 2022-2025
*
*  Authors: @LeoParD
*
*  Description: log class
*
*  Date: 12.02.2023
*/


#pragma once

#include "dbg_lib_types.h"

namespace krystallic
{
	namespace Debug
	{
		struct ILog : IBase
		{
		public:
			virtual ~ILog() = default;

			virtual ERRCODE LogBegin(char* ClientName) = 0;
			virtual ERRCODE LogEnd() = 0;

			virtual ERRCODE Print(const char* msg, int64 now, const char* file, uint line, uint infoType) = 0;
			virtual ERRCODE Print(const char* fmt, ...) = 0;
			virtual ERRCODE PrintError(const char* errorBuffer, uint errorBufferSize) = 0;

			virtual void SetPrintInfo(bool f) = 0;
			virtual bool GetPrintInfo() const = 0;
			virtual void SetPrintFile(bool f) = 0;
			virtual bool GetPrintFile() const = 0;
			virtual void SetPrintLine(bool f) = 0;
			virtual bool GetPrintLine() const = 0;
			virtual void SetPrintTime(bool f) = 0;
			virtual bool GetPrintTime() const = 0;

			virtual ERRCODE GetFile(FileSystem::IFileIO** outLogFile) = 0;

			inline static SGuid GUID()
			{
				return { 0x039b7eab, 0x8871, 0x4eb0, { 0xad, 0x3d, 0x27, 0xe7, 0xa2, 0xdd, 0x74, 0x40 } };
			}
		};
	}
}


