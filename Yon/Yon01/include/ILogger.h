#ifndef _YON_DEBUG_ILOGGER_H_
#define _YON_DEBUG_ILOGGER_H_

#include "config.h"
#include "IReferencable.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace yon{
namespace debug{

	enum MASK_APPENDER{
		MASK_APPENDER_CONSOLE = 0,	//控制台
		MASK_APPENDER_VS,			//VS输出窗口
		MASK_APPENDER_FILE,			//单一文件
		//MASK_APPENDER_DAILY,		//每日产生一文件
		MASK_APPENDER_COUNT
	};

	enum ENUM_LOG_LEVEL{
		ENUM_LOG_LEVEL_DEBUG = 0,
		ENUM_LOG_LEVEL_INFO,
		ENUM_LOG_LEVEL_WARN,
		ENUM_LOG_LEVEL_ERROR,
		ENUM_LOG_LEVEL_COUNT
	};

	class ILogger : public IReferencable{
	private:

	#ifdef YON_COMPILE_WITH_WIN32
		CRITICAL_SECTION	m_mutex;
	#else
		pthread_mutex_t		m_mutex;
	#endif

	public:

		virtual void setLevel(ENUM_LOG_LEVEL level) = 0;
		virtual void setAppender(MASK_APPENDER mask) = 0;

		virtual void debug() = 0;
		virtual void info() = 0;
		virtual void warn() = 0;
		virtual void error() = 0;

		virtual void lock() = 0;
		virtual void unlock() = 0;

	};
}//debug
}//yon
#endif