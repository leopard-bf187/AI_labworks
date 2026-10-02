/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: IError interface
*
*  Date: 13.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct IError : IBase
        {
            virtual ~IError() = default;

            virtual ERRCODE GetErrorCode() const = 0;
            virtual ERRCODE GetBackendErrorCode() const = 0;
            virtual const char* GetErrorMessage() = 0;
            virtual const char* GetBackendState() = 0;

            inline static SGuid GUID()
            {
                return {0x22bcefb2, 0x6232, 0x4275, {0xa3, 0x84, 0xef, 0xde, 0x3e, 0x39, 0x60, 0xb1}};
            }
        };
    } // namespace Common
} // namespace krystallic