// ra_ogg.cpp : 定义 DLL 应用程序的导出函数。
//

#pragma once 
#include "stdafx.h"
#include <string>
using namespace std;
#include "SongPlay.h"
#include "ogg/ogg.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"
#include "eq.h"


#define MAX_CHANNELS  8

static const			DWORD SIZE_DS_BUFFER = 32*1024;			//初始化一个默认值作为缓冲区的大小。以后可以修改此值来改变缓冲区大小；

/**************创建事件通知****************************************************************************/
static HANDLE			_eventNotify[3];						//创建DSound缓冲区通知事件,_eventNotify[3]为用户stop事件通知
HANDLE					_eventPipe = NULL;						//创建命名事件的句柄,用于播放网络数据流时同客户端的同步
DWORD					_eventResult = 0;						//消息等待返回值,用于同步
/**************创建事件通知****************************************************************************/

/**************DSound缓冲区创建************************************************************************/
HWND					_hWnd;									//主窗口句柄
DWORD					_uiNotifyThreadID = 0;					//读取线程ID
HANDLE					_hNotifyThread = NULL;					//读取线程句柄
WAVEFORMATEX			_fmtWave;								//缓冲区wav格式
DSBPOSITIONNOTIFY		_notifyPos[2];							//设置偏移地址通知，暂且设置两个。需要在添加；
IDirectSound8			*_pDS = NULL;							//设备对象；
IDirectSoundBuffer8		*_pDSBuffer = NULL;						//缓冲区对象；
IDirectSoundNotify8		*_pDSNotify = NULL;						//消息通知对象；
/**************DSound缓冲区创建************************************************************************/

/**********************ogg 定义************************************************************************/
OggVorbis_File				*m_OggVorbisFile = NULL;
//int							m_CurrentSection;
BOOL						m_bMono = FALSE;					//是否是单身道 FALSE 是双声道
DWORD						sampleRate;
WORD						numChannels;
//unsigned char				m_buffer[40000];
//unsigned int				m_buflen;
//unsigned char				m_pcmbuffer[SIZE_DS_BUFFER/2];
int							current_section;

//HANDLE						m_FileHandle = NULL;					//文件句柄
FILE						*inFile		= NULL;						//文件指针

//EQ
BOOL						bEQ = FALSE;
BOOL						bReset = FALSE;

//函数声明部分：
//void	GetDataFromFile(void **buffer,DWORD *bytes);
//BOOL	SetDataPosition(LARGE_INTEGER	*SampleNum);
//void	OpenFile();
//void	CloseFile();

//size_t vorbisRead(void *buffer,size_t size,size_t amount,void *file);
//int	   vorbisSeek(void *file,ogg_int64_t newpos,int set);
//int	   vorbisClose(void *file);
//long   vorbisTell(void *file);
/******************************************************************************************************/



/*************************播放数据结构定义*************************************************************/


/******************************************************************************************************/

/***********************更新读数据缓冲区***************************************************************/

/*******************************************************************************************************/



/************************更新读数据缓冲区计数***********************************************************/

/*******************************************************************************************************/



/************************更新初始化buff*****************************************************************/

/********************************************************************************************************/




/************************解码****************************************************************************/
int FillBuff( char *buff,unsigned int size)
/********************************************************************************************************/
{
	int length = size;
	int curr_pos = 0;
	do{
		int bytes_read = ov_read(m_OggVorbisFile,buff + curr_pos,length,0,2,1,&current_section);
		length -= bytes_read;
		curr_pos += bytes_read;
		if(bytes_read<=0)
			return -1;

	}while(length >0);
	return curr_pos;

}

