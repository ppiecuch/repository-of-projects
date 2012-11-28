#ifndef _YON_DEBUG_CLOGGER_H_
#define _YON_DEBUG_CLOGGER_H_

#include "ILogger.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace yon{
	namespace debug{

		class IDebugPrinter;

		class CLogger : public ILogger{
		private:
		//互斥变量
		#ifdef YON_COMPILE_WITH_WIN32
			CRITICAL_SECTION	m_mutex;
			HANDLE m_consolehwnd; //控制台句柄
		#else
			pthread_mutex_t		m_mutex;
		#endif
		//日志目录
			core::stringc m_path;
		//日志文件名
			core::stringc m_name;
		//日志文件指针
			FILE* m_pFile;
		//日志格式
			s32 m_format;
		//日志级别
			ENUM_LOG_LEVEL m_level;
		//日志输出
			s32 m_appender;
		//缓冲区
			const static u32 BUFFER_SIZE=65535;
			c8 m_buffer[BUFFER_SIZE];
		//日志级别名称
			const static c8* LEVEL_NAME[ENUM_LOG_LEVEL_COUNT];
		protected:
			virtual void lock();
			virtual void unlock();
			virtual void output(ENUM_LOG_LEVEL level,const c8* pFmt, va_list args);
			virtual void appendDateTime(int& index);
			virtual void appendLevel(int& index,ENUM_LOG_LEVEL level);
		public:
			CLogger();
			virtual ~CLogger();
			virtual void setPath(const core::stringc& path);
			virtual void setFileName(const core::stringc& name);

			virtual void setFormat(s32 mask);
			virtual void setLevel(ENUM_LOG_LEVEL level);
			virtual void setAppender(s32 mask);

			virtual void debug(const c8* pFmt, ...);
			virtual void info(const c8* pFmt, ...);
			virtual void warn(const c8* pFmt, ...);
			virtual void error(const c8* pFmt, ...);
		};
	}
}
#endif