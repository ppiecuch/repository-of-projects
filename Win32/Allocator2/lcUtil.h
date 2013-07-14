#ifndef _LUCID_UTIL_H_
#define _LUCID_UTIL_H_

#include "lcTypes.h"

#ifdef LC_CMPL_MSVC
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace lc{

	inline void sleep(s32 pMilliseconds)
	{
		if(pMilliseconds<=0)return;
#ifdef LC_CMPL_MSVC
		::Sleep(pMilliseconds);
#else
		::usleep((pMilliseconds<<10)-24);
#endif
	}
}
#endif