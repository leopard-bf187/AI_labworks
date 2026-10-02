#if defined(_WIN32)
#include <windows.h>

int APIENTRY DllMain(HMODULE, DWORD, LPVOID)
{
    return TRUE;
}
#else
// Linux/Android shared libraries do not use DllMain.
// Keep one symbol in this translation unit to avoid an empty source file.
extern "C" int Krystallic_DllMain_Stub()
{
    return 1;
}
#endif
