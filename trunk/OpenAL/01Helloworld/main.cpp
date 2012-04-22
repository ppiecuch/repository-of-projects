#include <al.h>
#include <alc.h>
#include <alu.h>
#include <alut.h>

#include <stdio.h>

int main(void)
{
	//���豸�������豸
	ALCdevice *dev = alcOpenDevice(NULL);
	ALCcontext *cc = alcCreateContext(dev, NULL);
	alcMakeContextCurrent(cc);

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

	alDeleteSources(1, &sid);
	alDeleteBuffers(1, &bid);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(cc);
	alcCloseDevice(dev);

	getchar();

	return 0;
}