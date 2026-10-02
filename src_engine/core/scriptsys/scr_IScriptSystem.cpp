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


IScriptContext* g_LuaCtx = 0;


int CScriptSystem::m_classMethod_mtbl = 0;
int CScriptSystem::m_classNativeMethod_mtbl = 0;


#if defined (KRYSTALLIC_OS_WINNT)
#include <windows.h>
#endif


CScriptSystem::CScriptSystem(IAllocator* allocator) : Inherit(allocator)
{
	m_lua = 0;
	m_classMethod_mtbl = 0;
	m_classNativeMethod_mtbl = 0;
}


CScriptSystem::~CScriptSystem()
{

}


uint32 CScriptSystem::QueryIFace(const SGuid& guid, void** IFace)
{
	if (!IFace)
		return 0;

	if (guid == IBase::GUID())
	{
		*IFace = static_cast<IBase*>(this);
		return this->IncRef();
	}

	if (guid == IScriptSystem::GUID())
	{
		*IFace = static_cast<IScriptSystem*>(this);
		return this->IncRef();
	}

	return 0;
}


uint32 CScriptSystem::Delete()
{
	uint refCount = DecRef();

	if (refCount == 0)
	{
		CScriptSystem::_Destroy(this);
		return 0;
	}

	return refCount;
}


ERRCODE CScriptSystem::Initialize(dword luaStdFlags, FileSystem::IFileSystem* fileSys)
{
	Stdlib::List<luaL_Reg> regList;
	Stdlib::List<luaL_Reg>::Node* n = 0;

//	if (!fileSys)
//		return 1;

	m_fileSys.Attach(fileSys);

	//m_scrCtx = CreateIScriptContext();
	//m_errBuff = CreateIScriptErrorBuffer();

	m_scrCtx.Attach(CScriptContext::_Create(m_allocator.Get()));
	m_errBuff.Attach(CScriptErrorBuffer::_Create(m_allocator.Get()));

	g_LuaCtx = m_scrCtx.Get();

	lua_State* lua = m_scrCtx.As<CScriptContext*>()->m_lua;
	m_lua = lua;

	if (luaStdFlags & LUA_STD_BASE)
		regList.PushBack({ LUA_GNAME, luaopen_base });

	if (luaStdFlags & LUA_STD_PACKAGE)
		regList.PushBack({ LUA_LOADLIBNAME, luaopen_package });

	if (luaStdFlags & LUA_STD_COROUTINE)
		regList.PushBack({ LUA_COLIBNAME, luaopen_coroutine });

	if (luaStdFlags & LUA_STD_TABLE)
		regList.PushBack({ LUA_TABLIBNAME, luaopen_table });

	if (luaStdFlags & LUA_STD_IO)
		regList.PushBack({ LUA_IOLIBNAME, luaopen_io });

	if (luaStdFlags & LUA_STD_OS)
		regList.PushBack({ LUA_OSLIBNAME, luaopen_os });

	if (luaStdFlags & LUA_STD_STRING)
		regList.PushBack({ LUA_STRLIBNAME, luaopen_string });

	if (luaStdFlags & LUA_STD_MATH)
		regList.PushBack({ LUA_MATHLIBNAME, luaopen_math });

	if (luaStdFlags & LUA_STD_UTF8)
		regList.PushBack({ LUA_UTF8LIBNAME, luaopen_utf8 });

	if (luaStdFlags & LUA_STD_DEBUG)
		regList.PushBack({ LUA_DBLIBNAME, luaopen_debug });

	if (luaStdFlags & LUA_STD_KRYSTALLIC)
	{
		regList.PushBack({ "krystallic", ext_KrystallicStd });
		ext_RegVector2(m_lua);
		ext_RegVector3(m_lua);
		ext_RegVector4(m_lua);
		ext_RegQuaternion(m_lua);
		ext_RegPlane(m_lua);
		ext_RegRGBA(m_lua);
	}

	for (n = regList.GetHead(); n; n = n->next)
	{
		luaL_Reg* r = &n->data;
		luaL_requiref(lua, r->name, r->func, 1);
		lua_pop(lua, 1);
	}

	this->InternalRegister();

	return 0;
}


