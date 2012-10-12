#include "CLogger.h"
#include "exception.h"
#include <memory.h>
#include <time.h>
#include <sys/timeb.h>

#ifdef YON_COMPILE_WITH_WIN32
#define FOREGROUND_WHITE FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE
#define FOREGROUND_YELLOW FOREGROUND_RED|FOREGROUND_GREEN
#elif defined(YON_COMPILE_WITH_ANDROID)
#include <android/log.h>
#endif

namespace yon{
	namespace debug{

#ifdef YON_COMPILE_WITH_ANDROID
//在标准C里，你不能省略可变参数，但是你却可以给它传递一个空的参数。为了解决这个问题，CPP使用一个特殊的‘##’操作。
#define LOGD(fmt,...) __android_log_print(ANDROID_LOG_DEBUG,YON_ENGINE_NAME,fmt,##__VA_ARGS__)
#define LOGI(fmt,...) __android_log_print(ANDROID_LOG_INFO,YON_ENGINE_NAME,fmt,##__VA_ARGS__)
#define LOGW(fmt,...) __android_log_print(ANDROID_LOG_WARN,YON_ENGINE_NAME,fmt,##__VA_ARGS__)
#define LOGE(fmt,...) __android_log_print(ANDROID_LOG_ERROR,YON_ENGINE_NAME,fmt,##__VA_ARGS__)
#endif

		//MFC下内存泄漏检测比const static对象的释放要早执行，所以会报内存泄漏
		//const core::stringc CLogger::LEVEL_NAME[ENUM_LOG_LEVEL_COUNT]={"DEBG","INFO","WARN","EROR"};
		const c8* CLogger::LEVEL_NAME[ENUM_LOG_LEVEL_COUNT]={"DEBG","INFO","WARN","EROR"};
		CLogger::CLogger():
			m_name("log.txt"),m_pFile(NULL),
			m_level(ENUM_LOG_LEVEL_DEBUG),
#ifdef YON_COMPILE_WITH_WIN32
			m_path(""),
			m_format(MASK_FORMAT_DATE|MASK_FORMAT_TIME|MASK_FORMAT_MSEC|MASK_FORMAT_LEVEL|MASK_FORMAT_LOG),
			m_appender(MASK_APPENDER_FILE|MASK_APPENDER_VS)
#elif defined(YON_COMPILE_WITH_ANDROID)
			m_path("/sdcard/"),
			m_format(MASK_FORMAT_LEVEL|MASK_FORMAT_LOG),
			m_appender(MASK_APPENDER_CONSOLE|MASK_APPENDER_FILE)
#endif
		{
#ifdef YON_COMPILE_WITH_WIN32
				m_consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE); //实例化句柄
				InitializeCriticalSection(&m_mutex);
#else
				pthread_mutex_init(&m_mutex,NULL);
#endif
		}
		CLogger::~CLogger(){
			info(YON_LOG_SUCCEED_FORMAT,"Release Logger");
			if(m_pFile)
				fclose(m_pFile);
			#ifdef YON_COMPILE_WITH_WIN32
			DeleteCriticalSection(&m_mutex);
			#else
			pthread_mutex_destroy(&m_mutex);
			#endif
		}

