/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 19.07.2026
*/


#include "linux_classes.h"
#include "../mem_smbios.h"
#include "../cpuinfo.h"
#include <sys/utsname.h>
#include <pwd.h>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <unistd.h>
#include <vector>


struct SProcMemInfo
{
    qword memTotal;
    qword swapTotal;
};


bool ReadProcMemInfo(SProcMemInfo& outInfo);
bool ReadLinuxSMBIOSTable(std::vector<byte>& table);
dword CountLinuxNumaNodes();
bool ReadOSReleaseValue(const char* key, char* outValue, size_t outValueSize);
bool ReadLinuxSMBIOSTable(byte** outTable, size_t* outTableSize);


static void TrimString(char* str)
{
    if (!str)
        return;

    size_t length = strlen(str);

    while (length > 0)
    {
        char c = str[length - 1];

        if (c != '\n' && c != '\r' && c != ' ' && c != '\t')
            break;

        str[--length] = '\0';
    }

    if (length >= 2 && str[0] == '"' && str[length - 1] == '"')
    {
        memmove(str, str + 1, length - 2);
        str[length - 2] = '\0';
    }
}


CPlatformSystemInfo::CPlatformSystemInfo(IAllocator* allocator) : Inherit(allocator)
{
    InitializeCPU();
    InitializeOS();
    InitializeRAM();
}


CPlatformSystemInfo::~CPlatformSystemInfo()
{

}


uint32 CPlatformSystemInfo::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CPlatformSystemInfo::_Destroy(this);
        return 0;
    }

    return ref;
}


uint32 CPlatformSystemInfo::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return this->IncRef();
    }

    if (guid == IPlatformSystemInfo::GUID())
    {
        *IFace = static_cast<IPlatformSystemInfo*>(this);
        return this->IncRef();
    }

    return 0;
}


const char* CPlatformSystemInfo::GetOSProductName() const
{
    return m_os.ProductName;
}


const char* CPlatformSystemInfo::GetOSEdition() const
{
    return m_os.Edition;
}


const char* CPlatformSystemInfo::GetOSVersion() const
{
    return m_os.Version;
}


const char* CPlatformSystemInfo::GetOSUserName() const
{
    return m_os.UserName;
}


const char* CPlatformSystemInfo::GetOSDesktopName() const
{
    return m_os.DesktopName;
}


const char* CPlatformSystemInfo::GetOSInstallDate() const
{
    return m_os.InstallDate;
}


const char* CPlatformSystemInfo::GetCPUVendor() const
{
    return m_cpu.vendor;
}


const char* CPlatformSystemInfo::GetCPUBrand() const
{
    return m_cpu.brand;
}


uint32 CPlatformSystemInfo::GetCPUPhysicalCoreCount() const
{
    return m_cpu.numPhysCores;
}


uint32 CPlatformSystemInfo::GetCPULogicalCoreCount() const
{
    return m_cpu.numLogicCores;
}


uint32 CPlatformSystemInfo::GetCPUSIMDFlags() const
{
    return m_cpu.cpuFeatures.mask;
}


uint64 CPlatformSystemInfo::GetPhysicalMemorySize() const
{
    return m_ram.installedPhysical;
}


uint64 CPlatformSystemInfo::GetAvailableMemory() const
{
    return 0ull;
}


