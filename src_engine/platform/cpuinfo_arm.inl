#if defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)
#include <sys/auxv.h>
#include <asm/hwcap.h>
#endif


#if defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)

void QueryArmTextInfo(char vendor[20], char brand[64])
{
    Stdlib::StrCopyN(vendor, "ARM", 20);
    Stdlib::StrCopyN(brand, "ARM Processor", 64);

    FILE* file = std::fopen("/proc/cpuinfo", "r");

    if (!file)
        return;

    char line[512]{};

    while (std::fgets(line, sizeof(line), file))
    {
        const char* separator = Stdlib::StrFindChar(line, ':');

        if (!separator)
            continue;

        const char* value = separator + 1;

        while (*value == ' ' || *value == '\t')
            ++value;

        char cleanValue[256]{};
        Stdlib::StrCopyN(cleanValue, value, sizeof(cleanValue));
        size_t length = Stdlib::StrLen(cleanValue);

        while (length > 0 && (cleanValue[length - 1] == '\n' || cleanValue[length - 1] == '\r'))
            cleanValue[--length] = '\0';

        if (Stdlib::StrNCmp(line, "Hardware", 8) == 0 && cleanValue[0] != '\0')
            Stdlib::StrCopyN(brand, cleanValue, 64);
        else if (Stdlib::StrNCmp(line, "model name", 10) == 0 && cleanValue[0] != '\0')
            Stdlib::StrCopyN(brand, cleanValue, 64);
    }

    std::fclose(file);
}

dword QueryArmFeatureMaskPosix()
{
    dword mask = CPU_FEATURE_NONE;

    const unsigned long hwcap = getauxval(AT_HWCAP);
    const unsigned long hwcap2 = getauxval(AT_HWCAP2);

#if defined(KRYSTALLIC_ARCH_ARMv8) || defined(KRYSTALLIC_ARCH_ARM64)
    
    mask |= CPU_FEATURE_NEON;

    #ifdef HWCAP_ASIMD
        if (hwcap & HWCAP_ASIMD) mask |= CPU_FEATURE_NEON;
    #endif

    #ifdef HWCAP_CRC32
        if (hwcap & HWCAP_CRC32) mask |= CPU_FEATURE_ARM_CRC32;
    #endif

    bool hasCrypto = true;

    #ifdef HWCAP_AES
        hasCrypto = hasCrypto && ((hwcap & HWCAP_AES) != 0);
    #else
        hasCrypto = false;
    #endif

    #ifdef HWCAP_PMULL
        hasCrypto = hasCrypto && ((hwcap & HWCAP_PMULL) != 0);
    #else
        hasCrypto = false;
    #endif

    #ifdef HWCAP_SHA1
        hasCrypto = hasCrypto && ((hwcap & HWCAP_SHA1) != 0);
    #else
        hasCrypto = false;
    #endif

    #ifdef HWCAP_SHA2
        hasCrypto = hasCrypto && ((hwcap & HWCAP_SHA2) != 0);
    #else
        hasCrypto = false;
    #endif

    if (hasCrypto)
        mask |= CPU_FEATURE_ARM_CRYPTO;

    #ifdef HWCAP_ASIMDDP
        if (hwcap & HWCAP_ASIMDDP) mask |= CPU_FEATURE_ARM_DOTPROD;
    #endif

    bool hasFp16 = true;

    #ifdef HWCAP_FPHP
        hasFp16 = hasFp16 && ((hwcap & HWCAP_FPHP) != 0);
    #else
        hasFp16 = false;
    #endif

    #ifdef HWCAP_ASIMDHP
        hasFp16 = hasFp16 && ((hwcap & HWCAP_ASIMDHP) != 0);
    #else
        hasFp16 = false;
    #endif

    if (hasFp16)
        mask |= CPU_FEATURE_ARM_FP16;

    #ifdef HWCAP_SVE
        if (hwcap & HWCAP_SVE) mask |= CPU_FEATURE_ARM_SVE;
    #endif

    #ifdef HWCAP2_SVE2
        if (hwcap2 & HWCAP2_SVE2) mask |= CPU_FEATURE_ARM_SVE2;
    #endif

    #ifdef HWCAP2_SVE2P1
        if (hwcap2 & HWCAP2_SVE2P1) mask |= CPU_FEATURE_ARM_SVE2P1;
    #endif

#elif defined(KRYSTALLIC_ARCH_ARMv7)

    #ifdef HWCAP_NEON
        if (hwcap & HWCAP_NEON) mask |= CPU_FEATURE_NEON;
    #endif

    #ifdef HWCAP2_CRC32
        if (hwcap2 & HWCAP2_CRC32) mask |= CPU_FEATURE_ARM_CRC32;
    #endif

    bool hasCrypto = true;

    #ifdef HWCAP2_AES
        hasCrypto = hasCrypto && ((hwcap2 & HWCAP2_AES) != 0);
    #else
        hasCrypto = false;
    #endif

    #ifdef HWCAP2_PMULL
        hasCrypto = hasCrypto && ((hwcap2 & HWCAP2_PMULL) != 0);
    #else
        hasCrypto = false;
    #endif

    #ifdef HWCAP2_SHA1
        hasCrypto = hasCrypto && ((hwcap2 & HWCAP2_SHA1) != 0);
    #else
        hasCrypto = false;
    #endif

    #ifdef HWCAP2_SHA2
        hasCrypto = hasCrypto && ((hwcap2 & HWCAP2_SHA2) != 0);
    #else
        hasCrypto = false;
    #endif

    if (hasCrypto)
        mask |= CPU_FEATURE_ARM_CRYPTO;

    #ifdef HWCAP_ASIMDDP
        if (hwcap & HWCAP_ASIMDDP) mask |= CPU_FEATURE_ARM_DOTPROD;
    #endif

    bool hasFp16 = true;

    #ifdef HWCAP_FPHP
        hasFp16 = hasFp16 && ((hwcap & HWCAP_FPHP) != 0);
    #else
        hasFp16 = false;
    #endif

    #ifdef HWCAP_ASIMDHP
        hasFp16 = hasFp16 && ((hwcap & HWCAP_ASIMDHP) != 0);
    #else
        hasFp16 = false;
    #endif

    if (hasFp16)
        mask |= CPU_FEATURE_ARM_FP16;

#else
    #error QueryArmFeatureMaskPosix requires an ARM target
#endif

    return mask;
}

