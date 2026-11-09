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
        struct IRHIFrameDebug : IBase
        {
            virtual ~IRHIFrameDebug() = default;

            virtual ERRCODE BeginFrame(const Stdlib::String& startEventName) = 0;
            virtual ERRCODE SetMarker(const Stdlib::String& eventName) = 0;
            virtual ERRCODE EndFrame() = 0;
            virtual ERRCODE SetObjectName(const SGuid& guid, IBase* rhiObject, const Stdlib::String& debugObjectName) = 0;

            virtual ERRCODE GetLastError(Common::IError** outErrorObject) = 0;

            inline static SGuid GUID() 
            { 
                return {0x146f837c, 0xdd8a, 0x4078, {0x8e, 0x98, 0xee, 0xcc, 0x41, 0xcc, 0x2d, 0x6f}}; 
            }
        };
    }
} // namespace krystallic