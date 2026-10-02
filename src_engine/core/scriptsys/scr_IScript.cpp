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


CScript::CScript(IAllocator* allocator) : Inherit(allocator)
{
	m_buffer = 0;
	m_bufferLen = 0;
	m_name = 0;
}


CScript::~CScript()
{
//	safe_del_arr(m_buffer);
	m_allocator->Free(m_buffer);
}


uint32 CScript::QueryIFace(const SGuid& guid, void** IFace)
{
	if (!IFace)
		return 0;

	if (guid == IBase::GUID())
	{
		*IFace = static_cast<IBase*>(this);
		return this->IncRef();
	}

	if (guid == IScript::GUID())
	{
		*IFace = static_cast<IScript*>(this);
		return this->IncRef();
	}

	return 0;
}


uint32 CScript::Delete()
{
	uint refCount = DecRef();

	if (refCount == 0)
	{
		CScript::_Destroy(this);
		return 0;
	}

	return refCount;
}


void CScript::SetName(const char* name)
{
	m_name = name;
}


const char* CScript::GetName()
{
	return m_name;
}


