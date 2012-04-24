#ifndef _YON_AUDIO_IAUDIODRIVER_H_
#define _YON_AUDIO_IAUDIODRIVER_H_

#include "IReferencable.h"
#include "path.h"

namespace yon{
namespace audio{

	class ISound;
	class IListener;

	class IAudioDriver : public virtual core::IReferencable{
	public:
		virtual ~IAudioDriver(){}
		virtual ISound* getSound(const io::path& filename) = 0;
		virtual IListener* getListener() = 0;
		virtual bool checkError(const c8* file,s32 line) = 0;
	};

	//注意:经函数只用于内部使用
	IAudioDriver* createAudioDriver();
}
}
#endif