#ifndef _YON_CORE_YONTYPES_H_
#define _YON_CORE_YONTYPES_H_

#include "config.h"

//中断宏

#if defined(_DEBUG)
#if defined(YON_COMPILE_WITH_WIN32)
	#define YON_DEBUG_BREAK_IF( _CONDITION_ ) if (_CONDITION_) {_asm int 3}
#elif defined(YON_COMPILE_WITH_WIN64)
	#include <crtdbg.h>
	#define YON_DEBUG_BREAK_IF( _CONDITION_ ) if (_CONDITION_) {_CrtDbgBreak();}
#else
	#include "assert.h"
	#define YON_DEBUG_BREAK_IF( _CONDITION_ ) assert( !(_CONDITION_) );
#endif
#else //Not debug
	#define YON_DEBUG_BREAK_IF( _CONDITION_ )
#endif

namespace yon{

#ifdef _MSC_VER
typedef unsigned __int32	u32;
#else
typedef unsigned int		u32;
#endif

#ifdef _MSC_VER
typedef unsigned __int16	u16;
#else
typedef unsigned short		u16;
#endif

#ifdef _MSC_VER
typedef __int32				s32;
#else
typedef signed int			s32;
#endif

typedef char				c8;
typedef wchar_t				c16;
typedef float				f32;
typedef double				f64;

#ifdef YON_WCHAR_FILESYSTEM
typedef wchar_t fschar;
#else
typedef char fschar;
#endif

//函数宏定义
#ifdef YON_COMPILE_WITH_ANDROID
#define sprintf_s(buffer,size,args...) sprintf(buffer,args)
#define vsprintf_s(buffer,size,fmt,args...) vsprintf(buffer,fmt,args)
#define vswprintf_s(buffer,size,fmt,args...)  swprintf(buffer,size,fmt,args)
#define OutputDebugStringA(args...)
#endif

}//yon

#endif 
