/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD
*
*  Description:
*
*  Date: 10.07.2025
*/


#pragma once


#include "scr_lib_types.h"


namespace krystallic
{
	namespace Runtime
	{
		struct IScriptErrorBuffer : public IBase
		{
			virtual ~IScriptErrorBuffer() = default;
		};
	}
}


