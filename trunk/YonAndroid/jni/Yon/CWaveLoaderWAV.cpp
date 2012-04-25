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
	wav�ļ�����ͷ�����������֣���ṹ���£������ļ�ͷ��ʼ�������У� 
	1���������ַ�����RIFF�� ��ռ4���ֽڡ� 
	2�����ο�Ĵ�С��DWORD��ռ4�ֽڡ����ο�Ĵ�С=(�ļ���С-8) 
	3���ַ���"WAVE"��ռ4���ֽڡ� 
	4���ַ�����fmt ����ռ4���ֽڣ�ע��fmt���и��ո��ַ���0x20���� 
	5����ʽ��Ĵ�С��DWORD��ռ4���ֽ� 
	6����ʽ�飬VC����WAVEFORMATEX�ṹ��������ռ18���ֽڣ�����sizeof��WAVEFORMATEX�����㡣 
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