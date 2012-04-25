#ifndef _YON_AUDIO_IWAVE_H_
#define _YON_AUDIO_IWAVE_H_

#include "IReferencable.h"

namespace yon{
namespace audio{

	//¸úOpenAL¶ÔÓ¦
	enum ENUM_WAVE_FORMAT{
		ENUM_WAVE_FORMAT_MONO8 = 0x1100,
		ENUM_WAVE_FORMAT_MONO16 = 0x1101,
		ENUM_WAVE_FORMAT_STEREO8 = 0x1102,
		ENUM_WAVE_FORMAT_STEREO16 = 0x1103,
		ENUM_WAVE_FORMAT_COUNT = 4
	};
	const static c8* WAVE_FORMAT_NAME[]=
	{
		"MONO8",
		"MONO16",
		"STEREO8",
		"STEREO16"
	};
	//8-bit data is expressed as an unsigned value over the range[0,255],
	//128 being an audio output level of 0.
	//16-bit data is expressed as and signed value over the range [-32768,32767],
	//0 being an audio output level of zero.
	class IWave : public virtual core::IReferencable{
	public:
		virtual u32 getFrequency() const = 0;
		//wave data format
		virtual ENUM_WAVE_FORMAT getFormat() const = 0;
		//numbers of bytes
		virtual u32 getSize() const = 0;

		virtual void* lock() = 0;
		virtual void unlock() = 0;
	};
}
}
#endif