ERRCODE CPlatformSystemInfo::GetFullOSInfo(SSystemOSInfo* outOSInfo) const
{
    if (!outOSInfo)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    memcpy(outOSInfo, &m_os, sizeof(SSystemOSInfo));

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformSystemInfo::GetFullCPUInfo(SSystemCPUInfo* outCpuInfo) const
{
    if (!outCpuInfo)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    memcpy(outCpuInfo, &m_cpu, sizeof(SSystemCPUInfo));

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformSystemInfo::GetFullRAMInfo(SSystemMemoryInfo* outCpuInfo) const
{
    if (!outCpuInfo)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    memcpy(outCpuInfo, &m_ram, sizeof(SSystemMemoryInfo));

    return PLATFORM_ERR_OK;
}


dword CPlatformSystemInfo::GetPhysicalMemoryDeviceCount() const
{
    byte* table = nullptr;
    size_t tableSize = 0;

    if (!ReadLinuxSMBIOSTable(&table, &tableSize))
        return 0;

    const dword count = CountSMBIOSMemoryDevices(table, tableSize);

    delete[] table;
    return count;
}


ERRCODE CPlatformSystemInfo::GetPhysicalMemoryDeviceInfo(dword index, SPhysicalMemoryDeviceInfo* outInfo) const
{
    if (!outInfo)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    memset(outInfo, 0, sizeof(SPhysicalMemoryDeviceInfo));
    outInfo->type = EMemoryType::MEM_TYPE_UNKNOWN;
    outInfo->formFactor = EMemoryFormFactor::MEM_FORM_FACTOR_UNKNOWN;

    byte* table = nullptr;
    size_t tableSize = 0;

    if (!ReadLinuxSMBIOSTable(&table, &tableSize))
        return PLATFORM_ERR_NOT_SUPPORTED;

    dword deviceCount = 0;
    const bool found = ParseSMBIOSMemoryDevice(table, tableSize, index, outInfo, &deviceCount);

    delete[] table;

    if (index >= deviceCount)
        return PLATFORM_ERR_OUT_OF_RANGE;

    return found ? PLATFORM_ERR_OK : PLATFORM_ERR_NOT_FOUND;
}


bool CPlatformSystemInfo::InitializeCPU()
{
    memset(&m_cpu, 0, sizeof(SSystemCPUInfo));
    return QuerySystemCPUInfo(&m_cpu);
    //return true;
}


bool CPlatformSystemInfo::InitializeOS()
{
    memset(&m_os, 0, sizeof(m_os));

    char prettyName[128] = {};
    char versionID[64] = {};
    char variant[64] = {};
    char variantID[64] = {};

    if (ReadOSReleaseValue("PRETTY_NAME", prettyName, sizeof(prettyName)))
        Stdlib::StrCopyN(m_os.ProductName, prettyName, sizeof(m_os.ProductName));
    else
        Stdlib::StrCopyN(m_os.ProductName, "Linux", sizeof(m_os.ProductName));

    if (ReadOSReleaseValue("VARIANT", variant, sizeof(variant)))
        Stdlib::StrCopyN(m_os.Edition, variant, sizeof(m_os.Edition));
    else if (ReadOSReleaseValue("VARIANT_ID", variantID, sizeof(variantID)))
        Stdlib::StrCopyN(m_os.Edition, variantID, sizeof(m_os.Edition));
    else
        Stdlib::StrCopyN(m_os.Edition, "N/A", sizeof(m_os.Edition));

    ReadOSReleaseValue("VERSION_ID", versionID, sizeof(versionID));

    utsname uts{};

    if (uname(&uts) == 0)
    {
        if (versionID[0])
            snprintf(m_os.Version,  sizeof(m_os.Version), "%s, kernel %s", versionID, uts.release);
        else
            Stdlib::StrCopyN(m_os.Version, uts.release, sizeof(m_os.Version));
    }
    else
    {
        Stdlib::StrCopyN(m_os.Version, versionID, sizeof(m_os.Version));
    }

    passwd pwd{};
    passwd* result = nullptr;

    long bufferSize = sysconf(_SC_GETPW_R_SIZE_MAX);

    if (bufferSize < 1024)
        bufferSize = 16384;

    char* buffer = static_cast<char*>(m_allocator->Alloc(static_cast<size_t>(bufferSize)));

    if (buffer)
    {
        if (getpwuid_r(geteuid(), &pwd, buffer, static_cast<size_t>(bufferSize), &result) == 0 && result && result->pw_name)
        {
            Stdlib::StrCopyN(m_os.UserName, result->pw_name, sizeof(m_os.UserName));
        }
        else
        {
            const char* user = getenv("USER");
            Stdlib::StrCopyN(m_os.UserName, user ? user : "Unknown", sizeof(m_os.UserName));
        }

        m_allocator->Free(buffer);
    }
    else
    {
        const char* user = getenv("USER");
        Stdlib::StrCopyN(m_os.UserName, user ? user : "Unknown", sizeof(m_os.UserName));
    }

    if (gethostname(m_os.DesktopName, sizeof(m_os.DesktopName)) != 0)
        Stdlib::StrCopyN(m_os.DesktopName, "Unknown", sizeof(m_os.DesktopName));

    m_os.DesktopName[sizeof(m_os.DesktopName) - 1] = '\0';

    Stdlib::StrCopyN(m_os.InstallDate, "Unknown", sizeof(m_os.InstallDate));

    return true;
}


bool CPlatformSystemInfo::InitializeRAM()
{
    memset(&m_ram, 0, sizeof(SSystemMemoryInfo));

    SProcMemInfo memoryInfo{};

    if (!ReadProcMemInfo(memoryInfo))
        return false;

    m_ram.installedPhysical = memoryInfo.memTotal;
    m_ram.usablePhysical = memoryInfo.memTotal;
    m_ram.hasSwap = memoryInfo.swapTotal != 0;

    const long pageSize = sysconf(_SC_PAGESIZE);

    if (pageSize > 0)
    {
        m_ram.pageSize = static_cast<qword>(pageSize);
        m_ram.allocationGranularity = static_cast<qword>(pageSize);
    }

    m_ram.numaNodeCount = CountLinuxNumaNodes();
    m_ram.isLowRamDevice = false;

    m_ram.moduleCount = GetPhysicalMemoryDeviceCount();

    m_ram.hasECC = false;

    for (dword i = 0; i < m_ram.moduleCount; ++i)
    {
        SPhysicalMemoryDeviceInfo device{};

        if (GetPhysicalMemoryDeviceInfo(i, &device) == PLATFORM_ERR_OK && device.hasECC)
        {
            m_ram.hasECC = true;
            break;
        }
    }

    return true;
}


bool ReadOSReleaseValue(const char* key, char* outValue, size_t outValueSize)
{
    if (!key || !outValue || outValueSize == 0)
        return false;

    outValue[0] = '\0';

    FILE* file = fopen("/etc/os-release", "r");

    if (!file)
        file = fopen("/usr/lib/os-release", "r");

    if (!file)
        return false;

    char line[512];
    const size_t keyLength = strlen(key);

    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, key, keyLength) != 0 || line[keyLength] != '=')
            continue;

        Stdlib::StrCopyN(outValue, line + keyLength + 1, outValueSize);
        TrimString(outValue);

        fclose(file);
        return true;
    }

    fclose(file);
    return false;
}


