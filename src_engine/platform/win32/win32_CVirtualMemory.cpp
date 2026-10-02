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


CVirtualMemory::CVirtualMemory(IAllocator* allocator) : 
    Inherit(allocator), m_baseAddress(nullptr), m_size(0ull)
{
}


CVirtualMemory::~CVirtualMemory()
{
    if (m_baseAddress)
    {
        ::VirtualFree(m_baseAddress, 0, MEM_RELEASE);
    }
}


uint32 CVirtualMemory::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CVirtualMemory::_Destroy(this);
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

    void* reservedAddr = ::VirtualAlloc(nullptr, static_cast<SIZE_T>(size), MEM_RESERVE, PAGE_READWRITE);
   
    if (!reservedAddr)
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

    void* result = ::VirtualAlloc(baseAddress, static_cast<SIZE_T>(size), MEM_COMMIT, PAGE_READWRITE);
   
    if (!result)
    {
        DWORD err = ::GetLastError();

        if (err == ERROR_NOT_ENOUGH_MEMORY || err == ERROR_COMMITMENT_LIMIT)
            return PLATFORM_ERR_OUT_OF_MEMORY;

        return PLATFORM_ERR_NOT_SUPPORTED;
    }

    return PLATFORM_ERR_OK;
}


#pragma warning(suppress : 6250)
ERRCODE CVirtualMemory::Decommit(void* baseAddress, uint64 size)
{
    if (!baseAddress || size == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!IsRangeValid(baseAddress, size))
        return PLATFORM_ERR_OUT_OF_BOUNDS;

    BOOL success = ::VirtualFree(baseAddress, static_cast<SIZE_T>(size), MEM_DECOMMIT);
    return success ? PLATFORM_ERR_OK : PLATFORM_ERR_NOT_SUPPORTED;
}


ERRCODE CVirtualMemory::LockPages(void* baseAddress, uint64 sizeInBytes)
{
    if (!baseAddress || sizeInBytes == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!IsRangeValid(baseAddress, sizeInBytes))
        return PLATFORM_ERR_OUT_OF_BOUNDS;

    BOOL success = ::VirtualLock(baseAddress, static_cast<SIZE_T>(sizeInBytes));
    return success ? PLATFORM_ERR_OK : PLATFORM_ERR_NOT_SUPPORTED;
}


ERRCODE CVirtualMemory::UnlockPages(void* baseAddress, uint64 sizeInBytes)
{
    if (!baseAddress || sizeInBytes == 0)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    if (!IsRangeValid(baseAddress, sizeInBytes))
        return PLATFORM_ERR_OUT_OF_BOUNDS;

    BOOL success = ::VirtualUnlock(baseAddress, static_cast<SIZE_T>(sizeInBytes));
    return success ? PLATFORM_ERR_OK : PLATFORM_ERR_NOT_SUPPORTED;
}


uint64 CVirtualMemory::GetPageSize()
{
    SYSTEM_INFO si;
    ::GetSystemInfo(&si);
    return static_cast<uint64>(si.dwPageSize);
}


uint64 CVirtualMemory::GetAllocationGranularity()
{
    SYSTEM_INFO si;
    ::GetSystemInfo(&si);
    return static_cast<uint64>(si.dwAllocationGranularity);
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


