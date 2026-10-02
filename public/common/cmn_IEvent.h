/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IEvent abstract class
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IEvent
        {
        private:
            dword m_id;

        public:
            IEvent(dword id) : m_id(id) {}
            virtual ~IEvent() = default;
            dword               GetEventID() const { return m_id; }
            virtual const char* GetEventType() const { return 0; }
            virtual dword       ListenerID() const { return ~0u; };
            virtual void        Reset() { m_id = ~0u; };
        };
    } // namespace Common
} // namespace krystallic