/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 18.08.2026
*/


#pragma once


#include "eng_dll_types.h"


namespace krystallic
{
	namespace Engine
	{
		struct ISystem : IBase
		{
			virtual ~ISystem() = default;

			inline static SGuid GUID()
			{
				return { 0xf17d3cbc, 0x5558, 0x4aa5, { 0xa8, 0x81, 0x6c, 0x71, 0xff, 0xa3, 0xa2, 0x7b } };
			}
		};
	}
}

