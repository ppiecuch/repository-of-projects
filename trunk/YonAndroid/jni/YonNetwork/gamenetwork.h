#ifndef _GAMENETWORK_H_
#define _GAMENETWORK_H_
#include "basenetwork.h"
#include "ISingleton.h"
#include "gamenetworkprotocol.h"

/**密钥包格式**/
//unsigned short	长度
//unsigned char		类型(目前只有一种)
//char*				内容

/**接收包格式**/
//unsigned short	长度
//char*				内容

/**发送包格式**/
//unsigned short	长度(本包长度，不包括长度数据的长度)
//unsigned char		循环序号
//char*				内容


class CCmdPacket;

class CGameNetwork : public CBaseNetwork, public CSingleton<CGameNetwork>
{
private:
	static const short SYS_PACKET_SET_SEED;		//加密种子协议号

#include "yonPack.h"
	struct SSeedPacket{
		unsigned char Type;
		unsigned int Seed;
	} PACK_STRUCT;
#include "yonUnpack.h"

	bool m_bWaitingSeed;
	int m_iSeed;
	int m_iSeq;

	typedef core::map< MSG_PROTOCOL_TYPE, IDelegate<CCmdPacket*>* > ProtocolMap;
	ProtocolMap	m_protocolDelegateMap;	//网络消息事件处理器map

	static int generateSecretKey(int seed);

	virtual void _sendProc();
	virtual void _recvProc();

	void decrypt(char* data,int offset,int size);
	void encrypt(char* data,int offset,int size);
public:
	CGameNetwork();
	virtual ~CGameNetwork();

	virtual void Init();
	virtual bool Connect(const char *remote,int port,char *bindIp=0,int bindPort=0);
	void Close();

	// 注册网络消息事件处理器
	template <typename T, typename TP1>
	void RegisterEventHandler(MSG_PROTOCOL_TYPE code, T* _object, void (T::*_method)( TP1 p1 ))
	{
		m_protocolDelegateMap.set(code,::createDelegate(_object, _method));
	}

	// 注销网络消息事件处理器
	void UnregisterEventHandler(MSG_PROTOCOL_TYPE code)
	{
		m_protocolDelegateMap.remove(code);
	}

	virtual void Update();

	
};

#define GameNetwork()	CGameNetwork::GetInstance()

#endif