/**********************装载数据解码放到DSound缓冲区*****************************************************/
int LoadDSData(unsigned int start,unsigned int count)	//填充缓冲区
/*******************************************************************************************************/
{
	
	LPVOID aptr1 = NULL;
	LPVOID aptr2 = NULL;
	DWORD abyte1 = 0;
	DWORD abyte2 = 0;
	
	
	HRESULT hr = _pDSBuffer->Lock(start,count,&aptr1,&abyte1,&aptr2,&abyte2,0);
	if(hr!=DS_OK)
	{	hr = _pDSBuffer->Restore();
		if(hr!=DS_OK)
		{
			return -1;
		}
	}
	unsigned int Temp = count;
	char *buff = new char[Temp];
	int length_read = FillBuff(buff,Temp);
	if(length_read == -1)
	{
		delete []buff;
		buff = NULL;
		return -1;			//播放结束
	}
	if(bEQ)
	{
		if(!bReset)
		{
			do_equliazer((short*)buff,length_read,44100,2);
		}
	}
	memcpy(aptr1,buff,length_read);
	if(aptr2!=NULL)
	{
		int ret = FillBuff(buff,Temp);
		if(ret == -1)
		{
			return -1;
		}
		memcpy(aptr2,buff,Temp);
	}
	_pDSBuffer->Unlock(aptr1,abyte1,aptr2,abyte2);
	delete []buff;
	buff = NULL;
	return 0;
}

/*******************释放DSound*****************************************************************/
void ReleaseDSound()					 //释放设备对象；
/**********************************************************************************************/
{
	if(_pDS != NULL)
	{
		_pDS->Release();
		_pDS = NULL;
	}
}

/*******************用户点击停止时释放线程*****************************************************/
void ReleaseNotifyThreadStop()
/**********************************************************************************************/
{
	if(_hNotifyThread != NULL)
	{
		SetEvent(_eventNotify[2]);
		Sleep(10);
		WaitForSingleObject(_hNotifyThread,INFINITE);
		TerminateThread(_hNotifyThread,0);
		CloseHandle(_hNotifyThread);	
		_hNotifyThread = NULL;
	}
	for(int i=0;i<3;i++)                  //此处，以后若增加了事件内核函数，要同时做出改变。
	{
		if(_eventNotify[i] != NULL)
		{
			CloseHandle(_eventNotify[i]);
			_eventNotify[i] = NULL;
		}
	}

	if(_pDSNotify != NULL)
	{
		_pDSNotify->Release();
		_pDSNotify = NULL;
	}
}

/*****************播放结束时释放线程***********************************************************/
void ReleaseNotifyThreadPlayOver()
/**********************************************************************************************/
{
	if(_hNotifyThread != NULL)
	{
		CloseHandle(_hNotifyThread);	
		_hNotifyThread = NULL;	
	}
	for(int i=0;i<3;i++)                  //此处，以后若增加了事件内核函数，要同时做出改变。
	{
		if(_eventNotify[i] != NULL)
		{
			CloseHandle(_eventNotify[i]);
			_eventNotify[i] = NULL;
		}
	}

	if(_pDSNotify != NULL)
	{
		_pDSNotify->Release();
		_pDSNotify = NULL;
	}
}

/******************释放Sound缓冲区*************************************************************/
void ReleaseDSoundBuffer()				//释放设备缓冲区对象；
/**********************************************************************************************/
{
	if(_pDSBuffer != NULL)
	{
		_pDSBuffer->Stop();
		_pDSBuffer->Release();
		_pDSBuffer = NULL;
	}
}

/**************释放解码库**********************************************************************/
void Release()
/**********************************************************************************************/
{
	ReleaseDSoundBuffer();
	if(m_OggVorbisFile)
	{
		ov_clear(m_OggVorbisFile);
		delete m_OggVorbisFile;
		m_OggVorbisFile = NULL;
	}
	if(inFile != NULL)
	{
		fclose(inFile);
		inFile = NULL;
	}
	uninit_equliazer();
	ReleaseDSound();
	
}

int Stop()										//停止；
{
	if(_pDSBuffer == NULL)
	{
		return -1;	
	}		
	_pDSBuffer->Stop();
	ReleaseNotifyThreadStop();
	Release();
	return 0;
}
int PlayOverStop()
{
	if(_pDSBuffer == NULL)
	{
		return -1;	
	}		
	_pDSBuffer->Stop();
	ReleaseNotifyThreadPlayOver();
	Release();
	::PostMessage(_hWnd,WM_PLAYOVER,NULL,NULL);
	return 0;
}
int PlayOver()             //播放暂停，在获得缓冲区通知时先暂停；
{
	
	return PlayOverStop();
}


