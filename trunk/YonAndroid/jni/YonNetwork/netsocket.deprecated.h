/*!
@file		NetSocket.h: 
@author		Guo Huafeng
@date		08/2012
@desc		interface for the NetSocket class.
			��װSocket API��ʹ���ƽ̨����WIN32, Linux, iOS, Android�ȵ�
*/
#ifndef __NET_SOCKET_H__
#define __NET_SOCKET_H__

#include "netplatform.h"

#ifdef WIN32
	/*
	for windows
	*/
	#include <winsock.h>
	#define GETERROR			WSAGetLastError()
	#define CLOSESOCKET(s)		closesocket(s)
	#define IOCTLSOCKET(s,c,a)  ioctlsocket(s,c,a)
	#define CONN_INPRROGRESS	WSAEWOULDBLOCK
	typedef int socklen_t;
#else
	/*
	for linux
	*/
	#include <sys/time.h>
	#include <stddef.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <sys/wait.h>
	typedef int            BOOL;
	typedef unsigned char  BYTE;
	typedef unsigned short WORD;
	typedef unsigned int   DWORD;
	#define TRUE  1
	#define FALSE 0

	/*
	for socket
	*/
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <sys/ioctl.h>
	#include <netdb.h>
	#include <sys/errno.h>
	#include <arpa/inet.h>

	typedef int SOCKET;
	typedef sockaddr_in			SOCKADDR_IN;
	typedef sockaddr			SOCKADDR;
	#define INVALID_SOCKET	    (-1)
	#define SOCKET_ERROR        (-1)
	#define GETERROR			errno
	#define WSAEWOULDBLOCK		EWOULDBLOCK
	#define CLOSESOCKET(s)		close(s)
	#define IOCTLSOCKET(s,c,a)  ioctl(s,c,a)
	#define CONN_INPRROGRESS	EINPROGRESS
#endif

const int PROTOCOL_UDP	=	1;
const int PROTOCOL_TCP	=	2;

class CNetSocket  
{
public:
	CNetSocket();
	virtual ~CNetSocket();
			
	//�ҽ��׽���
	bool Attach(SOCKET socket);
	//�ر��׽���
	bool Close();
	//���ӷ�����
	bool Connect(const char *szAddr,int port,unsigned long ip = 0);
	//��ʼ����
	bool Listen();
	//��ʼ��
	bool Initialize(int protocol);
	
	//������
	int Recv(char *buf,int len);
	//������
	int Send(char *buf,int len);

	//�����ݣ�UDPЭ��
	int RecvFrom(char *buf,int len,SOCKADDR_IN *addr,int *addrlen);
	//�����ݣ�UDPЭ��
	int SendTo(char *buf,int len,SOCKADDR_IN *addr);

	// �ж��׽����Ƿ�ɶ�/д��NOTE: ʹ��select IOģ��
	bool CanWrite();
	bool CanRead();
	// �Ƿ����쳣
	bool HasExcept();

	// ���÷�����״̬
	bool SetNonBlocking();
	// ��ip��ַ�Ͷ˿�
	bool BindAddr(char *ip,int port);
	// ��λ
	void Reset();

	// ���û�������С
	bool SetSendBufferSize(int len);
	bool SetRecvBufferSize(int len);
	bool SetReuseAddr(bool reuse);

	bool GetLocalAddr (char *addr, short *port,unsigned long *ip = NULL);
	bool GetRemoteAddr(char *addr,short *port,unsigned long *ip = NULL);
	
	//����һ������
	SOCKET Accept();

	SOCKET GetSocket() { return m_socket; }

private:
	bool _NetStartUp(int VersionHigh,int VersionLow);
	bool _NetCleanUp();

	SOCKET		m_socket;		//ƽ̨��ص��׽���
	static int	m_nCount;		//���ü���
};

#endif


