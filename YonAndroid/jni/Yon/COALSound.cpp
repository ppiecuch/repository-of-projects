#include "COALSound.h"

namespace yon{
namespace audio{
namespace oal{

	COALSound::COALSound(IWave* wave,const io::path& p)
		:ISound(p),m_source(0),m_buffer(0){
		alGenSources(1, &m_source);
		alGenBuffers(1, &m_buffer);

		alBufferData(m_buffer, wave->getFormat(), wave->lock(), wave->getSize(), wave->getFrequency());
		alSourcei(m_source, AL_BUFFER, m_buffer);
	}
	COALSound::~COALSound(){
		alDeleteSources(1, &m_source);
		alDeleteBuffers(1, &m_buffer);
	}

	void COALSound::play(){
		alSourcePlay(m_source);
	}
}
}
}