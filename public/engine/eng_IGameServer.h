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
		struct IGameServer : IBase
		{
			virtual ~IGameServer() = default;

			inline static SGuid GUID()
			{
				return { 0x7cb1a4ed, 0xd65b, 0x4355, { 0x90, 0xa8, 0x24, 0xfb, 0x27, 0x76, 0xae, 0xf5 } };
			}
		};
	}
}

