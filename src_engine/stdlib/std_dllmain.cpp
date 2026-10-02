#include "stdlib_dll.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
int WINAPI DllMain(HINSTANCE, DWORD dwReason, LPVOID)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_PROCESS_DETACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }

    return 1;
}
#endif
