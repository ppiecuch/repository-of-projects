#ifndef _YON_DEBUG_ILOGGER_H_
#define _YON_DEBUG_ILOGGER_H_

#include "config.h"
#include "IReferencable.h"
#include "yonString.h"



namespace yon{
namespace debug{

	enum MASK_FORMAT{
		MASK_FORMAT_NONE = 0,		//空
		MASK_FORMAT_DATE = 0x1,		//日期
		MASK_FORMAT_TIME = 0x2,		//时间
		MASK_FORMAT_LEVEL = 0x4,	//日志级别
		MASK_FORMAT_LOG = 0x8,		//日志信息
		MASK_FORMAT_COUNT = 5
	};

	enum MASK_APPENDER{
		MASK_APPENDER_CONSOLE = 0x1,	//控制台
		MASK_APPENDER_VS = 0x2,			//VS输出窗口
		MASK_APPENDER_FILE = 0x4,		//单一文件
		//MASK_APPENDER_DAILY,		//每日产生一文件
		MASK_APPENDER_COUNT = 3
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
	//日志对象,供多方共享
	YON_API extern ILogger* Logger;
}//debug
}//yon
#endif