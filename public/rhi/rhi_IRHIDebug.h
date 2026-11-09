/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 
*
*  Description: 
*
*  Date: 13.06.2026
*/


#pragma once


#include "rhi_dll_types.h"


namespace krystallic
{
    namespace RenderHardwareInterface
    {
        struct IRHIDebug : IBase
        {
            virtual ~IRHIDebug() = default;

            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;
            virtual ERRCODE ReportLiveObjects() = 0;
            virtual ERRCODE InsertMessage(ERHIDebugMessageSeverity severity, const Stdlib::StringView& message) = 0;

            inline static SGuid GUID()
            { 
                return {0xcfb58b9d, 0x87f9, 0x44ce, {0x8b, 0x54, 0xc6, 0xa5, 0x71, 0x25, 0xe2, 0x92}}; 
            }
        };
    }
} // namespace krystallic