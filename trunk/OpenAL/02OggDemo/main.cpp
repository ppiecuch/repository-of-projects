#include <al.h>
#include <alc.h>
#include <alu.h>
#include <alut.h>

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include <cstdio>  
#include <iostream>  
#include <vector>
using namespace std;  

#include <stdio.h>

FILE* oggFile;
int	m_nBufSize ;
ALenum format;
OggVorbis_File  oggStream;
vorbis_info*    vorbisInfo;

size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	return fread(ptr, size, nmemb, (FILE*)datasource);
}

int ov_seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	return fseek((FILE*)datasource, (long)offset, whence);
}

int ov_close_func(void *datasource)
{
	return fclose((FILE*)datasource);
}

long ov_tell_func(void *datasource)
{
	return ftell((FILE*)datasource);
}

void Swap(short &s1, short &s2) ;

unsigned long DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels)
{
	int current_section;
	long lDecodeSize;
	unsigned long ulSamples;
	short *pSamples;

	unsigned long ulBytesDone = 0;
	while (1)
	{
		lDecodeSize = ov_read(psOggVorbisFile, pDecodeBuffer + ulBytesDone, ulBufferSize - ulBytesDone, 0, 2, 1, &current_section);
		if (lDecodeSize > 0)
		{
			ulBytesDone += lDecodeSize;

			if (ulBytesDone >= ulBufferSize)
				break;
		}
		else
		{
			break;
		}
	}

	// Mono, Stereo and 4-Channel files decode into the same channel order as WAVEFORMATEXTENSIBLE,
	// however 6-Channels files need to be re-ordered
	if (ulChannels == 6)
	{		
		pSamples = (short*)pDecodeBuffer;
		for (ulSamples = 0; ulSamples < (ulBufferSize>>1); ulSamples+=6)
		{
			// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
			// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
			Swap(pSamples[ulSamples+1], pSamples[ulSamples+2]);
			Swap(pSamples[ulSamples+3], pSamples[ulSamples+5]);
			Swap(pSamples[ulSamples+4], pSamples[ulSamples+5]);
		}
	}

	return ulBytesDone;
}

void Swap(short &s1, short &s2)
{
	short sTemp = s1;
	s1 = s2;
	s2 = sTemp;
}


bool openOgg(const char* path )
{
	
	ov_callbacks	sCallbacks;
	
	vorbis_comment* vorbisComment;

	unsigned long	ulDataSize ;
	unsigned long	ulFrequency ;
	unsigned long	ulFormat ;
	ulDataSize = 0;
	ulFrequency = 0;
	ulFormat = 0;

	if(!(oggFile = fopen(path, "rb")))
		return false;

	//if(fn_ov_open(oggFile, &oggStream, NULL, 0) < 0)
	//{
	//    fclose(oggFile);
	//}

	sCallbacks.read_func = ov_read_func;
	sCallbacks.seek_func = ov_seek_func;
	sCallbacks.close_func = ov_close_func;
	sCallbacks.tell_func = ov_tell_func;

	if (ov_open_callbacks(oggFile, &oggStream, NULL, 0, sCallbacks) < 0)
	{
		fclose(oggFile);
		return false;
	}

	vorbisInfo = ov_info(&oggStream, -1);
	vorbisComment = ov_comment(&oggStream, -1);

	if( vorbisInfo->channels == 1 )
	{
		format = AL_FORMAT_MONO16;

		m_nBufSize = vorbisInfo->rate >> 1 ;
		m_nBufSize -= ( m_nBufSize % 2 ) ;
	}
	else if( vorbisInfo->channels == 2 )
	{
		format = AL_FORMAT_STEREO16;

		m_nBufSize = vorbisInfo->rate ;
		m_nBufSize -= ( m_nBufSize % 4 ) ;
	}
	else if (vorbisInfo->channels == 4)
	{
		format = alGetEnumValue("AL_FORMAT_QUAD16");

		m_nBufSize = ulFrequency * 2;
		m_nBufSize -= (m_nBufSize % 8);
	}
	else if (vorbisInfo->channels == 6)
	{
		format = alGetEnumValue("AL_FORMAT_51CHN16");

		m_nBufSize = ulFrequency * 3;
		m_nBufSize -= (m_nBufSize % 12);
	}

	return true;
}

