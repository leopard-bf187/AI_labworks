/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 11.07.2026
*/

#if defined(_WIN32)
#include <windows.h>

int APIENTRY DllMain(HMODULE, DWORD, LPVOID)
{
    return TRUE;
}

#else
// Linux/Android shared libraries do not use DllMain.
// Keep one symbol in this translation unit to avoid an empty source file.
extern "C" int Krystallic_Platform_DllMain_Stub()
{
    return 1;
}

#endif
