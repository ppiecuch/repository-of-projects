#include "StdAfx.h"
#include "Logger.h"
#include  "time.h"

#include <stdio.h>  


#ifndef LOGGER_LEVEL
#define LOGGER_LEVEL ENUM_LOGGER_LEVEL_INFO
#endif
#ifndef LOGGER_OUTPUT
#define LOGGER_OUTPUT ENUM_LOGGER_OUTPUT_CONSOLE
#endif
#ifndef LOGGER_FILE_PATH
#define LOGGER_FILE_PATH "./log.txt"
#endif

//Linux下不支持以下头文件
#ifdef WIN32
#include <wtypes.h>  
#include <stdarg.h>  
#include <tchar.h>
wchar_t* Logger::c2w(const char *str)
{
	size_t len = strlen(str) + 1;
	size_t converted = 0;
	wchar_t *wstr;
	wstr=(wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, wstr, len, str, _TRUNCATE);
	return wstr;
}
#endif

map<string,Logger*> LoggerFactory::instanceMap;

LoggerFactory::~LoggerFactory(void){
	map<string,Logger*>::iterator it = instanceMap.begin();
	for(;it!=instanceMap.end();++it)
	{
		delete it->second;
	}
	instanceMap.clear();
}
LoggerFactory& LoggerFactory::getInstance()
{  
	static LoggerFactory m_instance;
	return m_instance;
}
Logger::Logger()
{
}
Logger::Logger(const char* name):m_name(name),m_logFileName(LOGGER_FILE_PATH)
{
}

Logger::~Logger(void)
{
}


Logger* Logger::getInstance(const char* name)
{
	map<string,Logger*>& instanceMap=LoggerFactory::getInstance().instanceMap;
	map<string,Logger*>::iterator it = instanceMap.find(name);
	if(it == instanceMap.end())
	{
		Logger* logger=new Logger(name);
		instanceMap.insert(make_pair(name,logger));
		return logger;
	}
	return it->second;
}
void Logger::getTime(char tmpbuf[32])
{
	struct tm *newtime;
	time_t lt1;
	time(&lt1);
	newtime=localtime(&lt1);
	strftime(tmpbuf,32,"[%y-%m-%d %H:%M:%S]",newtime);
}
void Logger::debug(const char* pFmt, ...)
{
	if(LOGGER_LEVEL>ENUM_LOGGER_LEVEL_DEBUG)
		return;
	char tmpbuf[32];
	getTime(tmpbuf);
	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_CONSOLE)
	{
		printf(tmpbuf);
		va_list arg;
		va_start(arg, pFmt);
		printf("[DEBUG]");
		printf("{%s}",m_name.c_str());
		vprintf(pFmt, arg);
		printf("\r\n");
		va_end(arg );
	}

	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_FILE)
	{
		FILE * pFile;
#ifdef WIN32
		int result=fopen_s(&pFile,m_logFileName.c_str(),"a+");
		if(result)
			return;
#endif
#ifdef LINUX
		pFile=fopen(m_logFileName.c_str(),"ab");
		if(pFile==NULL)
			return;
#endif
		if (pFile != NULL)
		{
			fprintf(pFile,tmpbuf);
			va_list arg;
			va_start(arg, pFmt);
			fprintf(pFile,"[DEBUG]");
			fprintf(pFile,"{%s}",m_name.c_str());
			vfprintf(pFile, pFmt, arg);
			fprintf(pFile,"\r\n");
			va_end(arg);
			fclose(pFile);
		}
	}

#ifdef WIN32
	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_VS_OUTPUT)
	{
		wchar_t* tmwbuf=c2w(tmpbuf);
		OutputDebugString(tmwbuf);
		free(tmwbuf);
		va_list pArgs;
		char szMessageBuffer[16380]={0};
		va_start(pArgs, pFmt);
		vsnprintf(szMessageBuffer, 16380, pFmt, pArgs);
		va_end(pArgs);
		wchar_t* szWMessageBuffer=c2w(szMessageBuffer);
		OutputDebugString(L"[DEBUG]");
		wchar_t* wname=c2w(m_name.c_str());
		OutputDebugString(L"{");
		OutputDebugString(wname);
		OutputDebugString(L"}");
		free(wname);
		OutputDebugString(szWMessageBuffer);
		OutputDebugString(L"\r\n");
		free(szWMessageBuffer);
	}