#elif defined(KRYSTALLIC_OS_WINNT)

void QueryArmTextInfo(char vendor[20], char brand[64])
{
    Stdlib::StrCopyN(vendor, "ARM", 20);
    Stdlib::StrCopyN(brand, "ARM Processor", 64);
}

dword QueryArmFeatureMaskWindows()
{
    dword mask = CPU_FEATURE_NONE;

#if defined(KRYSTALLIC_ARCH_ARM64)
    mask |= CPU_FEATURE_NEON;
#elif defined(PF_ARM_NEON_INSTRUCTIONS_AVAILABLE)
    if (IsProcessorFeaturePresent(PF_ARM_NEON_INSTRUCTIONS_AVAILABLE))
        mask |= CPU_FEATURE_NEON;
#endif

#ifdef PF_ARM_V8_CRC32_INSTRUCTIONS_AVAILABLE
    if (IsProcessorFeaturePresent(PF_ARM_V8_CRC32_INSTRUCTIONS_AVAILABLE))
        mask |= CPU_FEATURE_ARM_CRC32;
#endif
#ifdef PF_ARM_V8_CRYPTO_INSTRUCTIONS_AVAILABLE
    if (IsProcessorFeaturePresent(PF_ARM_V8_CRYPTO_INSTRUCTIONS_AVAILABLE))
        mask |= CPU_FEATURE_ARM_CRYPTO;
#endif

    return mask;
}

#endif


bool QuerySystemCPUInfo(SSystemCPUInfo* outInfo)
{
    if (!outInfo)
        return false;

    memset(outInfo, 0, sizeof(SSystemCPUInfo));
    QueryArmTextInfo(outInfo->vendor, outInfo->brand);
    QuerySystemCoreCount(&outInfo->numPhysCores, &outInfo->numLogicCores);

#if defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)
    outInfo->cpuFeatures.mask = QueryArmFeatureMaskPosix();
#elif defined(KRYSTALLIC_OS_WINNT)
    outInfo->cpuFeatures.mask = QueryArmFeatureMaskWindows();
#else
    outInfo->cpuFeatures.mask = CPU_FEATURE_NONE;
#endif

    return true;
}
