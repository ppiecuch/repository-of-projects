#ifndef _YON_DEBUG_ILOGGER_H_
#define _YON_DEBUG_ILOGGER_H_

#include "config.h"
#include "IReferencable.h"
#include "yonString.h"



namespace yon{
namespace debug{

	enum MASK_FORMAT{
		MASK_FORMAT_DATE = 0,		//日期
		MASK_FORMAT_TIME,			//时间
		MASK_FORMAT_LEVEL,			//日志级别
		MASK_FORMAT_COUNT
	};

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

	class ILogger : public core::IReferencable{
	public:
		virtual ~ILogger(){};
		virtual void setPath(const core::stringc& path) = 0;
		virtual void setFileName(const core::stringc& name) = 0;

		virtual void setFormat(s32 mask) = 0;
		virtual void setLevel(ENUM_LOG_LEVEL level) = 0;
		virtual void setAppender(s32 mask) = 0;

		virtual void debug(const c8* pFmt, ...) = 0;
		virtual void info(const c8* pFmt, ...) = 0;
		virtual void warn(const c8* pFmt, ...) = 0;
		virtual void error(const c8* pFmt, ...) = 0;;

	};
}//debug
}//yon
#endif