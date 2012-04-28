#ifndef _YON_CORE_YONUTIL_H_
#define _YON_CORE_YONUTIL_H_

#include "yonString.h"
#include "path.h"
#include "yonMath.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <windows.h>
#elif defined(YON_COMPILE_WITH_ANDROID)
#include <unistd.h>
#endif

namespace yon{
namespace core{

	inline s32 isFileExtension (const io::path& filename,
		const io::path& ext0,const io::path& ext1,const io::path& ext2)
	{
		s32 extPos = filename.findLast('.');
		if ( extPos < 0 )
			return 0;

		extPos += 1;
		if ( filename.equalsSubstringIgnoreCase (ext0, extPos)) return 1;
		if ( filename.equalsSubstringIgnoreCase (ext1, extPos)) return 2;
		if ( filename.equalsSubstringIgnoreCase (ext2, extPos)) return 3;
		return 0;
	}

	inline bool hasFileExtension(const io::path& filename,
		const io::path& ext0,const io::path& ext1 = "",const io::path& ext2 = "")
	{
		return isFileExtension( filename,ext0,ext1,ext2)>0;
	}

	inline const io::path getFileName(const io::path& pathname){
		s32 index1 = pathname.findLast('/');
		s32 index2 = pathname.findLast('\\');
		s32 index = max_(index1,index2);
		if(index<0)
			return pathname;
		return pathname.subString(index+1);
	}

	template <class T1, class T2>
	inline void swap(T1& a, T2& b)
	{
		T1 c(a);
		a = b;
		b = c;
	}

	inline void yonSleep(s64 pMilliseconds)
	{
#ifdef YON_COMPILE_WITH_WIN32
		Sleep(pMilliseconds);
#elif defined(YON_COMPILE_WITH_ANDROID)
		::usleep((pMilliseconds<<10)-24);
#endif
	}
}//core
}//yon
#endif