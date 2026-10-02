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


CScriptCompiler::CScriptCompiler(IAllocator* allocator) :
	Inherit(allocator)
{

}


CScriptCompiler::~CScriptCompiler()
{

}


uint32 CScriptCompiler::Delete()
{
	uint refCount = DecRef();

	if (refCount == 0)
	{
		CScriptCompiler::_Destroy(this);
		return 0;
	}

	return refCount;
}


uint32 CScriptCompiler::QueryIFace(const SGuid& guid, void** IFace)
{
	if (!IFace)
		return 0;

	if (guid == IBase::GUID())
	{
		*IFace = static_cast<IBase*>(this);
		return this->IncRef();
	}

	if (guid == IScriptCompiler::GUID())
	{
		*IFace = static_cast<IScriptCompiler*>(this);
		return this->IncRef();
	}

	return 0;
}