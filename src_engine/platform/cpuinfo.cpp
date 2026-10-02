#include "cpuinfo.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

#if defined(KRYSTALLIC_OS_WINNT)
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#elif defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)
    #include <unistd.h>
#endif


#if defined(KRYSTALLIC_OS_WINNT)

        bool QueryWindowsCoreCount(int* physicalCores, int* logicalCores)
        {
            if (!physicalCores || !logicalCores)
                return false;

            *physicalCores = 0;
            *logicalCores = 0;

            DWORD requiredSize = 0;
            GetLogicalProcessorInformationEx(RelationProcessorCore, nullptr, &requiredSize);

            if (requiredSize == 0)
            {
                SYSTEM_INFO systemInfo{};
                GetNativeSystemInfo(&systemInfo);
                *physicalCores = static_cast<int>(systemInfo.dwNumberOfProcessors);
                *logicalCores = static_cast<int>(systemInfo.dwNumberOfProcessors);
                return true;
            }

            BYTE* buffer = new (std::nothrow) BYTE[requiredSize];

            if (!buffer)
                return false;

            if (!GetLogicalProcessorInformationEx(RelationProcessorCore, reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer), &requiredSize))
            {
                delete[] buffer;
                return false;
            }

            BYTE* current = buffer;
            BYTE* end = buffer + requiredSize;

            while (current < end)
            {
                auto* info = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(current);

                if (info->Relationship == RelationProcessorCore)
                {
                    ++(*physicalCores);

                    for (WORD groupIndex = 0; groupIndex < info->Processor.GroupCount; ++groupIndex)
                    {
                        KAFFINITY mask = info->Processor.GroupMask[groupIndex].Mask;

                        while (mask != 0)
                        {
                            *logicalCores += static_cast<int>(mask & 1);
                            mask >>= 1;
                        }
                    }
                }

                if (info->Size == 0)
                    break;

                current += info->Size;
            }

            delete[] buffer;
            return *physicalCores > 0 && *logicalCores > 0;
        }

#elif defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)

        bool QueryPosixCoreCount(int* physicalCores, int* logicalCores)
        {
            if (!physicalCores || !logicalCores)
                return false;

            const long onlineProcessors = sysconf(_SC_NPROCESSORS_ONLN);
            *logicalCores = onlineProcessors > 0 ? static_cast<int>(onlineProcessors) : 1;

            FILE* file = std::fopen("/proc/cpuinfo", "r");

            if (!file)
            {
                *physicalCores = *logicalCores;
                return true;
            }

            struct SCorePair
            {
                int packageId;
                int coreId;
            };

            SCorePair cores[1024]{};
            int coreCount = 0;
            int currentPackage = 0;
            int currentCore = -1;
            char line[256]{};

            auto commitCore = [&]()
            {
                if (currentCore < 0 || coreCount >= 1024)
                    return;

                for (int i = 0; i < coreCount; ++i)
                {
                    if (cores[i].packageId == currentPackage && cores[i].coreId == currentCore)
                        return;
                }

                cores[coreCount++] = { currentPackage, currentCore };
            };

            while (std::fgets(line, sizeof(line), file))
            {
                if (std::strncmp(line, "physical id", 11) == 0)
                {
                    const char* separator = std::strchr(line, ':');

                    if (separator)
                        currentPackage = std::atoi(separator + 1);
                }
                else if (std::strncmp(line, "core id", 7) == 0)
                {
                    const char* separator = std::strchr(line, ':');

                    if (separator)
                        currentCore = std::atoi(separator + 1);
                }
                else if (line[0] == '\n')
                {
                    commitCore();
                    currentPackage = 0;
                    currentCore = -1;
                }
            }

            commitCore();
            std::fclose(file);

            *physicalCores = coreCount > 0 ? coreCount : *logicalCores;
            return true;
        }

#endif

        bool QuerySystemCoreCount(int* physicalCores, int* logicalCores)
        {
#if defined(KRYSTALLIC_OS_WINNT)
            return QueryWindowsCoreCount(physicalCores, logicalCores);
#elif defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)
            return QueryPosixCoreCount(physicalCores, logicalCores);
#else
            if (physicalCores)
                *physicalCores = 1;

            if (logicalCores)
                *logicalCores = 1;

            return false;
#endif
        }

    dword GetMissingCpuFeatureMask(dword supportedMask, dword requiredMask)
    {
        return requiredMask & ~supportedMask;
    }

    bool ValidateCpuFeatureMask(dword supportedMask, dword requiredMask)
    {
        return GetMissingCpuFeatureMask(supportedMask, requiredMask) == CPU_FEATURE_NONE;
    }

#if defined(KRYSTALLIC_ARCH_X86) || defined(KRYSTALLIC_ARCH_X64)
    #include "cpuinfo_x86.inl"
#elif defined(KRYSTALLIC_ARCH_ARMv7) || defined(KRYSTALLIC_ARCH_ARMv8) || defined(KRYSTALLIC_ARCH_ARM64)
    #include "cpuinfo_arm.inl"
#else

    bool QuerySystemCPUInfo(SSystemCPUInfo* outInfo)
    {
        if (!outInfo)
            return false;

        memset(outInfo, 0, sizeof(SSystemCPUInfo));
        Stdlib::StrCopyN(outInfo->vendor, "Unknown", sizeof(outInfo->vendor));
        Stdlib::StrCopyN(outInfo->brand, "Unknown CPU", sizeof(outInfo->brand));
        QuerySystemCoreCount(&outInfo->numPhysCores, &outInfo->numLogicCores);
        return false;
    }

#endif

