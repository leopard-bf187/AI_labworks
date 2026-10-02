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


#include "../pch.h"
#include "../base.h"
#include "../common.h"
#include "../stdlib_dll.h"

#include "platform_dll_enums.h"


#define PLATFORM_VERSION 0x0003


#if defined(KRYSTALLIC_OS_WINNT)
#if defined(PLATFORM_API_EXPORT)
#define PLATFORM_API __declspec(dllexport)
#else
#define PLATFORM_API __declspec(dllimport)
#endif
#elif defined(KRYSTALLIC_OS_LINUX) || defined(KRYSTALLIC_OS_ANDROID)
#define PLATFORM_API __attribute__((visibility("default")))
#endif


#ifdef __cplusplus
extern "C" {
#endif


namespace krystallic
{
	namespace Platform
	{
        struct IPlatformManager;
        struct IPlatformMemoryManager;
        struct IPlatformInputManager;
        struct IPlatformProcessManager;
        struct IPlatformSystemInfo;
        struct IPlatformThreadManager;
        struct IPlatformTimeManager;
        struct IPlatformWindow;
        struct IPlatformWindowCallback;

        struct IVirtualMemory;
        struct ISharedMemory;
        struct ISystemTimer;


        struct SNativeHandle
        {
            void*  ptr;
            int    fd;
            uint32 type;
        };


        struct SPlatformWindowDesc
        {
            uint32 width;
            uint32 height;
            int32 x;
            int32 y;

            union
            {
                struct
                {
                    dword resizable : 1;
                    dword visible : 1;
                    dword fullscreen : 1;
                    dword caption : 1;
                    dword sysmenu : 1;
                    dword minimizeButton : 1;
                    dword maximizeButton : 1;
                };
                dword mask;
            } style;

            union
            {
                struct
                {
                    byte a;
                    byte r;
                    byte g;
                    byte b;
                };
                dword mask;
            } backgroundColor;

            SNativeHandle icon;
            SNativeHandle iconSmall;
            SNativeHandle cursor;
        };


        struct SSystemOSInfo
        {
            char ProductName[128];
            char Edition[128];
            char Version[128];
            char UserName[128];
            char DesktopName[128];
            char InstallDate[32];
        };
        

        struct SSystemCPUInfo
        {
            char vendor[20];
            char brand[64];
            int numPhysCores;
            int numLogicCores;

            union
            {
                struct
                {
                    dword mmx : 1;
                    dword sse : 1;
                    dword sse2 : 1;
                    dword sse3 : 1;
                    dword ssse3 : 1;
                    dword sse41 : 1;
                    dword sse42 : 1;
                    dword avx : 1;
                    dword avx2 : 1;
                    dword avx512f : 1;

                    dword fma : 1;
                    dword fma3 : 1;
                    dword f16c : 1;
                    dword popcnt : 1;
                    dword bmi1 : 1;
                    dword bmi2 : 1;
                    dword lzcnt : 1;
                    dword crc32 : 1;
                    dword aesni : 1;
                    dword sha : 1;

                    dword neon : 1;
                    dword crc32_arm : 1;
                    dword crypto : 1;
                    dword dotprod : 1;
                    dword fp16 : 1;
                    dword sve : 1;
                    dword sve2 : 1;
                    dword sve2p1 : 1;
                    dword reserved : 4;
                } bits;
                dword mask;
            } cpuFeatures;
        };
        

        struct SSystemMemoryInfo
        {
            qword installedPhysical;
            qword usablePhysical;
            qword pageSize;
            qword allocationGranularity;

            dword moduleCount;
            dword numaNodeCount;

            bool hasSwap;
            bool hasECC;
            bool isLowRamDevice;
        };


        struct SPhysicalMemoryDeviceInfo
        {
            qword capacity;

            dword speedMHz;
            dword configuredSpeedMHz;
            word dataWidth;
            word totalWidth;
            byte rank;

            EMemoryType type;
            EMemoryFormFactor formFactor;

            bool hasECC;

            char manufacturer[64];
            char partNumber[64];
            char serialNumber[64];
            char deviceLocator[64];
            char bankLocator[64];
        };


        struct SSystemMemoryStatus
        {
            qword availablePhysical;
            qword freePhysical;
            qword cached;
            qword buffers;

            qword swapTotal;
            qword swapAvailable;

            qword committed;
            qword commitLimit;

            dword memoryLoadPercent;
            bool lowMemory;
        };


        struct SProcessMemoryStatus
        {
            uint64 virtualSize;
            uint64 peakVirtualSize;
                   
            uint64 residentSize;
            uint64 peakResidentSize;
                   
            uint64 privateSize;
            uint64 sharedSize;
                   
            uint64 swapSize;
            uint64 pageFaultCount;
        };


        constexpr dword GetRequiredCpuFeatures()
        {
            dword flags = CPU_FEATURE_NONE;

#if defined(SIMD_SSE42)
            flags |= CPU_FEATURE_SSE42;
#elif defined(SIMD_AVX)
            flags |= CPU_FEATURE_SSE42;
            flags |= CPU_FEATURE_AVX;
#elif defined(SIMD_AVX2)
            flags |= CPU_FEATURE_SSE42;
            flags |= CPU_FEATURE_AVX;
            flags |= CPU_FEATURE_AVX2;
#elif defined(SIMD_AVX512F)
            flags |= CPU_FEATURE_SSE42;
            flags |= CPU_FEATURE_AVX;
            flags |= CPU_FEATURE_AVX2;
            flags |= CPU_FEATURE_AVX512F;
#endif

#if defined(SIMD_EXT_FMA3)
            flags |= CPU_FEATURE_FMA3;
#endif

#if defined(SIMD_EXT_F16C)
            flags |= CPU_FEATURE_F16C;
#endif

#if defined(SIMD_EXT_POPCNT)
            flags |= CPU_FEATURE_POPCNT;
#endif

#if defined(SIMD_EXT_BMI1)
            flags |= CPU_FEATURE_BMI1;
#endif

#if defined(SIMD_EXT_BMI2)
            flags |= CPU_FEATURE_BMI2;
#endif

#if defined(SIMD_EXT_LZCNT)
            flags |= CPU_FEATURE_LZCNT;
#endif

#if defined(SIMD_EXT_CRC32)
            flags |= CPU_FEATURE_CRC32;
#endif

#if defined(SIMD_EXT_AES)
            flags |= CPU_FEATURE_AES;
#endif

#if defined(SIMD_EXT_SHA)
            flags |= CPU_FEATURE_SHA;
#endif

#if defined(SIMD_ARM_NEON)
            flags |= CPU_FEATURE_NEON;
#endif

#if defined(SIMD_ARM_EXT_CRC32)
            flags |= CPU_FEATURE_ARM_CRC32;
#endif

#if defined(SIMD_ARM_EXT_CRYPTO)
            flags |= CPU_FEATURE_ARM_CRYPTO;
#endif

#if defined(SIMD_ARM_EXT_DOTPROD)
            flags |= CPU_FEATURE_ARM_DOTPROD;
#endif

#if defined(SIMD_ARM_EXT_FP16)
            flags |= CPU_FEATURE_ARM_FP16;
#endif

#if defined(SIMD_ARM_EXT_SVE)
            flags |= CPU_FEATURE_ARM_SVE;
#endif

#if defined(SIMD_ARM_EXT_SVE2)
            flags |= CPU_FEATURE_ARM_SVE2;
#endif

#if defined(SIMD_ARM_EXT_SVE2P1)
            flags |= CPU_FEATURE_ARM_SVE2P1;
#endif

            return flags;
        }

        typedef uint64 Tick;

        typedef ERRCODE (*CreatePlatformManagerFn)(Common::IAllocator* allocator, const SNativeHandle* applicationInstance, IPlatformManager** outManager);

        PLATFORM_API ERRCODE CreatePlatformManager(Common::IAllocator* allocator, const SNativeHandle* applicationInstance, IPlatformManager** outManager);
	}
}



#ifdef __cplusplus
}
#endif
