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
        struct IGUIRenderStrategy
        {
            virtual ~IGUIRenderStrategy() = default;

            virtual ERRCODE
            OnResourceCreate(GUI_RENDERABLE_RESOURCE_TYPE type, void* creationStructurePtr, void** outHandle) = 0;
            virtual ERRCODE OnResourceDestroy(GUI_RENDERABLE_RESOURCE_TYPE type, void* handle) = 0;

            virtual ERRCODE OnResourceMap(GUI_RENDERABLE_RESOURCE_TYPE type,
                                          void*                        resourceHandle,
                                          uint                         subresource,
                                          GUI_MAPPED_RESOURCE*         outMappedResource) = 0;
            virtual ERRCODE
            OnResourceUnmap(GUI_RENDERABLE_RESOURCE_TYPE type, void* resourceHandle, uint subresource) = 0;

            virtual ERRCODE OnResourceUpdateRegion(GUI_RENDERABLE_RESOURCE_TYPE type,
                                                   void*                        resourceHandle,
                                                   const GUI_RECT&              dstRectPixels,
                                                   const void*                  srcData,
                                                   uint                         srcRowPitch)
            {
                return 0;
            }

            virtual ERRCODE OnRenderCreate() = 0;
            virtual ERRCODE OnRenderDestroy() = 0;
            virtual ERRCODE OnRenderReset() = 0;
            virtual ERRCODE OnRenderUpdate() = 0;
            virtual ERRCODE OnRenderResize(GUI_VIEWPORT vp) = 0;

            virtual ERRCODE OnRenderBegin(InLayoutHandle inputLayout, VBuffHandle vbuff, IBuffHandle ibuff) = 0;
            virtual ERRCODE OnRenderEnd() = 0;

            virtual ERRCODE PreRender(GUI_DRAW_COMMAND* command) = 0;
            virtual ERRCODE OnRender(GUI_DRAW_COMMAND* command) = 0;
            virtual ERRCODE PostRender() = 0;

            inline static SGuid& GUID() {}
        };
    } // namespace GUI
} // namespace krystallic
