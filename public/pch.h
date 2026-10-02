/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: main header file with all configured macroses to cross compile
*
*  Programmers: 
*   - Leonid (@leopard-bf187) Parmacli
*   - Victor (@RisovoePole) Anisimov
*   - Alexandr (@yorunikakeru4) Croitor
* 
*  Date: 02.06.2026
*/

#pragma once

#if defined(KRYSTALLIC_OS_WINNT) || defined(_WIN32) || defined(_WIN64)

#ifdef KRYSTALLIC_API_EXPORT
#define KRYSTALLIC_API __declspec(dllexport)
#else
#define KRYSTALLIC_API __declspec(dllimport)
#endif

#elif defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)

#define KRYSTALLIC_API __attribute__((visibility("default")))

#endif



#include "__config.h"

#include "data_types.h"
#include "global_defs.h"


#ifdef __cplusplus
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#endif


#ifndef _MSC_VER
#   define _snprintf snprintf
#endif

