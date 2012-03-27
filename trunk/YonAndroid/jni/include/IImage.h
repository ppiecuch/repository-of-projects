#ifndef _YON_VIDEO_IMAGE_H_
#define _YON_VIDEO_IMAGE_H_

#include "IReferencable.h"
#include "dimension2d.h"
#include "SColor.h"

namespace yon{
namespace video{
	enum ENUM_COLOR_FORMAT
	{
		ENUM_COLOR_FORMAT_R5G5B5A1 = 0,
		ENUM_COLOR_FORMAT_R5G6B5,
		ENUM_COLOR_FORMAT_R8G8B8,
		ENUM_COLOR_FORMAT_R8G8B8A8,
		ENUM_COLOR_FORMAT_COUNT
	};
	static u32 getPixelBitsByFormat(const ENUM_COLOR_FORMAT format)
	{
		switch(format)
		{
		case ENUM_COLOR_FORMAT_R5G5B5A1:
			return 16;
		case ENUM_COLOR_FORMAT_R5G6B5:
			return 16;
		case ENUM_COLOR_FORMAT_R8G8B8:
			return 24;
		case ENUM_COLOR_FORMAT_R8G8B8A8:
			return 32;
		default:
			return 0;
		}
	}
	class IImage : public virtual core::IReferencable{
	public:
		virtual const core::dimension2du& getDimension() const = 0;
		virtual ENUM_COLOR_FORMAT getColorFormat() const = 0;
		virtual SColor getPixel(u32 x, u32 y) const = 0;
		
		virtual u32 getByteCountPerRow() const =0;
		virtual u32 getByteCountPerPixel() const =0;

		virtual void* lock() = 0;

		virtual void unlock() = 0;
	};
}//video
}//yon
#endif