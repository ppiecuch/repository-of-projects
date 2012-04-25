#ifndef _YON_AUDIO_CWAVELOADEROGG_H_
#define _YON_AUDIO_CWAVELOADEROGG_H_

#include "IWaveLoader.h"

namespace yon{
namespace audio{

	class CWaveLoaderOGG : public IWaveLoader{
	public:
		virtual bool checkFileExtension(const io::path& filename) const;
		virtual bool checkFileHeader(io::IReadStream* file) const;
		virtual audio::IWave* loadWave(io::IReadStream* file) const;
	};

	IWaveLoader* createWaveLoaderOGG();
}
}
#endif