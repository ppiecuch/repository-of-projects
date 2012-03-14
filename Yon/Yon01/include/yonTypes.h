#ifndef _YON_CORE_YONTYPES_H_
#define _YON_CORE_YONTYPES_H_

#include "config.h"

//ÖÐ¶Ïºê

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
typedef __int32				s32;
#else
typedef signed int			s32;
#endif

typedef char				c8;
typedef wchar_t				c16;
typedef float				f32;

#ifdef YON_WCHAR_FILESYSTEM
typedef wchar_t fschar;
#else
typedef char fschar;
#endif

}//yon

#endif 