void SetListenervalues()
{
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

	alListenerfv(AL_POSITION,    ListenerPos);
	alListenerfv(AL_VELOCITY,    ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}

#define BUFFER_SIZE     32768       // 32 KB buffers  
bool LoadOGG(char *name, vector<char> &buffer, ALenum &format, ALsizei &freq)  
{  
	int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian  
	int bitStream;  
	long bytes;  
	char array[BUFFER_SIZE];                // Local fixed size array  
	FILE *f;  

	f = fopen(name, "rb");  

	if (f == NULL)  
		return false;   

	vorbis_info *pInfo;  
	OggVorbis_File oggFile;  

	//ov_open is one of three initialization functions used to initialize an OggVorbis_File structure and prepare a bitstream for playback.
	//The first argument must be a file pointer to an already opened file or pipe 
	//(it need not be seekable--though this obviously restricts what can be done with the bitstream). 
	//vf should be a pointer to the OggVorbis_File structure -- this is used for ALL the externally visible libvorbisfile functions. 
	//Once this has been called, the same OggVorbis_File struct should be passed to all the libvorbisfile functions.
	//The vf structure initialized using ov_fopen() must eventually be cleaned using ov_clear(). 
	//Once a FILE * handle is passed to ov_open() successfully, the application MUST NOT fclose() or in any other way manipulate that file handle. 
	//Vorbisfile will close the file in ov_clear(). 
	//If the application must be able to close the FILE * handle itself, see ov_open_callbacks() with the use of OV_CALLBACKS_NOCLOSE.
	// Try opening the given file  
#if WITH_CLOSE
	if (ov_open(f, &oggFile, NULL, 0) != 0)  
		return false;   
#else
	if (ov_open_callbacks(f, &oggFile, NULL, 0, OV_CALLBACKS_NOCLOSE) != 0)
		return false;   
#endif

	pInfo = ov_info(&oggFile, -1);  

	if (pInfo->channels == 1)  
		format = AL_FORMAT_MONO16;  
	else  
		format = AL_FORMAT_STEREO16;  

	freq = pInfo->rate;

	// Get playtime in secs
	//mPlayTime = static_cast<float>(ov_time_total(&mOggStream, -1));

	//http://droughtdeer.googlecode.com/svn/trunk/OgreOggSound/src/OgreOggStaticSound.cpp
	unsigned int mBufferSize=0;
	/*switch(pInfo->channels)
	{
	case 1:
		{
			//mFormat = AL_FORMAT_MONO16;
			// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
			mBufferSize = pInfo->rate >> 1;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 2);
		}
		break;
	case 2:
		{
			//mFormat = AL_FORMAT_STEREO16;
			// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
			mBufferSize = pInfo->rate;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 4);
		}
		break;
	case 4:
		{
			//mFormat = alGetEnumValue("AL_FORMAT_QUAD16");
			//if (!mFormat) return false;
			// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
			mBufferSize = pInfo->rate * 2;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 8);
		}
		break;
	case 6:
		{
			//mFormat = alGetEnumValue("AL_FORMAT_51CHN16");
			//if (!mFormat) return false;
			// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
			mBufferSize = pInfo->rate * 3;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 12);
		}
		break;
	case 7:
		{
			//mFormat = alGetEnumValue("AL_FORMAT_61CHN16");
			//if (!mFormat) return false;
			// Set BufferSize to 250ms (Frequency * 16 (16bit 7-channel) divided by 4 (quarter of a second))
			mBufferSize = pInfo->rate * 4;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 16);
		}
		break;
	case 8:
		{
			//mFormat = alGetEnumValue("AL_FORMAT_71CHN16");
			//if (!mFormat) return false;
			// Set BufferSize to 250ms (Frequency * 20 (16bit 8-channel) divided by 4 (quarter of a second))
			mBufferSize = pInfo->rate * 5;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 20);
		}
		break;
	default:
		{
			// Couldn't determine buffer format so log the error and default to mono
			//Ogre::LogManager::getSingleton().logMessage("!!WARNING!! Could not determine buffer format!  Defaulting to MONO");

			//mFormat = AL_FORMAT_MONO16;
			// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
			mBufferSize = pInfo->rate >> 1;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 2);
		}
		break;
	}*/
	unsigned int count=0;
	do  
	{   
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);  

		if (bytes < 0)  
		{  
			ov_clear(&oggFile);  
			//cerr << "Error decoding " << fileName << "..." << endl;  
			printf("Error decoding:%s\n",name);
			exit(-1);  
		}  

		buffer.insert(buffer.end(), array, array + bytes);  

		count+=bytes;
	}  
	while (bytes > 0);

	printf("mBufferSize:%d,bytes:%d\n",mBufferSize,count);

	ov_clear(&oggFile);
#if WITH_CLOSE
#else
	fclose(f);
#endif
	return true;   
}  

