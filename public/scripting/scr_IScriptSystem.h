/*
*  Copyright (c) BytesForge 2022-2025. All rights reserved.
*
*  This file is part of the "Krystallic Engine" project and is considered confidential.
*
*  Authors: @LeoParD
*
*  Description:
*
*  Date: 10.07.2025
*/


#pragma once


#include "scr_lib_types.h"


namespace krystallic
{
	namespace Runtime
	{
		struct IScriptSystem : public IBase
		{
			virtual ~IScriptSystem() = default;

			virtual ERRCODE Initialize(dword luaStdFlags, FileSystem::IFileSystem* fileSys) = 0;
			virtual ERRCODE GetDefaultScriptContext(IScriptContext** ctx) = 0;
			//virtual ERRCODE CreateAdditionalScriptContext(dword contextID, dword luaStdFlags, IScriptContext** ctx) = 0;
			virtual ERRCODE GetErrorBuffer(IScriptErrorBuffer** errBuff) = 0;
			virtual ERRCODE GetScriptCompiler(IScriptCompiler** compiler) = 0;
			virtual ERRCODE Finalize() = 0;

			virtual ERRCODE LoadScriptFromFile(const char* file, const char* name, dword flags, IScript** outScript) = 0;
			virtual ERRCODE LoadScriptFromFile(FileSystem::IFileIO* file, const char* name, dword flags, IScript** outScript) = 0;
			virtual ERRCODE LoadScriptFromBuffer(const char* buffer, uint buffSize, const char* name, dword flags, IScript** outScript) = 0;
			virtual ERRCODE LoadScriptFromBuffer(Common::IBuffer* buff, const char* name, dword flags, IScript** outScript) = 0;
			
			virtual ERRCODE SaveScriptToFile(const char* file, const char* buffer, uint buffSize, FileSystem::IFileIO** outFile = 0) = 0;
			virtual ERRCODE SaveScriptToFile(const char* file, Common::IBuffer* buff, FileSystem::IFileIO** outFile = 0) = 0;
			virtual ERRCODE SaveScriptToFile(const char* file, IScript* script, FileSystem::IFileIO** outFile = 0) = 0;
			virtual ERRCODE SaveScriptToBuffer(IScript* script, Common::IBuffer** outBuffer) = 0;

			virtual ERRCODE Compile(const char* buffer, uint bufferSize, dword flags, Common::IBuffer** outBuffer) = 0;
			virtual ERRCODE Compile(IScript* script, dword flags, Common::IBuffer** outBuffer) = 0;
			virtual ERRCODE Compile(Common::IBuffer* scriptBuffer, dword flags, Common::IBuffer** outBuffer) = 0;

			virtual ERRCODE RegisterFunction(const char* fnName, EScriptArgumentType returnType, uint numArgs, EScriptArgumentType* args, GlobalScriptFn fn, const char* desc = 0) = 0;
			virtual ERRCODE CallScriptFunction(const char* fnName, CArgStack* args, int numResults) = 0;
			virtual ERRCODE GetFunctionDesc(const char* fnName, SScriptFunctionDesc* desc) = 0;

			virtual ERRCODE Execute(IScript* script) = 0;
			virtual ERRCODE Execute(const char* buffer, uint bufferSize) = 0;
			virtual ERRCODE Execute(Common::IBuffer* buffer) = 0;

			virtual ERRCODE ExecuteScripts() = 0;
			virtual ERRCODE AddScript(const char* name, IScript* script) = 0;
			virtual ERRCODE GetScript(const char* name, IScript** script) = 0;
			virtual ERRCODE ReloadScript() = 0;
			virtual ERRCODE ReloadAllScripts() = 0;

			virtual ERRCODE DumpStack(Common::IBuffer** outBuffer) = 0;
		};
	}
}



