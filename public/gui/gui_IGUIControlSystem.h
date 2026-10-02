/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187 & @Ra192192
*
*  Description:
*
*  Date: 15.10.2025
*/


#pragma once


#include "gui_dll_types.h"


namespace krystallic
{
    namespace GUI
    {
        struct IGUIControlSystem
        {
            virtual ~IGUIControlSystem() = default;
            virtual ERRCODE OnCreate(GUI_CONTROL_INITIAL_DATA* data, void* subtypeDescStructPtr, void* destStruct) = 0;
            virtual ERRCODE OnDestroy(void* subtypeControlData) = 0;
            virtual ERRCODE OnEvent(const GUI_EVENT_DATA* evt, void* subtypeControlData) = 0;
            virtual ERRCODE OnDraw(IGUIRenderContext* rndContext,
                                   void*              subtypeControlData,
                                   GUI_RECT           world,
                                   dword              state,
                                   dword              style,
                                   dword              flags) = 0;
        };
    } // namespace GUI
} // namespace krystallic
