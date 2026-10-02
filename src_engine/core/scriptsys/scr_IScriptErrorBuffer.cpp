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


#include "scr_classes.h"


CScriptErrorBuffer::CScriptErrorBuffer(IAllocator* allocator) :
	Inherit(allocator)
{

}


CScriptErrorBuffer::~CScriptErrorBuffer()
{

}


uint32 CScriptErrorBuffer::Delete()
{
	uint refCount = DecRef();

	if (refCount == 0)
	{
		CScriptErrorBuffer::_Destroy(this);
		return 0;
	}

	return refCount;
}


uint32 CScriptErrorBuffer::QueryIFace(const SGuid& guid, void** IFace)
{
	if (!IFace)
		return 0;

	if (guid == IBase::GUID())
	{
		*IFace = static_cast<IBase*>(this);
		return this->IncRef();
	}

	if (guid == IScriptErrorBuffer::GUID())
	{
		*IFace = static_cast<IScriptErrorBuffer*>(this);
		return this->IncRef();
	}

	return 0;
}