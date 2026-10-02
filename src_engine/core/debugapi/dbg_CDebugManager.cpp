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


CDebugManager::CDebugManager(IAllocator* allocator) : Inherit(allocator)
{
}


CDebugManager::~CDebugManager()
{

}


ERRCODE CDebugManager::Delete()
{
	uint refCount = DecRef();

	if (refCount == 0)
	{
		CDebugManager::_Destroy(this);
		return 0;
	}

	return refCount;
}


ERRCODE CDebugManager::QueryIFace(const SGuid& guid, void** IFace)
{
	if (!IFace)
		return 0;

	if (guid == IBase::GUID())
	{
		*IFace = static_cast<IBase*>(this);
		return this->IncRef();
	}

	if (guid == IDebugManager::GUID())
	{
		*IFace = static_cast<IDebugManager*>(this);
		return this->IncRef();
	}

	return 0;
}


ERRCODE CDebugManager::CreateLog(const char* file, ILog** logFile)
{
	if (!file || !logFile)
		return DBG_ERR_INVALID_ARGUMENT;

	RefCounted<CLog> log;
	log.Attach(CLog::_Create(m_allocator.Get(), file));

	if (!log.Get())
		return DBG_ERR_OUT_OF_MEMORY;

	*logFile = static_cast<ILog*>(log.Get());
	log->IncRef();

	return 0;
}