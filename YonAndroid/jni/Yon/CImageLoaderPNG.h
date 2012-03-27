#ifndef _YON_VIDEO_IMAGE_CIMAGELOADERPNG_H_
#define _YON_VIDEO_IMAGE_CIMAGELOADERPNG_H_

#include "IImageLoader.h"

namespace yon{
namespace video{

	class CImageLoaderPng : public video::IImageLoader
	{
	public:
		virtual bool checkFileExtension(const io::path& filename) const;
		virtual bool checkFileHeader(io::IReadFile* file) const;
		virtual video::IImage* loadImage(io::IReadFile* file) const;
	};
}
}
#endif