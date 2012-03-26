#ifndef _YON_VIDEO_IMAGE_H_
#define _YON_VIDEO_IMAGE_H_

#include "IReferencable.h"
#include "dimension2d.h"

namespace yon{
namespace video{
	enum ENUM_COLOR_FORMAT
	{
		ENUM_COLOR_FORMAT_A1R5G5B5 = 0,
		ENUM_COLOR_FORMAT_R5G6B5,
		ENUM_COLOR_FORMAT_R8G8B8,
		ENUM_COLOR_FORMAT_A8R8G8B8,
		ENUM_COLOR_FORMAT_COUNT
	};
	class IImage : public virtual core::IReferencable{
	public:
		virtual const core::dimension2du& getDimension() const = 0;
		virtual ENUM_COLOR_FORMAT getColorFormat() const = 0;
	};
}//video
}//yon
#endif