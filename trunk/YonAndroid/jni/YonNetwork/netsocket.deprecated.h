/*!
@file		NetSocket.h: 
@author		Guo Huafeng
@date		08/2012
@desc		interface for the NetSocket class.
			封装Socket API，使其跨平台，如WIN32, Linux, iOS, Android等等
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
			
	//挂接套接字
	bool Attach(SOCKET socket);
	//关闭套接字
	bool Close();
	//连接服务器
	bool Connect(const char *szAddr,int port,unsigned long ip = 0);
	//开始监听
	bool Listen();
	//初始化
	bool Initialize(int protocol);
	
	//收数据
	int Recv(char *buf,int len);
	//发数据
	int Send(char *buf,int len);

	//收数据，UDP协议
	int RecvFrom(char *buf,int len,SOCKADDR_IN *addr,int *addrlen);
	//发数据，UDP协议
	int SendTo(char *buf,int len,SOCKADDR_IN *addr);

	// 判断套接字是否可读/写，NOTE: 使用select IO模型
	bool CanWrite();
	bool CanRead();
	// 是否发生异常
	bool HasExcept();

	// 设置非阻塞状态
	bool SetNonBlocking();
	// 绑定ip地址和端口
	bool BindAddr(char *ip,int port);
	// 复位
	void Reset();

	// 设置缓冲区大小
	bool SetSendBufferSize(int len);
	bool SetRecvBufferSize(int len);
	bool SetReuseAddr(bool reuse);

	bool GetLocalAddr (char *addr, short *port,unsigned long *ip = NULL);
	bool GetRemoteAddr(char *addr,short *port,unsigned long *ip = NULL);
	
	//接受一个连接
	SOCKET Accept();

	SOCKET GetSocket() { return m_socket; }

private:
	bool _NetStartUp(int VersionHigh,int VersionLow);
	bool _NetCleanUp();

	SOCKET		m_socket;		//平台相关的套接字
	static int	m_nCount;		//引用计数
};

#endif


