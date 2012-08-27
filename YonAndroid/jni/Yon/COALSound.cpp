#include "COALSound.h"
#if defined(YON_AUDIO_MODE_OAL)

namespace yon{
namespace audio{
namespace oal{

	COALSound::COALSound(IWave* wave,const io::path& p)
		:ISound(p),m_source(0),m_buffer(0),m_secOffset(0){
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
		//setSecondOffset(m_secOffset);
		alSourcePlay(m_source);
	}
	void COALSound::pause(){
		//getSecondOffset();
		alSourcePause(m_source);
	}
	void COALSound::stop(){
		alSourceStop(m_source);
		m_secOffset=0;
	}
	void COALSound::rewind(){
		alSourceRewind(m_source);
		m_secOffset=0;
	}

	ALint COALSound::getState(){
		static ALint state;
		alGetSourcei(m_source, AL_SOURCE_STATE, &state);
		return state;
	}
	bool COALSound::isPlaying(){
		return getState()==AL_PLAYING;
	}
	bool COALSound::isPaused(){
		return getState()==AL_PAUSED;
	}
	bool COALSound::isStopped(){
		return getState()==AL_STOPPED;
	}

	void COALSound::setLooping(bool on){
		//AL_LOOPING can be chaned on a source in any execution state.In particular, it can be changed on a AL_PLAYING source.
		alSourcei(m_source, AL_LOOPING,on?AL_TRUE:AL_FALSE);
	}
	bool COALSound::isLooping(){
		static ALint loop;
		alGetSourcei(m_source, AL_LOOPING, &loop);
		return loop==AL_TRUE;
	}

	void COALSound::setGain(f32 gain){
		alSourcef(m_source, AL_GAIN, gain);
	}
	f32 COALSound::getGain(){
		static float gain;
		alGetSourcef(m_source, AL_GAIN, &gain);
		return gain;
	}

	f32 COALSound::getSecondOffset(){
		alGetSourcef(m_source, AL_SEC_OFFSET, &m_secOffset);
		return m_secOffset;
	}
	void COALSound::setSecondOffset(const f32 offset){
		alSourcef(m_source, AL_SEC_OFFSET, offset);
	}

}
}
}
#endif