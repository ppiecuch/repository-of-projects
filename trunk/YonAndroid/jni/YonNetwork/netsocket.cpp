#include "netsocket.h"
#include <stdio.h>

#ifdef WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

CNetSocket::CNetSocket():m_socket(INVALID_SOCKET)
{
}

CNetSocket::~CNetSocket()
{
	Close();
}

core::stringc CNetSocket::getErrorStr(int err)
{
#ifdef WIN32
	char errmsg[1024]={0};
	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,0,err,0,errmsg,1024,NULL);
	return errmsg;
#else
	char errmsg[1024]={0};
	strerror_r(err,errmsg,1024);
	core::stringc str=errmsg;
	return str;
#endif
	return core::stringc("");
}

bool CNetSocket::_NetStartUp(int VersionHigh,int VersionLow)
{
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(VersionHigh,VersionLow);
	err=WSAStartup(wVersionRequested, &wsaData);

	/* startup failed */
	if (err)									
	{
		err=GETERROR;
		YON_ERROR(YON_LOG_FAILED_FORMAT,core::stringc("socket startup,for:%s",getErrorStr(err).c_str()).c_str());
		WSACleanup();
		return false;
	}

	/* version error */
	if (LOBYTE(wsaData.wVersion)!= VersionLow ||
		HIBYTE(wsaData.wVersion)!= VersionHigh ) 
	{
		YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("socket startup with unexpected version:%d",wsaData.wVersion).c_str());
	}

	YON_DEBUG("socket startup completed!\r\n");
#endif
	return true;
}

bool CNetSocket::_NetCleanUp()
{
#ifdef WIN32	
	WSACleanup();
#endif
	return true;
}


bool CNetSocket::Initialize()
{
	if (!_NetStartUp(1,1)) 
		return false;

	m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_socket==SOCKET_ERROR)
	{
		s32 error=GETERROR;
		YON_ERROR(YON_LOG_FAILED_FORMAT,core::stringc("init socket failed,for:%s ",getErrorStr(error).c_str()).c_str());
		return false;
	}
	return true;
}

bool CNetSocket::BindAddr(char *ip,int port)
{
	SOCKADDR_IN addrLocal;
	addrLocal.sin_family=AF_INET;
	addrLocal.sin_port=htons(port);
	if(ip)
	{
		addrLocal.sin_addr.s_addr=inet_addr(ip);
	}
	else
	{
		addrLocal.sin_addr.s_addr=htonl(INADDR_ANY);
	}
	if(bind(m_socket,(SOCKADDR *)&addrLocal,sizeof(addrLocal))==SOCKET_ERROR)
	{
		s32 error=GETERROR;
		YON_ERROR(YON_LOG_FAILED_FORMAT,core::stringc("socket bind:%s:%d failed,for:%s",ip,port,getErrorStr(error).c_str()).c_str());
		return false;
	}
	return true;
}

bool CNetSocket::Connect(const char *szAddr,int port,unsigned long ip)
{
	SOCKADDR_IN addrRemote;
	hostent *host=NULL;

	memset(&addrRemote,0,sizeof(addrRemote));
	addrRemote.sin_family=AF_INET;
	addrRemote.sin_port=htons(port);

	if(szAddr)
		addrRemote.sin_addr.s_addr = inet_addr(szAddr);
	else
		addrRemote.sin_addr.s_addr = ip;

	if(addrRemote.sin_addr.s_addr==INADDR_NONE)
	{
		if(!szAddr) return false;
		host=gethostbyname(szAddr);
		if(!host) return false;
		memcpy(&addrRemote.sin_addr,host->h_addr_list[0],host->h_length);
	}

	if(connect(m_socket,(SOCKADDR *)&addrRemote,sizeof(addrRemote))==SOCKET_ERROR)
	{
		int err = GETERROR;
		if (err != CONN_INPRROGRESS)
		{
			YON_ERROR(YON_LOG_FAILED_FORMAT,core::stringc("socket connect:%s:%d failed,for:%s",szAddr,port,getErrorStr(err).c_str()).c_str());
			return false;
		}
	}
	return true;
}

/*
* return value 
* =  0 recv failed
* >  0	bytes recv
* = -1 net dead
*/
int CNetSocket::Recv(char *buf,int len)
{
	int ret,err;
	/* in linux be careful of SIGPIPE */
	do {
		ret = recv(m_socket, buf, len, 0);
		if (ret == SOCKET_ERROR) {
			err = GETERROR;
		} 
	} while (ret == SOCKET_ERROR && err == WSAEINTR);

	if (ret==0)
	{
	//	/* remote closed */
		YON_DEBUG("recv remote close\r\n");
		return -1;
	}

	if (ret==SOCKET_ERROR)
	{
		int err=GETERROR;
		if (err!=WSAEWOULDBLOCK)
		{
			YON_DEBUG("recv local close\r\n");
			return -1;
		}
	}
	return ret;
}

/*
* return value 
* =  0 send failed
* >  0	bytes send
* = -1 net dead
*/
int CNetSocket::Send(const char *buf,int len)
{
	int ret,err;
	do {
		ret = send(m_socket,buf,len,0);
		if (ret == SOCKET_ERROR) {
			err = GETERROR;
			YON_WARN("send SOCKET_ERROR\r\n");
		} 
	} while (ret == SOCKET_ERROR && err == WSAEINTR);

	if (ret==SOCKET_ERROR)
	{
		int err=GETERROR;
		if (err==WSAEWOULDBLOCK) return 0;
		//if (err==WSAECONNABORTED)
		//	YON_DEBUG("send local close\r\n");
		//else
		//	YON_DEBUG("send remote close\r\n");
		return -1;
	}
	return ret;
}

bool CNetSocket::Close()
{
	if (m_socket == INVALID_SOCKET)
		return false;
	shutdown(m_socket,SHUT_RDWR);
	CLOSESOCKET(m_socket);
	m_socket = INVALID_SOCKET;
	_NetCleanUp();
	YON_DEBUG("socket close completed!\r\n");
	return true;
}