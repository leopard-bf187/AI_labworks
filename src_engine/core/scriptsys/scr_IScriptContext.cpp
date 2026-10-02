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


static void* luaAllocatorKrystallic(void* ud, void* ptr, size_t osize, size_t nsize)
{
	//if (nsize == 0)
	//{
	//	allocator->Free(ptr);
	//	return nullptr;
	//}

	//if (ptr == nullptr)
	//	return allocator->Alloc(nsize);

	//void* newPtr = allocator->Alloc(nsize);

	//if (!newPtr)
	//	return nullptr;

	//memcpy(newPtr, ptr, (osize < nsize) ? osize : nsize);

	//allocator->Free(ptr);

	//return newPtr;

	IAllocator* allocator = static_cast<IAllocator*>(ud);

	if (nsize == 0) 
	{
		allocator->Free(ptr);
		return nullptr;
	}

	return allocator->Realloc(ptr, nsize);
}


CScriptContext::CScriptContext(IAllocator* allocator) : Inherit(allocator)
{
	m_lua = lua_newstate(luaAllocatorKrystallic, allocator);
	m_stackStart = 0;
	m_numInputs = 0;
	m_numOutputs = 0;
	m_id = 0;
}


CScriptContext::~CScriptContext()
{
	lua_close(m_lua);
}


uint32 CScriptContext::QueryIFace(const SGuid& guid, void** IFace)
{
	if (!IFace)
		return 0;

	if (guid == IBase::GUID())
	{
		*IFace = static_cast<IBase*>(this);
		return this->IncRef();
	}

	if (guid == IScriptContext::GUID())
	{
		*IFace = static_cast<IScriptContext*>(this);
		return this->IncRef();
	}

	return 0;
}


uint32 CScriptContext::Delete()
{
	uint refCount = DecRef();

	if (refCount == 0)
	{
		CScriptContext::_Destroy(this);
		return 0;
	}

	return refCount;
}


void* CScriptContext::GetContextState()
{
	return m_lua;
}


void CScriptContext::ResetContext(int stackStart, int numInputs, int numOutputs)
{
	m_stackStart = stackStart;
	m_numInputs = numInputs;
	m_numOutputs = numOutputs;
}


void CScriptContext::PushBool(bool v)
{
	lua_pushboolean(m_lua, v);
	m_numOutputs++;
}


void CScriptContext::PushInt(int v)
{
	lua_pushinteger(m_lua, v);
	m_numOutputs++;
}


void CScriptContext::PushFloat(float v)
{
	lua_pushnumber(m_lua, v);
	m_numOutputs++;
}


void CScriptContext::PushString(char* v)
{
	lua_pushstring(m_lua, v);
	m_numOutputs++;
}


void CScriptContext::PushVector2(const CVector2& v)
{
	*ext_Vec2Create(m_lua) = v;
	m_numOutputs++;
}


void CScriptContext::PushVector3(const CVector3& v)
{
	*ext_Vec3Create(m_lua) = v;
	m_numOutputs++;
}


void CScriptContext::PushVector4(const CVector4& v)
{
	*ext_Vec4Create(m_lua) = v;
	m_numOutputs++;
}


void CScriptContext::PushQuaternion(const CQuaternion& v)
{
	m_numOutputs++;
}


void CScriptContext::PushColorRGBA(const CColorRGBA& v)
{
	m_numOutputs++;
}


void CScriptContext::PushPlane(const CPlane& v)
{
	m_numOutputs++;
}


bool CScriptContext::AsBool(int i)
{
	int stack = this->ValidateArg(i);
	int type = lua_type(m_lua, stack);

	if (!type)
		return 0;

	if (type == LUA_TBOOLEAN)
		return lua_toboolean(m_lua, stack) != 0;

	if (type == LUA_TNUMBER)
		return (int)lua_tonumber(m_lua, stack) != 0;

	return 1;
}


int	CScriptContext::AsInt(int i)
{
	int stack = this->ValidateArg(i);
	int type = lua_type(m_lua, stack);

	if (type == LUA_TNUMBER)
		return (int)lua_tonumber(m_lua, stack);

	if (type == LUA_TSTRING)
		return atoi(lua_tostring(m_lua, stack));

	luaL_checktype(m_lua, stack, LUA_TNUMBER);
	return 0;
}


float CScriptContext::AsFloat(int i)
{
	int stack = this->ValidateArg(i);
	int type = lua_type(m_lua, stack);

	if (type == LUA_TNUMBER)
		return lua_tonumber(m_lua, stack);

	if (type == LUA_TSTRING)
		return atof(lua_tostring(m_lua, stack));

	luaL_checktype(m_lua, stack, LUA_TNUMBER);
	return 0.0f;
}


char* CScriptContext::AsString(int i)
{
	int stack = this->ValidateArg(i);
//	int type = lua_type(m_lua, stack);

	luaL_checktype(m_lua, stack, LUA_TSTRING);
	return (char*)lua_tostring(m_lua, stack);
}


CVector2* CScriptContext::AsVector2(int i)
{
	int stack = ValidateArg(i);

//	assert(ext_CheckType(m_lua, i, LUA_EXT_TVECTOR2));

	return (CVector2*)luaL_testudata(m_lua, stack, "CVector2_mtbl");
}
			   
			   
CVector3* CScriptContext::AsVector3(int i)
{
	int stack = ValidateArg(i);

//	assert(ext_CheckType(m_lua, i, LUA_EXT_TVECTOR3));

	return (CVector3*)luaL_testudata(m_lua, stack, "CVector3_mtbl");
}
			   
			   
CVector4* CScriptContext::AsVector4(int i)
{
	int stack = ValidateArg(i);

//	assert(ext_CheckType(m_lua, i, LUA_EXT_TVECTOR4));

	return (CVector4*)luaL_testudata(m_lua, stack, "CVector4_mtbl");
}


CQuaternion* CScriptContext::AsQuaternion(int i)
{
	int stack = ValidateArg(i);

//	assert(ext_CheckType(m_lua, i, LUA_EXT_TQUATERNION));

	return (CQuaternion*)lua_touserdata(m_lua, stack);
}


CColorRGBA* CScriptContext::AsColorRGBA(int i)
{
	int stack = ValidateArg(i);

//	assert(ext_CheckType(m_lua, i, LUA_EXT_TRGBA));

	return (CColorRGBA*)lua_touserdata(m_lua, stack);
}


CPlane* CScriptContext::AsPlane(int i)
{
	int stack = ValidateArg(i);

//	assert(ext_CheckType(m_lua, i, LUA_EXT_TPLANE));

	return (CPlane*)lua_touserdata(m_lua, stack);
}


int CScriptContext::GetArgCount()
{
	int top = lua_gettop(m_lua);
	int type = lua_type(m_lua, 1);

	if (type == LUA_TUSERDATA || type == LUA_TTABLE)
		return top - 1;
	else
		return top;
}


int CScriptContext::GetTotalArgCount()
{
	return lua_gettop(m_lua);
}


int CScriptContext::ValidateArg(int i)
{
	if (i < 0)
	{
		lua_pushstring(m_lua, "Not enough arguments");
		lua_error(m_lua);
	}
	return i + this->m_stackStart;
}


