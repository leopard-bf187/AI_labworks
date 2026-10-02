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


// TODO: add Initialize() call to android main() during platform managers initialization, and pass path to application cache directory


#include "android_classes.h"
#include "posix_mem.inl"


CPlatformMemoryManager::CPlatformMemoryManager(IAllocator* allocator) :
    Inherit(allocator)
{
    this->Initialize("./");
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
}


uint32 CPlatformMemoryManager::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return IncRef();
    }
        
    if(guid == IPlatformMemoryManager::GUID())
    {
        *IFace = static_cast<IPlatformMemoryManager*>(this);
        return IncRef();
    }

    return 0;
}


ERRCODE CPlatformMemoryManager::ReserveVirtualMemory(uint64 size, IVirtualMemory** outMemory)
{
    if (!outMemory || size == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if(*outMemory)
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

    char filePath[512];
    CSharedMemory::ResolveShmPath(m_cacheDir, name, filePath, sizeof(filePath));

    int fd = ::open(filePath, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
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
        ::unlink(filePath);
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

    char filePath[512];
    CSharedMemory::ResolveShmPath(m_cacheDir, name, filePath, sizeof(filePath));

    int fd = ::open(filePath, O_RDWR);

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

    char filePath[512];
    if (name[0] == '/')
        name++;

    if (m_cacheDir[0] == '\0')
    {
        snprintf(filePath, sizeof(filePath), "/data/local/tmp/krystallic_shm_%s", name);
    }
    else
    {
        snprintf(filePath, sizeof(filePath), "%s/kryst_shm_%s", m_cacheDir, name);
    }

    if (::unlink(filePath) == -1)
    {
        if (errno == ENOENT)
            return PLATFORM_ERR_NOT_FOUND;

        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    return PLATFORM_ERR_OK;
}


void CPlatformMemoryManager::Initialize(const char* internalDataPath)
{
    if (internalDataPath)
        strncpy(m_cacheDir, internalDataPath, sizeof(m_cacheDir) - 1);

    ::strncpy(m_cacheDir, internalDataPath, sizeof(m_cacheDir) - 1);
    m_cacheDir[sizeof(m_cacheDir) - 1] = '\0';

    const char* shmPrefix = "krystallic_shm_";
    size_t      prefixLen = ::strlen(shmPrefix);

    DIR* dir = ::opendir(m_cacheDir);
    if (dir)
    {
        struct dirent* entry;
        char fullPath[1024];

        while ((entry = ::readdir(dir)) != nullptr)
        {
            if (::strncmp(entry->d_name, shmPrefix, prefixLen) == 0)
            {
                ::snprintf(fullPath, sizeof(fullPath), "%s/%s", m_cacheDir, entry->d_name);
                ::unlink(fullPath);
            }
        }
        ::closedir(dir);
    }
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

    /*
        Это native-эвристика. Точное Android-состояние lowMemory и системный
        threshold доступны через ActivityManager.MemoryInfo по JNI.
    */
    outStatus->lowMemory = outStatus->memoryLoadPercent >= SYSTEM_LOW_MEMORY_LOAD_PERCENT;
    return PLATFORM_ERR_OK;
}


