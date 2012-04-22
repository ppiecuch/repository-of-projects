#include <al.h>
#include <alc.h>
#include <alu.h>
#include <alut.h>

#include <stdio.h>

int main(void)
{
	//打开设备，创建设备
	ALCdevice *dev = alcOpenDevice(NULL);
	ALCcontext *cc = alcCreateContext(dev, NULL);
	alcMakeContextCurrent(cc);

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

	alDeleteSources(1, &sid);
	alDeleteBuffers(1, &bid);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(cc);
	alcCloseDevice(dev);

	getchar();

	return 0;
}