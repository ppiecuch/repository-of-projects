#ifndef _YON_AUDIO_OAL_COALAUDIODRIVER_H_
#define _YON_AUDIO_OAL_COALAUDIODRIVER_H_

#include "IAudioDriver.h"
#include "yonArray.h"
#include "IWaveLoader.h"

#include <al.h>
#include <alc.h>
#include <alu.h>
//#include <alut.h>

#ifdef YON_COMPILE_WITH_WIN32
//#pragma comment(lib,"freealut.lib")
#pragma comment(lib,"OpenAL32.lib")
#endif

namespace yon{
namespace audio{
namespace oal{

	class COALAudioDriver : public IAudioDriver{
	private:
		ALCdevice *m_pDevice;
		ALCcontext *m_pContext;

		core::array<audio::ISound*> m_sounds;
		core::array<audio::IWaveLoader*> m_waveLoaders;

		void addSound(audio::ISound* sound);
		audio::ISound* loadSoundFromFile(io::IReadStream* file);
	public:
		COALAudioDriver(io::IFileSystem* fs);
		virtual ~COALAudioDriver();

		virtual ISound* getSound(const io::path& filename);
		virtual ISound* findSound(const io::path& filename) const;

		virtual IWave* createWaveFromFile(const io::path& filename);
		virtual IWave* createWaveFromFile(io::IReadStream* file);

		virtual IListener* getListener();
		virtual bool checkError(const c8* file,s32 line);
	};
}
}
}
#endif