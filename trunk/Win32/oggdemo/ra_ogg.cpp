// ra_ogg.cpp : ���� DLL Ӧ�ó���ĵ���������
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

static const			DWORD SIZE_DS_BUFFER = 32*1024;			//��ʼ��һ��Ĭ��ֵ��Ϊ�������Ĵ�С���Ժ�����޸Ĵ�ֵ���ı仺������С��

/**************�����¼�֪ͨ****************************************************************************/
static HANDLE			_eventNotify[3];						//����DSound������֪ͨ�¼�,_eventNotify[3]Ϊ�û�stop�¼�֪ͨ
HANDLE					_eventPipe = NULL;						//���������¼��ľ��,���ڲ�������������ʱͬ�ͻ��˵�ͬ��
DWORD					_eventResult = 0;						//��Ϣ�ȴ�����ֵ,����ͬ��
/**************�����¼�֪ͨ****************************************************************************/

/**************DSound����������************************************************************************/
HWND					_hWnd;									//�����ھ��
DWORD					_uiNotifyThreadID = 0;					//��ȡ�߳�ID
HANDLE					_hNotifyThread = NULL;					//��ȡ�߳̾��
WAVEFORMATEX			_fmtWave;								//������wav��ʽ
DSBPOSITIONNOTIFY		_notifyPos[2];							//����ƫ�Ƶ�ַ֪ͨ������������������Ҫ����ӣ�
IDirectSound8			*_pDS = NULL;							//�豸����
IDirectSoundBuffer8		*_pDSBuffer = NULL;						//����������
IDirectSoundNotify8		*_pDSNotify = NULL;						//��Ϣ֪ͨ����
/**************DSound����������************************************************************************/

/**********************ogg ����************************************************************************/
OggVorbis_File				*m_OggVorbisFile = NULL;
//int							m_CurrentSection;
BOOL						m_bMono = FALSE;					//�Ƿ��ǵ���� FALSE ��˫����
DWORD						sampleRate;
WORD						numChannels;
//unsigned char				m_buffer[40000];
//unsigned int				m_buflen;
//unsigned char				m_pcmbuffer[SIZE_DS_BUFFER/2];
int							current_section;

//HANDLE						m_FileHandle = NULL;					//�ļ����
FILE						*inFile		= NULL;						//�ļ�ָ��

//EQ
BOOL						bEQ = FALSE;
BOOL						bReset = FALSE;

//�����������֣�
//void	GetDataFromFile(void **buffer,DWORD *bytes);
//BOOL	SetDataPosition(LARGE_INTEGER	*SampleNum);
//void	OpenFile();
//void	CloseFile();

//size_t vorbisRead(void *buffer,size_t size,size_t amount,void *file);
//int	   vorbisSeek(void *file,ogg_int64_t newpos,int set);
//int	   vorbisClose(void *file);
//long   vorbisTell(void *file);
/******************************************************************************************************/



/*************************�������ݽṹ����*************************************************************/


/******************************************************************************************************/

/***********************���¶����ݻ�����***************************************************************/

/*******************************************************************************************************/



/************************���¶����ݻ���������***********************************************************/

/*******************************************************************************************************/



/************************���³�ʼ��buff*****************************************************************/

/********************************************************************************************************/




/************************����****************************************************************************/
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

/**********************װ�����ݽ���ŵ�DSound������*****************************************************/
int LoadDSData(unsigned int start,unsigned int count)	//��仺����
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
		return -1;			//���Ž���
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

/*******************�ͷ�DSound*****************************************************************/
void ReleaseDSound()					 //�ͷ��豸����
/**********************************************************************************************/
{
	if(_pDS != NULL)
	{
		_pDS->Release();
		_pDS = NULL;
	}
}

/*******************�û����ֹͣʱ�ͷ��߳�*****************************************************/
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
	for(int i=0;i<3;i++)                  //�˴����Ժ����������¼��ں˺�����Ҫͬʱ�����ı䡣
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

