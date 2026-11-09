/*
*  Copyright (c) BytesForge 2022-2025
*
*  Authors: @LeoParD
*
*  Description: minidump class
*
*  Date: 12.02.2023
*/


#pragma once


#include "dbg_lib_types.h"


namespace krystallic
{
	namespace Debug
	{
		struct IMiniDump : IBase
		{
			virtual ~IMiniDump() = default;

			inline static SGuid GUID()
			{
				return { 0xf3b531ac, 0x2398, 0x4e59, { 0xa6, 0x82, 0x8f, 0x18, 0x5c, 0xf9, 0x59, 0xba } };
			}
		};
	}
}

