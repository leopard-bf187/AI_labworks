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


#pragma once


#include <cstddef>
#include <cstdlib>
#include <utility>
#include <new>


#include "common.h"
#include "platform.h"
#include "inherit.h"


using namespace krystallic;
using namespace krystallic::Common;
using namespace krystallic::Platform;


static constexpr dword SYSTEM_LOW_MEMORY_LOAD_PERCENT = 90;



struct CError : krystallic::Inherit<CError, IError>
{
	CError();
	virtual ~CError();

	virtual uint32 Delete();
	virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

	virtual ERRCODE GetErrorCode() const;
	virtual ERRCODE GetBackendErrorCode() const;
	virtual const char* GetErrorMessage();
	virtual const char* GetBackendState();
};


inline const char* PlatformErrorToString(ERRCODE code)
{
	static const char* arr[] =
	{
		"Execution success",
		"Invalid argument value passed in function: nullptr or invalid value",
		"Function not implemented and doesn\'t works",
		"Functional not supported",
		"Failed to allocate memory to create object or array",
		"Backend object already exists",
		"Backend object not found in storages or doesn\'t exists in system",
		"Backend value doesn\'t matches limitations",
		"Failed to create or initialize backend object",
		"Failed to allocate memory to create backend object or array",
	};

	if (code == 0)
		return nullptr;

	if (code >= PLATFORM_ERR_LAST_VALUE)
		return "Invalid error code";

	return arr[code];
}



