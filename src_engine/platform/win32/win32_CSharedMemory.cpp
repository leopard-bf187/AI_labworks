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


CSharedMemory::CSharedMemory(IAllocator* allocator) : 
    Inherit(allocator), m_hMapping(nullptr), m_mappedPtr(nullptr), m_size(0ull)
{
}


CSharedMemory::~CSharedMemory()
{
    Unmap();

    if (m_hMapping)
    {
        ::CloseHandle(m_hMapping);
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


uint64 CSharedMemory::GetSharedMemorySizeFromHandle(HANDLE hMap)
{
    if (!hMap) return 0;

    void* ptr = ::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 1);

    if (!ptr) return 0;

    uint64 size = 0;
    MEMORY_BASIC_INFORMATION mbi{};

    if (::VirtualQuery(ptr, &mbi, sizeof(mbi)) != 0)
        size = static_cast<uint64>(mbi.RegionSize);

    ::UnmapViewOfFile(ptr);

    return size;
}


void* CSharedMemory::Map()
{
    if (m_mappedPtr)
    {
        return m_mappedPtr;
    }

    m_mappedPtr = ::MapViewOfFile(m_hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    return m_mappedPtr;
}


void CSharedMemory::Unmap()
{
    if (m_mappedPtr)
    {
        ::UnmapViewOfFile(m_mappedPtr);
        m_mappedPtr = nullptr;
    }
}


ERRCODE CSharedMemory::GetNativeHandle(SNativeHandle* outHandle) const
{
    if (!outHandle)
        return PLATFORM_ERR_INVALID_ARGUMENT;

    outHandle->ptr = m_hMapping;
    outHandle->fd = -1;
    outHandle->type = NATIVE_HANDLE_SHARED_MEMORY;

    return PLATFORM_ERR_OK;
}


