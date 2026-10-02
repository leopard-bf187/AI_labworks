#if defined(_WIN32)
    #define KRYSTALLIC_EXPORT extern "C" __declspec(dllexport)
#else
    #define KRYSTALLIC_EXPORT extern "C" __attribute__((visibility("default")))
#endif

KRYSTALLIC_EXPORT void Krystallic_Dll_Stub()
{
}
