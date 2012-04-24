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

	//������Դ�ͻ�����
	ALuint bids[2], sids[2];
	alGenSources(2, sids);
	alGenBuffers(2, bids);

	//ȡ��pcm����,�û�������������
	ALvoid *data;
	ALsizei size, bits, freq;
	ALenum format;
	ALboolean loop;

	alutLoadWAVFile("test1.wav", &format, &data, &size, &freq, &loop);
	alBufferData(bids[0], format, data, size, freq);
	alSourcei(sids[0], AL_BUFFER, bids[0]);

	alutLoadWAVFile("test2.wav", &format, &data, &size, &freq, &loop);
	alBufferData(bids[1], format, data, size, freq);
	alSourcei(sids[1], AL_BUFFER, bids[1]);

	//������ԴȻ��ȴ�ֱ�����
	//Ȼ���ͷ���
	alSourcePlay(sids[0]);
	alSourcePlay(sids[1]);
	ALint states[2];
	do{
		alGetSourcei(sids[0], AL_SOURCE_STATE, &states[0]);
		alGetSourcei(sids[1], AL_SOURCE_STATE, &states[1]);
	}while(states[0] == AL_PLAYING||states[1] == AL_PLAYING);

	alDeleteSources(2, sids);
	alDeleteBuffers(2, bids);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(cc);
	alcCloseDevice(dev);

	return 0;
}