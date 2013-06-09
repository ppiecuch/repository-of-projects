#include "MyGUI_UString.h"
#include "basenetwork.h"
#include "framework.h"

#ifdef WIN32
#include <process.h>
#else
#include <pthread.h>
#endif

int CBaseNetwork::bigEndian(unsigned int num)
{
	int result;
	int *p=&result;
	num= (num << 24) | ((num & 0xFF00)  << 8) | ((num & 0x00FF0000) >> 8) | (num >> 24);
	p=reinterpret_cast<int*>(&num);
	return *p;

}
unsigned short CBaseNetwork::bigEndian(unsigned short num)
{
	return (num << 8) | (num >> 8);
}

CBaseNetwork::CBaseNetwork()
:m_bSendThreadRunnable(true),m_bRecvThreadRunnable(true),m_bDisconnectNotified(false),m_pSocket(NULL),m_eState(ENUM_STATE_NONE),
m_sendSpeedCounter(getEngine()->getTimer(),1500),m_recvSpeedCounter(getEngine()->getTimer(),1500),m_bDataProccessible(true)
{
}

CBaseNetwork::~CBaseNetwork()
{
	m_pool.clear();
}

SPacketBuffer* CBaseNetwork::getBuffer()
{
	CScopedMutexLock lock(m_poolMutex,__FILE__,__LINE__);
	SPacketBuffer* buffer=m_pool.get();
	buffer->hasOwner=false;
	return buffer;
}

void CBaseNetwork::recycleBuffer(SPacketBuffer* buffer)
{
	CScopedMutexLock lock(m_poolMutex,__FILE__,__LINE__);
	m_pool.recycle(buffer);
}

void CBaseNetwork::SendPacket(SPacketBuffer *packet)
{
	m_sendMutex.Lock(__FILE__,__LINE__);
	m_sendBufferList.push_back(packet);
	m_sendMutex.Unlock(__FILE__,__LINE__);
}

void CBaseNetwork::notifyDisconnected()
{
	CScopedMutexLock lock(m_notifyMutex,__FILE__,__LINE__);
	if(m_bDisconnectNotified)
		return;
	m_disconnectDlgt();
	m_bDisconnectNotified=true;
}

void CBaseNetwork::setOnConnectDelegate(IDelegateVoid* dlgt)
{
	if(dlgt==NULL)
	{
		m_connectDlgt.clear();
		return;
	}
	m_connectDlgt=dlgt;
}

void CBaseNetwork::setOnDisconnectDelegate(IDelegateVoid* dlgt)
{
	if(dlgt==NULL)
	{
		m_disconnectDlgt.clear();
		return;
	}
	m_disconnectDlgt=dlgt;
}

void CBaseNetwork::_recvProc()
{
	while(m_bRecvThreadRunnable)
	{
		if(m_eState!=ENUM_STATE_CONNECTED)
			core::yonSleep(500);
		else
		{
			u16 packLen=0;
			int len=0;
			len=m_pSocket->Recv((char*)&packLen,2);

			if(len==-1)
			{
				m_eState=ENUM_STATE_INITIALIZED;
				notifyDisconnected();
				continue;
			}
			else if(len==0)
			{
				YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("recv1 len:%d,packLen:%u",len,packLen).c_str());
				continue;
			}

			//packLen = (packLen << 8) | (packLen >> 8);
			packLen = bigEndian(packLen);

			SPacketBuffer* buffer=getBuffer();
			buffer->Buffer.reserve(packLen);

			memset(buffer->Buffer.pointer(),0x0,buffer->Buffer.capacity());
			
			len=m_pSocket->Recv(buffer->Buffer.pointer(),packLen);
			//YON_DEBUG("Receive data:%d\r\n",len);

			if(len==-1)
			{
				recycleBuffer(buffer);
				m_eState=ENUM_STATE_INITIALIZED;
				notifyDisconnected();
			}
			else if(len==0)
				YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("recv2 len:%d,packLen:%u",len,packLen).c_str());
			else
			{
				buffer->Buffer.repair(packLen);

				m_recvMutex.Lock(__FILE__,__LINE__);
				m_recvBufferList.push_back(buffer);
				m_recvMutex.Unlock(__FILE__,__LINE__);

				m_recvSpeedCounter.increase(len);

				core::yonSleep(1);
			}
		}
	}
	m_bRecvThreadRunnable=true;
	YON_DEBUG("Shutdown network receiver thread\r\n");
}

#ifdef WIN32
unsigned int WINAPI CBaseNetwork::ReceiverThread( void* data )
#else
void* CBaseNetwork::ReceiverThread( void* data )
#endif
{
	CBaseNetwork* instance=static_cast<CBaseNetwork*>(data);
	instance->_recvProc();
	return 0;
}

void CBaseNetwork::_sendProc()
{
	yon::ITimer* timer=getEngine()->getTimer();
	SPacketBuffer* buffer=NULL;
	while(m_bSendThreadRunnable)
	{
		if(m_eState!=ENUM_STATE_CONNECTED)
			core::yonSleep(500);
		else
		{
			m_sendMutex.Lock(__FILE__,__LINE__);
			if(!m_sendBufferList.empty())
			{
				PacketBufferList::Iterator it=m_sendBufferList.begin();
				buffer=*it;
				m_sendBufferList.erase(it);
			}
			else
				buffer=NULL;
			m_sendMutex.Unlock(__FILE__,__LINE__);

			u32 start=timer->getRealTime();

			if(buffer==NULL)
			{
				core::yonSleep(5);
			}
			else
			{
				int len=buffer->Buffer.length();
				len=m_pSocket->Send(buffer->Buffer.c_str(),len);
				YON_DEBUG("send use time:%u\r\n",timer->getRealTime()-start);
				recycleBuffer(buffer);
				if(len==-1)
				{
					m_eState=ENUM_STATE_INITIALIZED;
					notifyDisconnected();
				}
				else
					m_sendSpeedCounter.increase(len);

				core::yonSleep(5);
			}
		}
	}
	m_bSendThreadRunnable=true;
	YON_DEBUG("Shutdown network sender thread\r\n");
}

