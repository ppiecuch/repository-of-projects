#include "CImageLoaderPNG.h"

#include "lpng/png.h"

#include "CImage.h"
#include "CReadFile.h"

namespace yon{
namespace video{
namespace image{

	bool CImageLoaderPng::canLoad(const io::path& filename) const{
		return false;
	}

	IImage* CImageLoaderPng::loadImage(io::IReadFile* file) const{
		return NULL;
	}
}//image
}//video
}//yon