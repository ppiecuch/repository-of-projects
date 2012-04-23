#include <al.h>
#include <alc.h>
#include <alu.h>
#include <alut.h>

#include <stdio.h>

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

	//创建音源和缓冲器
	ALuint bid, sid;
	alGenSources(1, &sid);
	alGenBuffers(1, &bid);

	//取得pcm数据,用缓冲区来关联它
	ALvoid *data;
	ALsizei size, bits, freq;
	ALenum format;
	ALboolean loop;

	alutLoadWAVFile("helloworld.wav", &format, &data, &size, &freq, &loop);
	alBufferData(bid, format, data, size, freq);

	//用音源关联缓冲器
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