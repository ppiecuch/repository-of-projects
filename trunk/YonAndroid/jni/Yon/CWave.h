#ifndef _YON_AUDIO_CWAVE_H_
#define _YON_AUDIO_CWAVE_H_

#include "IWave.h"

namespace yon{
namespace audio{

	class CWave : public IWave{
	private:
		u8* m_pData;
		u32 m_uSize;
		u32 m_uFreq;
		ENUM_WAVE_FORMAT m_format;
		bool m_bDeleteData;
	public:
		//构造空Wave
		CWave(ENUM_WAVE_FORMAT format, u32 size, u32 freq);

		//copyData:是否复制数据
		CWave(ENUM_WAVE_FORMAT format, u32 size, u32 freq, void* data, bool copyData= true,bool deleteData=true);

		virtual ~CWave();

		virtual u32 getFrequency() const{return m_uFreq;}
		virtual ENUM_WAVE_FORMAT getFormat() const{return m_format;}
		virtual u32 getSize() const{return m_uSize;}

		virtual void* lock()
		{
			return m_pData;
		}
		virtual void unlock() {}
	};
}
}
#endif