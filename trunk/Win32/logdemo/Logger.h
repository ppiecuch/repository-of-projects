/**********************************************************
* �ļ����ƣ�Logger
* ����ժҪ����־��¼��
* ����˵����
* ��ǰ�汾��v0.1
* ��    �ߣ����Ǳ�
* ������ڣ�
* ����ƽ̨��Win32 Linux
*
* �޸ļ�¼1��
*    �޸����ڣ�
*    �� �� �ţ�
*    �� �� �ˣ�
*    �޸����ݣ�
*
*
ʹ��˵��:
1���ڿ�ʼͷ�ļ�������Logger.h
2��������Ҫ��OUTPUT��LEVEL���˲����Ǳ���ģ�
������
#include "Logger.h"
#define LOGGER_OUTPUT ENUM_LOGGER_OUTPUT_VS_OUTPUT|ENUM_LOGGER_OUTPUT_CONSOLE|ENUM_LOGGER_OUTPUT_FILE
#define LOGGER_LEVEL ENUM_LOGGER_LEVEL_DEBUG
**********************************************************/
//TODO ʹ�û���д�ɱ�����������ٵ��ô���
//TODO VSOUTPUT��CONSOLE������֧��
//TODO Logger֧�ֶ��Ե�LEVEL
//TODO �ĳ�inline
#pragma once

#include <map>
#include <string>
using namespace std;

#ifndef WIN32
#define LINUX
#endif

enum LoggerLevel{
	ENUM_LOGGER_LEVEL_DEBUG = 0,
	ENUM_LOGGER_LEVEL_INFO,
	ENUM_LOGGER_LEVEL_WARN,
	ENUM_LOGGER_LEVEL_ERROR,
	ENUM_LOGGER_LEVEL_COUNT
};
enum LoggerOutput{
	ENUM_LOGGER_OUTPUT_CONSOLE = 1,
	ENUM_LOGGER_OUTPUT_VS_OUTPUT = 2,
	ENUM_LOGGER_OUTPUT_FILE = 4,
	ENUM_LOGGER_OUTPUT_COUNT = 3
};
class Logger;
class LoggerFactory{
private:
	static map<string,Logger*> instanceMap;
	LoggerFactory(void){}
	LoggerFactory(const LoggerFactory&){}
	LoggerFactory& operator = (const LoggerFactory&){}
	static LoggerFactory& getInstance();
public:
	~LoggerFactory(void);

	friend class Logger;
};
class Logger
{
private:
	string m_name;
	string m_logFileName;
	Logger();
	Logger(const char* name);
	Logger& operator = (const Logger& logger){}
	void getTime(char tmpbuf[32]);
#ifdef WIN32
	wchar_t* c2w(const char *str);
#endif
public:
	~Logger(void);
	static Logger* getInstance(const char* name);


	void debug(const char* pFmt, ...);
	void info(const char* pFmt, ...);
	void warn(const char* pFmt, ...);
	void error(const char* pFmt, ...);

	//Linuxƽ̨�ݲ�֧�ֿ��ַ�
#ifdef WIN32
	void debug(const wchar_t* pFmt, ...);
#endif
};
