#pragma once
#if defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)


#include <stdio.h>
#include <string.h>


struct SProcMemoryStatus
{
    qword memTotal;
    qword memFree;
    qword memAvailable;
    qword buffers;
    qword cached;
    qword swapCached;
    qword swapTotal;
    qword swapFree;
    qword commitLimit;
    qword committedAS;
    qword sReclaimable;
    qword shmem;
};


bool ReadProcMemoryStatus(SProcMemoryStatus* outStatus)
{
    if (!outStatus)
        return false;

    memset(outStatus, 0, sizeof(SProcMemoryStatus));

    FILE* file = fopen("/proc/meminfo", "r");

    if (!file)
        return false;

    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        unsigned long long valueKB = 0;

        if (sscanf(line, "MemTotal: %llu kB", &valueKB) == 1)
            outStatus->memTotal = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "MemFree: %llu kB", &valueKB) == 1)
            outStatus->memFree = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "MemAvailable: %llu kB", &valueKB) == 1)
            outStatus->memAvailable = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "Buffers: %llu kB", &valueKB) == 1)
            outStatus->buffers = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "Cached: %llu kB", &valueKB) == 1)
            outStatus->cached = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "SwapCached: %llu kB", &valueKB) == 1)
            outStatus->swapCached = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "SwapTotal: %llu kB", &valueKB) == 1)
            outStatus->swapTotal = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "SwapFree: %llu kB", &valueKB) == 1)
            outStatus->swapFree = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "CommitLimit: %llu kB", &valueKB) == 1)
            outStatus->commitLimit = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "Committed_AS: %llu kB", &valueKB) == 1)
            outStatus->committedAS = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "SReclaimable: %llu kB", &valueKB) == 1)
            outStatus->sReclaimable = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "Shmem: %llu kB", &valueKB) == 1)
            outStatus->shmem = static_cast<qword>(valueKB) * 1024ull;
    }

    fclose(file);
    return outStatus->memTotal != 0;
}

#endif