#endif
}
void Logger::info(const char* pFmt, ...)
{
	if(LOGGER_LEVEL>ENUM_LOGGER_LEVEL_INFO)
		return;

	char tmpbuf[32];
	getTime(tmpbuf);
	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_CONSOLE)
	{
		printf(tmpbuf);
		va_list arg;
		va_start(arg, pFmt);
		printf("[INFO]");
		printf("{%s}",m_name.c_str());
		vprintf(pFmt, arg);
		printf("\r\n");
		va_end(arg );
	}

	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_FILE)
	{
		FILE * pFile;
#ifdef WIN32
		int result=fopen_s(&pFile,m_logFileName.c_str(),"a+");
		if(result)
			return;
#endif
#ifdef LINUX
		pFile=fopen(m_logFileName.c_str(),"ab");
		if(pFile==NULL)
			return;
#endif
		if (pFile != NULL)
		{
			fprintf(pFile,tmpbuf);
			va_list arg;
			va_start(arg, pFmt);
			fprintf(pFile,"[INFO]");
			fprintf(pFile,"{%s}",m_name.c_str());
			vfprintf(pFile, pFmt, arg);
			fprintf(pFile,"\r\n");
			va_end(arg);
			fclose(pFile);
		}
	}

#ifdef WIN32
	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_VS_OUTPUT)
	{
		wchar_t* tmwbuf=c2w(tmpbuf);
		OutputDebugString(tmwbuf);
		free(tmwbuf);
		va_list pArgs;
		char szMessageBuffer[16380]={0};
		va_start(pArgs, pFmt);
		vsnprintf(szMessageBuffer, 16380, pFmt, pArgs);
		va_end(pArgs);
		wchar_t* szWMessageBuffer=c2w(szMessageBuffer);
		OutputDebugString(L"[INFO]");
		wchar_t* wname=c2w(m_name.c_str());
		OutputDebugString(L"{");
		OutputDebugString(wname);
		OutputDebugString(L"}");
		free(wname);
		OutputDebugString(szWMessageBuffer);
		OutputDebugString(L"\r\n");
		free(szWMessageBuffer);
	}
#endif
}

void Logger::warn(const char* pFmt, ...)
{
	if(LOGGER_LEVEL>ENUM_LOGGER_LEVEL_WARN)
		return;
	char tmpbuf[32];
	getTime(tmpbuf);
	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_CONSOLE)
	{
		printf(tmpbuf);
		va_list arg;
		va_start(arg, pFmt);
		printf("[WARN]");
		printf("{%s}",m_name.c_str());
		vprintf(pFmt, arg);
		printf("\r\n");
		va_end(arg );
	}

	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_FILE)
	{
		FILE * pFile;
#ifdef WIN32
		int result=fopen_s(&pFile,m_logFileName.c_str(),"a+");
		if(result)
			return;
#endif
#ifdef LINUX
		pFile=fopen(m_logFileName.c_str(),"ab");
		if(pFile==NULL)
			return;
#endif
		if (pFile != NULL)
		{
			fprintf(pFile,tmpbuf);
			va_list arg;
			va_start(arg, pFmt);
			fprintf(pFile,"[WARN]");
			fprintf(pFile,"{%s}",m_name.c_str());
			vfprintf(pFile, pFmt, arg);
			fprintf(pFile,"\r\n");
			va_end(arg);
			fclose(pFile);
		}
	}

#ifdef WIN32
	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_VS_OUTPUT)
	{
		wchar_t* tmwbuf=c2w(tmpbuf);
		OutputDebugString(tmwbuf);
		free(tmwbuf);
		va_list pArgs;
		char szMessageBuffer[16380]={0};
		va_start(pArgs, pFmt);
		vsnprintf(szMessageBuffer, 16380, pFmt, pArgs);
		va_end(pArgs);
		wchar_t* szWMessageBuffer=c2w(szMessageBuffer);
		OutputDebugString(L"[WARN]");
		wchar_t* wname=c2w(m_name.c_str());
		OutputDebugString(L"{");
		OutputDebugString(wname);
		OutputDebugString(L"}");
		free(wname);
		OutputDebugString(szWMessageBuffer);
		OutputDebugString(L"\r\n");
		free(szWMessageBuffer);
	}
