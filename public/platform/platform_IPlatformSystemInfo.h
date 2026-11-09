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
        struct IPlatformSystemInfo : IBase
        {
            virtual ~IPlatformSystemInfo() = default;

            virtual const char* GetOSProductName() const = 0;
            virtual const char* GetOSEdition() const = 0;
            virtual const char* GetOSVersion() const = 0;

            virtual const char* GetOSUserName() const = 0;
            virtual const char* GetOSDesktopName() const = 0;
            virtual const char* GetOSInstallDate() const = 0;

            virtual const char* GetCPUVendor() const = 0;
            virtual const char* GetCPUBrand() const = 0;

            virtual uint32 GetCPUPhysicalCoreCount() const = 0;
            virtual uint32 GetCPULogicalCoreCount() const = 0;

            virtual uint32 GetCPUSIMDFlags() const = 0;

            virtual uint64 GetPhysicalMemorySize() const = 0;
            virtual uint64 GetAvailableMemory() const = 0;

            virtual ERRCODE GetFullOSInfo(SSystemOSInfo* outOSInfo) const = 0;
            virtual ERRCODE GetFullCPUInfo(SSystemCPUInfo* outCpuInfo) const = 0;
            virtual ERRCODE GetFullRAMInfo(SSystemMemoryInfo* outCpuInfo) const = 0;
            
            virtual dword GetPhysicalMemoryDeviceCount() const = 0;
            virtual ERRCODE GetPhysicalMemoryDeviceInfo(dword index, SPhysicalMemoryDeviceInfo* outInfo) const = 0;

            inline static SGuid GUID()
            {
                return {0xdff399ab, 0x4b4a, 0x4e04, {0xbe, 0xf9, 0x2b, 0x61, 0x39, 0x94, 0xd, 0x46}};
            }
        };
    } 
} 