		void CLogger::setPath(const core::stringc& path){
			m_path=path;
		}
		void CLogger::setFileName(const core::stringc& name){
			m_name=name;
		}
		void CLogger::setFormat(s32 mask){
			m_format=mask;
		}
		void CLogger::setLevel(ENUM_LOG_LEVEL level){
			m_level=level;
		}
		void CLogger::setAppender(s32 mask){
			m_appender=mask;
		}
		void CLogger::lock(){
			#ifdef YON_COMPILE_WITH_WIN32
			EnterCriticalSection(&m_mutex);
			#else
			if(pthread_mutex_lock(&m_mutex) != 0 )
				throw core::exception("can not lock mutex!(pthread_mutex_lock return nonzero)");
			#endif
		}
		void CLogger::unlock(){
			#ifdef YON_COMPILE_WITH_WIN32
			LeaveCriticalSection(&m_mutex);
			#else
			if(pthread_mutex_unlock(&m_mutex) != 0 )
				throw core::exception("can not unlock mutex!(pthread_mutex_unlock return nonzero)");
			#endif
		}
		void CLogger::appendDateTime(int& index){
			if((m_format&MASK_FORMAT_DATE)||(m_format&MASK_FORMAT_TIME)){
#ifdef YON_COMPILE_WITH_WIN32
				static struct tm ttime;
				static struct _timeb timebuffer;
				_ftime64_s(&timebuffer); 
				localtime_s(&ttime,&timebuffer.time);
				struct tm* newtime=&ttime;
#elif defined(YON_COMPILE_WITH_ANDROID)
				static struct tm* newtime;
				static struct timeb timebuffer;
				ftime (&timebuffer);
				newtime=localtime(&timebuffer.time);
#endif
				sprintf_s(m_buffer+index,2,"[");
				++index;
				if(m_format&MASK_FORMAT_DATE){
					strftime(m_buffer+index,11,"%Y-%m-%d",newtime);
					index+=10;
				}
				if((m_format&MASK_FORMAT_DATE)&&(m_format&MASK_FORMAT_TIME)){
					sprintf_s(m_buffer+index,2," ");
					++index;
				}
				if(m_format&MASK_FORMAT_TIME){
					strftime(m_buffer+index,9,"%H:%M:%S",newtime);
					index+=8;
				}
				if((m_format&MASK_FORMAT_TIME)&&(m_format&MASK_FORMAT_MSEC)){
					sprintf_s(m_buffer+index,2,".");
					++index;
				}
				if(m_format&MASK_FORMAT_MSEC){
					sprintf_s(m_buffer+index,4,"%03d",timebuffer.millitm);
					index+=3;
				}
				/*static struct tm newtime;
				static time_t lt1;
				time(&lt1);
				localtime_s(&newtime,&lt1);
				sprintf_s(m_buffer+index,2,"[");
				++index;
				if(m_format&MASK_FORMAT_DATE){
					strftime(m_buffer+index,11,"%Y-%m-%d",&newtime);
					index+=10;
				}
				if((m_format&MASK_FORMAT_DATE)&&(m_format&MASK_FORMAT_TIME)){
					sprintf_s(m_buffer+index,2," ");
					++index;
				}
				if(m_format&MASK_FORMAT_TIME){
					strftime(m_buffer+index,9,"%H:%M:%S",&newtime);
					index+=8;
				}*/
				//int sprintf_s(char *buffer,size_t sizeOfBuffer,const char *format [,argument] ...);
				//sprintf_s()是sprintf()的安全版本，通过指定缓冲区长度来避免sprintf()存在的溢出风险
				sprintf_s(m_buffer+index,2,"]");
				++index;
			}
		}
		void CLogger::appendLevel(int& index,ENUM_LOG_LEVEL level){
			if(m_format&MASK_FORMAT_LEVEL){
				sprintf_s(m_buffer+index,8,"[%s]",LEVEL_NAME[level]);
				index+=6;
			}
		}
		void CLogger::output(ENUM_LOG_LEVEL level,const c8* pFmt,va_list args){
			if(level<m_level)
				return;
			if(m_pFile==NULL&&(m_appender&MASK_APPENDER_FILE)){
#ifdef YON_COMPILE_WITH_WIN32
				errno_t result=fopen_s(&m_pFile,(m_path+m_name).c_str(),"w+");
				if(result)
				{
					core::stringc msg("open log file:");
					msg+=m_path+m_name;
					msg+=" failed!";
					throw core::exception(msg);
				}

#elif defined(YON_COMPILE_WITH_ANDROID)
				m_pFile = fopen((m_path+m_name).c_str(),"w+");
				if(m_pFile==NULL)
				{
					core::stringc msg("open log file:");
					msg+=m_path+m_name;
					msg+=" failed!";
					LOGE("%s",msg.c_str());
				}else{
					LOGI("create log:%s%s success",m_path.c_str(),m_name.c_str());
				}
#endif
			}
			lock();
			memset(m_buffer,0x0,BUFFER_SIZE);
			int index=0;
			appendDateTime(index);
			appendLevel(index,level);
			//_vsnprintf_s(m_buffer,BUFFER_SIZE,pFmt,args);
			//vsnprintf_s和_vsnprintf_s没有多少区别只是和以前的相兼容
			//使用_vsnprintf_s偶尔会发生内存泄露
			//而使用vsnprintf_s则不会
			//vsnprintf_s(m_buffer+index,BUFFER_SIZE-index,_TRUNCATE,pFmt,args);
			if(m_format&MASK_FORMAT_LOG){
				vsprintf_s(m_buffer+index,BUFFER_SIZE-index,pFmt,args);
			}else{
				sprintf_s(m_buffer+index,2,"\n");
			}
			unlock();
			if(m_appender&MASK_APPENDER_FILE){
				fprintf(m_pFile,"%s",m_buffer);
			}
			if(m_appender&MASK_APPENDER_CONSOLE){
#ifdef YON_COMPILE_WITH_WIN32
				switch(level)
				{
				case ENUM_LOG_LEVEL_DEBUG:
					SetConsoleTextAttribute(m_consolehwnd, FOREGROUND_WHITE);//设置字体颜色
					break;
				case ENUM_LOG_LEVEL_INFO:
					SetConsoleTextAttribute(m_consolehwnd, FOREGROUND_GREEN);//设置字体颜色
					break;
				case ENUM_LOG_LEVEL_WARN:
					SetConsoleTextAttribute(m_consolehwnd, FOREGROUND_YELLOW);//设置字体颜色
					break;
				case ENUM_LOG_LEVEL_ERROR:
					SetConsoleTextAttribute(m_consolehwnd, FOREGROUND_RED);//设置字体颜色
					break;
				}
				printf("%s",m_buffer);
#elif defined(YON_COMPILE_WITH_ANDROID)
				switch(level)
				{
				case ENUM_LOG_LEVEL_DEBUG:
					LOGD("%s",m_buffer);
					break;
				case ENUM_LOG_LEVEL_INFO:
					LOGI("%s",m_buffer);
					break;
				case ENUM_LOG_LEVEL_WARN:
					LOGW("%s",m_buffer);
					break;
				case ENUM_LOG_LEVEL_ERROR:
					LOGE("%s",m_buffer);
					break;
				}
#endif//YON_COMPILE_WITH_WIN32
			}
			if(m_appender&MASK_APPENDER_VS){
				OutputDebugStringA(m_buffer);
			}
		}
		void CLogger::debug(const c8* pFmt, ...){
			va_list arg;
			va_start(arg,pFmt);
			output(ENUM_LOG_LEVEL_DEBUG,pFmt,arg);
			va_end(arg);
		}
		void CLogger::info(const c8* pFmt, ...){
			va_list arg;
			va_start(arg,pFmt);
			output(ENUM_LOG_LEVEL_INFO,pFmt,arg);
			va_end(arg);
		}
		void CLogger::warn(const c8* pFmt, ...){
			va_list arg;
			va_start(arg,pFmt);
			output(ENUM_LOG_LEVEL_WARN,pFmt,arg);
			va_end(arg);
		}
		void CLogger::error(const c8* pFmt, ...){
			va_list arg;
			va_start(arg,pFmt);
			output(ENUM_LOG_LEVEL_ERROR,pFmt,arg);
			va_end(arg);
		}
	}//debug
}//yon

#ifdef YON_COMPILE_WITH_WIN32
#undef FOREGROUND_WHITE
#undef FOREGROUND_YELLOW
#endif