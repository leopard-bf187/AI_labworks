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


#include "win32_classes.h"
#include "../cpuinfo.h"
#include "../mem_smbios.h"


// DECLARATIONS

#pragma pack(push, 1)

struct SRawSMBIOSData
{
    byte used20CallingMethod;
    byte majorVersion;
    byte minorVersion;
    byte dmiRevision;
    dword length;
    byte tableData[1];
};

#pragma pack(pop)

bool ReadWindowsSMBIOSTable(IAllocator* allocator, byte** outTable, dword* outTableSize);

/////////////////////////////////////////////////////////////////////////


CPlatformSystemInfo::CPlatformSystemInfo(IAllocator* allocator) : Inherit(allocator)
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
    return 0ul;
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
    dword tableSize = 0;

    if (!ReadWindowsSMBIOSTable(const_cast<IAllocator*>(m_allocator.Get()), &table, &tableSize))
        return 0;

    const dword count = CountSMBIOSMemoryDevices(table, tableSize);

    const_cast<IAllocator*>(m_allocator.Get())->Free(table);
    return count;
}


ERRCODE CPlatformSystemInfo::GetPhysicalMemoryDeviceInfo(dword index, SPhysicalMemoryDeviceInfo* outMemDeviceInfo) const
{
    if (!outMemDeviceInfo)
        return PLATFORM_ERR_INVALID_ARGUMENT;


    memset(outMemDeviceInfo, 0, sizeof(SPhysicalMemoryDeviceInfo));
    outMemDeviceInfo->type = EMemoryType::MEM_TYPE_UNKNOWN;
    outMemDeviceInfo->formFactor = EMemoryFormFactor::MEM_FORM_FACTOR_UNKNOWN;

    byte* table = nullptr;
    dword tableSize = 0;

    if (!ReadWindowsSMBIOSTable(const_cast<IAllocator*>(m_allocator.Get()), &table, &tableSize))
        return PLATFORM_ERR_NOT_SUPPORTED;

    dword deviceCount = 0;
    const bool found = ParseSMBIOSMemoryDevice(table, tableSize, index, outMemDeviceInfo, &deviceCount);

    const_cast<IAllocator*>(m_allocator.Get())->Free(table);

    if (index >= deviceCount)
        return PLATFORM_ERR_OUT_OF_RANGE;

    return found ? PLATFORM_ERR_OK : PLATFORM_ERR_NOT_FOUND;
}


bool CPlatformSystemInfo::InitializeCPU()
{
    memset(&m_cpu, 0, sizeof(SSystemCPUInfo));
    return QuerySystemCPUInfo(&m_cpu);
}


bool CPlatformSystemInfo::InitializeOS()
{
    memset(&m_os, 0, sizeof(SSystemOSInfo));

    HKEY hkey;
    if (FAILED(RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hkey)))
        return false;

    char build[16] = { 0 };
    char version[16] = { 0 };
    char ubrstr[16] = { 0 };
    dword majorVer = 0;
    dword minorVer = 0;
    dword ubr = 0;
    uint szDw = sizeof(DWORD);
    uint szStr = sizeof(m_os.ProductName);
    uint szStr16 = sizeof(version);

    RegQueryValueExA(hkey, "ProductName", 0, 0, (byte*)m_os.ProductName, (DWORD*)&szStr);
    RegQueryValueExA(hkey, "EditionID", 0, 0, (byte*)m_os.Edition, (DWORD*)&szStr);

    RegQueryValueExA(hkey, "CurrentBuildNumber", 0, 0, (byte*)build, (DWORD*)&szStr16);

    RegQueryValueExA(hkey, "UBR", 0, 0, (byte*)&ubr, (DWORD*)&szDw);
    int32_to_str(ubr, ubrstr);

    if ((RegQueryValueExA(hkey, "CurrentMajorVersionNumber", 0, 0, (byte*)&majorVer, (DWORD*)&szDw) == ERROR_FILE_NOT_FOUND) ||
        (RegQueryValueExA(hkey, "CurrentMinorVersionNumber", 0, 0, (byte*)&minorVer, (DWORD*)&szDw) == ERROR_FILE_NOT_FOUND))
    {
        RegQueryValueExA(hkey, "CurrentVersion", 0, 0, (byte*)version, (DWORD*)&szStr16);
    }
    else
    {
        char majb[16] = { 0 };
        char minb[16] = { 0 };
        int32_to_str(majorVer, majb);
        int32_to_str(minorVer, minb);
        Stdlib::StrCat(version, majb);
        Stdlib::StrCat(version, ".");
        Stdlib::StrCat(version, minb);
    }

    Stdlib::StrCat(m_os.Version, version);
    Stdlib::StrCat(m_os.Version, ".");
    Stdlib::StrCat(m_os.Version, build);
    Stdlib::StrCat(m_os.Version, ".");
    Stdlib::StrCat(m_os.Version, ubrstr);

    RegCloseKey(hkey);

    DWORD len = sizeof(m_os.UserName);
    const char* failedStr = "<failed to get>";
    
    if (GetUserNameA(m_os.UserName, &len) == 0)
    {
        Stdlib::StrCopyN(m_os.UserName, failedStr, 16);
        m_os.UserName[16] = '\0';
    }

    len = sizeof(m_os.UserName);
    if(GetComputerNameA(m_os.DesktopName, &len) == 0)
    {
        Stdlib::StrCopyN(m_os.DesktopName, failedStr, 16);
        m_os.DesktopName[16] = '\0';
    }

    return true;
}


