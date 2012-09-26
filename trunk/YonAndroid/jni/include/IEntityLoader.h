#ifndef _YON_SCENE_IENTITYLOADER_H_
#define _YON_SCENE_IENTITYLOADER_H_

#include "IReferencable.h"
#include "path.h"
#include "IStream.h"
#include "IAnimatedEntity.h"

namespace yon{
namespace scene{

	class IEntityLoader : public core::IReferencable{
	public:
		virtual bool checkFileExtension(const io::path& filename) const = 0;
		virtual bool checkFileHeader(io::IReadStream* file) const = 0;
		virtual scene::IAnimatedEntity* loadEntity(io::IReadStream* file) const = 0;
	};
}
}
#endif