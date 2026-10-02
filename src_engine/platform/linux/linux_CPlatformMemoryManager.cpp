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

// TODO: prevent system shared memory leaks in case of closing application before calling DestroySharedMemoryObject() manually


#include "linux_classes.h"

#include "../posix_mem.inl"


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
        delete this;
        return 0;
    }

    return ref;
}

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

    if (*outMemory)
    {
        (*outMemory)->Delete();
        *outMemory = nullptr;
    }

    RefCounted<CVirtualMemory> virtualMemObject;
    virtualMemObject.Attach(CVirtualMemory::_Create(m_allocator.Get()));

    if (!virtualMemObject.Get())
        return PLATFORM_ERR_OUT_OF_MEMORY;

    if (virtualMemObject->Initialize(size) == PLATFORM_ERR_BACKEND_OUT_OF_MEMORY)
    {
        if (errno == ENOMEM)
            return PLATFORM_ERR_OUT_OF_MEMORY;

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

    RefCounted<CSharedMemory> sharedObj;
    sharedObj.Attach(CSharedMemory::_Create(m_allocator.Get()));

    if (!sharedObj)
        return PLATFORM_ERR_OUT_OF_MEMORY;

    int fd = ::shm_open(name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        if (errno == EEXIST)
            return PLATFORM_ERR_ALREADY_EXISTS;

        if (errno == ENOMEM)
            return PLATFORM_ERR_OUT_OF_MEMORY;

        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    if (::ftruncate(fd, size) == -1)
    {
        ::close(fd);
        ::shm_unlink(name);
        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    if (outShared)
    {
        if (*outShared)
        {
            (*outShared)->Delete();
            *outShared = nullptr;
        }

        sharedObj->m_shmFd = fd;
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

    if (!sharedObj)
        return PLATFORM_ERR_OUT_OF_MEMORY;

    int fd = ::shm_open(name, O_RDWR, 0);
    if (fd == -1)
    {
        if (errno == ENOENT)
            return PLATFORM_ERR_NOT_FOUND;

        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    struct stat sb;
    if (::fstat(fd, &sb) == -1)
    {
        ::close(fd);
        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    sharedObj->m_shmFd = fd;
    sharedObj->m_mappedPtr = nullptr;
    sharedObj->m_size = sb.st_size;

    *outShared = static_cast<ISharedMemory*>(sharedObj.Get());
    sharedObj->IncRef();

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformMemoryManager::DestroySharedMemoryObject(const char* name)
{
    if (!name)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (::shm_unlink(name) == -1)
    {
        if (errno == ENOENT)
            return PLATFORM_ERR_NOT_FOUND;
        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    return PLATFORM_ERR_OK;
}


ERRCODE CPlatformMemoryManager::GetMemoryStatus(SSystemMemoryStatus* outStatus)
{
    if (!outStatus)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    memset(outStatus, 0, sizeof(SSystemMemoryStatus));

    SProcMemoryStatus memoryStatus{};

    if (!ReadProcMemoryStatus(&memoryStatus))
        return PLATFORM_ERR_BACKEND_SYSTEM_CALL_FAILED;

    outStatus->availablePhysical = memoryStatus.memAvailable != 0 ? memoryStatus.memAvailable : memoryStatus.memFree;
    outStatus->freePhysical = memoryStatus.memFree;

    /*
        Cached �� �������� SwapCached. SReclaimable �������� ������ slab,
        ������� ���� �������� ����������, ������� � ������� ��������� � cache.
        Shmem ����������, ����� �� ��������� shared memory ������.
    */
    outStatus->cached = memoryStatus.cached + memoryStatus.sReclaimable;

    if (outStatus->cached >= memoryStatus.shmem)
        outStatus->cached -= memoryStatus.shmem;

    outStatus->buffers = memoryStatus.buffers;
    outStatus->swapTotal = memoryStatus.swapTotal;
    outStatus->swapAvailable = memoryStatus.swapFree;
    outStatus->committed = memoryStatus.committedAS;
    outStatus->commitLimit = memoryStatus.commitLimit;

    if (memoryStatus.memTotal != 0)
    {
        const qword usedPhysical = memoryStatus.memTotal > outStatus->availablePhysical ? 
            memoryStatus.memTotal - outStatus->availablePhysical : 0;

        outStatus->memoryLoadPercent = static_cast<dword>((usedPhysical * 100ull) / memoryStatus.memTotal);
    }

    outStatus->lowMemory = outStatus->memoryLoadPercent >= SYSTEM_LOW_MEMORY_LOAD_PERCENT;
    return PLATFORM_ERR_OK;
}

