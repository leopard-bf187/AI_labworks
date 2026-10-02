/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD
*
*  Description:
*
*  Date: 17.07.2025
*/


#include "dbg_classes.h"


CMiniDump::CMiniDump(IAllocator* allocator) : Inherit(allocator)
{
}


CMiniDump::~CMiniDump()
{

}


uint32 CMiniDump::Delete()
{
	uint refCount = DecRef();

	if (refCount == 0)
	{
		CMiniDump::_Destroy(this);
		return 0;
	}

	return refCount;
}


uint32 CMiniDump::QueryIFace(const SGuid& guid, void** IFace)
{
	if (!IFace)
		return 0;

	if (guid == IBase::GUID())
	{
		*IFace = static_cast<IBase*>(this);
		return this->IncRef();
	}

	if (guid == IMiniDump::GUID())
	{
		*IFace = static_cast<IMiniDump*>(this);
		return this->IncRef();
	}

	return 0;
}