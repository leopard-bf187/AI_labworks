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


#include "platform_helpers.h"


CError::CError() : Inherit(0)
{

}


CError::~CError()
{

}


uint32 CError::Delete()
{
	return 0;
}


uint32 CError::QueryIFace(const SGuid& guid, void** IFace)
{
	return 0;
}


ERRCODE CError::GetErrorCode() const
{
	return 0;
}


ERRCODE CError::GetBackendErrorCode() const
{
	return 0;
}


const char* CError::GetErrorMessage()
{
	return 0;
}


const char* CError::GetBackendState()
{
	return 0;
}

