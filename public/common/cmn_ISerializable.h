/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description: ISerializable interface
*
*  Date: 02.06.2026
*/

#pragma once

#include "cmn_types.h"
#include "cmn_ISerializer.h"

namespace krystallic
{
    namespace Common
    {
        struct ISerializable
        {
            virtual ERRCODE Serialize(ISerializer* serializer) = 0;
            virtual ERRCODE Deserialize(ISerializer* serializer)
            {
                if (serializer->GetMode() != SERIALIZER_MODE_READ)
                    return SER_ERR_INVALID_SERIALIZER_MODE;

                return Serialize(serializer);
            };
        };
    } // namespace Common
} // namespace krystallic