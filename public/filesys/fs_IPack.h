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
		struct IPack : IBase
		{
			virtual ~IPack() = default;

			virtual ERRCODE Open() = 0;
			virtual ERRCODE Close() = 0;

			inline static SGuid GUID()
			{
				return { 0x3546e1b0, 0x4ca6, 0x45b1, { 0x93, 0x34, 0x40, 0x72, 0xa3, 0xaa, 0x2e, 0x5a } };
			}
		};
	}
}