ERRCODE CScriptSystem::GetDefaultScriptContext(IScriptContext** ctx)
{
	if (!ctx)
		return SCR_ERR_INVALID_ARGUMENT;

	*ctx = m_scrCtx.Get();
	m_scrCtx.InternalIncRef();

	return SCR_ERR_OK;
}


ERRCODE CScriptSystem::CreateAdditionalScriptContext(dword id, dword luaStdFlags, IScriptContext** ctx)
{
	/*
	lua_State* lua = 0;
	Stdlib::List<luaL_Reg> regList;
	Stdlib::List<luaL_Reg>::Node* n = 0;

	if (!ctx)
		return 0;

	//CScriptContext* context = (CScriptContext*)CreateIScriptContext();

	CScriptContext* ctx;

	context->m_id = id;
	context->m_errBuff = m_errBuff;
	context->IncRef();
	*ctx = context;

	lua = context->m_lua;

	if (luaStdFlags & LUA_STD_KRYSTALLIC)
	{
		regList.PushBack({ "krystallic", ext_KrystallicStd });
		regList.PushBack({ LUA_GNAME, ext_RegVector2 });
		regList.PushBack({ LUA_GNAME, ext_RegVector3 });
		regList.PushBack({ LUA_GNAME, ext_RegVector4 });
		//regList.PushBack({ LUA_GNAME, ext_RegQuaternion });
		//regList.PushBack({ LUA_GNAME, ext_RegPlane });
		//regList.PushBack({ LUA_GNAME, ext_RegRGB });
		//regList.PushBack({ LUA_GNAME, ext_RegRGBA });
	}

	if (luaStdFlags & LUA_STD_BASE)
		regList.PushBack({ LUA_GNAME, luaopen_base});

	if (luaStdFlags & LUA_STD_PACKAGE)
		regList.PushBack({ LUA_LOADLIBNAME, luaopen_package });

	if (luaStdFlags & LUA_STD_COROUTINE)
		regList.PushBack({ LUA_COLIBNAME, luaopen_coroutine });

	if (luaStdFlags & LUA_STD_TABLE)
		regList.PushBack({ LUA_TABLIBNAME, luaopen_table });

	if (luaStdFlags & LUA_STD_IO)
		regList.PushBack({ LUA_IOLIBNAME, luaopen_io });

	if (luaStdFlags & LUA_STD_OS)
		regList.PushBack({ LUA_OSLIBNAME, luaopen_os });

	if (luaStdFlags & LUA_STD_STRING)
		regList.PushBack({ LUA_STRLIBNAME, luaopen_string });

	if (luaStdFlags & LUA_STD_MATH)
		regList.PushBack({ LUA_MATHLIBNAME, luaopen_math });

	if (luaStdFlags & LUA_STD_UTF8)
		regList.PushBack({ LUA_UTF8LIBNAME, luaopen_utf8 });

	if (luaStdFlags & LUA_STD_DEBUG)
		regList.PushBack({ LUA_DBLIBNAME, luaopen_debug });

	for (n = regList.GetHead(); n; n = n->next)
	{
		luaL_Reg* r = &n->data;
		luaL_requiref(lua, r->name, r->func, 1);
		lua_pop(lua, 1);
	}

	return 0;

	*/

	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::GetErrorBuffer(IScriptErrorBuffer** errBuff)
{
	if (!errBuff)
		return SCR_ERR_INVALID_ARGUMENT;

	*errBuff = m_errBuff.Get();
	m_errBuff.InternalIncRef();

	return SCR_ERR_OK;
}


ERRCODE CScriptSystem::GetScriptCompiler(IScriptCompiler** compiler)
{
	if (!compiler)
		return SCR_ERR_INVALID_ARGUMENT;

	*compiler = m_compiler.Get();
	m_compiler.InternalIncRef();

	return SCR_ERR_OK;
}


ERRCODE CScriptSystem::Finalize()
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::LoadScriptFromFile(const char* file, const char* name, dword flags, IScript** outScript)
{
	if (!file || !outScript)
		return SCR_ERR_INVALID_ARGUMENT;

	RefCounted<IFileIO> f;
	ERRCODE err = SCR_ERR_OK;

	if (m_fileSys.Get())
		err = m_fileSys->LoadFile(file, f.ReleaseAndGetAddressOf());
	else
		err = StdFileOpen(file, "rb", f.ReleaseAndGetAddressOf(), m_allocator.Get());

	if (err != SCR_ERR_OK)
		return err;

	return LoadScriptFromFile(f.Get(), name, flags, outScript);
}


ERRCODE CScriptSystem::LoadScriptFromFile(FileSystem::IFileIO* file, const char* name, dword flags, IScript** outScript)
{
	if (!file || !outScript)
		return SCR_ERR_INVALID_ARGUMENT;

	RefCounted<CScript> scr;
	scr.Attach(CScript::_Create(m_allocator.Get()));

	if (!scr.Get())
		return SCR_ERR_OUT_OF_MEMORY;

	uint32 sz = static_cast<uint32>(file->GetFileSize());
	char* data = static_cast<char*>(scr->m_allocator->Alloc(sz + (sz % 4), alignof(int)));

	file->ReadData(data, sz);
	data[sz] = '\0';

	scr->m_buffer = data;
	scr->m_bufferLen = sz;
	scr->m_name = name;

	*outScript = static_cast<IScript*>(scr.Get());
	scr->IncRef();

	return SCR_ERR_OK;
}


ERRCODE CScriptSystem::LoadScriptFromBuffer(const char* buffer, uint buffSize, const char* name, dword flags, IScript** outScript)
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::LoadScriptFromBuffer(Common::IBuffer* buff, const char* name, dword flags, IScript** outScript)
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::SaveScriptToFile(const char* file, const char* buffer, uint buffSize, FileSystem::IFileIO** outFile)
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::SaveScriptToFile(const char* file, Common::IBuffer* buff, FileSystem::IFileIO** outFile)
{
	return SaveScriptToFile(file, (char*)buff->GetBufferPointer(), buff->GetBufferSize(), outFile);
}


ERRCODE CScriptSystem::SaveScriptToFile(const char* file, IScript* script, FileSystem::IFileIO** outFile)
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::SaveScriptToBuffer(IScript* script, Common::IBuffer** outBuffer)
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::Compile(const char* buffer, uint bufferSize, dword flags, Common::IBuffer** outBuffer)
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::Compile(IScript* script, dword flags, Common::IBuffer** outBuffer)
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::Compile(Common::IBuffer* scriptBuffer, dword flags, Common::IBuffer** outBuffer)
{
	return Compile((char*)scriptBuffer->GetBufferPointer(), scriptBuffer->GetBufferSize(), flags, outBuffer);
}


ERRCODE CScriptSystem::RegisterFunction(const char* fnName, EScriptArgumentType returnType, uint numArgs, EScriptArgumentType* args, GlobalScriptFn fn, const char* desc)
{
	if (!fnName || !fn)
		return 1;

	m_funcs[fnName] = fn;

	SScriptFunctionDesc& d = m_funcDesc[fnName];

	d.m_fnName = fnName;
	d.m_returnType = returnType;
	d.m_numArgs = numArgs;
	memcpy(d.m_args, args, sizeof(EScriptArgumentType) * numArgs);

	m_funcs[fnName] = fn;

	void* func = lua_newuserdata(m_lua, sizeof(GlobalScriptFn));
	func = reinterpret_cast<GlobalScriptFn*>(fn);
	
	lua_newtable(m_lua);
	lua_pushstring(m_lua, "__call");
	lua_pushcfunction(m_lua, lua_callGlobalFunction);
	lua_settable(m_lua, -3);

	lua_setmetatable(m_lua, -2);

	lua_pushglobaltable(m_lua);
	lua_pushstring(m_lua, fnName);
	lua_pushvalue(m_lua, -3);
	lua_settable(m_lua, -3);
	lua_pop(m_lua, 1);

	return SCR_ERR_OK;
}


ERRCODE CScriptSystem::CallScriptFunction(const char* fnName, CArgStack* args, int numResults)
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::GetFunctionDesc(const char* fnName, SScriptFunctionDesc* desc)
{
	if (!fnName || !desc)
		return 1;

	memcpy(desc, &m_funcDesc[fnName], sizeof(SScriptFunctionDesc));

	return SCR_ERR_OK;
}


ERRCODE CScriptSystem::Execute(IScript* script)
{
	CScript* scr = (CScript*)script;

	int i = luaL_dostring(m_lua, scr->m_buffer);

	if (i)
	{
		const char* error = lua_tostring(m_lua, -1);

#if defined (KRYSTALLIC_OS_WINNT)
		OutputDebugString("Lua error: ");
		OutputDebugString(error);
		OutputDebugString("\n");
#endif

	}

	return SCR_ERR_OK;
}


ERRCODE CScriptSystem::Execute(const char* buffer, uint bufferSize)
{
	luaL_dostring(m_lua, buffer);
	return SCR_ERR_OK;
}


ERRCODE CScriptSystem::Execute(Common::IBuffer* buffer)
{
	return Execute((char*)buffer->GetBufferPointer(), buffer->GetBufferSize());
}


ERRCODE CScriptSystem::ExecuteScripts()
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::AddScript(const char* name, IScript* script)
{
	if (!script)
		return SCR_ERR_INVALID_ARGUMENT;

	if (!script->GetName())
		script->SetName(name);

	//m_scripts[script->GetName()] = script;

	RefCounted<IScript> scriptlet(script);

	std::pair<const char*, RefCounted<CScript>> p(script->GetName(), scriptlet.As<CScript*>());

	m_scripts.insert(p);

	return SCR_ERR_OK;
}


ERRCODE CScriptSystem::GetScript(const char* name, IScript** script)
{
	if (!script || !name)
		return SCR_ERR_INVALID_ARGUMENT;

	if (!m_scripts[name])
		return SCR_ERR_NOT_EXISTS;

	std::map<const char*, RefCounted<CScript>>::iterator it = m_scripts.find(name);

	*script = it->second.Get();
	(*script)->IncRef();

	return SCR_ERR_OK;
}


ERRCODE CScriptSystem::RemoveScript(const char* name)
{
	if (!name)
		return SCR_ERR_INVALID_ARGUMENT;

	std::map<const char*, RefCounted<CScript>>::iterator it = m_scripts.find(name);

	IScript* scr = it->second.Get();
	scr->Delete();

	m_scripts.erase(name);

	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::ReloadScript()
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::ReloadAllScripts()
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


ERRCODE CScriptSystem::DumpStack(Common::IBuffer** outBuffer)
{
	return SCR_ERR_NOT_IMPLEMENTED;
}


void CScriptSystem::InternalRegister()
{
//	lua_pushglobaltable(m_lua);

//	lua_pop(m_lua, 1);

	lua_newtable(m_lua);
	lua_pushstring(m_lua, "__call");
	lua_pushcfunction(m_lua, lua_callClassMethod);
	lua_settable(m_lua, -3);
	CScriptSystem::m_classMethod_mtbl = luaL_ref(m_lua, LUA_REGISTRYINDEX);

	lua_newtable(m_lua);
	lua_pushstring(m_lua, "__call");
	lua_pushcfunction(m_lua, lua_callClassNativeMethod);
	lua_settable(m_lua, -3);
	CScriptSystem::m_classNativeMethod_mtbl = luaL_ref(m_lua, LUA_REGISTRYINDEX);
}