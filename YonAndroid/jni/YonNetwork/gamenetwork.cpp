#include "MyGUI_UString.h"
#include "gamenetwork.h"
#include "cmdpacket.h"
#include "framework.h"

const short CGameNetwork::SYS_PACKET_SET_SEED   = 0x02;

int CGameNetwork::generateSecretKey(int seed) 
{
	int r1 = seed;
	int r2 = 0x28887c7;
	int r3 = 0x228f2967;
	int r4 = 0x45edfad;
	int ii = r1 ^ r1 << 11;
	r1 = r2;
	r2 = r3;
	r3 = r4;
	return r4 = r4 ^ r4 >> 19 ^ (ii ^ ii >> 8);
}

CGameNetwork::CGameNetwork():m_iSeed(0),m_bWaitingSeed(true),m_iSeq(0)
{
}
CGameNetwork::~CGameNetwork()
{
	ProtocolMap::Iterator it=m_protocolDelegateMap.getIterator();
	for(;!it.atEnd();++it)
		delete it->getValue();
	m_protocolDelegateMap.clear();
}
void CGameNetwork::Init()
{
	CBaseNetwork::Init();
	m_bWaitingSeed=true;
}

void CGameNetwork::Close()
{
	CBaseNetwork::Close();
	m_bWaitingSeed=true;
}

bool CGameNetwork::Connect(const char *remote,int port,char *bindIp,int bindPort)
{
	m_iSeq=0;
	return CBaseNetwork::Connect(remote,port,bindIp,bindPort);
}

void CGameNetwork::Update()
{
	if(!m_bDataProccessible)
		return;

	m_recvMutex.Lock(__FILE__,__LINE__);
	PacketBufferList::Iterator it=m_recvBufferList.begin();
	for(;it!=m_recvBufferList.end();++it)
	{
		SPacketBuffer* buffer=*it;
		buffer->hasOwner=true;

		CCmdPacket pak;
		pak.BeginRead(buffer->Buffer.pointer(),buffer->Buffer.length());
		MSG_PROTOCOL_TYPE code;
		pak.ReadByte(&code);

		ProtocolMap::Node* node=m_protocolDelegateMap.find(code);
		if(node==NULL)
			YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("can not find protocol delegate with code:%d",code).c_str());
		else
		{
			IDelegate<CCmdPacket*>* d=node->getValue();
			//(*d)(&pak);
			d->invoke(&pak);
		}

		recycleBuffer(buffer);
	}
	m_recvBufferList.clear();
	m_recvMutex.Unlock(__FILE__,__LINE__);
}

void CGameNetwork::decrypt(char* data,int offset,int size)
{
	for(int i=offset;i<size;++i)
	{
		//unsigned char tmp=(unsigned char)data[i];
		data[i] ^= m_iSeed;
		//unsigned char tmp2=(unsigned char)data[i];
		//YON_DEBUG("(%d):%02X,%02X\r\n",i,tmp,tmp2);
	}
}

void CGameNetwork::encrypt(char* data,int offset,int size) 
{
	//m_iSeed=0x15091B50;
	int key=m_iSeed;
	core::array<c8> unarr;
	core::array<c8> arr;
	for (int i=offset; i<size; ++i) 
	{
		//unsigned char tmp=(unsigned char)data[i];
		unarr.push_back(data[i]);
		data[i] ^= m_iSeed;
		data[i] ^= key;
		key = data[i];
		//unsigned char tmp2=(unsigned char)data[i];
		//YON_DEBUG("(%d):%02X,%02X\r\n",i,tmp,tmp2);
		arr.push_back(data[i]);
	}

	core::stringc unstr;
	for(u32 i=0;i<unarr.size();++i)
		unstr.append(core::stringc(",%d",unarr[i]));
	YON_DEBUG("unencrypt:%s\r\n",unstr.c_str());

	core::stringc str;
	for(u32 i=0;i<arr.size();++i)
		str.append(core::stringc(",%d",arr[i]));
	YON_DEBUG("encrypt:%s\r\n",str.c_str());
}