bool CPlatformSystemInfo::InitializeRAM()
{
    memset(&m_ram, 0, sizeof(SSystemMemoryInfo));

    MEMORYSTATUSEX memoryStatus{};
    memoryStatus.dwLength = sizeof(memoryStatus);

    if (!GlobalMemoryStatusEx(&memoryStatus))
        return false;

    m_ram.usablePhysical = memoryStatus.ullTotalPhys;
    m_ram.hasSwap = memoryStatus.ullTotalPageFile > memoryStatus.ullTotalPhys;

    ULONGLONG installedMemoryKB = 0;

    if (GetPhysicallyInstalledSystemMemory(&installedMemoryKB))
        m_ram.installedPhysical = installedMemoryKB * 1024ull;
    else
        m_ram.installedPhysical = m_ram.usablePhysical;

    SYSTEM_INFO systemInfo{};
    GetNativeSystemInfo(&systemInfo);

    m_ram.pageSize = systemInfo.dwPageSize;
    m_ram.allocationGranularity = systemInfo.dwAllocationGranularity;

    ULONG highestNodeNumber = 0;

    if (GetNumaHighestNodeNumber(&highestNodeNumber))
        m_ram.numaNodeCount = highestNodeNumber + 1;
    else
        m_ram.numaNodeCount = 1;

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

    m_ram.isLowRamDevice = false;
    return true;
}


/////////////////////////////////////////////////////////////////////////

bool ReadWindowsSMBIOSTable(IAllocator* allocator, byte** outTable, dword* outTableSize)
{
    if (!outTable || !outTableSize)
        return false;

    *outTable = nullptr;
    *outTableSize = 0;

    const dword provider = 'RSMB';
    const dword requiredSize = GetSystemFirmwareTable(provider, 0, nullptr, 0);

    if (requiredSize < offsetof(SRawSMBIOSData, tableData))
        return false;

    //byte* buffer = new byte[requiredSize];
    byte* buffer = (byte*)allocator->Alloc(requiredSize);

    if (GetSystemFirmwareTable(provider, 0, buffer, requiredSize) != requiredSize)
    {
        allocator->Free(buffer);
        return false;
    }

    const SRawSMBIOSData* raw = reinterpret_cast<const SRawSMBIOSData*>(buffer);
    const size_t headerSize = offsetof(SRawSMBIOSData, tableData);

    if (raw->length > requiredSize - headerSize)
    {
        allocator->Free(buffer);
        return false;
    }

    byte* table = (byte*)allocator->Alloc(raw->length);
    memcpy(table, raw->tableData, raw->length);

    allocator->Free(buffer);

    *outTable = table;
    *outTableSize = raw->length;
    return true;
}