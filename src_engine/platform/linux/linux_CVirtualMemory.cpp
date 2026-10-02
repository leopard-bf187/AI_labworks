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

#include "linux_classes.h"
#include <new>
#include <unistd.h>
#include <errno.h>
#include <cstdint>


CVirtualMemory::CVirtualMemory(IAllocator* allocator) :
    Inherit(allocator), m_baseAddress(nullptr), m_size(0ull)
{
}


CVirtualMemory::~CVirtualMemory()
{
    if (m_baseAddress && m_size > 0)
    {
        ::munmap(m_baseAddress, m_size);
    }
}


uint32 CVirtualMemory::Delete()
{
    uint32 ref = DecRef();
    if (ref == 0)
    {
        delete this;
        return 0;
    }
    return ref;
}


uint32 CVirtualMemory::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return IncRef();
    }

    if (guid == IVirtualMemory::GUID())
    {
        *IFace = static_cast<IVirtualMemory*>(this);
        return IncRef();
    }

    return 0;
}


ERRCODE CVirtualMemory::Initialize(uint64 size)
{
    if (size == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    void* reservedAddr = ::mmap(nullptr, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (reservedAddr == MAP_FAILED)
        return PLATFORM_ERR_BACKEND_OUT_OF_MEMORY;

    m_baseAddress = reservedAddr;
    m_size = size;

    return PLATFORM_ERR_OK;
}


void* CVirtualMemory::GetBaseAddress()
{
    return m_baseAddress;
}


const void* CVirtualMemory::GetBaseAddress() const
{
    return m_baseAddress;
}


uint64 CVirtualMemory::GetReservedBytes() const
{
    return m_size;
}


ERRCODE CVirtualMemory::Commit(void* baseAddress, uint64 size)
{
    if (!baseAddress || size == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!IsRangeValid(baseAddress, size))
        return PLATFORM_ERR_OUT_OF_BOUNDS;

    if (::mprotect(baseAddress, size, PROT_READ | PROT_WRITE) == -1)
    {
        if (errno == ENOMEM)
            return PLATFORM_ERR_OUT_OF_MEMORY;

        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    ::madvise(baseAddress, size, MADV_WILLNEED);

    return PLATFORM_ERR_OK;
}


ERRCODE CVirtualMemory::Decommit(void* baseAddress, uint64 size)
{
    if (!baseAddress || size == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!IsRangeValid(baseAddress, size))
        return PLATFORM_ERR_OUT_OF_BOUNDS;

    if (::mprotect(baseAddress, size, PROT_NONE) == -1)
        return PLATFORM_ERR_NOT_SUPPORTED;

    ::madvise(baseAddress, size, MADV_DONTNEED);

    return PLATFORM_ERR_OK;
}


ERRCODE CVirtualMemory::LockPages(void* baseAddress, uint64 sizeInBytes)
{
    if (!baseAddress || sizeInBytes == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!IsRangeValid(baseAddress, sizeInBytes))
        return PLATFORM_ERR_OUT_OF_BOUNDS;

    if (::mlock(baseAddress, sizeInBytes) == -1)
        return PLATFORM_ERR_NOT_SUPPORTED;

    return PLATFORM_ERR_OK;
}


ERRCODE CVirtualMemory::UnlockPages(void* baseAddress, uint64 sizeInBytes)
{
    if (!baseAddress || sizeInBytes == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!IsRangeValid(baseAddress, sizeInBytes))
        return PLATFORM_ERR_OUT_OF_BOUNDS;

    if (::munlock(baseAddress, sizeInBytes) == -1)
        return PLATFORM_ERR_NOT_SUPPORTED;

    return PLATFORM_ERR_OK;
}


uint64 CVirtualMemory::GetPageSize()
{
    return static_cast<uint64>(::sysconf(_SC_PAGESIZE));
}


uint64 CVirtualMemory::GetAllocationGranularity()
{
    return static_cast<uint64>(::sysconf(_SC_PAGESIZE));
}


ERRCODE CVirtualMemory::GetNativeHandle(SNativeHandle* outHandle) const
{
    if (!outHandle)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    outHandle->ptr = m_baseAddress;
    outHandle->fd = -1;
    outHandle->type = NATIVE_HANDLE_VIRTUAL_MEMORY;

    return PLATFORM_ERR_OK;
}