void CGameNetwork::_sendProc()
{
	yon::ITimer* timer=getEngine()->getTimer();
	SPacketBuffer* buffer=NULL;
	while(m_bSendThreadRunnable)
	{
		if(m_eState!=ENUM_STATE_CONNECTED||m_bWaitingSeed)
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
				YON_DEBUG_BREAK_IF(buffer->Buffer.length()>0xFFFF);

				u16 len=(u16)buffer->Buffer.length();
				u16 beLen=bigEndian((u16)(len-2));
				memcpy(buffer->Buffer.pointer(),&beLen,2);
				buffer->Buffer[2]=m_iSeq;
				encrypt(buffer->Buffer.pointer(),0,buffer->Buffer.length());
				int sendLen=m_pSocket->Send(buffer->Buffer.c_str(),len);
				YON_DEBUG("send use time:%u(%d,%d,%d)\r\n",timer->getRealTime()-start,m_iSeq,len,sendLen);
				recycleBuffer(buffer);
				if(sendLen==-1)
				{
					m_eState=ENUM_STATE_INITIALIZED;
					notifyDisconnected();
				}
				else
					m_sendSpeedCounter.increase(len);

				core::yonSleep(5);
				m_iSeq++;
			}
		}
	}
	m_bSendThreadRunnable=true;
	YON_DEBUG("Shutdown network sender thread\r\n");
}
void CGameNetwork::_recvProc()
{
	while(m_bRecvThreadRunnable)
	{
		if(m_eState!=ENUM_STATE_CONNECTED)
			core::yonSleep(500);
		else if(m_bWaitingSeed)
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

			packLen = bigEndian(packLen);

			YON_DEBUG_BREAK_IF(packLen<sizeof(SSeedPacket));

			SSeedPacket pak;
			len=m_pSocket->Recv((char*)&pak,packLen);
			int seed=bigEndian(pak.Seed);
			YON_DEBUG("Seed pack len:%d,pak.Seed:%d,seed:%d\r\n",packLen,pak.Seed,seed);

			YON_DEBUG_BREAK_IF(pak.Type!=SYS_PACKET_SET_SEED);

			if(len==-1)
			{
				m_eState=ENUM_STATE_INITIALIZED;
				notifyDisconnected();
			}
			else if(len==0)
				YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("recv2 len:%d,packLen:%u",len,packLen).c_str());
			else
			{
				//seed=0x5B9F77F1;
				m_iSeed=generateSecretKey(seed);
				YON_DEBUG("Get seed:%08X(%d)->%08X(%d)\r\n",seed,seed,m_iSeed,m_iSeed);
				m_recvSpeedCounter.increase(len);
				m_bWaitingSeed=false;
			}
		}
		else
		{
			u16 head=0;
			int len=0;
			len=m_pSocket->Recv((char*)&head,sizeof(head));

			if(len==-1)
			{
				m_eState=ENUM_STATE_INITIALIZED;
				notifyDisconnected();
				continue;
			}
			else if(len==0)
				YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("recv1 len:%d,packLen:%u",len,head).c_str());

			u16 packLen = bigEndian(head);
			decrypt((char*)&packLen,0,2);
			
			YON_DEBUG("packLen:%d\r\n",packLen);

			SPacketBuffer* buffer=getBuffer();
			buffer->Buffer.reserve(packLen);

			memset(buffer->Buffer.pointer(),0x0,buffer->Buffer.capacity());

			len=m_pSocket->Recv(buffer->Buffer.pointer(),packLen);

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

				decrypt(buffer->Buffer.pointer(),0,buffer->Buffer.length());

				m_recvMutex.Lock(__FILE__,__LINE__);
				m_recvBufferList.push_back(buffer);
				m_recvMutex.Unlock(__FILE__,__LINE__);

				m_recvSpeedCounter.increase(len);

				core::yonSleep(5);
			}
		}
	}
	m_bRecvThreadRunnable=true;
	YON_DEBUG("Shutdown network receiver thread\r\n");
}