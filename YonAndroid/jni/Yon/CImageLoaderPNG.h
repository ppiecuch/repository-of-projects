#ifndef _YON_VIDEO_IMAGE_CIMAGELOADERPNG_H_
#define _YON_VIDEO_IMAGE_CIMAGELOADERPNG_H_

#include "IImageLoader.h"

namespace yon{
namespace video{
namespace image{

	class CImageLoaderPng : public video::IImageLoader
	{
	public:
		virtual bool canLoad(const io::path& filename) const;
		virtual video::IImage* loadImage(io::IReadFile* file) const;
	};
}
}
}
#endif