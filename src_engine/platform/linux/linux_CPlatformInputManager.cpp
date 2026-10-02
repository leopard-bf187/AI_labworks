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


#include "linux_classes.h"


uint32 CPlatformInputManager::Delete()
{
    uint32 ref = DecRef();

    if (ref == 0)
    {
        CPlatformInputManager::_Destroy(this);
        return 0;
    }

    return ref;
}


uint32 CPlatformInputManager::QueryIFace(const SGuid& guid, void** IFace)
{
    if (!IFace)
        return 0;

    if (guid == IBase::GUID())
    {
        *IFace = static_cast<IBase*>(this);
        return this->IncRef();
    }

    if (guid == IPlatformInputManager::GUID())
    {
        *IFace = static_cast<IPlatformInputManager*>(this);
        return this->IncRef();
    }

    return 0;
}

