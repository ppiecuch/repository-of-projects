#ifndef _YON_CORE_YONUTIL_H_
#define _YON_CORE_YONUTIL_H_

#include "yonString.h"
#include "path.h"
#include "yonMath.h"

#ifdef YON_COMPILE_WITH_WIN32
#include <windows.h>
#include <direct.h>//for mkdir
#elif defined(YON_COMPILE_WITH_ANDROID)
#include <unistd.h>
#include <sys/stat.h>//for mkdir
#endif

namespace yon{
namespace core{

	//! Convert a simple string of base 10 digits into a signed 32 bit integer.
	//! \param[in] in: The string of digits to convert. Only a leading - or + followed 
	//!					by digits 0 to 9 will be considered.  Parsing stops at the
	//!					first non-digit.
	//! \param[out] out: (optional) If provided, it will be set to point at the first
	//!					 character not used in the calculation.
	//! \return The signed integer value of the digits. If the string specifies too many
	//!			digits to encode in an s32 then +INT_MAX or -INT_MAX will be returned.
	inline s32 strtol10(const char* in, const char** out=0)
	{
		if(!in)
			return 0;

		bool negative = false;
		if('-' == *in)
		{
			negative = true;
			++in;
		}
		else if('+' == *in)
			++in;

		u32 unsignedValue = 0;

		while ( ( *in >= '0') && ( *in <= '9' ))
		{
			unsignedValue = ( unsignedValue * 10 ) + ( *in - '0' );
			++in;

			if(unsignedValue > (u32)INT_MAX)
			{
				unsignedValue = (u32)INT_MAX;
				break;
			}
		}
		if (out)
			*out = in;

		if(negative)
			return -((s32)unsignedValue);
		else
			return (s32)unsignedValue;
	}

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
		return pathname.subString(0,index);
	}

	inline s32 _mkdirs(c8* path)
	{
		s32 retval;

#ifdef YON_COMPILE_WITH_WIN32
		while (0 != (retval = _mkdir(path)))
#elif defined(YON_COMPILE_WITH_ANDROID)
		while (0 != (retval = mkdir(path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)))
#endif
		{
			c8 subpath[512] = "";
			c8 *delim;

			if (NULL == (delim = strrchr(path, '/')))
				return retval;

			strncat(subpath, path, delim - path);
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

	template <class T1, class T2>
	inline void swap(T1& a, T2& b)
	{
		T1 c(a);
		a = b;
		b = c;
	}

	inline void yonSleep(s32 pMilliseconds)
	{
#ifdef YON_COMPILE_WITH_WIN32
		Sleep(pMilliseconds);
#elif defined(YON_COMPILE_WITH_ANDROID)
		::usleep((pMilliseconds<<10)-24);
#endif
	}
}
}
#endif