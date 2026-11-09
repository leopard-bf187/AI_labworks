/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IEventManager interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"
#include "stdlib/std_SharedPtr.h"

namespace krystallic
{
    namespace Common
    {
        struct IEventManager : IBase
        {
            virtual ~IEventManager() = default;

            virtual ERRCODE AddListener(dword listenerID, IEventListener* ev) = 0;
            virtual ERRCODE RemoveListener(dword listenerID, IEventListener* ev) = 0;
            virtual ERRCODE PushEvent(const Stdlib::SharedPtr<IEvent>& eventObj) = 0;
            virtual void    Wait(bool wait) = 0;
            virtual bool    IsWaiting() const = 0;
            virtual ERRCODE ProcessEvents() = 0;

            inline static SGuid GUID()
            {
                return {0x5d30a128, 0x1178, 0x42b6, {0xa7, 0xf9, 0x3e, 0x84, 0x83, 0xc3, 0x37, 0x06}};
            }
        };
    } // namespace Common
} // namespace krystallic