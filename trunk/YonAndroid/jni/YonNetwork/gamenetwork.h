#ifndef _GAMENETWORK_H_
#define _GAMENETWORK_H_
#include "basenetwork.h"
#include "ISingleton.h"
#include "gamenetworkprotocol.h"

/**��Կ����ʽ**/
//unsigned short	����
//unsigned char		����(Ŀǰֻ��һ��)
//char*				����

/**���հ���ʽ**/
//unsigned short	����
//char*				����

/**���Ͱ���ʽ**/
//unsigned short	����(�������ȣ��������������ݵĳ���)
//unsigned char		ѭ�����
//char*				����


class CCmdPacket;

class CGameNetwork : public CBaseNetwork, public CSingleton<CGameNetwork>
{
private:
	static const short SYS_PACKET_SET_SEED;		//��������Э���

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
	ProtocolMap	m_protocolDelegateMap;	//������Ϣ�¼�������map

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

	// ע��������Ϣ�¼�������
	template <typename T, typename TP1>
	void RegisterEventHandler(MSG_PROTOCOL_TYPE code, T* _object, void (T::*_method)( TP1 p1 ))
	{
		m_protocolDelegateMap.set(code,::createDelegate(_object, _method));
	}

	// ע��������Ϣ�¼�������
	void UnregisterEventHandler(MSG_PROTOCOL_TYPE code)
	{
		m_protocolDelegateMap.remove(code);
	}

	virtual void Update();

	
};

#define GameNetwork()	CGameNetwork::GetInstance()

#endif