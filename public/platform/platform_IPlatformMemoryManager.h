/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @Ra192192
*
*  Description:
*
*  Date: 02.06.2026
*/
#pragma once

#include "platform_dll_types.h"

namespace krystallic
{
    namespace Platform
    {
        struct IPlatformMemoryManager : public krystallic::IBase
        {
            virtual ~IPlatformMemoryManager() = default;

            virtual ERRCODE ReserveVirtualMemory(uint64 size, IVirtualMemory** outMemory) = 0;
            virtual ERRCODE CreateSharedMemoryObject(const char* name, uint64 size, ISharedMemory** outShared = nullptr) = 0;
            virtual ERRCODE OpenSharedMemoryObject(const char* name, ISharedMemory** outShared) = 0;
            virtual ERRCODE DestroySharedMemoryObject(const char* name) = 0;

            virtual ERRCODE GetMemoryStatus(SSystemMemoryStatus* memStatus) = 0;

            inline static SGuid GUID()
            {
                return {0xda1b9832, 0xfa8c, 0x4a32, {0xac, 0x8c, 0x1b, 0xef, 0xc8, 0xf2, 0xc4, 0xad}};
            }
        };


        struct IVirtualMemory : IBase
        {
            virtual ~IVirtualMemory() = default;

            virtual void*       GetBaseAddress() = 0;
            virtual const void* GetBaseAddress() const = 0;

            virtual uint64 GetReservedBytes() const = 0;
            //virtual const uint64 GetReservedBytes() const = 0;

            virtual ERRCODE Commit(void* baseAddress, uint64 size) = 0;
            virtual ERRCODE Decommit(void* baseAddress, uint64 size) = 0;

            virtual ERRCODE LockPages(void* baseAddress, uint64 sizeInBytes) = 0;
            virtual ERRCODE UnlockPages(void* baseAddress, uint64 sizeInBytes) = 0;

            virtual uint64 GetPageSize() = 0;
            virtual uint64 GetAllocationGranularity() = 0;

            virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const = 0;

            inline static SGuid GUID()
            {
                return {0x50a425bb, 0xfb7b, 0x41b6, {0xb4, 0xf5, 0xa1, 0x2e, 0xdf, 0xe5, 0x4f, 0xe4}};
            }
        };


        struct ISharedMemory : IBase
        {
            virtual ~ISharedMemory() = default;

            virtual void* Map() = 0;
            virtual void  Unmap() = 0;

            virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const = 0;

            inline static SGuid GUID()
            {
                return {0x49a07e8a, 0xfa6, 0x4ceb, {0xa8, 0x75, 0x8, 0xaf, 0x1e, 0xdd, 0xc6, 0xb5}};
            }
        };
    } 
} 
