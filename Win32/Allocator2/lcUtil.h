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

	/*inline core::stringc formatSize(f64 size) {
		c8 buf[20];
		memset(buf,0x0,20);

		int i = 0;
		const c8* units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
		while (size > 1024) {
			size /= 1024;
			i++;
		}
		sprintf_s(buf,20,"%.*f%s", i, size, units[i]);
		return buf;
	}*/
}
#endif