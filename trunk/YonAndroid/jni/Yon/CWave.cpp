#include "CWave.h"
#include <memory.h>

#ifdef YON_COMPILE_WITH_ANDROID
#include <string.h>
#endif

#include "ILogger.h"

namespace yon{
namespace audio{

	CWave::CWave(ENUM_WAVE_FORMAT format, u32 size, u32 freq)
		:m_format(format),m_uSize(size),m_uFreq(freq),m_bDeleteData(true){
			m_pData = new u8[size];
	}

	CWave::CWave(ENUM_WAVE_FORMAT format, u32 size, u32 freq, void* data, bool copyData,bool deleteData)
		:m_format(format),m_uSize(size),m_uFreq(freq),m_pData(NULL),m_bDeleteData(deleteData){
			if(copyData){
				m_pData = new u8[size];
				memcpy(m_pData, data, size);
			}else{
				m_pData=(u8*)data;
			}
	}

	CWave::~CWave(){
		if(m_bDeleteData){
			delete[] m_pData;
		}
	}
}
}