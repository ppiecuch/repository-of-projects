#ifndef _YON_AUDIO_OAL_COALAUDIODRIVER_H_
#define _YON_AUDIO_OAL_COALAUDIODRIVER_H_

#include "IAudioDriver.h"

#include <al.h>
#include <alc.h>
#include <alu.h>
#include <alut.h>

#ifdef YON_COMPILE_WITH_WIN32
#pragma comment(lib,"freealut.lib")
#pragma comment(lib,"OpenAL32.lib")
#endif

namespace yon{
namespace audio{
namespace oal{

	class COALAudioDriver : public IAudioDriver{
	private:
		ALCdevice *m_pDevice;
		ALCcontext *m_pContext;
	public:
		COALAudioDriver();
		virtual ~COALAudioDriver();

		virtual ISound* getSound(const io::path& filename);
		virtual IListener* getListener();
		virtual bool checkError(const c8* file,s32 line);
	};
}
}
}
#endif