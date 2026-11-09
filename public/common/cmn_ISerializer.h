/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: ISerizalizer interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"

namespace krystallic
{
    namespace Common
    {
        struct ISerializer
        {
            virtual ESerializerMode GetMode() const = 0;

            virtual bool IsReading() const = 0;
            virtual bool IsWriting() const = 0;

            virtual ERRCODE BeginObject(const char* name) = 0;
            virtual ERRCODE EndObject() = 0;

            virtual ERRCODE BeginArray(const char* name, uint32* count) = 0;
            virtual ERRCODE EndArray() = 0;

            virtual ERRCODE BeginObjectArray(const char* name, uint32* count) = 0;
            virtual ERRCODE EndObjectArray() = 0;

            virtual ERRCODE Value(const char* name, int32* value) = 0;
            virtual ERRCODE Value(const char* name, uint32* value) = 0;
            virtual ERRCODE Value(const char* name, int64* value) = 0;
            virtual ERRCODE Value(const char* name, uint64* value) = 0;
            virtual ERRCODE Value(const char* name, float* value) = 0;
            virtual ERRCODE Value(const char* name, double* value) = 0;
            virtual ERRCODE Value(const char* name, bool* value) = 0;

            virtual ERRCODE String(const char* name, char* buffer, uint32 bufferSize) = 0;

            virtual ERRCODE Bytes(const char* name, void* data, uint64 size) = 0;
        };
    } // namespace Common
} // namespace krystallic