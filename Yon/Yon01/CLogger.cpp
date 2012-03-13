#include "CLogger.h"
#include "exception.h"
#include <memory.h>
#include  <time.h>

namespace yon{
	namespace debug{

		#define _TRUNCATE ((size_t)-1)
		CLogger::CLogger():
			m_path(""),m_name("log.txt"),m_pFile(NULL),
			m_format(MASK_FORMAT_DATE|MASK_FORMAT_TIME|MASK_FORMAT_LEVEL),
			m_level(ENUM_LOG_LEVEL_INFO),m_appender(MASK_APPENDER_FILE){

				resetLevelStr();

				#ifdef YON_COMPILE_WITH_WIN32
				InitializeCriticalSection(&m_mutex);
				#else
				pthread_mutex_init(&m_mutex,NULL);
				#endif
			}
		CLogger::~CLogger(){
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
			resetLevelStr();
		}
		void CLogger::setAppender(s32 mask){
			m_appender=mask;
		}
		void CLogger::resetLevelStr(){
			switch(m_level){
			case ENUM_LOG_LEVEL_DEBUG:
				m_levelStr="DEBUG";
				break;
			case ENUM_LOG_LEVEL_INFO:
				m_levelStr="INFO";
				break;
			case ENUM_LOG_LEVEL_WARN:
				m_levelStr="WARN";
				break;
			case ENUM_LOG_LEVEL_ERROR:
				m_levelStr="ERROR";
				break;
			}
		}
		void CLogger::lock(){
			#ifdef YON_COMPILE_WITH_WIN32
			EnterCriticalSection(&m_mutex);
			#else
			if(pthread_mutex_lock(&m_mutex) != 0 )!
				throw core::exception("can not lock mutex!(pthread_mutex_lock return nonzero)");
			#endif
		}
		void CLogger::unlock(){
			#ifdef YON_COMPILE_WITH_WIN32
			LeaveCriticalSection(&m_mutex);
			#else
			if(pthread_mutex_unlock(&m_mutex) != 0 )!
				throw core::exception("can not unlock mutex!(pthread_mutex_unlock return nonzero)");
			#endif
		}
		void CLogger::appendDateTime(int& index){
			if((m_format&MASK_FORMAT_DATE)||(m_format&MASK_FORMAT_TIME)){
				static struct tm newtime;
				static time_t lt1;
				time(&lt1);
				localtime_s(&newtime,&lt1);
				if(m_format&MASK_FORMAT_DATE){
					strftime(m_buffer+index,10,"[%y-%m-%d",&newtime);
					index+=9;
				}
				if(m_format&MASK_FORMAT_TIME){
					strftime(m_buffer+index,10," %H:%M:%S",&newtime);
					index+=9;
				}
				//int sprintf_s(char *buffer,size_t sizeOfBuffer,const char *format [,argument] ...);
				//sprintf_s()是sprintf()的安全版本，通过指定缓冲区长度来避免sprintf()存在的溢出风险
				sprintf_s(m_buffer+index,2,"]");
				++index;
			}
		}
		void CLogger::appendLevel(int& index){
			if(m_format&MASK_FORMAT_LEVEL){
				sprintf_s(m_buffer+index,8,"[%s]",m_levelStr.c_str());
				index+=m_levelStr.length()+2;
			}
		}
		void CLogger::output(ENUM_LOG_LEVEL level,const c8* pFmt,va_list args){
			if(level<m_level)
				return;
			if(m_pFile==NULL&&(m_appender&MASK_APPENDER_FILE)){
				//m_pFile = fopen((m_path+m_name).c_str(),"ab");
				errno_t result=fopen_s(&m_pFile,(m_path+m_name).c_str(),"a+");
				if(result)
				{
					core::stringc msg("open log file:");
					msg+=m_path+m_name;
					msg+=" failed!";
					throw core::exception(msg);
				}
			}
			lock();
			memset(m_buffer,0x0,BUFFER_SIZE);
			int index=0;
			//appendDateTime(index);
			//appendLevel(index);
			//_vsnprintf_s(m_buffer,BUFFER_SIZE,pFmt,args);
			//vsnprintf_s和_vsnprintf_s没有多少区别只是和以前的相兼容
			//使用_vsnprintf_s偶尔会发生内存泄露
			//而使用vsnprintf_s则不会
			vsnprintf_s(m_buffer+index,BUFFER_SIZE,_TRUNCATE,pFmt,args);
			unlock();
			if(m_appender&MASK_APPENDER_FILE){
				fprintf(m_pFile,"%s",m_buffer);
			}
			if(m_appender&MASK_APPENDER_CONSOLE){
				printf("%s",m_buffer);
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
		#undef _TRUNCATE
	}//debug
}//yon