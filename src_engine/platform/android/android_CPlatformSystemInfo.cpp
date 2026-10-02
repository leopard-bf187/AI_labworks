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


#include "android_classes.h"
#include "../cpuinfo.h"
#include <sys/system_properties.h>
#include <sys/utsname.h>
#include <pwd.h>


struct SAndroidProcMemInfo
{
    qword memTotal;
    qword memAvailable;
    qword swapTotal;
};

static bool ReadAndroidProperty(const char* name, char* outValue, size_t outValueSize);
bool ReadAndroidProcMemInfo(SAndroidProcMemInfo& outInfo);




CPlatformSystemInfo::CPlatformSystemInfo(IAllocator* allocator) : Inherit(allocator), m_physicalMemoryDevices(nullptr), m_physicalMemoryDeviceCount(0)
{
    this->InitializeCPU();
    this->InitializeOS();
    this->InitializeRAM();
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
    SAndroidProcMemInfo memoryInfo{};

    if (!ReadAndroidProcMemInfo(memoryInfo))
        return 0ull;

    return memoryInfo.memAvailable;
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
    return m_physicalMemoryDeviceCount;
}


ERRCODE CPlatformSystemInfo::GetPhysicalMemoryDeviceInfo(dword index, SPhysicalMemoryDeviceInfo* outInfo) const
{
    if (!outInfo)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (index >= m_physicalMemoryDeviceCount || !m_physicalMemoryDevices)
        return PLATFORM_ERR_OUT_OF_RANGE;

    *outInfo = m_physicalMemoryDevices[index];
    return PLATFORM_ERR_OK;
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

    char release[PROP_VALUE_MAX] = {};
    char sdk[PROP_VALUE_MAX] = {};
    char buildID[PROP_VALUE_MAX] = {};
    char incremental[PROP_VALUE_MAX] = {};
    char manufacturer[PROP_VALUE_MAX] = {};
    char model[PROP_VALUE_MAX] = {};
    char device[PROP_VALUE_MAX] = {};

    ReadAndroidProperty("ro.build.version.release", release, sizeof(release));
    ReadAndroidProperty("ro.build.version.sdk", sdk, sizeof(sdk));
    ReadAndroidProperty("ro.build.id", buildID, sizeof(buildID));
    ReadAndroidProperty("ro.build.version.incremental", incremental, sizeof(incremental));
    ReadAndroidProperty("ro.product.manufacturer", manufacturer, sizeof(manufacturer));
    ReadAndroidProperty("ro.product.model", model, sizeof(model));
    ReadAndroidProperty("ro.product.device", device, sizeof(device));

    if (release[0])
        snprintf(m_os.ProductName, sizeof(m_os.ProductName), "Android %s", release);
    else
        Stdlib::StrCopyN(m_os.ProductName, "Android", sizeof(m_os.ProductName));

    if (manufacturer[0])
        Stdlib::StrCopyN(m_os.Edition, manufacturer, sizeof(m_os.Edition));
    else
        Stdlib::StrCopyN(m_os.Edition, "N/A", sizeof(m_os.Edition));

    if (sdk[0] && buildID[0])
    {
        snprintf(m_os.Version, sizeof(m_os.Version), "%s, API %s, build %s", release[0] ? release : "Unknown", sdk, buildID);
    }
    else if (release[0])
    {
        Stdlib::StrCopyN(m_os.Version, release, sizeof(m_os.Version));
    }
    else
    {
        Stdlib::StrCopyN(m_os.Version, "Unknown", sizeof(m_os.Version));
    }

    passwd pwd{};
    passwd* result = nullptr;
    char pwdBuffer[1024] = {};

    if (getpwuid_r(geteuid(), &pwd, pwdBuffer, sizeof(pwdBuffer), &result) == 0 &&
        result && result->pw_name)
    {
        Stdlib::StrCopyN(m_os.UserName, result->pw_name, sizeof(m_os.UserName));
    }
    else
    {
        snprintf(m_os.UserName, sizeof(m_os.UserName), "uid_%u", static_cast<uint32>(geteuid()));
    }

    if (manufacturer[0] && model[0])
    {
        if (strstr(model, manufacturer) == model)
            Stdlib::StrCopyN(m_os.DesktopName, model, sizeof(m_os.DesktopName));
        else
            snprintf(m_os.DesktopName, sizeof(m_os.DesktopName), "%s %s", manufacturer, model);
    }
    else if (model[0])
    {
        Stdlib::StrCopyN(m_os.DesktopName, model, sizeof(m_os.DesktopName));
    }
    else if (device[0])
    {
        Stdlib::StrCopyN(m_os.DesktopName, device, sizeof(m_os.DesktopName));
    }
    else
    {
        Stdlib::StrCopyN(m_os.DesktopName, "Android Device", sizeof(m_os.DesktopName));
    }

    Stdlib::StrCopyN(m_os.InstallDate, "Unknown", sizeof(m_os.InstallDate));

    return true;
}


bool CPlatformSystemInfo::InitializeRAM()
{
    memset(&m_ram, 0, sizeof(SSystemMemoryInfo));

    SAndroidProcMemInfo memoryInfo{};

    if (!ReadAndroidProcMemInfo(memoryInfo))
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

    m_ram.numaNodeCount = 1;
    m_ram.isLowRamDevice = false;

    return true;
}


bool ReadAndroidProperty(const char* name, char* outValue, size_t outValueSize)
{
    if (!name || !outValue || outValueSize == 0)
        return false;

    char value[PROP_VALUE_MAX] = {};

    int length = __system_property_get(name, value);

    if (length <= 0)
    {
        outValue[0] = '\0';
        return false;
    }

    snprintf(outValue, outValueSize, "%s", value);
    return true;
}

bool ReadAndroidProcMemInfo(SAndroidProcMemInfo& outInfo)
{
    memset(&outInfo, 0, sizeof(outInfo));

    FILE* file = fopen("/proc/meminfo", "r");

    if (!file)
        return false;

    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        unsigned long long valueKB = 0;

        if (sscanf(line, "MemTotal: %llu kB", &valueKB) == 1)
            outInfo.memTotal = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "MemAvailable: %llu kB", &valueKB) == 1)
            outInfo.memAvailable = static_cast<qword>(valueKB) * 1024ull;
        else if (sscanf(line, "SwapTotal: %llu kB", &valueKB) == 1)
            outInfo.swapTotal = static_cast<qword>(valueKB) * 1024ull;
    }

    fclose(file);
    return outInfo.memTotal != 0;
}