int main(void)
{
	/*//打开设备，创建设备
	ALCdevice *dev = alcOpenDevice(NULL);
	ALCcontext *cc = alcCreateContext(dev, NULL);
	alcMakeContextCurrent(cc);

	//创建音源和缓冲器
	ALuint bid, sid;
	
	openOgg("bg.ogg");
	char* m_pDecodeBuf = new char[m_nBufSize] ;
	alGenSources(1, &sid);
	alGenBuffers(1, &bid);

	alSourcei (sid, AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcef (sid, AL_PITCH,           1.0    );
	alSourcef (sid, AL_GAIN,            1.0    );
	//alSource3f(sid, AL_DIRECTION,       0.0, 0.2, 0.0);
	alSource3f(sid, AL_POSITION,        0.0, 0.0, 0.0);
	alSource3f(sid, AL_VELOCITY,        0.0, 0.0, 0.2);
	//alSourcei (sid, AL_LOOPING,         m_bLoop      );
	alSourcei (sid, AL_LOOPING,         0    );
	SetListenervalues();

	
	unsigned long byteWritten = 0;
	byteWritten = DecodeOggVorbis( &oggStream , m_pDecodeBuf , m_nBufSize , vorbisInfo->channels ) ;

	if( byteWritten )
	{
		alBufferData(bid , format , m_pDecodeBuf , byteWritten , vorbisInfo->rate ) ;
	}

	//alSourceQueueBuffers(sid , 1 , &bid) ;
	alSourcei(sid, AL_BUFFER, bid);
	alSourcePlay(sid);



	//用音源关联缓冲器
	//alSourcei(sid, AL_BUFFER, bid);

	//播放音源然后等待直到完成
	//然后释放它
	//alSourcePlay(sid);
	ALint state;
	do{
		alGetSourcei(sid, AL_SOURCE_STATE, &state);
	}while(state == AL_PLAYING);

	alDeleteSources(1, &sid);
	alDeleteBuffers(1, &bid);

	ov_clear(&oggStream);
	delete []m_pDecodeBuf ;
	fclose(oggFile);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(cc);
	alcCloseDevice(dev);*/

	ALCdevice* pDevice;  
	ALCcontext* pContext;  

	ALint state;                            // The state of the sound source  
	ALuint bufferID;                        // The OpenAL sound buffer ID  
	ALuint sourceID;                        // The OpenAL sound source  
	ALenum format;                          // The sound data format  
	ALsizei freq;                           // The frequency of the sound data  
	vector<char> bufferData;                // The sound buffer data from file  

	ALCdevice *device;  
	ALCcontext *context;   

	device = alcOpenDevice(0);  
	context = alcCreateContext(device,0);  
	ALboolean initStatus = alcMakeContextCurrent(context);      

	//set distance mode
	alDistanceModel(AL_NONE);

	// Create sound buffer and source  
	alGenBuffers(1, &bufferID);  
	alGenSources(1, &sourceID);  

	// Set the source and listener to the same location  
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);  
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
	//Al.AL_PITCH Field. Specifies the pitch to be applied, either at source, or on mixer results, at listener. The accepted range is 0.5 to 2.0, the default value is 1.0.
	//源的AL_PITCH属性用于控制某一声音的相对音高。取值为1.0的时候，渲染的音源上无需调高。每减少50%会导致一个八度（-12半音）的音高变化。
	//alSourcef(sourceID, AL_PITCH, 0.5f);
	//alSourcef(sourceID, AL_GAIN, 0.2f);
	//AL_LOOPING is a flag that indicates that the source will not be in AL_STOPPED state once it reaches the end of last buffer in the buffer queue.
	//Instead, the source will immediately promote to AL_INITIAL and AL_PLAYING. The default vale is immediately promote to AL_INITIAL and AL_PLAYING.
	//The default value is AL_FALSE.
	//AL_LOOPING can be chaned on a source in any execution state.In particular, it can be changed on a AL_PLAYING source.
	//alSourcei(sourceID, AL_LOOPING,AL_TRUE);
	//OpenAL的核心是将声音的衰减表现为某一距离函数。OpenAL有一系列的距离模型可以在运行的时候选择。
	//函数alDistaneceModel()用于在不同的距离模型中进行了选择。默认的距离模型是AL_INVERSE_DISTANCE_CLAMPED，遵守下面的公式：
	//G_db=clamp(GAIN-20*log10(1+Rf*(dist-Rd)/Rd, MinG, MaxG))
	//此公式中Rf和Ed对应于音源的两个属性：AL_ROLLOFF_FACTOR和AL_RDFERENCE_DISTANCE。
	//MinG和MaxG分别对应于音源的最小增益属性AL_MIN_GAIN和最大增益属性AL_MAX_GAIN。
	//参考距离dist是listen体验增益（GAIN）的距离。依音源而定的rolloff系数（高低频规律性衰减系数）能够在值变化量的
	//负方向上改变音源的范围。当rolloff系数为0表明对于音源没有衰减。
	//alSourcef(sourceID, AL_ROLLOFF_FACTOR, 0.2);
	//alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_FALSE);

	// Load the OGG file into memory  
	LoadOGG("bg.ogg", bufferData, format, freq);  

	// Upload sound data to buffer  
	alBufferData(bufferID, format, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);  

	// Attach sound buffer to source  
	alSourcei(sourceID, AL_BUFFER, bufferID);  
	

	//alSourcef (sourceID, AL_GAIN, 1.0 );  

	// Finally, play the sound!!!  
	alSourcePlay(sourceID);  

	do  
	{  
		// Query the state of the souce  
		alGetSourcei(sourceID, AL_SOURCE_STATE, &state);  
	}  
	//while (state == AL_PLAYING);  
	while(state!=AL_STOPPED);

	// Clean up sound buffer and source  
	alDeleteSources(1, &sourceID);  
	alDeleteBuffers(1, &bufferID);  
	
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);  
	alcCloseDevice(device);

	return 0;  
}