#ifdef WIN32
unsigned int WINAPI CBaseNetwork::SenderThread( void* data )
#else
void* CBaseNetwork::SenderThread( void* data )
#endif
{
	CBaseNetwork* instance=static_cast<CBaseNetwork*>(data);
	instance->_sendProc();
	return 0;
}
void CBaseNetwork::Init()
{
	YON_DEBUG_BREAK_IF(m_eState!=ENUM_STATE_NONE);

	//创建网络工作线程
#ifdef WIN32
	unsigned int netRecvThreadId,netSendThreadId;
	_beginthreadex( NULL, NULL, ReceiverThread, this, NULL, &netRecvThreadId );
	_beginthreadex( NULL, NULL, SenderThread, this, NULL, &netSendThreadId );
#else
	pthread_t netRecvThreadId,netSendThreadId;
	pthread_create(&netRecvThreadId, NULL, &ReceiverThread, this);
	pthread_create(&netSendThreadId, NULL, &SenderThread, this);
#endif
	m_eState=ENUM_STATE_INITIALIZED;
	YON_DEBUG("Create network thread:%u,%u\r\n",netRecvThreadId,netSendThreadId);
}

bool CBaseNetwork::Connect(const char *remote,int port,char *bindIp,int bindPort)
{
	YON_DEBUG_BREAK_IF(m_eState!=ENUM_STATE_INITIALIZED);
	if(m_pSocket==NULL)
	{
		m_pSocket=new CNetSocket();
	}
	if(!m_pSocket->Initialize())
		return false;
	if(!m_pSocket->BindAddr(bindIp,bindPort))
		return false;
	if(m_pSocket->Connect(remote,port))
	{
		YON_DEBUG("Connect to %s:%d successfully!\r\n",remote,port);
		m_eState=ENUM_STATE_CONNECTED;
		m_bDisconnectNotified=false;
		m_connectDlgt();
		return true;
	}
	return false;
}

void CBaseNetwork::Close()
{
	if(m_eState==ENUM_STATE_NONE)
		return;
	YON_DEBUG_BREAK_IF(!m_bSendThreadRunnable||!m_bRecvThreadRunnable);
	m_sendMutex.Lock(__FILE__,__LINE__);
	PacketBufferList::Iterator it=m_sendBufferList.begin();
	for(;it!=m_sendBufferList.end();++it)
	{
		SPacketBuffer* buffer=*it;
		recycleBuffer(buffer);
	}
	m_sendBufferList.clear();
	m_sendMutex.Unlock(__FILE__,__LINE__);

	m_bSendThreadRunnable=false;
	while(!m_bSendThreadRunnable)core::yonSleep(1);
	if(m_pSocket)
	{
		m_pSocket->Close();
		delete m_pSocket;
		m_pSocket=NULL;
	}
	m_bRecvThreadRunnable=false;
	while(!m_bRecvThreadRunnable)core::yonSleep(1);

	m_recvMutex.Lock(__FILE__,__LINE__);
	it=m_recvBufferList.begin();
	for(;it!=m_recvBufferList.end();++it)
	{
		SPacketBuffer* buffer=*it;
		if(buffer->hasOwner==false)
			recycleBuffer(buffer);
	}
	m_recvBufferList.clear();
	m_recvMutex.Unlock(__FILE__,__LINE__);

	m_eState=ENUM_STATE_NONE;
}

void CBaseNetwork::Lock()
{
	m_bDataProccessible=false;
}

void CBaseNetwork::UnLock()
{
	m_bDataProccessible=true;
}

bool CBaseNetwork::IsLocked()
{
	return !m_bDataProccessible;
}
void CBaseNetwork::Update()
{
	if(!m_bDataProccessible)
		return;

	static i18n::II18NManager* i18nMgr=getEngine()->getI18NManager();
	
	/*m_recvMutex.Lock(__FILE__,__LINE__);
	PacketBufferList::Iterator it=m_recvBufferList.begin();
	for(;it!=m_recvBufferList.end();++it)
	{
		SPacketBuffer* buffer=*it;
		buffer->hasOwner=true;
		//TODO
		//YON_DEBUG("recv:%s\r\n",i18nMgr->convert(buffer->Buffer.c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
		recycleBuffer(buffer);
	}
	m_recvBufferList.clear();
	m_recvMutex.Unlock(__FILE__,__LINE__);*/

	SPacketBuffer* buffer;
	bool recvEmpty=false;
	do{
		m_recvMutex.Lock(__FILE__,__LINE__);
		recvEmpty=m_recvBufferList.empty();
		if(!recvEmpty)
		{
			PacketBufferList::Iterator it=m_recvBufferList.begin();
			buffer=*it;
			m_recvBufferList.erase(it);
		}
		else
			buffer=NULL;
		m_recvMutex.Unlock(__FILE__,__LINE__);

		if(buffer!=NULL)
		{
			recycleBuffer(buffer);
		}
	}while(!recvEmpty);
}
