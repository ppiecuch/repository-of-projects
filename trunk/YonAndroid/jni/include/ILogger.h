#ifndef _YON_DEBUG_ILOGGER_H_
#define _YON_DEBUG_ILOGGER_H_

#include "yonConfig.h"
#include "IReferencable.h"
#include "yonString.h"
#include "position2d.h"
#include "SColor.h"


namespace yon{
	namespace video{
		class IVideoDriver;
	}
namespace debug{

	enum MASK_FORMAT{
		MASK_FORMAT_NONE = 0,		//空
		MASK_FORMAT_DATE = 0x1,		//日期
		MASK_FORMAT_TIME = 0x2,		//时间
		MASK_FORMAT_MSEC = 0x4,		//毫秒
		MASK_FORMAT_LEVEL = 0x8,	//日志级别
		MASK_FORMAT_LOG = 0x10,		//日志信息
		MASK_FORMAT_COUNT = 6
	};

	enum MASK_APPENDER{
		MASK_APPENDER_CONSOLE = 0x1,	//控制台
		MASK_APPENDER_VS = 0x2,			//VS输出窗口
		MASK_APPENDER_FILE = 0x4,		//单一文件
		MASK_APPENDER_SCREEN = 0x8,		//屏幕
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

	class ILogger : public virtual core::IReferencable{
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
		virtual void error(const c8* pFmt, ...) = 0;

		//virtual void setDebugPrinter(const video::IVideoDriver* driver,IDebugPrinter* printer) = 0;
		//virtual IDebugPrinter* getDebugPrinter() const = 0;
		//virtual IDebugPrinter* getDebugPrinter(const video::IVideoDriver* driver) const = 0;
		virtual void drawString(const video::IVideoDriver* driver,const core::stringc& str,const core::position2di& pos=core::ORIGIN_VECTOR2DI,const video::SColor& color=video::COLOR_WHITE) = 0;
		virtual void render(const video::IVideoDriver* driver) = 0;

	};
	//日志对象,供多方共享
	YON_API extern ILogger* Logger;
}
	using namespace debug;
}
#ifdef YON_USE_LOGGER
#define YON_DEBUG(format, ...) yon::debug::Logger->debug(format,##__VA_ARGS__)
#define YON_INFO(format, ...) yon::debug::Logger->info(format,##__VA_ARGS__)
#define YON_WARN(format, ...) yon::debug::Logger->warn(format,##__VA_ARGS__)
#define YON_ERROR(format, ...) yon::debug::Logger->error(format,##__VA_ARGS__)
#else
#define YON_DEBUG(format, ...)
#define YON_INFO(format, ...)
#define YON_WARN(format, ...)
#define YON_ERROR(format, ...)
#endif

#endif