/********************缓冲区通知线程************************************************************/
static DWORD NotifyHandleProc(LPVOID param)			//通知时间处理过程函数；
/**********************************************************************************************/
{
	try{
	while(true)
	{
		DWORD ret = WaitForMultipleObjects(3,_eventNotify,FALSE,INFINITE/*,QS_ALLEVENTS*/);
		if(ret == WAIT_FAILED)
		{
			return -1;
		}

		DWORD notify = ret - WAIT_OBJECT_0;
		if(notify == 0)
		{
			if(LoadDSData(0,SIZE_DS_BUFFER/2)!=0)
				break;
		}
		else if(notify == 1)
		{
			if(LoadDSData(SIZE_DS_BUFFER/2,SIZE_DS_BUFFER/2)!=0)
				break;
		}
		else if(notify == 2)
		{
			return 0;
		}
		else
		{
			continue;
		}
	}
	}
	catch(...)
	{

	}
	PlayOver();
	return 0;
}

int CreateDSound()         //创建设备对象；
{
	HRESULT hr = DirectSoundCreate8(NULL,&_pDS,NULL);
	if(hr != DS_OK)
	{
		return -1 ;
	}
	hr = _pDS->SetCooperativeLevel(_hWnd,DSSCL_NORMAL);
	if(hr != DS_OK)
	{
		return -1;
	}
	return 0;
}

int Init(HWND hwnd)                       //初始化应用程序窗口，便于设置协作级别；初始化构建均衡器对象。
{
	_hWnd = hwnd;
	return CreateDSound();
}

/*******************设置缓冲区格式*************************************************************/
int SetWaveFormat()				//设置WMV格式；
/**********************************************************************************************/
{
	
	
	memset(&_fmtWave,0,sizeof(WAVEFORMATEX));
	_fmtWave.wFormatTag = WAVE_FORMAT_PCM;
	_fmtWave.nChannels = numChannels;
	_fmtWave.nSamplesPerSec = sampleRate;
	_fmtWave.wBitsPerSample = 16;
	_fmtWave.nBlockAlign = 2 *numChannels;
	_fmtWave.nAvgBytesPerSec = sampleRate *2*numChannels;
	
	
	return 0;
}

/**********************创建通知线程和监听事件**************************************************/
int CreateNotifyThread()           //创建消息通知线程；
/**********************************************************************************************/
{
	_eventNotify[0] = CreateEvent(NULL,FALSE,FALSE,NULL);
	_eventNotify[1] = CreateEvent(NULL,FALSE,FALSE,NULL);
	_eventNotify[2] = CreateEvent(NULL,FALSE,FALSE,NULL);

	if(_hNotifyThread!=NULL)
	{
		CloseHandle(_hNotifyThread);
		_hNotifyThread = NULL;
	}
	_hNotifyThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)NotifyHandleProc,
									NULL,0,&_uiNotifyThreadID);
	if(_hNotifyThread == NULL)
	{
		return -1;
	}

	HRESULT hr = _pDSBuffer->QueryInterface(IID_IDirectSoundNotify8,(void**)&_pDSNotify);
	if(hr != DS_OK)
	{
		return -1;
	}

	_notifyPos[0].dwOffset = (SIZE_DS_BUFFER /2)-1;     //暂且设置偏移在缓冲区中间位置，以后可以调整。
	_notifyPos[0].hEventNotify = _eventNotify[0];

	_notifyPos[1].dwOffset = SIZE_DS_BUFFER-1;          //暂且设置第二个偏移在缓冲区尾位置，以后可以调整。
	_notifyPos[1].hEventNotify = _eventNotify[1];

	hr = _pDSNotify->SetNotificationPositions(2,_notifyPos);    //此处若增加了偏移位置。要对数量做出改变。
	if(hr != DS_OK)
	{
		return -1;
	}
	return 0;
}




int InitDSData()
{
	
	if(LoadDSData(0,SIZE_DS_BUFFER) != 0)
	{
		return -1;
	}
	return 0;
}


