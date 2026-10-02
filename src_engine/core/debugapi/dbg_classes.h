#pragma once


#include "debug.h"
#include "stdlib_dll.h"
#include "common.h"
#include "inherit.h"


using namespace krystallic;
using namespace krystallic::Debug;
using namespace krystallic::FileSystem;
using namespace krystallic::Common;


struct CDebugManager;
struct CLog;
struct CMiniDump;


struct CDebugManager : Inherit<CDebugManager, IDebugManager>
{
	CDebugManager(IAllocator* allocator);
	virtual ~CDebugManager();

	virtual uint32 Delete();
	virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

	virtual ERRCODE CreateLog(const char* file, ILog** logFile);
};


struct CLog : Inherit<CLog, ILog>
{
	RefCounted<IFileIO> m_logFile;
	bool m_printInfo;
	bool m_printFile;
	bool m_printLine;
	bool m_printTime;
	char m_format[64];

	void UpdateFormat();

	CLog(IAllocator* allocator);
	CLog(IAllocator* allocator, const char* filename);
	virtual ~CLog();

	virtual uint32 Delete();
	virtual uint32 QueryIFace(const SGuid& guid, void** IFace);

	virtual ERRCODE LogBegin(char* ClientName);
	virtual ERRCODE LogEnd();

	virtual ERRCODE Print(const char* msg, int64 now, const char* file, uint line, uint infoType);
	virtual ERRCODE Print(const char* fmt, ...);
	virtual ERRCODE PrintError(const char* errorBuffer, uint errorBufferSize);
	virtual void SetPrintInfo(bool f);
	virtual bool GetPrintInfo() const;
	virtual void SetPrintFile(bool f);
	virtual bool GetPrintFile() const;
	virtual void SetPrintLine(bool f);
	virtual bool GetPrintLine() const;
	virtual void SetPrintTime(bool f);
	virtual bool GetPrintTime() const;

	virtual ERRCODE GetFile(FileSystem::IFileIO** outLogFile);
};


struct CMiniDump : Inherit<CMiniDump, IMiniDump>
{
	CMiniDump(IAllocator* allocator);
	virtual ~CMiniDump();

	virtual uint32 Delete();
	virtual uint32 QueryIFace(const SGuid& guid, void** IFace);
};