#endif
}

void Logger::error(const char* pFmt, ...)
{
	if(LOGGER_LEVEL>ENUM_LOGGER_LEVEL_ERROR)
		return;
	char tmpbuf[32];
	getTime(tmpbuf);
	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_CONSOLE)
	{
		printf(tmpbuf);
		va_list arg;
		va_start(arg, pFmt);
		printf("[ERROR]");
		printf("{%s}",m_name.c_str());
		vprintf(pFmt, arg);
		printf("\r\n");
		va_end(arg );
	}

	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_FILE)
	{
		FILE * pFile;
#ifdef WIN32
		int result=fopen_s(&pFile,m_logFileName.c_str(),"a+");
		if(result)
			return;
#endif
#ifdef LINUX
		pFile=fopen(m_logFileName.c_str(),"ab");
		if(pFile==NULL)
			return;
#endif
		if (pFile != NULL)
		{
			fprintf(pFile,tmpbuf);
			va_list arg;
			va_start(arg, pFmt);
			fprintf(pFile,"[ERROR]");
			fprintf(pFile,"{%s}",m_name.c_str());
			vfprintf(pFile, pFmt, arg);
			fprintf(pFile,"\r\n");
			va_end(arg);
			fclose(pFile);
		}
	}

#ifdef WIN32
	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_VS_OUTPUT)
	{
		wchar_t* tmwbuf=c2w(tmpbuf);
		OutputDebugString(tmwbuf);
		free(tmwbuf);
		va_list pArgs;
		char szMessageBuffer[16380]={0};
		va_start(pArgs, pFmt);
		vsnprintf(szMessageBuffer, 16380, pFmt, pArgs);
		va_end(pArgs);
		wchar_t* szWMessageBuffer=c2w(szMessageBuffer);
		OutputDebugString(L"[ERROR]");
		wchar_t* wname=c2w(m_name.c_str());
		OutputDebugString(L"{");
		OutputDebugString(wname);
		OutputDebugString(L"}");
		free(wname);
		OutputDebugString(szWMessageBuffer);
		OutputDebugString(L"\r\n");
		free(szWMessageBuffer);
	}
#endif
}

/************************************************************************/
/* 宽字符部分                                                           */
/************************************************************************/
#ifdef WIN32
void Logger::debug(const wchar_t* pFmt, ...)
{
	if(LOGGER_LEVEL>ENUM_LOGGER_LEVEL_INFO)
		return;
	char tmpbuf[32];
	getTime(tmpbuf);
	wchar_t* tmwbuf=c2w(tmpbuf);
	wchar_t* wname=c2w(m_name.c_str());
	
	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_CONSOLE)
	{
		wprintf(tmwbuf);
		va_list arg;
		va_start(arg, pFmt);
		wprintf(L"[DEBUG]");
		wprintf(L"{%s}",wname);
		vwprintf(pFmt, arg);
		wprintf(L"\r\n");
		va_end(arg );
	}

	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_FILE)
	{
		FILE * pFile;
		errno_t result=fopen_s(&pFile,m_logFileName.c_str(),"a+");
		if(result)
			return;
		if (pFile != NULL)
		{
			fwprintf(pFile,tmwbuf);
			va_list arg;
			va_start(arg, pFmt);
			fwprintf(pFile,L"[DEBUG]");
			fwprintf(pFile,L"{%s}",wname);
			vfwprintf(pFile, pFmt, arg);
			fwprintf(pFile,L"\r\n");
			va_end(arg);
			fclose(pFile);
		}
	}

	if(LOGGER_OUTPUT&ENUM_LOGGER_OUTPUT_VS_OUTPUT)
	{
		OutputDebugString(tmwbuf);
		va_list pArgs;
		wchar_t szMessageBuffer[16380]={0};
		va_start(pArgs, pFmt);
		_vsnwprintf_s(szMessageBuffer, 16380, pFmt, pArgs);
		va_end(pArgs);
		OutputDebugString(L"[DEBUG]");
		OutputDebugString(L"{");
		OutputDebugString(wname);
		OutputDebugString(L"}");
		OutputDebugString(szMessageBuffer);
		OutputDebugString(L"\r\n");
	}
	free(tmwbuf);
	free(wname);
}
#endif
