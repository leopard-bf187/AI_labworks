/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @Ra192192
*
*  Description:
*
*  Date: 19.07.2026
*/


#pragma once


#include "platform_dll_types.h"


namespace krystallic
{
    namespace Platform
    {
        struct IPlatformWindow : IBase
        {
            virtual ~IPlatformWindow() = default;

            virtual ERRCODE Show() = 0;
            virtual ERRCODE Hide() = 0;

            virtual ERRCODE Minimize() = 0;
            virtual ERRCODE Maximize() = 0;
            virtual ERRCODE Restore() = 0;

            virtual ERRCODE SetTitle(const Stdlib::String& title) = 0;
            virtual ERRCODE SetSize(uint32 width, uint32 height) = 0;
            virtual ERRCODE SetPosition(int32 x, int32 y) = 0;

            virtual void SetCallback(IPlatformWindowCallback* callback) = 0;
            virtual IPlatformWindowCallback* GetCallback() = 0;

            virtual ERRCODE GetDesc(SPlatformWindowDesc* outDesc) const = 0;

            virtual uint32 GetWidth() const = 0;
            virtual uint32 GetHeight() const = 0;

            /*
            virtual ERRCODE GetClientRect(Rect* outRect);
            virtual ERRCODE GetWindowRect(Rect* outRect);
            */

            virtual bool IsVisible() const = 0;
            virtual bool IsActive() const = 0;

            virtual ERRCODE GetNativeHandle(SNativeHandle* outHandle) const = 0;

            inline static SGuid GUID()
            {
                return { 0xdc856905, 0x5674, 0x411f, { 0x88, 0xe9, 0x1c, 0xa9, 0xc7, 0x53, 0x17, 0x5a } };
            }
        };

        struct IPlatformWindowCallback
        {
            virtual void OnCreate(const SNativeHandle* window) {};

            virtual void OnCreateEx(const SNativeHandle* window) {};

            virtual void OnHittest(const SNativeHandle* window) {};

            virtual void OnDestroy(const SNativeHandle* window) {};

            virtual void OnShow(const SNativeHandle* window) {};
            virtual void OnHide(const SNativeHandle* window) {};

            virtual void OnActivate(const SNativeHandle* window, bool active) {};
            virtual void OnSetFocus(const SNativeHandle* window) {};
            virtual void OnKillFocus(const SNativeHandle * window) {};

            virtual void OnSize(const SNativeHandle* window, uint32 width, uint32 height) {};
            virtual void OnSizing(const SNativeHandle* window, uint32 width, uint32 height) {};
            virtual void OnMinimize(const SNativeHandle* window) {};
            virtual void OnMaximize(const SNativeHandle* window) {};

            virtual void OnPaint(const SNativeHandle* window) {};

            virtual void OnInput(const SNativeHandle* window) {};
            virtual void OnChar(const SNativeHandle* window) {};
        };
    }
}