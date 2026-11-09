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

namespace krystallic
{
    namespace Platform
    {
        enum EPlatformType
        {
            PLATFORM_WINDOWS,
            PLATFORM_LINUX,
            PLATFORM_ANDROID,
        };

        enum EPlatformError : ERRCODE
        {
            PLATFORM_ERR_OK,
            PLATFORM_ERR_INVALID_ARGUMENT,
            PLATFORM_ERR_NOT_IMPLEMENTED,
            PLATFORM_ERR_NOT_SUPPORTED,
            PLATFORM_ERR_OUT_OF_MEMORY,
            PLATFORM_ERR_OUT_OF_RANGE,
            PLATFORM_ERR_ALREADY_EXISTS,
            PLATFORM_ERR_NOT_FOUND,
            PLATFORM_ERR_OUT_OF_BOUNDS,
            PLATFORM_ERR_BACKEND_FAILED_TO_CREATE, 
            PLATFORM_ERR_BACKEND_ERROR,
            PLATFORM_ERR_BACKEND_OUT_OF_MEMORY,
            PLATFORM_ERR_BACKEND_SYSTEM_CALL_FAILED,
            PLATFORM_ERR_LAST_VALUE,
        };

        enum ENativeHandleType : uint32
        {
            NATIVE_HANDLE_APP_INSTANCE,
            NATIVE_HANDLE_WINDOW,
            NATIVE_HANDLE_ICON,
            NATIVE_HANDLE_ICON_SMALL,
            NATIVE_HANDLE_CURSOR,
            NATIVE_HANDLE_THREAD,
            NATIVE_HANDLE_PROCESS,
            NATIVE_HANDLE_VIRTUAL_MEMORY,
            NATIVE_HANDLE_SHARED_MEMORY,
            NATIVE_HANDLE_RAW_DEVICE,
            NATIVE_HANDLE_MONITOR,
            NATIVE_HANDLE_MOUSE,
            NATIVE_HANDLE_KEYBOARD,
            NATIVE_HANDLE_GAMEPAD,
        };

        enum ECpuFeature : dword
        {
            CPU_FEATURE_NONE = 0ull,

            // x86/x64 instruction levels
            CPU_FEATURE_MMX     = (1u << 0),
            CPU_FEATURE_SSE     = (1u << 1),
            CPU_FEATURE_SSE2    = (1u << 2),
            CPU_FEATURE_SSE3    = (1u << 3),
            CPU_FEATURE_SSSE3   = (1u << 4),
            CPU_FEATURE_SSE41   = (1u << 5),
            CPU_FEATURE_SSE42   = (1u << 6),
            CPU_FEATURE_AVX     = (1u << 7),
            CPU_FEATURE_AVX2    = (1u << 8),
            CPU_FEATURE_AVX512F = (1u << 9),

            // x86/x64 extensions
            CPU_FEATURE_FMA     = (1u << 10),
            CPU_FEATURE_FMA3    = (1u << 11),
            CPU_FEATURE_F16C    = (1u << 12),
            CPU_FEATURE_POPCNT  = (1u << 13),
            CPU_FEATURE_BMI1    = (1u << 14),
            CPU_FEATURE_BMI2    = (1u << 15),
            CPU_FEATURE_LZCNT   = (1u << 16),
            CPU_FEATURE_CRC32   = (1u << 17),
            CPU_FEATURE_AES     = (1u << 18),
            CPU_FEATURE_SHA     = (1u << 19),

            // ARM instruction levels
            CPU_FEATURE_NEON    = (1u << 20),

            // ARM extensions
            CPU_FEATURE_ARM_CRC32   = (1u << 21),
            CPU_FEATURE_ARM_CRYPTO  = (1u << 22),
            CPU_FEATURE_ARM_DOTPROD = (1u << 23),
            CPU_FEATURE_ARM_FP16    = (1u << 24),
            CPU_FEATURE_ARM_SVE     = (1u << 25),
            CPU_FEATURE_ARM_SVE2    = (1u << 26),
            CPU_FEATURE_ARM_SVE2P1  = (1u << 27)
        };

        enum EMemoryType : byte
        {
            MEM_TYPE_UNSPECIFIED          = 0x00,
            MEM_TYPE_OTHER                = 0x01,
            MEM_TYPE_UNKNOWN              = 0x02,
            MEM_TYPE_DRAM                 = 0x03,
            MEM_TYPE_EDRAM                = 0x04,
            MEM_TYPE_VRAM                 = 0x05,
            MEM_TYPE_SRAM                 = 0x06,
            MEM_TYPE_RAM                  = 0x07,
            MEM_TYPE_ROM                  = 0x08,
            MEM_TYPE_FLASH                = 0x09,
            MEM_TYPE_EEPROM               = 0x0A,
            MEM_TYPE_FEPROM               = 0x0B,
            MEM_TYPE_EPROM                = 0x0C,
            MEM_TYPE_CDRAM                = 0x0D,
            MEM_TYPE_3DRAM                = 0x0E,
            MEM_TYPE_SDRAM                = 0x0F,
            MEM_TYPE_SGRAM                = 0x10,
            MEM_TYPE_RDRAM                = 0x11,
            MEM_TYPE_DDR                  = 0x12,
            MEM_TYPE_DDR2                 = 0x13,
            MEM_TYPE_DDR2_FB_DIMM         = 0x14,
            MEM_TYPE_DDR3                 = 0x18,
            MEM_TYPE_FBD2                 = 0x19,
            MEM_TYPE_DDR4                 = 0x1A,
            MEM_TYPE_LPDDR                = 0x1B,
            MEM_TYPE_LPDDR2               = 0x1C,
            MEM_TYPE_LPDDR3               = 0x1D,
            MEM_TYPE_LPDDR4               = 0x1E,
            MEM_TYPE_LOGICAL_NON_VOLATILE = 0x1F,
            MEM_TYPE_HBM                  = 0x20,
            MEM_TYPE_HBM2                 = 0x21,
            MEM_TYPE_DDR5                 = 0x22,
            MEM_TYPE_LPDDR5               = 0x23,
            MEM_TYPE_HBM3                 = 0x24
        };

        enum EMemoryFormFactor : byte
        {
            MEM_FORM_FACTOR_UNSPECIFIED      = 0x00,
            MEM_FORM_FACTOR_OTHER            = 0x01,
            MEM_FORM_FACTOR_UNKNOWN          = 0x02,
            MEM_FORM_FACTOR_SIMM             = 0x03,
            MEM_FORM_FACTOR_SIP              = 0x04,
            MEM_FORM_FACTOR_CHIP             = 0x05,
            MEM_FORM_FACTOR_DIP              = 0x06,
            MEM_FORM_FACTOR_ZIP              = 0x07,
            MEM_FORM_FACTOR_PROPRIETARY_CARD = 0x08,
            MEM_FORM_FACTOR_DIMM             = 0x09,
            MEM_FORM_FACTOR_TSOP             = 0x0A,
            MEM_FORM_FACTOR_ROW_OF_CHIPS     = 0x0B,
            MEM_FORM_FACTOR_RIMM             = 0x0C,
            MEM_FORM_FACTOR_SODIMM           = 0x0D,
            MEM_FORM_FACTOR_SRIMM            = 0x0E,
            MEM_FORM_FACTOR_FB_DIMM          = 0x0F,
            MEM_FORM_FACTOR_DIE              = 0x10
        };
    } 
}
