/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IEventListener interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IEventListener
        {
            virtual ~IEventListener() = default;

            virtual EEventListenerPriority GetListenerPriority() const = 0;
            virtual ERRCODE                OnEvent(IEvent* eventObj) = 0;
            virtual bool                   IsValidEventType(dword eventID) = 0;

            inline static SGuid GUID()
            {
                return {0xf44b5c2b, 0xa5d1, 0x4399, {0xa5, 0x3d, 0x0b, 0xb3, 0x4a, 0xc1, 0xcb, 0xfe}};
            }
        };
    } // namespace Common
} // namespace krystallic