bool ReadLinuxSMBIOSTable(byte** outTable, size_t* outTableSize)
{
    if (!outTable || !outTableSize)
        return false;

    *outTable = nullptr;
    *outTableSize = 0;

    FILE* file = fopen("/sys/firmware/dmi/tables/DMI", "rb");

    if (!file)
        return false;

    if (fseek(file, 0, SEEK_END) != 0)
    {
        fclose(file);
        return false;
    }

    const long fileSize = ftell(file);

    if (fileSize <= 0)
    {
        fclose(file);
        return false;
    }

    rewind(file);

    byte* table = new byte[static_cast<size_t>(fileSize)];
    const size_t readSize = fread(table, 1, static_cast<size_t>(fileSize), file);

    fclose(file);

    if (readSize != static_cast<size_t>(fileSize))
    {
        delete[] table;
        return false;
    }

    *outTable = table;
    *outTableSize = readSize;
    return true;
}


    bool ReadProcMemInfo(SProcMemInfo& outInfo)
    {
        std::memset(&outInfo, 0, sizeof(outInfo));

        FILE* file = std::fopen("/proc/meminfo", "r");
        if (!file)
            return false;

        char key[64];
        unsigned long long valueKB;
        char unit[16];

        while (std::fscanf(file, "%63[^:]: %llu %15s\n", key, &valueKB, unit) >= 2)
        {
            const qword value = static_cast<qword>(valueKB) * 1024ull;

            if (std::strcmp(key, "MemTotal") == 0)
                outInfo.memTotal = value;
            else if (std::strcmp(key, "SwapTotal") == 0)
                outInfo.swapTotal = value;
        }

        std::fclose(file);
        return outInfo.memTotal != 0;
    }

    
    dword CountLinuxNumaNodes()
    {
        DIR* directory = opendir("/sys/devices/system/node");
        if (!directory)
            return 1;

        dword count = 0;
        dirent* entry = nullptr;

        while ((entry = readdir(directory)) != nullptr)
        {
            if (std::strncmp(entry->d_name, "node", 4) != 0)
                continue;

            const char* number = entry->d_name + 4;

            if (*number >= '0' && *number <= '9')
                ++count;
        }

        closedir(directory);
        return count != 0 ? count : 1;
    }


    bool ReadLinuxSMBIOSTable(std::vector<byte>& table)
    {
        FILE* file = std::fopen("/sys/firmware/dmi/tables/DMI", "rb");
        if (!file)
            return false;

        if (std::fseek(file, 0, SEEK_END) != 0)
        {
            std::fclose(file);
            return false;
        }

        const long size = std::ftell(file);

        if (size <= 0)
        {
            std::fclose(file);
            return false;
        }

        std::rewind(file);
        table.resize(static_cast<size_t>(size));

        const size_t readSize = std::fread(table.data(), 1, table.size(), file);
        std::fclose(file);

        if (readSize != table.size())
        {
            table.clear();
            return false;
        }

        return true;
    }


    