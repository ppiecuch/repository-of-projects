#ifndef _YON_CORE_YONUTIL_H_
#define _YON_CORE_YONUTIL_H_

#include "yonString.h"
#include "path.h"
#include "yonMath.h"
#include <memory.h>
#ifdef YON_COMPILE_WITH_WIN32
#include <io.h> // for _access
#include <windows.h>
#include <direct.h>//for mkdir
#define F_OK 00 //Existence only
#define W_OK 02 //Write-only
#define R_OK 04 //Read-only
#define X_OK 06 //Read and write
#elif defined(YON_COMPILE_WITH_ANDROID) || defined (YON_COMPILE_WITH_MACOSX)
#include <unistd.h>
#include <sys/stat.h>//for mkdir
#include <string.h>
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

	inline const io::path getParentName(const io::path& pathname){
		s32 index1 = pathname.findLast('/');
		s32 index2 = pathname.findLast('\\');
		s32 index = max_(index1,index2);
		if(index<0)
			return pathname;
		return pathname.subString(0,index+1);
	}

	//TODO not support on linux
	/*
	inline bool yonAccess(const wchar_t* path){
#ifdef YON_COMPILE_WITH_WIN32
		return (_waccess(path, F_OK) != -1);
#elif defined(YON_COMPILE_WITH_ANDROID)
		return (access(path, F_OK) != -1);
#endif
	}*/

	inline bool yonAccess(const c8* path){
#ifdef YON_COMPILE_WITH_WIN32
		return (_access(path, F_OK) != -1);
#elif defined(YON_COMPILE_WITH_ANDROID) || defined (YON_COMPILE_WITH_MACOSX)
		return (access(path, F_OK) != -1);
#endif
	}

	inline s32 _mkdirs(c8* path)
	{
		s32 retval;

#ifdef YON_COMPILE_WITH_WIN32
		while (0 != (retval = _mkdir(path))&&yonAccess(path)==false)
#elif defined(YON_COMPILE_WITH_ANDROID) || defined (YON_COMPILE_WITH_MACOSX)
		while (0 != (retval = mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))&&yonAccess(path)==false)
#endif
		{
			c8 subpath[512] = "";
			c8 *delim;

			if (NULL == (delim = strrchr(path, '/')))
				return retval;

#ifdef YON_COMPILE_WITH_WIN32
			strncat_s(subpath, path, delim - path);
#elif defined(YON_COMPILE_WITH_ANDROID) || defined (YON_COMPILE_WITH_MACOSX)
			strncat(subpath, path, delim - path);
#endif
			_mkdirs(subpath);
		}
		return retval;
	}
	inline s32 mkdirs(const io::path& pathname)
	{
		io::path temp=pathname;
		temp.replace('\\','/');
		return _mkdirs(const_cast<c8*>(temp.c_str()));
	}

	//regularize the path, using slash to end it if there is no slash at the end.
	//additionally, this function will replace all backslash with slash
	inline void regularize(io::path& pathname){
		pathname.replace('\\','/');
		s32 index = pathname.findLast('/');
		if(index!=pathname.length()-1)
			pathname.append('/');
	}

	template <class T1, class T2>
	inline void swap(T1& a, T2& b)
	{
		T1 c(a);
		a = b;
		b = c;
	}

	inline void yonSleep(s32 pMilliseconds)
	{
		if(pMilliseconds<=0)return;
#ifdef YON_COMPILE_WITH_WIN32
		Sleep(pMilliseconds);
#elif defined(YON_COMPILE_WITH_ANDROID) || defined (YON_COMPILE_WITH_MACOSX)
		::usleep((pMilliseconds<<10)-24);
#endif
	}

	//size£º×Ö½ÚÊý
	inline c8* yonFormatSize(f64 size) {
		static c8 buf[20];
		memset(buf,0x0,20);

		int i = 0;
		const c8* units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
		while (size > 1024) {
			size /= 1024;
			i++;
		}
		sprintf_s(buf,20,"%.*f%s", i, size, units[i]);
		return buf;
	}
}
}
#ifdef YON_COMPILE_WITH_WIN32
#undef F_OK
#undef W_OK
#undef R_OK
#undef X_OK
#endif
#endif