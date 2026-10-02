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


uint32 CPlatformProcessManager::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CPlatformProcessManager::_Destroy(this);
        return 0;
    }

    return ref;
};


uint32 CPlatformProcessManager::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return this->IncRef();
    }

    if (guid == IPlatformProcessManager::GUID())
    {
        *IFace = static_cast<IPlatformProcessManager*>(this);
        return this->IncRef();
    }

    return 0;
}

