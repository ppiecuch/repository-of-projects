#ifndef _YON_CORE_YONTYPES_H_
#define _YON_CORE_YONTYPES_H_

#include "yonConfig.h"

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
typedef unsigned __int8		u8;
#else
typedef unsigned char		u8;
#endif

#ifdef _MSC_VER
typedef __int8				s8;
#else
typedef signed char			s8;
#endif

#ifdef _MSC_VER
typedef unsigned __int16	u16;
#else
typedef unsigned short		u16;
#endif

#ifdef _MSC_VER
typedef __int16				s16;
#else
typedef signed short		s16;
#endif


#ifdef _MSC_VER
typedef unsigned __int32	u32;
#else
typedef unsigned int		u32;
#endif

#ifdef _MSC_VER
typedef __int32				s32;
#else
typedef signed int			s32;
#endif

#ifdef _MSC_VER
typedef unsigned __int64	u64;
#else
typedef unsigned long long	u64;
#endif

#ifdef _MSC_VER
typedef __int64				s64;
#else
typedef signed long long	s64;
#endif

typedef char				c8;
//wchar_t has 16 bit on windows and 32 bit on other operating systems.
//typedef wchar_t				c16;
typedef unsigned short		c16;
typedef unsigned long		c32;
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

/*
enum ENUM_DIMEN_MODE{
	ENUM_DIMEN_MODE_2D = 2,
	ENUM_DIMEN_MODE_3D = 3
};
*/

//编码类型
enum ENUM_ENCODING{
	ENUM_ENCODING_ASCII = 0,
	ENUM_ENCODING_UTF8,
	ENUM_ENCODING_UTF16,
	ENUM_ENCODING_UTF32,
	ENUM_ENCODING_COUNT
};

}//yon

#endif 
