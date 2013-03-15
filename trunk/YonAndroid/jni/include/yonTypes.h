#ifndef _YON_CORE_YONTYPES_H_
#define _YON_CORE_YONTYPES_H_

#include "yonConfig.h"

//中断宏

/*

// Define for breakpointing.
#if defined (ROCKET_PLATFORM_WIN32)
#if defined (__MINGW32__)
#define ROCKET_BREAK asm("int $0x03")
#else
#define ROCKET_BREAK _asm { int 0x03 }
#endif
#elif defined (ROCKET_PLATFORM_LINUX)
#if defined __i386__ || defined __x86_64__
#define ROCKET_BREAK asm ("int $0x03" )
#else
#define ROCKET_BREAK
#endif
#elif defined (ROCKET_PLATFORM_MACOSX)
#include <TargetConditionals.h>

#if TARGET_OS_IPHONE
#define ROCKET_BREAK
#else
#define ROCKET_BREAK {__asm__("int $3\n" : : );}
#endif
#endif
*/

#if defined(_DEBUG)
#if defined(YON_COMPILE_WITH_WIN32)
	#define YON_DEBUG_BREAK_IF( _CONDITION_ ) do { if (_CONDITION_) {_asm int 3} }while(0)
#elif defined(YON_COMPILE_WITH_WIN64)
	#include <crtdbg.h>
	#define YON_DEBUG_BREAK_IF( _CONDITION_ ) do { if (_CONDITION_) {_CrtDbgBreak();} }while(0)
#else
	#include "assert.h"
	#define YON_DEBUG_BREAK_IF( _CONDITION_ ) do { assert( !(_CONDITION_) ); }while(0)
#endif
#else //Not debug
	#define YON_DEBUG_BREAK_IF( _CONDITION_ )
#endif

#define _YON_STR(p) #p
#define YON_STR(p) _YON_STR(p)

#define _YON_COMBINE(a,b)	a##b
#define YON_COMBINE(a,b)	_YON_COMBINE(a,b)


namespace yon{

typedef void*				pvoid;

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

//因为WIN32/65下LONG都是32位。
//LINUX32/64下LONG是32/64
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

// define the wchar_t type if not already built in.
#ifdef _MSC_VER
#ifndef _WCHAR_T_DEFINED
//! A 16 bit wide character type.
/**
Defines the wchar_t-type.
In VS6, its not possible to tell
the standard compiler to treat wchar_t as a built-in type, and
sometimes we just don't want to include the huge stdlib.h or wchar.h,
so we'll use this.
*/
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif // wchar is not defined
#endif // microsoft compiler

#ifdef YON_WCHAR_FILESYSTEM
typedef wchar_t fschar;
#else
typedef char fschar;
#endif

//函数宏定义
#if defined (YON_COMPILE_WITH_ANDROID) || defined (YON_COMPILE_WITH_MACOSX)
#define sprintf_s(buffer,size,args...) sprintf(buffer,args)
#define vsprintf_s(buffer,size,fmt,args...) vsprintf(buffer,fmt,args)
#define vswprintf_s(buffer,size,fmt,args...)  swprintf(buffer,size,fmt,args)
#define sscanf_s(buffer,size,fmt,args...) sscanf(buffer,size,fmt,args)
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
	ENUM_ENCODING_GB18030,
	ENUM_ENCODING_UTF8,
	ENUM_ENCODING_UTF16,
	ENUM_ENCODING_UTF32,
	ENUM_ENCODING_COUNT
};

//通用长度
enum ENUM_SIZE{
	ENUM_SIZE_0			= 0,
	ENUM_SIZE_1			= 1<<0,
	ENUM_SIZE_2			= 1<<1,
	ENUM_SIZE_4			= 1<<2,
	ENUM_SIZE_8			= 1<<3,
	ENUM_SIZE_16		= 1<<4,
	ENUM_SIZE_32		= 1<<5,
	ENUM_SIZE_64		= 1<<6,
	ENUM_SIZE_128		= 1<<7,
	ENUM_SIZE_256		= 1<<8,
	ENUM_SIZE_512		= 1<<9,
	ENUM_SIZE_1024		= 1<<10,
	ENUM_SIZE_2048		= 1<<11,
	ENUM_SIZE_4096		= 1<<12,
	ENUM_SIZE_8192		= 1<<13,
	ENUM_SIZE_16384		= 1<<14,
	ENUM_SIZE_32768		= 1<<15,
	ENUM_SIZE_65536		= 1<<16,
	ENUM_SIZE_COUNT		= 18
};

}

#endif 
