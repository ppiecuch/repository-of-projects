#include "CWaveLoaderWAV.h"
#include "yonUtil.h"
#include "CWave.h"

#include <al.h>
#include <alc.h>
#include <alu.h>
#include <alut.h>


namespace yon{
namespace audio{

	/*
	wav文件包括头和数据两部分，其结构如下：（从文件头开始依次排列） 
	1）首先是字符串“RIFF” ，占4个字节。 
	2）波形块的大小：DWORD，占4字节。波形块的大小=(文件大小-8) 
	3）字符串"WAVE"，占4个字节。 
	4）字符串“fmt ”，占4个字节，注意fmt后有个空格字符（0x20）。 
	5）格式块的大小，DWORD，占4个字节 
	6）格式块，VC中用WAVEFORMATEX结构体描述，占18个字节，可用sizeof（WAVEFORMATEX）计算。 
	*/
	bool CWaveLoaderWAV::checkFileExtension(const io::path& filename) const{
		return core::hasFileExtension(filename,"wav");
	}
	bool CWaveLoaderWAV::checkFileHeader(io::IReadStream* file) const{
		//TODO
		return true;
	}
	audio::IWave* CWaveLoaderWAV::loadWave(io::IReadStream* file) const{
		ALvoid *data;
		ALsizei size, bits, freq;
		ALenum format;
		ALboolean loop;
		alutLoadWAVFile((ALbyte*)file->getPath().c_str(), &format, &data, &size, &freq, &loop);

		CWave* wave=new CWave((audio::ENUM_WAVE_FORMAT)format,size,freq,data,false,true);
		return wave;
	}

	IWaveLoader* createWaveLoaderWAV()
	{
		return new CWaveLoaderWAV();
	}
}
}