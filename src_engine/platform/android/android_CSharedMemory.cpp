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


CSharedMemory::CSharedMemory(IAllocator* allocator) : 
    Inherit(allocator), m_shmFd(-1), m_mappedPtr(nullptr), m_size(0ull)
{
}


CSharedMemory::~CSharedMemory()
{
    Unmap();
    if (m_shmFd != -1)
    {
        ::close(m_shmFd);
        m_shmFd = -1;
    }
}


uint32 CSharedMemory::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CSharedMemory::_Destroy(this);
        return 0;
    }

    return ref;
}


uint32 CSharedMemory::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return IncRef();
    }

    if (guid == ISharedMemory::GUID())
    {
        *IFace = static_cast<ISharedMemory*>(this);
        return IncRef();
    }

    return 0;
}


void CSharedMemory::ResolveShmPath(const char* cacheDir, const char* name, char* outPath, size_t maxLen)
{
    if (name[0] == '/')
        name++;

    // for local tests, if no cachedir provided 
    if (!cacheDir || cacheDir[0] == '\0')
    {
        snprintf(outPath, maxLen, "/data/local/tmp/krystallic_shm_%s", name);
    }
    else
    {
        snprintf(outPath, maxLen, "%s/krystallic_shm_%s", cacheDir, name);
    }
}


void* CSharedMemory::Map()
{
    if (m_mappedPtr)
        return m_mappedPtr;

    if (m_shmFd == -1)
        return nullptr;

    void* ptr = ::mmap(nullptr, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_shmFd, 0);
    if (ptr == MAP_FAILED)
        return nullptr;

    m_mappedPtr = ptr;
    return m_mappedPtr;
}


void CSharedMemory::Unmap()
{
    if (m_mappedPtr)
    {
        ::munmap(m_mappedPtr, m_size);
        m_mappedPtr = nullptr;
    }
}


ERRCODE CSharedMemory::GetNativeHandle(SNativeHandle* outHandle) const
{
    if (!outHandle)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    outHandle->ptr = m_mappedPtr;
    outHandle->fd = m_shmFd;
    outHandle->type = NATIVE_HANDLE_SHARED_MEMORY;

    return PLATFORM_ERR_OK;
}


