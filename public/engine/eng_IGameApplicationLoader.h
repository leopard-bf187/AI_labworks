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
        struct IGameApplicationLoader
        {
            virtual ~IGameApplicationLoader() = default;
            virtual ERRCODE GetGameAppDesc(SGameAppDesc* outDesc) = 0;
            virtual qword GetEngineSystemMask() const = 0;
            virtual qword GetEngineSubsystemMask() const = 0;
            virtual ERRCODE CreateGameClient(Common::IAllocator* allocator, IGameClient** outGameClient) = 0;
            virtual ERRCODE CreateGameServer(Common::IAllocator* allocator, IGameServer** outGameServer) = 0;
        };
	}
}

