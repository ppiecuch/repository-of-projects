#ifndef _YON_AUDIO_IAUDIODRIVER_H_
#define _YON_AUDIO_IAUDIODRIVER_H_

#include "IReferencable.h"
#include "path.h"
#include "IFileSystem.h"
#include "IEventReceiver.h"

namespace yon{
namespace audio{

	class ISound;
	class IListener;
	class IWave;

	class IAudioDriver : public virtual core::IReferencable,public event::IEventReceiver{
	protected:
		io::IFileSystem* m_pFileSystem;
	public:
		IAudioDriver(io::IFileSystem* fs)
			:m_pFileSystem(fs){
				if(m_pFileSystem)
					m_pFileSystem->grab();
		}
		virtual ~IAudioDriver(){
			if(m_pFileSystem)
				m_pFileSystem->drop();
		}
		virtual ISound* getSound(const io::path& filename) = 0;
		virtual ISound* findSound(const io::path& filename) = 0;
		virtual void removeSound(ISound* sound) = 0;
		virtual void grabSound(ISound* sound) = 0;
		virtual void dropSound(ISound* sound) = 0;

		virtual IWave* createWaveFromFile(const io::path& filename) = 0;
		virtual IWave* createWaveFromFile(io::IReadStream* file) =0;

		virtual IListener* getListener() = 0;
		virtual bool checkError(const c8* file,s32 line) = 0;
	};

	//注意:经函数只用于内部使用
	IAudioDriver* createAudioDriver(io::IFileSystem* fs);
}
}
#endif