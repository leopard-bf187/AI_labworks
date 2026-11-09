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
		struct IEngineSystem : IBase
		{
			virtual ~IEngineSystem() = default;

			inline static SGuid GUID()
			{
				return {};
			}
		};
	}
}

