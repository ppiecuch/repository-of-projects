#include "CImageLoaderPNG.h"

#include "lpng/png.h"

#include "CImage.h"
#include "CReadFile.h"

namespace yon{
namespace scene{
namespace image{

	bool CImageLoaderPng::canLoad(const io::path& filename) const{
	}

	IImage* CImageLoaderPng::loadImage(io::IReadFile* file) const{}
}//image
}//scene
}//yon