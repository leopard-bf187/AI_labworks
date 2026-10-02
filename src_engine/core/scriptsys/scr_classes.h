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


#pragma once


#include "scripting.h"
#include "stdlib_dll.h"
#include "common.h"
#include "filesys.h"

#include <map>

using namespace krystallic;
using namespace krystallic::Runtime;
using namespace krystallic::FileSystem;
using namespace krystallic::SIMDMath;
using namespace krystallic::Common;


#include "scr_extlib.h"


#include "inherit.h"


struct CScriptContext;
struct CScriptErrorBuffer;
struct CScriptCompiler;
struct CScript;


struct CScriptSystem : Inherit<CScriptSystem, IScriptSystem>
{
	lua_State* m_lua;
	RefCounted<IFileSystem> m_fileSys;
	RefCounted<CScriptContext> m_scrCtx;
	RefCounted<CScriptErrorBuffer> m_errBuff;
	RefCounted<CScriptCompiler> m_compiler;
	std::map<const char*, RefCounted<CScript>> m_scripts;
	std::map<const char*, GlobalScriptFn> m_funcs;
	std::map<const char*, SScriptFunctionDesc> m_funcDesc;
	static int m_classMethod_mtbl;
	static int m_classNativeMethod_mtbl;

	CScriptSystem(IAllocator* allocator);
	virtual ~CScriptSystem();

	virtual uint32 Delete();
	virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

	virtual ERRCODE Initialize(dword luaStdFlags, FileSystem::IFileSystem* fileSys);
	virtual ERRCODE GetDefaultScriptContext(IScriptContext** ctx);
	virtual ERRCODE CreateAdditionalScriptContext(dword id, dword luaStdFlags, IScriptContext** ctx);
	virtual ERRCODE GetErrorBuffer(IScriptErrorBuffer** errBuff);
	virtual ERRCODE GetScriptCompiler(IScriptCompiler** compiler);
	virtual ERRCODE Finalize();

	virtual ERRCODE LoadScriptFromFile(const char* file, const char* name, dword flags, IScript** outScript);
	virtual ERRCODE LoadScriptFromFile(FileSystem::IFileIO* file, const char* name, dword flags, IScript** outScript);
	virtual ERRCODE LoadScriptFromBuffer(const char* buffer, uint buffSize, const char* name, dword flags, IScript** outScript);
	virtual ERRCODE LoadScriptFromBuffer(Common::IBuffer* buff, const char* name, dword flags, IScript** outScript);

	virtual ERRCODE SaveScriptToFile(const char* file, const char* buffer, uint buffSize, FileSystem::IFileIO** outFile);
	virtual ERRCODE SaveScriptToFile(const char* file, Common::IBuffer* buff, FileSystem::IFileIO** outFile);
	virtual ERRCODE SaveScriptToFile(const char* file, IScript* script, FileSystem::IFileIO** outFile);
	virtual ERRCODE SaveScriptToBuffer(IScript* script, Common::IBuffer** outBuffer);

	virtual ERRCODE Compile(const char* buffer, uint bufferSize, dword flags, Common::IBuffer** outBuffer);
	virtual ERRCODE Compile(IScript* script, dword flags, Common::IBuffer** outBuffer);
	virtual ERRCODE Compile(Common::IBuffer* scriptBuffer, dword flags, Common::IBuffer** outBuffer);

	virtual ERRCODE RegisterFunction(const char* fnName, EScriptArgumentType returnType, uint numArgs, EScriptArgumentType* args, GlobalScriptFn fn, const char* desc);
	virtual ERRCODE CallScriptFunction(const char* fnName, CArgStack* args, int numResults);
	virtual ERRCODE GetFunctionDesc(const char* fnName, SScriptFunctionDesc* desc);

	virtual ERRCODE Execute(IScript* script);
	virtual ERRCODE Execute(const char* buffer, uint bufferSize);
	virtual ERRCODE Execute(Common::IBuffer* buffer);

	virtual ERRCODE ExecuteScripts();
	virtual ERRCODE AddScript(const char* name, IScript* script);
	virtual ERRCODE GetScript(const char* name, IScript** script);
	virtual ERRCODE RemoveScript(const char* name);
	virtual ERRCODE ReloadScript();
	virtual ERRCODE ReloadAllScripts();

	virtual ERRCODE DumpStack(Common::IBuffer** outBuffer);

	inline void InternalRegister();
};


struct CScriptContext : Inherit<CScriptContext, IScriptContext>
{
	dword m_id;
	lua_State* m_lua;
	int m_stackStart;
	int m_numInputs;
	int m_numOutputs;
	RefCounted<CScriptErrorBuffer> m_errBuff;

	CScriptContext(IAllocator* allocator);
	virtual ~CScriptContext();

	virtual uint32 Delete();
	virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

	virtual void* GetContextState();
	virtual void ResetContext(int stackStart, int numInputs, int numOutputs);

	virtual void PushBool(bool v);
	virtual void PushInt(int v);
	virtual void PushFloat(float v);
	virtual void PushString(char* v);
	virtual void PushVector2(const CVector2& v);
	virtual void PushVector3(const CVector3& v);
	virtual void PushVector4(const CVector4& v);
	virtual void PushQuaternion(const CQuaternion& v);
	virtual void PushColorRGBA(const CColorRGBA& v);
	virtual void PushPlane(const CPlane& v);

	virtual bool		 AsBool(int i);
	virtual int			 AsInt(int i);
	virtual float		 AsFloat(int i);
	virtual char*		 AsString(int i);
	virtual CVector2*	 AsVector2(int i);
	virtual CVector3*	 AsVector3(int i);
	virtual CVector4*	 AsVector4(int i);
	virtual CQuaternion* AsQuaternion(int i);
	virtual CColorRGBA*	 AsColorRGBA(int i);
	virtual CPlane*		 AsPlane(int i);

	virtual int GetArgCount();
	virtual int GetTotalArgCount();
	virtual int ValidateArg(int i);
};


struct CScriptCompiler : Inherit<CScriptCompiler, IScriptCompiler>
{
	CScriptCompiler(IAllocator* allocator);
	virtual ~CScriptCompiler();

	virtual uint32 Delete();
	virtual uint32 QueryIFace(const SGuid& guid, void** IFace);
};


struct CScriptErrorBuffer : Inherit<CScriptErrorBuffer, IScriptErrorBuffer>
{
	CScriptErrorBuffer(IAllocator* allocator);
	virtual ~CScriptErrorBuffer();

	virtual uint32 Delete();
	virtual uint32 QueryIFace(const SGuid& guid, void** IFace);
};


struct CScript : Inherit<CScript, IScript>
{
	char* m_buffer;
	uint m_bufferLen;
	const char* m_name;

	CScript(IAllocator* allocator);
	virtual ~CScript();

	virtual uint32 Delete();
	virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

	virtual void SetName(const char* name);
	virtual const char* GetName();
};

