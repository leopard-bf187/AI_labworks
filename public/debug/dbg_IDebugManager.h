/*
*  Copyright (c) BytesForge 2022-2025
*
*  Authors: @LeoParD
*
*  Description: debug manager, main factory class
*
*  Date: 12.02.2023
*/


#pragma once

#include "dbg_lib_types.h"

namespace krystallic
{
	namespace Debug
	{
		struct IDebugManager : IBase
		{
		public:
			virtual ~IDebugManager() = default;

			virtual ERRCODE CreateLog(const char* file, ILog** logFile) = 0;

			inline static SGuid GUID()
			{
				return { 0x205796fe, 0x3222, 0x4bff, { 0x8d, 0x00, 0x3b, 0x34, 0x52, 0x3b, 0x71, 0xa0 } };
			}
		};
	}
}