/***********************创建DSound播放缓冲区***************************************************/
int CreateDSoundBuffer()   //创建设备缓冲区对象；
/**********************************************************************************************/
{
	IDirectSoundBuffer *psBuffer = NULL;

	DSBUFFERDESC  desc;    //缓冲区描述结构体；
	memset(&desc,0,sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_LOCSOFTWARE |DSBCAPS_CTRLPAN ;
	desc.dwBufferBytes = SIZE_DS_BUFFER;
	desc.lpwfxFormat = &_fmtWave;

	if(_pDSBuffer != NULL)
	{
		_pDSBuffer->Release();
	}

	HRESULT hr = _pDS->CreateSoundBuffer(&desc,&psBuffer,NULL);
	if(hr != DS_OK)
	{
		return -1;
	}
	hr = psBuffer->QueryInterface(IID_IDirectSoundBuffer8,(void**)&_pDSBuffer);
	psBuffer->Release();
	if(hr != DS_OK)
	{
		return -1;
	}
	
	if(CreateNotifyThread()!= 0)
	{
		return -1;
	}

	if(InitDSData()!= 0)
	{
		return -1;
	}

	_pDSBuffer->SetCurrentPosition(0);
	return 0;
}


bool IsReady()                      //是否创建完毕；
{
	return (_pDS != NULL &&_pDSBuffer != NULL);
}

int Play()                                 //播放；
{
	if(_pDSBuffer == NULL)
	{
		return -1;
	}	
	_pDSBuffer->Play(0,0,DSBPLAY_LOOPING);
	
	return 0;
}

int Pause()                                //暂停；
{
	if(_pDSBuffer == NULL)
	{
		return -1;
	}
	_pDSBuffer->Stop();
	return 0;
}



bool IsPlaying()                     //判断是否正在播放 true = 播放；
{
	if(_pDSBuffer == NULL)
	{
		return false;
	}

	DWORD status = 0;
	HRESULT hr = _pDSBuffer->GetStatus(&status);
	if(hr != DS_OK)
	{
		return false;
	}
	
	return ((status & DSBSTATUS_PLAYING) == DSBSTATUS_PLAYING ? true:false);
}

int Duration(long *time)							//持续播放时间；
{
	//*time = fileread/_fmtWave.wBitsPerSample;
	return 0;
}

int SetVolum(long vol)								//设置缓冲区流音量；
{
	HRESULT hr = DSERR_GENERIC;
	if(vol>DSBVOLUME_MAX)
		vol = DSBVOLUME_MAX;
	if(vol<DSBVOLUME_MIN)
		vol = DSBVOLUME_MIN;
	
	if(_pDSBuffer != NULL)
	{
		hr = _pDSBuffer->SetVolume(vol);
		if(hr != S_OK)
			return -1;
	}
	else
	{
		return -1;
	}
	return 0;
}

int GetVolum(long *vol)								 //获取缓冲区流音量；
{
	HRESULT hr = DSERR_GENERIC;
	if(_pDSBuffer!=NULL)
	{
		hr = _pDSBuffer->GetVolume(vol);
		if(hr != S_OK)
			return -1;
	}
	else
		return -1;
	return 0;
}

int SetPlayPos(unsigned int pos)					//设置播放位置；  
{
	double position = (pos*1.0) /1000;
	ov_time_seek(m_OggVorbisFile, position);
	if(LoadDSData(0,SIZE_DS_BUFFER)!=0)
	{
		return -1;
	}
	_pDSBuffer->SetCurrentPosition(0);
	return 0;
}

int GetPlayPos(long* pos)						//获取播放位置；
{

	return 0;

}

int GetFormatTag()									//获取标签 WMV文件应为WMV_FORMAT_PCM；
{
	return _fmtWave.wFormatTag;
}

int GetChannels()  
{
	return _fmtWave.nChannels;
}

int GetSamples()									//获取采样率；
{
	return _fmtWave.nSamplesPerSec;
}

int GetBPS()										//获取位数；
{
	return _fmtWave.wBitsPerSample;
}

int GetBlockAlign()									//获取块对齐大小；
{
	return _fmtWave.nBlockAlign;
}

int GetABPS()										//获取字节数 == 采样率*块对齐大小；
{
	return _fmtWave.nAvgBytesPerSec;
}

int SetPan(long pan)								//设置左右声道
{
	HRESULT hr = DSERR_GENERIC;
	if(_pDSBuffer!=NULL)
	{
		hr = _pDSBuffer->SetPan(pan);
	}
	if(hr != DS_OK)
		return -1;
	return 0;
}

int GetPan(long *pan)								//获取左右声道
{
	HRESULT hr = DSERR_GENERIC;
	if(_pDSBuffer!=NULL)
	{
		hr = _pDSBuffer->GetPan(pan);
	}
	if(hr!=DS_OK)
		return -1;
	return 0;
}


	

/*************************创建命名事件句柄*****************************************************/
int CreateNamePipe()
/**********************************************************************************************/
{
	_eventPipe = CreateEvent(NULL,FALSE,FALSE,L"SyncPlayEvent");
	if(_eventPipe!=NULL)
	{
		return 0;
	}
	return -1;
}

void GetFormatOgg(SONGFILEPATH* pFilePath,SONGINFO* pOggInfo)
{
	SONGFILEPATH *strPath = pFilePath;
	OggVorbis_File *pOggVorbis = NULL;
	FILE *pFile = NULL;
	_wfopen_s(&pFile,strPath->FilePath.c_str(),L"rb");
	if(pFile == NULL)
		return;
	pOggVorbis = new OggVorbis_File;
	memset(pOggVorbis,0,sizeof(OggVorbis_File));
	ov_clear(pOggVorbis);

	if(0!=ov_open(pFile,pOggVorbis,NULL,0))
	{
		ov_clear(pOggVorbis);
		delete pOggVorbis;
		pOggVorbis = NULL;
		if(pFile)
		{
			fclose(pFile);
			pFile = NULL;
		}
		return;
	}
	WORD channels ;
	if(pOggVorbis->vi->channels == 1)
	{
		channels = 1;
	}
	else
	{
		channels = 2;
	}
	pOggInfo->tag = "Vorbis/Ogg Audio"; 
	pOggInfo->bit = 16;
	if(channels ==1)
	{
		pOggInfo->channel = "单声道";
	}
	else if(channels ==2)
	{
		pOggInfo->channel = "双声道";
	}
	pOggInfo->samplerate = pOggVorbis->vi->rate;
	pOggInfo->framerate = pOggVorbis->vi->bitrate_nominal;
	pOggInfo->time = (int)(ov_time_total(pOggVorbis,0) * 1000);
	pOggInfo->Title = pOggVorbis->vc->vendor;
	//for(int i=0;i<pOggVorbis->vc->comments;i++)
	{
		pOggInfo->Artist = pOggVorbis->vc->user_comments[0];
	}

	if(pOggVorbis)
	{
		ov_clear(pOggVorbis);
		delete pOggVorbis;
		pOggVorbis = NULL;
	}
	if(pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
		

}
/*************************从文件加载播放流*****************************************************/
int LoadFile(const std::wstring &file)   
/**********************************************************************************************/
{

	if(inFile != NULL)
	{
		fclose(inFile);
		inFile = NULL;
	}
	_wfopen_s(&inFile,file.c_str(),L"rb");
	if(inFile == NULL)
	{
		return -1;
	}
	if(m_OggVorbisFile)
	{
		ov_clear(m_OggVorbisFile);
		delete m_OggVorbisFile;
		m_OggVorbisFile = NULL;
	}
	m_OggVorbisFile = new OggVorbis_File;
	memset(m_OggVorbisFile,0,sizeof(OggVorbis_File));
	ov_clear(m_OggVorbisFile);

	if(0 != ov_open(inFile,m_OggVorbisFile,NULL,0))
	{
		ov_clear(m_OggVorbisFile);
		delete m_OggVorbisFile;
		m_OggVorbisFile = NULL;
		if(inFile)
		{
			fclose(inFile);
			inFile = NULL;
		}
		return -1;
	}

	if(m_OggVorbisFile->vi->channels == 1)
	{
		m_bMono = TRUE;
	}
	else
	{
		m_bMono = FALSE;
	}
	sampleRate = m_OggVorbisFile->vi->rate;
	if(m_bMono)
	{
		numChannels = 1;
	}
	else
	{
		numChannels = 2;
	}


	SetWaveFormat();
	init_equliazer(0);
	if(CreateDSoundBuffer()!=0)
	{
		if(m_OggVorbisFile)
		{
			ov_clear(m_OggVorbisFile);
			delete m_OggVorbisFile;
			m_OggVorbisFile = NULL;
		}
		if(inFile)
		{
			fclose(inFile);
			inFile = NULL;
		}
		return -1;
	}

	return 0;
}

/************************加载网络数据流********************************************************/
int LoadMem(unsigned char *buffer)
/**********************************************************************************************/
{
	//FileHandle = CreateFile(L"C:\\logFile.txt",FILE_ALL_ACCESS,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	return 0;
}
int GetSpectrum(int channel,int index)
{
	return 0;
}

/*****************************唯一外派接口*****************************************************/
extern "C" _declspec(dllexport) int raGetSoundAddControl(int nMsgType,LPVOID lpReceiveData,LPVOID lpSendData) 
/**********************************************************************************************/
{
	switch(nMsgType)
	{
	case CONTROL_EVENTHANDLE:
		{
			int ret = CreateNamePipe();
			if(ret == 0)
			{
				return 0;
			}
			return -1;
		}
	case CONTROL_INITINFO:
		{
			SONGINIT *mp3InitInfo = (SONGINIT*)lpReceiveData;
			//m_FilePath = mp3InitInfo->file;
			Init(mp3InitInfo->hwnd);
			LoadFile(mp3InitInfo->file);
		}
	case CONTROL_PLAY:
		{
			int ret = Play();
			if(ret == 0)
			{
				return 0;
			}
			else
				return -1;
		}
	case CONTROL_PAUSE:
		{
			int ret = Pause();
			if(ret == 0)
				return 0;
			else
				return -1;
			
		}
	case CONTROL_STOP:
		{
			int ret = Stop();
			if(ret == 0)
				return 0;
			else 
				return -1;
			
		}
	case CONTROL_SETPLAYPOS:
		{
			unsigned int *pos = (unsigned int*)lpReceiveData;
			int ret = SetPlayPos(*pos);
			if(ret == 0)
				return 0;
			else
				return -1;
		
		}
	case CONTROL_GETPLAYPOS:
		{
			GetPlayPos((long*) lpSendData);
			
			return 0;
		}
	case CONTROL_GETINFORMATION:
		{
			GetFormatOgg((SONGFILEPATH*)lpReceiveData,(SONGINFO*)lpSendData);
			return 0;
		}
	case CONTROL_DURATION:
		{
			Duration((long*)lpSendData);
			return 0;
		}
	case CONTROL_SETLRMODE:
		{
			long *pan = (long*)lpReceiveData; 
			int ret = SetPan(*pan);
			if(ret == 0)
				return 0;
			else
				return -1;

		}
	case CONTROL_GETLRMODE:
		{
			int ret = GetPan((long*)lpSendData);
			if(ret == 0)
				return 0;
			else
				return -1;
		}
	case CONTROL_SETVOLUM:
		{
			long *vol = (long*)lpReceiveData;
			int ret = SetVolum(*vol);
			if(ret == 0)
				return 0;
			else
				return -1;
		}
	case CONTROL_GETVOLUM:
		{
			int ret = GetVolum((long*)lpSendData);
			if(ret == 0)
				return 0;
			else
				return -1;
		}
	case CONTROL_SETEQ:
		{
			SONGEQ *eq = (SONGEQ*)lpReceiveData;
			int index;
			for(index =0;index<10;index++)
			{
				if(eq->value[index] == 0)
				{
					continue;
				}
				else
				{
					if(index<10)
					{
						bReset = FALSE;
						break;
					}
				}
			}
			if(index ==10)
			{
				bReset = TRUE;
			}
			for(index =0;index<10;index++)
			{
				set_eq_value(eq->value[index],index,2);
			}
			return 0;
		}
	case CONTROL_SETEQOPENDOWN:
		{
			SONGEQOPENDOWN *beq = (SONGEQOPENDOWN*)lpReceiveData;
			bEQ = beq->bOpen;
			return 0;
		}
	default:
		return 0;
	}
}


