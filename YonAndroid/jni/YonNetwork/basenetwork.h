#ifndef _BASENETWORK_H_
#define _BASENETWORK_H_

#include "yon.h"
using namespace yon;
#include "netsocket.h"
#include "XCMutex.h"
#include "SSpeedCounter.h"
#include "delegate.h"
#include "ISingleton.h"


struct SPacketBuffer : public core::IRecyclable{
	//core::array<c8> Buffer;
	core::stringc Buffer;
	bool hasOwner;
	virtual void reset(){
		//Buffer.set_used(0);
		memset(Buffer.pointer(),0x0,Buffer.capacity());
		Buffer.repair(0);
	}

	void setSize(u32 size){
		Buffer.reserve(size);
		reset();
	}
};

//class CBaseNetwork : public CSingleton<CBaseNetwork>
class CBaseNetwork
{
protected:

	enum ENUM_STATE{
		ENUM_STATE_NONE = 0,
		ENUM_STATE_INITIALIZED,
		ENUM_STATE_CONNECTED
	};

	typedef core::list<SPacketBuffer*> PacketBufferList;
	typedef core::CObjectPoolFast<SPacketBuffer> PacketBufferPool;

	bool m_bSendThreadRunnable,m_bRecvThreadRunnable;
	CXCMutex m_notifyMutex;
	CXCMutex m_sendMutex;
	CXCMutex m_recvMutex;
	CXCMutex m_poolMutex;
	bool m_bDisconnectNotified;
	PacketBufferList m_sendBufferList;
	PacketBufferList m_recvBufferList;
	PacketBufferPool m_pool;
	bool m_bDataProccessible;
	

	SSpeedCounter m_sendSpeedCounter;
	SSpeedCounter m_recvSpeedCounter;

	CNetSocket* m_pSocket;
	ENUM_STATE m_eState;
	CDelegateVoid m_connectDlgt;
	CDelegateVoid m_disconnectDlgt;
	void notifyDisconnected();

	virtual void _sendProc();
	virtual void _recvProc();

	static int bigEndian(unsigned int num);
	static unsigned short bigEndian(unsigned short num);
public:
	CBaseNetwork();
	virtual ~CBaseNetwork();

	
	SPacketBuffer* getBuffer();
	void recycleBuffer(SPacketBuffer* buffer);

	virtual void Init();
	virtual bool Connect(const char *remote,int port,char *bindIp=0,int bindPort=0);
	void Close();

	bool isConnected(){return m_eState==ENUM_STATE_CONNECTED;}

	void Lock();
	void UnLock();
	bool IsLocked();

	void setOnConnectDelegate(IDelegateVoid* dlgt);
	void setOnDisconnectDelegate(IDelegateVoid* dlgt);

	void SendPacket(SPacketBuffer *packet);

	u32 getPoolSize(){
		return m_pool.getSize();
	}

	u32 getPoolCapacity(){
		return m_pool.getCapacity();
	}

	core::stringc getSendSpeed(){
		return m_sendSpeedCounter.getSpeed();
	}

	core::stringc getRecvSpeed(){
		return m_recvSpeedCounter.getSpeed();
	}

#ifdef WIN32
	static unsigned int WINAPI ReceiverThread( void* lpData );
	static unsigned int WINAPI SenderThread( void* lpData );
#else
	static void* ReceiverThread( void* data );
	static void* SenderThread( void* data );
#endif

	// 更新方法，需要周期性定时调用，通常每帧调用一次
	virtual void Update();
};

//#define GameNetwork()	CBaseNetwork::GetInstance()

#endif