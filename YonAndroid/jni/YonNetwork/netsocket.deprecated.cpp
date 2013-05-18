/*
netsocket.cpp: implementation of the CNetSocket class.
*/

#include "netsocket.h"
#include <stdio.h>

#ifdef WIN32
	#pragma comment(lib, "Ws2_32.lib")
#endif

int CNetSocket::m_nCount = 0;

CNetSocket::CNetSocket()
{
	Reset();
}

CNetSocket::~CNetSocket()
{
	Close();
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
	if (err!=0)									
	{
		Sys_Log("WSAStartup Error");
		WSACleanup();
		return false;
	}

	/* version error */
	if (LOBYTE(wsaData.wVersion)!= VersionLow ||
        HIBYTE(wsaData.wVersion)!= VersionHigh ) 
	{
		Sys_Log("WSAStartup Version Error");
		WSACleanup();
		return false;
	}
	Sys_Log("WSAStartup OK");
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

bool CNetSocket::Initialize(int protocol)
{
	if (m_nCount++==0)
		if (!_NetStartUp(1,1)) return false;

	if (protocol==PROTOCOL_UDP)
	{
		m_socket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}
	else 
	{
		m_socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	}

	if(m_socket==INVALID_SOCKET) 
	{
		return false;
	}
	SetNonBlocking();
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
		Sys_Log("bind socket error");
		return false;
	}
	return true;
}

bool CNetSocket::Listen()
{
	if(listen(m_socket,SOMAXCONN)==SOCKET_ERROR)
	{
		Sys_Log("NetSocket:listen error");
		return false;
	}
	return true;
}

bool CNetSocket::Connect(const char *szAddr, int port, unsigned long ip)
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
	
	if(connect(m_socket,(SOCKADDR *)&addrRemote,sizeof(addrRemote))
		==SOCKET_ERROR)
	{
		int err = GETERROR;
		if (err != CONN_INPRROGRESS)
		{
			Sys_Log("socket connect error = %d",err); 
			return false;
		}
	}
	return true;
}

/*
 * return value 
 * =  0 send failed
 * >  0	bytes send
 * = -1 net dead
 */
int CNetSocket::Send(char *buf, int len)
{
	if (!CanWrite()) return 0;
	
	int ret;
	/*
	in linux be careful of SIGPIPE
	*/
	ret = send(m_socket,buf,len,0);
	if (ret==SOCKET_ERROR)
	{
		int err=GETERROR;
		if (err==WSAEWOULDBLOCK) return 0;
		return -1;
	}
	return ret;
}

/*
 * return value 
 * =  0 recv failed
 * >  0	bytes recv
 * = -1 net dead
 */
int CNetSocket::Recv(char *buf, int len)
{
	if (CanRead()==false) 
		return 0;

	int ret;
	/* in linux be careful of SIGPIPE */
	ret = recv(m_socket,buf,len,0);
	
	if (ret==0)
	{
		/* remote closed */
		return -1;
	}

	if (ret==SOCKET_ERROR)
	{
		int err=GETERROR;
		if (err!=WSAEWOULDBLOCK)
		{
			return -1;
		}
	}
	return ret;
}

bool CNetSocket::CanRead()
{
	fd_set readfds;
	timeval timeout;

	timeout.tv_sec=0;
	timeout.tv_usec=0;
	FD_ZERO(&readfds);
	FD_SET(m_socket,&readfds);
	int ret = select(FD_SETSIZE,&readfds,NULL,NULL,&timeout);
	if(ret > 0 && FD_ISSET(m_socket,&readfds))
		return true;
	else 
		return false;
}

bool CNetSocket::CanWrite()
{
	fd_set writefds;
	timeval timeout;

	timeout.tv_sec=0;
	timeout.tv_usec=0;
	FD_ZERO(&writefds);
	FD_SET(m_socket,&writefds);
	int ret = select(FD_SETSIZE,NULL,&writefds,NULL,&timeout);
	if(ret > 0 && FD_ISSET(m_socket,&writefds))
		return true;
	else 
		return false;
}

bool CNetSocket::Close()
{
	if (m_socket == INVALID_SOCKET) return false;
	CLOSESOCKET(m_socket);
	Reset();
	if (--m_nCount==0)
		_NetCleanUp();
	return true;
}

