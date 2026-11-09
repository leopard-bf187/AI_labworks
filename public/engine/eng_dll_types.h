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


#include "pch.h"
#include "base.h"
#include "common.h"


#define		FILESYS_VERSION		0x02


#if defined(KRYSTALLIC_OS_WINNT)
#if defined(ENGINE_API_EXPORT)
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#elif defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)
#define ENGINE_API __attribute__((visibility("default")))
#endif


#include "eng_dll_enums.h"


namespace krystallic
{
	namespace Engine
	{
        struct IEngineSystem;
        struct IEngineSubsystem;

        struct ISystem;
        struct IKernel;
        struct IEngine;

        struct IGameClient;
        struct IGameServer;

        struct IGameApplicationLoader;

        struct SGameAppDesc
        {
            int unused;
        };
	}
}

