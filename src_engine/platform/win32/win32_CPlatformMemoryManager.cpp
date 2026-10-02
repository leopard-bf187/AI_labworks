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
#include <Psapi.h>


CPlatformMemoryManager::CPlatformMemoryManager(IAllocator* allocator) : 
    Inherit(allocator)
{
}


CPlatformMemoryManager::~CPlatformMemoryManager()
{
}


uint32 CPlatformMemoryManager::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CPlatformMemoryManager::_Destroy(this);
        return 0;
    }

    return ref;
};


uint32 CPlatformMemoryManager::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return this->IncRef();
    }

    if (guid == IPlatformMemoryManager::GUID())
    {
        *IFace = static_cast<IPlatformMemoryManager*>(this);
        return this->IncRef();
    }

    return 0;
}


ERRCODE CPlatformMemoryManager::ReserveVirtualMemory(uint64 size, IVirtualMemory** outMemory)
{
    if (!outMemory || size == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    *outMemory = nullptr;

    RefCounted<CVirtualMemory> virtualMemObject;
    virtualMemObject.Attach(CVirtualMemory::_Create(m_allocator.Get()));

    if (!virtualMemObject)
        return PLATFORM_ERR_OUT_OF_MEMORY;

    if (virtualMemObject->Initialize(size) == PLATFORM_ERR_BACKEND_OUT_OF_MEMORY)
    {
        DWORD err = ::GetLastError();

        if (err == ERROR_NOT_ENOUGH_MEMORY || err == ERROR_COMMITMENT_LIMIT)
            return PLATFORM_ERR_BACKEND_OUT_OF_MEMORY;

        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    *outMemory = static_cast<IVirtualMemory*>(virtualMemObject.Get());
    virtualMemObject->IncRef();

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformMemoryManager::CreateSharedMemoryObject(const char* name, uint64 size, ISharedMemory** outShared)
{
    if (!name || size == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    DWORD sizeHigh = static_cast<DWORD>(size >> 32);
    DWORD sizeLow = static_cast<DWORD>(size & 0xFFFFFFFF);

    RefCounted<CSharedMemory> sharedObj;
    sharedObj.Attach(CSharedMemory::_Create(m_allocator.Get()));

    if (!sharedObj.Get())
        return PLATFORM_ERR_OUT_OF_MEMORY;

    HANDLE hMap = ::CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, sizeHigh, sizeLow, name);

    if (!hMap)
    {
        DWORD err = ::GetLastError();

        if (err == ERROR_ALREADY_EXISTS) // another type of resource with same name in system
            return PLATFORM_ERR_ALREADY_EXISTS;

        if (err == ERROR_NOT_ENOUGH_MEMORY)
            return PLATFORM_ERR_OUT_OF_MEMORY;

        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    if (::GetLastError() == ERROR_ALREADY_EXISTS) // another Shared memory object with same name
    {
        ::CloseHandle(hMap);
        return PLATFORM_ERR_ALREADY_EXISTS;
    }

    if (outShared)
    {
        if (*outShared)
        {
            (*outShared)->Delete();
            *outShared = nullptr;
        }

        sharedObj->m_hMapping = hMap;
        sharedObj->m_size = size;
        *outShared = static_cast<ISharedMemory*>(sharedObj.Get());
        sharedObj->IncRef();
    }

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformMemoryManager::OpenSharedMemoryObject(const char* name, ISharedMemory** outShared)
{
    if (!name || !outShared)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (*outShared)
    {
        (*outShared)->Delete();
        *outShared = nullptr;
    }

    RefCounted<CSharedMemory> sharedObj;
    sharedObj.Attach(CSharedMemory::_Create(m_allocator.Get()));

    if (!sharedObj.Get())
        return PLATFORM_ERR_OUT_OF_MEMORY;

    HANDLE hMap = ::OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, name);
    if (!hMap)
    {
        DWORD err = ::GetLastError();

        if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND)
            return PLATFORM_ERR_NOT_FOUND;

        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    uint64 realSize = CSharedMemory::GetSharedMemorySizeFromHandle(hMap);
    if (realSize == 0)
    {
        ::CloseHandle(hMap);
        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    sharedObj->m_hMapping = hMap;
    sharedObj->m_mappedPtr = nullptr;
    sharedObj->m_size = realSize;

    *outShared = static_cast<ISharedMemory*>(sharedObj.Get());
    sharedObj->IncRef();

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformMemoryManager::DestroySharedMemoryObject(const char* name)
{
    if (!name)
    {
        return PLATFORM_ERR_INVALID_ARGUMENT;
    }

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformMemoryManager::GetMemoryStatus(SSystemMemoryStatus* outStatus)
{
    if (!outStatus)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    memset(outStatus, 0, sizeof(SSystemMemoryStatus));

    MEMORYSTATUSEX memoryStatus{};
    memoryStatus.dwLength = sizeof(memoryStatus);

    if (!GlobalMemoryStatusEx(&memoryStatus))
        return PLATFORM_ERR_BACKEND_SYSTEM_CALL_FAILED;

    PERFORMANCE_INFORMATION performanceInfo{};
    performanceInfo.cb = sizeof(performanceInfo);

    if (!GetPerformanceInfo(&performanceInfo, sizeof(performanceInfo)))
        return PLATFORM_ERR_BACKEND_SYSTEM_CALL_FAILED;

    const qword pageSize = static_cast<qword>(performanceInfo.PageSize);
    const qword physicalTotal = static_cast<qword>(performanceInfo.PhysicalTotal) * pageSize;
    const qword commitTotal = static_cast<qword>(performanceInfo.CommitTotal) * pageSize;
    const qword commitLimit = static_cast<qword>(performanceInfo.CommitLimit) * pageSize;

    outStatus->availablePhysical = memoryStatus.ullAvailPhys;

    /*
        Windows не предоставляет через эти API отдельное значение полностью
        свободных страниц, аналогичное Linux MemFree. ullAvailPhys включает
        память, которую ОС способна быстро предоставить приложению.
    */
    outStatus->freePhysical = memoryStatus.ullAvailPhys;

    outStatus->cached = static_cast<qword>(performanceInfo.SystemCache) * pageSize;
    outStatus->buffers = 0;

    /*
        CommitLimit включает физическую память и page files. Это оценка
        суммарного объёма файлов подкачки, а не точный размер каждого файла.
    */
    outStatus->swapTotal = commitLimit > physicalTotal ? commitLimit - physicalTotal : 0;

    const qword usedSwapEstimate = commitTotal > physicalTotal ? commitTotal - physicalTotal : 0;
    outStatus->swapAvailable = outStatus->swapTotal > usedSwapEstimate ? outStatus->swapTotal - usedSwapEstimate : 0;

    outStatus->committed = commitTotal;
    outStatus->commitLimit = commitLimit;
    outStatus->memoryLoadPercent = memoryStatus.dwMemoryLoad;
    outStatus->lowMemory = outStatus->memoryLoadPercent >= SYSTEM_LOW_MEMORY_LOAD_PERCENT;

    return PLATFORM_ERR_OK;
}


