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
	wav�ļ�����ͷ�����������֣���ṹ���£������ļ�ͷ��ʼ�������У� 
	1���������ַ�����RIFF�� ��ռ4���ֽڡ� 
	2�����ο�Ĵ�С��DWORD��ռ4�ֽڡ����ο�Ĵ�С=(�ļ���С-8) 
	3���ַ���"WAVE"��ռ4���ֽڡ� 
	4���ַ�����fmt ����ռ4���ֽڣ�ע��fmt���и��ո��ַ���0x20���� 
	5����ʽ��Ĵ�С��DWORD��ռ4���ֽ� 
	6����ʽ�飬VC����WAVEFORMATEX�ṹ��������ռ18���ֽڣ�����sizeof��WAVEFORMATEX�����㡣 
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