SOCKET CNetSocket::Accept()
{
	SOCKADDR_IN addr;
	int len = sizeof(addr);
	SOCKET tmp;
	tmp = accept(m_socket,(SOCKADDR *)&addr,(socklen_t *)&len);
	if (tmp == INVALID_SOCKET || tmp == NULL)
	{
		Sys_Log("accept error");
		return NULL;
	}
	m_nCount++;
	return tmp;	
}

bool CNetSocket::Attach(SOCKET socket)
{
	m_socket = socket;
	SetNonBlocking();
	return true;
}

int CNetSocket::SendTo(char *buf, int len, SOCKADDR_IN *addr)
{
	if (!CanWrite()) return 0;
	
	int ret;
	ret = sendto(m_socket,buf,len,0,(SOCKADDR *)addr,sizeof(SOCKADDR_IN));
	if (ret==SOCKET_ERROR)
	{
		int err=GETERROR;
		if (err!=WSAEWOULDBLOCK)
		{
			return -1;
		}
	}
	return ret;
}

int CNetSocket::RecvFrom(char *buf, int len, SOCKADDR_IN *addr ,int *addrlen)
{
	if (!CanRead()) return 0;

	int ret;
	ret = recvfrom(m_socket,buf,len,0,(SOCKADDR *)addr,(socklen_t *)addrlen);
	if (ret==SOCKET_ERROR)
	{
		int err=GETERROR;
		if (err!=WSAEWOULDBLOCK)
		{
			return -1;
		}
	}
	return ret;
}

bool CNetSocket::HasExcept()
{
	fd_set exceptfds;
	timeval timeout;

	timeout.tv_sec=0;
	timeout.tv_usec=0;
	FD_ZERO(&exceptfds);
	FD_SET(m_socket,&exceptfds);
	int ret = select(FD_SETSIZE,NULL,NULL,&exceptfds,&timeout);
	if(ret > 0 && FD_ISSET(m_socket,&exceptfds))
		return true;
	else 
		return false;
}

void CNetSocket::Reset()
{
	m_socket = INVALID_SOCKET;
}

bool CNetSocket::SetNonBlocking()
{
	/* set to nonblocking mode */
	u_long arg;
	arg = 1;
	if (IOCTLSOCKET(m_socket,FIONBIO,&arg)==SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CNetSocket::SetSendBufferSize(int len)
{
	int ret;
	ret = setsockopt(m_socket,SOL_SOCKET,SO_SNDBUF,(char *)&len,sizeof(len));
	if (ret == SOCKET_ERROR) return false;
	return true;
}

bool CNetSocket::SetRecvBufferSize(int len)
{
	int ret;
	ret = setsockopt(m_socket,SOL_SOCKET,SO_RCVBUF,(char *)&len,sizeof(len));
	if (ret == SOCKET_ERROR) return false;
	return true;
}

/* 
 * get local address 
 */
bool CNetSocket::GetLocalAddr(char *addr, short *port, unsigned long *ip)
{
	SOCKADDR_IN addrLocal;
	socklen_t len = sizeof(addrLocal);
	if(getsockname(m_socket,(SOCKADDR*)&addrLocal,&len)==SOCKET_ERROR)
		return false;
	
	char *tmp = inet_ntoa(addrLocal.sin_addr);
	if(!tmp) 
		return false;
	if(addr) 
		strcpy(addr,tmp);
	if(port)
		*port = ntohs(addrLocal.sin_port);
	if(ip) 
		*ip = addrLocal.sin_addr.s_addr;
	return true;
}

/* 
 * get remote address 
 */
bool CNetSocket::GetRemoteAddr(char *addr, short *port,unsigned long *ip)
{
	sockaddr_in addrRemote;
	int len = sizeof(addrRemote);
	if(getpeername(m_socket,(sockaddr *)&addrRemote,(socklen_t *)&len)==SOCKET_ERROR)
		return false;
	
	char *tmp = inet_ntoa(addrRemote.sin_addr);
	if(!tmp) 
		return false;
	if(addr)
		strcpy(addr,tmp);
	if(port)
		*port = ntohs(addrRemote.sin_port);
	if(ip)
		*ip = addrRemote.sin_addr.s_addr; 
	return true;
}

bool CNetSocket::SetReuseAddr(bool reuse)
{
#ifndef WIN32
	/* only useful in linux */
	int opt = 0;
	unsigned int len = sizeof(opt);

	if(reuse) opt = 1;
	if(setsockopt(m_socket,SOL_SOCKET,SO_REUSEADDR,
		(const void*)&opt,len)==SOCKET_ERROR)
	{
		return false;
	}
	else
	{
		return true;
	}
#endif
	return true;
}
