#ifndef _YON_VIDEO_IIMAGELOADER_H_
#define _YON_VIDEO_IIMAGELOADER_H_

#include "IReferencable.h"
#include "path.h"
#include "IReadFile.h"

namespace yon{
namespace video{
	
	class IImageLoader : public virtual core::IReferencable{
	public:
		virtual bool canLoad(const io::path& filename) const = 0;
		virtual IImage* loadImage(io::IReadFile* file) const = 0;
	};
}
}
#endif