#ifndef _YON_CORE_YONUTIL_H_
#define _YON_CORE_YONUTIL_H_

#include "yonString.h"
#include "path.h"

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
}//core
}//yon
#endif