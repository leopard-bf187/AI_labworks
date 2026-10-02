/*
*  Copyright (c) BytesForge 2022-2026. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @leopard-bf187
*
*  Description:
*
*  Date: 11.07.2026
*/


#include "android_classes.h"


uint32 CPlatformThreadManager::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CPlatformThreadManager::_Destroy(this);
        return 0;
    }

    return ref;
};


uint32 CPlatformThreadManager::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return this->IncRef();
    }

    if (guid == IPlatformThreadManager::GUID())
    {
        *IFace = static_cast<IPlatformThreadManager*>(this);
        return this->IncRef();
    }

    return 0;
}