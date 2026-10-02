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
#include <time.h>


extern const char* g_LogInfoType[7];


CLog::CLog(IAllocator* allocator) : CLog(allocator, "__default_log_name__.log") 
{
}


CLog::CLog(IAllocator* allocator, const char* filename) : Inherit(allocator)
{
	ERRCODE err = StdFileOpen(filename, "w", m_logFile.ReleaseAndGetAddressOf(), allocator);

	assert(err == FS_ERR_OK);

	m_printInfo = true;
	m_printFile = true;
	m_printLine = true;
	m_printTime = true;
	m_format[0] = '\0';
}


CLog::~CLog()
{

}


ERRCODE CLog::Delete()
{
	uint refCount = DecRef();

	if (refCount == 0)
	{
		CLog::_Destroy(this);
		return 0;
	}

	return refCount;
}


ERRCODE CLog::QueryIFace(const SGuid& guid, void** IFace)
{
	if (!IFace)
		return 0;

	if (guid == IBase::GUID())
	{
		*IFace = static_cast<IBase*>(this);
		return this->IncRef();
	}

	if (guid == ILog::GUID())
	{
		*IFace = static_cast<ILog*>(this);
		return this->IncRef();
	}

	return 0;
}


void CLog::UpdateFormat()
{

}


ERRCODE CLog::LogBegin(char* ClientName)
{
	time_t now = time(0);
	tm* t = localtime(&now);

	m_logFile->PrintF(ClientName);
	m_logFile->PrintF("\nKrystallic Engine - " KRYSTALLIC_BUILD_STR_FULL "\n\n");
	m_logFile->PrintF("CLIENT STARTED AT: %s", asctime(t));
	m_logFile->PrintF("+----------------------------------------------------------------------------------------------------------------------------+\n");
	m_logFile->PrintF("|                                                         LOG BEGIN                                                          |\n");
	m_logFile->PrintF("+----------------------------------------------------------------------------------------------------------------------------+\n");

	return 0;
}


ERRCODE CLog::LogEnd()
{
	time_t now = time(0);
	tm* t = localtime(&now);

	m_logFile->PrintF("+----------------------------------------------------------------------------------------------------------------------------+\n");
	m_logFile->PrintF("|                                                          LOG END                                                           |\n");
	m_logFile->PrintF("+----------------------------------------------------------------------------------------------------------------------------+\n");
	m_logFile->PrintF("CLIENT STOPPED AT: %s\n", asctime(t));

	return 0;
}


ERRCODE CLog::Print(const char* msg, int64 now, const char* file, uint line, uint infoType)
{
	char timeBuff[9] = { 0 };
	tm t {};
	time_t tNow = static_cast<time_t>(now);

#ifdef _DEBUG
	const char* fileFormat = "[%64s]";
#else
	const char* fileFormat = "[%32s]";
#endif 

	if (m_printInfo)
		m_logFile->PrintF(g_LogInfoType[infoType]);

	if (m_printFile)
		m_logFile->PrintF(fileFormat, file);

	if (m_printLine)
		m_logFile->PrintF("[%5d]", line);

	if (m_printTime)
	{
#if defined(_WIN32)
		localtime_s(&t, &tNow);
#else
		localtime_r(&tNow, &t);
#endif
		strftime(timeBuff, sizeof(timeBuff), "%H:%M:%S", &t);
		m_logFile->PrintF("[%8s]", timeBuff);
	}

	if (m_printInfo || m_printFile || m_printLine || m_printTime)
		m_logFile->PrintF("  -->  ");

	m_logFile->PrintF("%s\n", msg);
	return 0;
}


ERRCODE CLog::Print(const char* fmt, ...)
{
	return 0;
}


ERRCODE CLog::PrintError(const char* errorBuffer, uint errorBufferSize)
{
	m_logFile->PrintF("\n");

	m_logFile->PrintF(errorBuffer);

	m_logFile->PrintF("\n\n");

	return 0;
}


void CLog::SetPrintInfo(bool f)
{
	m_printInfo = f;
}


bool CLog::GetPrintInfo() const
{
	return m_printInfo;
}


void CLog::SetPrintFile(bool f)
{
	m_printFile = f;
}


bool CLog::GetPrintFile() const
{
	return m_printFile;
}


void CLog::SetPrintLine(bool f)
{
	m_printLine = f;
}


bool CLog::GetPrintLine() const
{
	return m_printLine;
}


void CLog::SetPrintTime(bool f)
{
	m_printTime = f;
}


bool CLog::GetPrintTime() const
{
	return m_printTime;
}


ERRCODE CLog::GetFile(FileSystem::IFileIO** outLogFile)
{
	if (!outLogFile)
		return DBG_ERR_INVALID_ARGUMENT;

	if (m_logFile.Get() == nullptr)
		return DBG_ERR_INVALID_FILE;

	if (*outLogFile)
		(*outLogFile)->Delete();
	
	*outLogFile = static_cast<IFileIO*>(m_logFile.Get());
	m_logFile->IncRef();

	return DBG_ERR_OK;
}