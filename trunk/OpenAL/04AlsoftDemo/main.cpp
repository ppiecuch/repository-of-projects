#include <al.h>
#include <alc.h>
#include <alu.h>

#include <vector>
#include <iostream>

#include <stdio.h>


typedef struct {
	char  riff[4];//'RIFF'
	unsigned int riffSize;
	char  wave[4];//'WAVE'
	char  fmt[4];//'fmt '
	unsigned int fmtSize;
	unsigned short format;
	unsigned short channels;
	unsigned int samplesPerSec;
	unsigned int bytesPerSec;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char  data[4];//'data'
	unsigned int dataSize;
}BasicWAVEHeader;


//WARNING: This Doesn't Check To See If These Pointers Are Valid
char* readWAV(char* filename,BasicWAVEHeader* header){
	char* buffer = 0;
	FILE* file = fopen(filename,"rb");
	if (!file) {
		return 0;
	}

	if (fread(header,sizeof(BasicWAVEHeader),1,file)){
		if (!(//these things *must* be valid with this basic header
			memcmp("RIFF",header->riff,4) ||
			memcmp("WAVE",header->wave,4) ||
			memcmp("fmt ",header->fmt,4)  ||
			memcmp("data",header->data,4)
			)){

				buffer = (char*)malloc(header->dataSize);
				if (buffer){
					if (fread(buffer,header->dataSize,1,file)){
						fclose(file);
						return buffer;
					} 
					free(buffer);
				} 
		}
	}
	fclose(file);
	return 0;
}

ALuint createBufferFromWave(char* data,BasicWAVEHeader header){

	ALuint buffer = 0;
	ALuint format = 0;
	switch (header.bitsPerSample){
	case 8:
		format = (header.channels == 1) ? AL_FORMAT_MONO8 : AL_FORMAT_STEREO8;
		break;
	case 16:
		format = (header.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		break;
	default:
		return 0;
	}

	alGenBuffers(1,&buffer);
	alBufferData(buffer,format,data,header.dataSize,header.samplesPerSec);
	return buffer;
}

int main(void)
{
	//打开设备，创建设备
	//open default device
	ALCdevice *dev = alcOpenDevice(NULL);
	//create context
	ALCcontext *cc = alcCreateContext(dev, NULL);
	//set active constext
	alcMakeContextCurrent(cc);

	const char *actual_devicename;
	actual_devicename=alcGetString(dev,ALC_DEVICE_SPECIFIER);
	int major,minor;

	alcGetIntegerv(dev,ALC_MAJOR_VERSION,sizeof(int),&major);
	alcGetIntegerv(dev,ALC_MINOR_VERSION,sizeof(int),&minor);

	printf("device: <%s>,openal version: %d.%d\n",actual_devicename,major,minor);

	const ALchar* version=alGetString(AL_VERSION);
	const ALchar* renderer=alGetString(AL_RENDERER);
	const ALchar* vendor=alGetString(AL_VENDOR);
	const ALchar* extensions=alGetString(AL_EXTENSIONS);
	printf("version:%s\n",version);
	printf("renderer:%s\n",renderer);
	printf("vendor:%s\n",vendor);
	printf("extensions:%s\n",extensions);

	ALCint count;
	alcGetIntegerv( dev, ALC_ATTRIBUTES_SIZE, 1, &count);
	std::vector<ALCint> attrs(count);
	alcGetIntegerv( dev, ALC_ALL_ATTRIBUTES, count, &attrs[0] );
	for(int i=0; i<attrs.size(); ++i)
	{
		if( attrs[i] == ALC_MONO_SOURCES )
		{
			std::cout << "max mono sources: " << attrs.at(i+1) << std::endl;
		}
		else if( attrs[i] == ALC_STEREO_SOURCES )
		{
			std::cout << "max stereo sources: " << attrs.at(i+1) << std::endl;
		}
	}

	ALCint nummono, numstereo;
	alcGetIntegerv(dev, ALC_MONO_SOURCES, 1, &nummono);
	alcGetIntegerv(dev, ALC_STEREO_SOURCES, 1, &numstereo);
	std::cout << "max mono sources: " << nummono << std::endl;
	std::cout << "max stereo sources: " << numstereo << std::endl;

	//创建音源和缓冲器
	ALuint bid, sid;
	alGenSources(1, &sid);
	alGenBuffers(1, &bid);

	BasicWAVEHeader header;
	char* data = readWAV("helloworld.wav",&header);
	if (data){
		//Now We've Got A Wave In Memory, Time To Turn It Into A Usable Buffer
		bid = createBufferFromWave(data,header);
		if (!bid){
			free(data);
			return -1;
		}

	} else {
		return -1;
	}

	alSourcei(sid, AL_BUFFER, bid);

	//播放音源然后等待直到完成
	//然后释放它
	alSourcePlay(sid);
	ALint state;
	do{
		alGetSourcei(sid, AL_SOURCE_STATE, &state);
	}while(state == AL_PLAYING);

	//A playing source can be deleted--the source will be stopped automatically and the deleted.
	alDeleteSources(1, &sid);
	//A buffer which is attached to a source can not be deleted.
	alDeleteBuffers(1, &bid);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(cc);
	alcCloseDevice(dev);

	getchar();

	return 0;
}