#include "CWaveLoaderWAV.h"
#include "yonUtil.h"
#include "CWave.h"

#include <al.h>
#include <alc.h>
#include <alu.h>
//#include <alut.h>


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
	typedef struct {
		c8  riff[4];//'RIFF'
		u32 riffSize;
		c8  wave[4];//'WAVE'
		c8  fmt[4];//'fmt '
		u32 fmtSize;
		u16 format;
		u16 channels;
		u32 samplesPerSec;
		u32 bytesPerSec;
		u16 blockAlign;
		u16 bitsPerSample;
		c8  data[4];//'data'
		u32 dataSize;
	}BasicWAVEHeader;

	
	bool CWaveLoaderWAV::checkFileExtension(const io::path& filename) const{
		return core::hasFileExtension(filename,"wav");
	}
	bool CWaveLoaderWAV::checkFileHeader(io::IReadStream* file) const{
		//TODO
		/*
		if (!(//these things *must* be valid with this basic header
		memcmp("RIFF",header->riff,4) ||
		memcmp("WAVE",header->wave,4) ||
		memcmp("fmt ",header->fmt,4)  ||
		memcmp("data",header->data,4)
		))
		*/
		return true;
	}
	audio::IWave* CWaveLoaderWAV::loadWave(io::IReadStream* file) const{
		/*ALvoid *data;
		ALsizei size, bits, freq;
		ALenum format;
		ALboolean loop;
		alutLoadWAVFile((ALbyte*)file->getPath().c_str(), &format, &data, &size, &freq, &loop);*/

		BasicWAVEHeader header;
		file->read((u8*)&header,sizeof(BasicWAVEHeader));

		audio::ENUM_WAVE_FORMAT format;
		switch (header.bitsPerSample){
		case 8:
			format = (header.channels == 1) ? ENUM_WAVE_FORMAT_MONO8 : ENUM_WAVE_FORMAT_STEREO8;
			break;
		case 16:
		default:
			format = (header.channels == 1) ? ENUM_WAVE_FORMAT_MONO16 : ENUM_WAVE_FORMAT_STEREO16;
		}

		u8* data= new u8[header.dataSize];
		file->read(data,header.dataSize);
		CWave* wave=new CWave(format,header.dataSize,header.samplesPerSec,data,false,true);
		return wave;
	}

	IWaveLoader* createWaveLoaderWAV()
	{
		return new CWaveLoaderWAV();
	}
}
}