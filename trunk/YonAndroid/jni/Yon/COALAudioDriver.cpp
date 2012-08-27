#include "COALAudioDriver.h"

#if defined(YON_AUDIO_MODE_OAL)

#include "CWaveLoaderWAV.h"
#include "CWaveLoaderOGG.h"
#include "COALSound.h"
#include "yonUtil.h"
#include "SDummySound.h"

#ifdef YON_COMPILE_WITH_WIN32
#include "alMain.h"
#include "alThunk.h"
#endif

#include "ILogger.h"

namespace yon{
namespace audio{
namespace oal{

	COALAudioDriver::COALAudioDriver(io::IFileSystem* fs)
		:m_pDevice(NULL),m_pContext(NULL),IAudioDriver(fs){
			const ALCchar *default_device;
			default_device = alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);
			Logger->info("using default device: %s\r\n", default_device);
			m_pDevice=alcOpenDevice(default_device);
			if(m_pDevice==NULL)
			{
				Logger->warn(YON_LOG_WARN_FORMAT,"failed to open sound device!");
				return;
			}
			m_pContext=alcCreateContext(m_pDevice, NULL);
			alcMakeContextCurrent(m_pContext);
			
			m_waveLoaders.push_back(createWaveLoaderWAV());
			m_waveLoaders.push_back(createWaveLoaderOGG());

			Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance COALAudioDriver");
	}
	COALAudioDriver::~COALAudioDriver(){
		u32 i=0;
		u32 size=0;
		size=m_waveLoaders.size();
		for(i=0;i<m_waveLoaders.size();++i)
			m_waveLoaders[i]->drop();
		Logger->info("Release %d/%d WaveLoader\n",i,size);

		size=m_sounds.size();
		for(i=0;i<m_sounds.size();++i)
			m_sounds[i].sound->drop();
		Logger->info("Release %d/%d Sound\n",i,size);

		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_pContext);
		alcCloseDevice(m_pDevice);
#ifdef YON_COMPILE_WITH_WIN32
		ReleaseALC();
		FreeALConfig();
		ALTHUNK_EXIT();
#endif
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release COALAudioDriver");
	}

	void COALAudioDriver::doze(){
		for(s32 i=0;i<m_sounds.size();++i){
			m_sounds[i].playing=m_sounds[i].sound->isPlaying();
			if(m_sounds[i].playing)
				m_sounds[i].sound->pause();
		}
	}
	void COALAudioDriver::wake(){
		for(s32 i=0;i<m_sounds.size();++i){
			if(m_sounds[i].playing)
				m_sounds[i].sound->play();
		}
	}

	bool COALAudioDriver::onEvent(const event::SEvent& event)
	{
		switch(event.type)
		{
		case event::ENUM_EVENT_TYPE_SYSTEM:
			switch(event.systemInput.type)
			{
			case event::ENUM_SYSTEM_INPUT_TYPE_DOZE:
				doze();
				break;
			case event::ENUM_SYSTEM_INPUT_TYPE_WAKE:
				wake();
				break;
			}
			break;
		}
		return false;
	}

	ISound* COALAudioDriver::getSound(const io::path& filename){
		ISound* sound = findSound(filename);
		if (sound){
			Logger->debug("getSound(%s) finded!\n",getFileName(filename).c_str());
			return sound;
		}

		io::IReadStream* file = m_pFileSystem->createAndOpenReadFileStream(filename);

		if(!file)
		{
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("getSound(%s) failed,for file do not exist!",filename.c_str()).c_str());
			return NULL;
		}

		sound = loadSoundFromFile(file);
		file->drop();

		if (sound)
		{
			addSound(sound);
			sound->drop(); // drop it because we created it, one grab too much
		}
		else
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("addSound for %s failed!",filename.c_str()).c_str());
		return sound;
	}

	ISound* COALAudioDriver::findSound(const io::path& filename){
		/*const io::path absolutePath = m_pFileSystem->getResourcePath(filename);
		for(u32 i=0;i<m_sounds.size();++i){
			//Logger->debug("check %s==%s\n",m_sounds[i]->getPath().c_str(),absolutePath.c_str());
			if(m_sounds[i].sound->getPath()==absolutePath){
				return m_sounds[i].sound;
			}
		}*/
		SWave wave;
		SDummySound dummy(filename);
		wave.sound = &dummy;
		wave.playing=false;

		s32 index = m_sounds.binary_search(wave);
		if (index != -1)
			return m_sounds[index].sound;

		const io::path absolutePath = m_pFileSystem->getResourcePath(filename);
		if(absolutePath!="")
		{
			dummy.setPath(absolutePath);
			index = m_sounds.binary_search(wave);
			if (index != -1)
				return m_sounds[index].sound;
		}
		return NULL;
	}

	audio::ISound* COALAudioDriver::loadSoundFromFile(io::IReadStream* file){
		ISound* sound = NULL;
		Logger->debug("start load sound:%s\n",getFileName(file->getPath()).c_str());
		IWave* wave = createWaveFromFile(file);

		if (wave)
		{
			sound = new COALSound(wave, file->getPath());
			Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("end load sound:%s",getFileName(file->getPath()).c_str()).c_str());
			wave->drop();
		}
		else
		{
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("loadSoundFromFile for %s failed!",getFileName(file->getPath()).c_str()).c_str());
		}

		return sound;
	}

	IWave*  COALAudioDriver::createWaveFromFile(const io::path& filename){
		io::IReadStream* file = m_pFileSystem->createAndOpenReadFileStream(filename);
		IWave* wave=createWaveFromFile(file);
		file->drop();
		return wave;
	}
	IWave*  COALAudioDriver::createWaveFromFile(io::IReadStream* file){
		if (!file)
			return NULL;

		IWave* wave = NULL;

		u32 i;

		for (i=0; i<m_waveLoaders.size(); ++i)
		{
			if (m_waveLoaders[i]->checkFileExtension(file->getPath()))
			{
				file->seek(0);
				wave = m_waveLoaders[i]->loadWave(file);
				if (wave)
					return wave;
			}
		}

		for (i=0; i<m_waveLoaders.size(); ++i)
		{
			file->seek(0);
			if (m_waveLoaders[i]->checkFileHeader(file))
			{
				file->seek(0);
				wave = m_waveLoaders[i]->loadWave(file);
				if (wave)
					return wave;
			}
		}

		return NULL;
	}

	void COALAudioDriver::addSound(audio::ISound* sound){
		if (sound)
		{
			sound->grab();
			SWave wave;
			wave.sound=sound;
			wave.playing=false;
			m_sounds.push_back(wave);
		}
	}

	IListener* COALAudioDriver::getListener(){
		//TODO
		return NULL;
	}
	bool COALAudioDriver::checkError(const c8* file,s32 line){
		//TODO
		return false;
	}
}
	IAudioDriver* createAudioDriver(io::IFileSystem* fs){
		return new oal::COALAudioDriver(fs);
	}
}
}
#endif