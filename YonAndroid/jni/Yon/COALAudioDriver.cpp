#include "COALAudioDriver.h"

#include "ILogger.h"

namespace yon{
namespace audio{
namespace oal{

	COALAudioDriver::COALAudioDriver()
		:m_pDevice(NULL),m_pContext(NULL){
			m_pDevice=alcOpenDevice(NULL);
			m_pContext=alcCreateContext(m_pDevice, NULL);
			alcMakeContextCurrent(m_pContext);
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance COALAudioDriver");
	}
	COALAudioDriver::~COALAudioDriver(){
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_pContext);
		alcCloseDevice(m_pDevice);
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release COALAudioDriver");
	}

	ISound* COALAudioDriver::getSound(const io::path& filename){
		//TODO
		return NULL;
	}

	IListener* COALAudioDriver::getListener(){
		//TODO
		return NULL;
	}
	bool COALAudioDriver::checkError(const c8* file,s32 line){
		//TODO
		return false;
	}
}
	IAudioDriver* createAudioDriver(){
		return new oal::COALAudioDriver();
	}
}
}