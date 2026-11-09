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
		struct IKernel : IBase
		{
			virtual ~IKernel() = default;

			inline static SGuid GUID()
			{
				return { 0x97b936e8, 0x473a, 0x405f, { 0x9f, 0xaa, 0xa6, 0x23, 0xb7, 0x75, 0x7b, 0xc0 } };
			}
		};
	}
}