/*****************���Ž���ʱ�ͷ��߳�***********************************************************/
void ReleaseNotifyThreadPlayOver()
/**********************************************************************************************/
{
	if(_hNotifyThread != NULL)
	{
		CloseHandle(_hNotifyThread);	
		_hNotifyThread = NULL;	
	}
	for(int i=0;i<3;i++)                  //�˴����Ժ����������¼��ں˺�����Ҫͬʱ�����ı䡣
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

/******************�ͷ�Sound������*************************************************************/
void ReleaseDSoundBuffer()				//�ͷ��豸����������
/**********************************************************************************************/
{
	if(_pDSBuffer != NULL)
	{
		_pDSBuffer->Stop();
		_pDSBuffer->Release();
		_pDSBuffer = NULL;
	}
}

/**************�ͷŽ����**********************************************************************/
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

int Stop()										//ֹͣ��
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
int PlayOver()             //������ͣ���ڻ�û�����֪ͨʱ����ͣ��
{
	
	return PlayOverStop();
}


/********************������֪ͨ�߳�************************************************************/
static DWORD NotifyHandleProc(LPVOID param)			//֪ͨʱ�䴦����̺�����
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

int CreateDSound()         //�����豸����
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

int Init(HWND hwnd)                       //��ʼ��Ӧ�ó��򴰿ڣ���������Э�����𣻳�ʼ����������������
{
	_hWnd = hwnd;
	return CreateDSound();
}

/*******************���û�������ʽ*************************************************************/
int SetWaveFormat()				//����WMV��ʽ��
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

/**********************����֪ͨ�̺߳ͼ����¼�**************************************************/
int CreateNotifyThread()           //������Ϣ֪ͨ�̣߳�
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

	_notifyPos[0].dwOffset = (SIZE_DS_BUFFER /2)-1;     //��������ƫ���ڻ������м�λ�ã��Ժ���Ե�����
	_notifyPos[0].hEventNotify = _eventNotify[0];

	_notifyPos[1].dwOffset = SIZE_DS_BUFFER-1;          //�������õڶ���ƫ���ڻ�����βλ�ã��Ժ���Ե�����
	_notifyPos[1].hEventNotify = _eventNotify[1];

	hr = _pDSNotify->SetNotificationPositions(2,_notifyPos);    //�˴���������ƫ��λ�á�Ҫ�����������ı䡣
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


/***********************����DSound���Ż�����***************************************************/
int CreateDSoundBuffer()   //�����豸����������
/**********************************************************************************************/
{
	IDirectSoundBuffer *psBuffer = NULL;

	DSBUFFERDESC  desc;    //�����������ṹ�壻
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


bool IsReady()                      //�Ƿ񴴽���ϣ�
{
	return (_pDS != NULL &&_pDSBuffer != NULL);
}

int Play()                                 //���ţ�
{
	if(_pDSBuffer == NULL)
	{
		return -1;
	}	
	_pDSBuffer->Play(0,0,DSBPLAY_LOOPING);
	
	return 0;
}

int Pause()                                //��ͣ��
{
	if(_pDSBuffer == NULL)
	{
		return -1;
	}
	_pDSBuffer->Stop();
	return 0;
}



bool IsPlaying()                     //�ж��Ƿ����ڲ��� true = ���ţ�
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

int Duration(long *time)							//��������ʱ�䣻
{
	//*time = fileread/_fmtWave.wBitsPerSample;
	return 0;
}

int SetVolum(long vol)								//���û�������������
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

int GetVolum(long *vol)								 //��ȡ��������������
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

int SetPlayPos(unsigned int pos)					//���ò���λ�ã�  
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

int GetPlayPos(long* pos)						//��ȡ����λ�ã�
{

	return 0;

}

int GetFormatTag()									//��ȡ��ǩ WMV�ļ�ӦΪWMV_FORMAT_PCM��
{
	return _fmtWave.wFormatTag;
}

int GetChannels()  
{
	return _fmtWave.nChannels;
}

int GetSamples()									//��ȡ�����ʣ�
{
	return _fmtWave.nSamplesPerSec;
}

int GetBPS()										//��ȡλ����
{
	return _fmtWave.wBitsPerSample;
}

int GetBlockAlign()									//��ȡ������С��
{
	return _fmtWave.nBlockAlign;
}

int GetABPS()										//��ȡ�ֽ��� == ������*������С��
{
	return _fmtWave.nAvgBytesPerSec;
}

int SetPan(long pan)								//������������
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

int GetPan(long *pan)								//��ȡ��������
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


	

/*************************���������¼����*****************************************************/
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
		pOggInfo->channel = "������";
	}
	else if(channels ==2)
	{
		pOggInfo->channel = "˫����";
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
/*************************���ļ����ز�����*****************************************************/
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

/************************��������������********************************************************/
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

/*****************************Ψһ���ɽӿ�*****************************************************/
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


