#ifndef _YON_AUDIO_IWAVELOADER_H_
#define _YON_AUDIO_IWAVELOADER_H_

#include "IReferencable.h"
#include "path.h"
#include "IStream.h"
#include "IWave.h"

namespace yon{
namespace audio{

	class IWaveLoader : public virtual core::IReferencable{
	public:
		virtual bool checkFileExtension(const io::path& filename) const = 0;
		virtual bool checkFileHeader(io::IReadStream* file) const = 0;
		virtual audio::IWave* loadWave(io::IReadStream* file) const = 0;
	};
}
}
#endif