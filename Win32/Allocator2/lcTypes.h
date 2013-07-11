#ifndef _LUCID_TYPES_H_
#define _LUCID_TYPES_H_

#include "lcConfig.h"

namespace lc{

#ifdef LC_CMPL_MSVC
	typedef unsigned __int8		u8;
#elif defined(LC_CMPL_GCC)
	typedef unsigned char		u8;
#endif

#ifdef LC_CMPL_MSVC
	typedef __int8				s8;
#elif defined(LC_CMPL_GCC)
	typedef signed char			s8;
#endif

#ifdef LC_CMPL_MSVC
	typedef unsigned __int16	u16;
#elif defined(LC_CMPL_GCC)
	typedef unsigned short		u16;
#endif

#ifdef LC_CMPL_MSVC
	typedef __int16				s16;
#elif defined(LC_CMPL_GCC)
	typedef signed short		s16;
#endif


#ifdef LC_CMPL_MSVC
	typedef unsigned __int32	u32;
#elif defined(LC_CMPL_GCC)
	typedef unsigned int		u32;
#endif

#ifdef LC_CMPL_MSVC
	typedef __int32				s32;
#elif defined(LC_CMPL_GCC)
	typedef signed int			s32;
#endif

#ifdef LC_CMPL_MSVC
	typedef unsigned __int64	u64;
#elif defined(LC_CMPL_GCC)
	typedef unsigned long long	u64;
#endif

#ifdef LC_CMPL_MSVC
	typedef __int64				s64;
#elif defined(LC_CMPL_GCC)
	typedef signed long long	s64;
#endif

typedef char					c8;
//wchar_t has 16 bit on windows and 32 bit on other operating systems.
//typedef wchar_t				c16;
typedef unsigned short			c16;
typedef unsigned long			c32;
typedef float					f32;
typedef double					f64;
}
#endif