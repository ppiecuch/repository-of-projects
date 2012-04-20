/**********************************************************
* 文件名称：Logger
* 内容摘要：日志记录类
* 其它说明：
* 当前版本：v0.1
* 作    者：洪智标
* 完成日期：
* 兼容平台：Win32 Linux
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
*
*
使用说明:
1、在开始头文件中引用Logger.h
2、定义需要的OUTPUT与LEVEL（此步不是必须的）
举例：
#include "Logger.h"
#define LOGGER_OUTPUT ENUM_LOGGER_OUTPUT_VS_OUTPUT|ENUM_LOGGER_OUTPUT_CONSOLE|ENUM_LOGGER_OUTPUT_FILE
#define LOGGER_LEVEL ENUM_LOGGER_LEVEL_DEBUG
**********************************************************/
//TODO 使用汇编改写可变参数函数的再调用处理
//TODO VSOUTPUT与CONSOLE的中文支持
//TODO Logger支持独自的LEVEL
//TODO 改成inline
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

	//Linux平台暂不支持宽字符
#ifdef WIN32
	void debug(const wchar_t* pFmt, ...);
#endif
};
