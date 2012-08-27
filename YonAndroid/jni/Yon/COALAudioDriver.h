#ifndef _YON_AUDIO_OAL_COALAUDIODRIVER_H_
#define _YON_AUDIO_OAL_COALAUDIODRIVER_H_

#include "yonConfig.h"
#if defined(YON_AUDIO_MODE_OAL)

#include "IAudioDriver.h"
#include "yonArray.h"
#include "IWaveLoader.h"
#include "ISound.h"

#include <al.h>
#include <alc.h>
#include <alu.h>



namespace yon{
namespace audio{
namespace oal{

	class COALAudioDriver : public IAudioDriver{
	private:
		ALCdevice *m_pDevice;
		ALCcontext *m_pContext;

		struct SWave{
			audio::ISound* sound;
			bool playing;

			bool operator < (const SWave& other) const
			{
				return sound->getPath() < other.sound->getPath();
			}
		};

		//core::array<audio::ISound*> m_sounds;
		core::array<SWave> m_sounds;
		core::array<audio::IWaveLoader*> m_waveLoaders;

		void addSound(audio::ISound* sound);
		audio::ISound* loadSoundFromFile(io::IReadStream* file);

		void doze();
		void wake();
	public:
		COALAudioDriver(io::IFileSystem* fs);
		virtual ~COALAudioDriver();

		virtual ISound* getSound(const io::path& filename);
		virtual ISound* findSound(const io::path& filename);

		virtual IWave* createWaveFromFile(const io::path& filename);
		virtual IWave* createWaveFromFile(io::IReadStream* file);

		virtual IListener* getListener();
		virtual bool checkError(const c8* file,s32 line);

		virtual bool onEvent(const event::SEvent& event);
	};
}
}
}
#endif
#endif