#ifndef _YON_SCENE_IMAGE_CIMAGELOADERPNG_H_
#define _YON_SCENE_IMAGE_CIMAGELOADERPNG_H_

#include "IImageLoader.h"

namespace yon{
namespace scene{
namespace image{

	class CImageLoaderPng : public IImageLoader
	{
	public:
		virtual bool canLoad(const io::path& filename) const;
		virtual IImage* loadImage(io::IReadFile* file) const;
	};
}
}
}
#endif