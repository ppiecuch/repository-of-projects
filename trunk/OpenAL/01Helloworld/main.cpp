#include <al.h>
#include <alc.h>
#include <alu.h>
#include <alut.h>

#include <stdio.h>

int main(void)
{
	//���豸�������豸
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

	//������Դ�ͻ�����
	ALuint bid, sid;
	alGenSources(1, &sid);
	alGenBuffers(1, &bid);

	//ȡ��pcm����,�û�������������
	ALvoid *data;
	ALsizei size, bits, freq;
	ALenum format;
	ALboolean loop;

	alutLoadWAVFile("helloworld.wav", &format, &data, &size, &freq, &loop);
	alBufferData(bid, format, data, size, freq);

	//����Դ����������
	alSourcei(sid, AL_BUFFER, bid);

	//������ԴȻ��ȴ�ֱ�����
	//Ȼ���ͷ���
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