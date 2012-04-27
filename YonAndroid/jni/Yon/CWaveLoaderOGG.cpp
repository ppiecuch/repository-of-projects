#include "CWaveLoaderOGG.h"
#include "yonUtil.h"
#include "CWave.h"

#include "ILogger.h"

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

//#include <vector>
//using namespace std;  
#include "yonArray.h"

namespace yon{
namespace audio{

	bool CWaveLoaderOGG::checkFileExtension(const io::path& filename) const{
		return core::hasFileExtension(filename,"ogg");
	}
	bool CWaveLoaderOGG::checkFileHeader(io::IReadStream* file) const{
		//TODO
		return true;
	}
	audio::IWave* CWaveLoaderOGG::loadWave(io::IReadStream* file) const{
		vorbis_info *pInfo;
		OggVorbis_File oggFile;
		
		if (ov_open_callbacks(file->pointer(), &oggFile, NULL, 0, OV_CALLBACKS_NOCLOSE) != 0){
			Logger->warn(YON_LOG_FAILED_FORMAT,core::stringc("ov_open_callbacks(%s)",getFileName(file->getPath()).c_str()).c_str());
			return NULL;
		}
		pInfo=ov_info(&oggFile, -1);

		ENUM_WAVE_FORMAT format;
		if (pInfo->channels == 1)
			format = ENUM_WAVE_FORMAT_MONO16;
		else
			format = ENUM_WAVE_FORMAT_STEREO16;

		u32 freq = pInfo->rate;

		long bytes;
		long count=0;
		int bitStream;
		int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
		c8 buffer[32768];
		core::array<c8> result;
		//vector<c8> result;
		Logger->debug("start ov_read:%s\n",getFileName(file->getPath()).c_str());
		do{
			bytes = ov_read(&oggFile, buffer, 32768, endian, 2, 1, &bitStream);
			if (bytes < 0){
				ov_clear(&oggFile);  
				Logger->warn(YON_LOG_FAILED_FORMAT,core::stringc("Decode(%s)",getFileName(file->getPath()).c_str()).c_str());
				return NULL;
			}
			//result.insert(result.end(), buffer, buffer+bytes);//¿ìÁËÈý±¶
			result.insert(result.size(), buffer, bytes);
			count+=bytes;
		}while(bytes > 0);
		Logger->debug("end ov_read:%s\n",getFileName(file->getPath()).c_str());
		ov_clear(&oggFile);

		CWave* wave=new CWave(format,count,freq,result.pointer(),true,true);
		//CWave* wave=new CWave(format,count,freq,&result[0],true,true);
		Logger->debug("create wave completed:%s\n",getFileName(file->getPath()).c_str());
		return wave;
	}

	IWaveLoader* createWaveLoaderOGG()
	{
		return new CWaveLoaderOGG();
	}
}
}