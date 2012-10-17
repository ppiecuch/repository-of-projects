#include <stdio.h>

#ifdef _WIN32
#include <winsock.h>
#define socklen_t int
#pragma comment(lib, "Ws2_32.lib")
#else
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#define closesocket(_x) close(_x)
#endif


#ifdef _WIN32
#define THREAD_PROC WINAPI
#define THREAD_RETURN DWORD
#define THREAD_PARAM void *
#define THREAD_ID DWORD
#define CPO CRITICAL_SECTION
#define CPO_Init(_x) InitializeCriticalSection(&_x)
#define CPO_Dele(_x) DeleteCriticalSection(&_x)
#define CPO_Enter(_x) EnterCriticalSection(&_x)
#define CPO_Leave(_x) LeaveCriticalSection(&_x)
#else
struct CPO { pthread_mutex_t m; pthread_t t;};
#define THREAD_PROC
#define THREAD_RETURN void *
#define THREAD_PARAM void *
#define THREAD_ID pthread_t
#define CPO_Init(_x) { _x.t=0; pthread_mutex_init(&_x.m, NULL); }
#define CPO_Dele(_x) { pthread_mutex_destroy(&_x.m); }
#define CPO_Enter(_x) while(true) 

{ 
	if(_x.t==0) 
	{ 
		pthread_mutex_lock(&_x.m); 
		_x.t=pthread_self(); 
		break;
	}
	if(pthread_self()==_x.t)
		break; 
	pthread_mutex_lock(&_x.m); 
	_x.t=pthread_self();
	break; 
}
#define CPO_Leave(_x) { pthread_mutex_unlock(&_x.m); _x.t=0; }
#endif


typedef THREAD_RETURN THREAD_PROC THREAD_FUNCTION(THREAD_PARAM thread_param);
#ifdef _WIN32

THREAD_ID bcreate_thread(THREAD_FUNCTION pfun, void * pparam)
{
	THREAD_ID tid;
	if(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pfun, (LPVOID)pparam, 0, &tid)==NULL)
		return (THREAD_ID)-1;
	return tid;
}
#else
THREAD_ID bcreate_thread(THREAD_FUNCTION pfun, void * pparam)
{
	THREAD_ID tid;
	if(pthread_create(&tid, NULL, (void * (*)(void *))pfun, pparam)<1)
		return (THREAD_ID)-1;
	pthread_detach(tid);
	return tid;
}
#endif

#ifndef SOCKET
#define SOCKET int
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

bool bsocket_init()
{
#ifdef _WIN32
	WSADATA wsad;
	WORD wVersionReq;
	wVersionReq=MAKEWORD(1,1);
	if(WSAStartup(wVersionReq,&wsad))
		return false;
	return true;
#else
	signal(SIGPIPE, SIG_IGN);
	return true;
#endif
}

void bsocket_cleanup()
{
#ifdef _WIN32
	WSACleanup();
#else
	signal(SIGPIPE, SIG_DFL);
	return;
#endif
}

#define WEB_SERVER_PORT 90
#define WEB_SERVER_IP "127.0.0.1"
#define HTTP_HEAD "HTTP/1.0 200 OK\x0d\x0a""Content-type: text/html\x0d\x0a\x0d\x0a"
#define WELCOME_HTML "<html>\n<body>\n<B>Welcome to my Website!</B><hr>\n<center>g_hits: %d</center>\n</body>\n</html>\n"

CPO g_cpo;
int g_hits;

THREAD_RETURN THREAD_PROC do_accept(THREAD_PARAM param)
{
	SOCKET sock=((SOCKET *)param)[0];
	char ptmp[2048];
	recv(sock, ptmp, 2048, 0);
	CPO_Enter(g_cpo);
	int a=g_hits;
	sprintf(ptmp, WELCOME_HTML, a++);
	g_hits=a;
	CPO_Leave(g_cpo);
	send(sock, HTTP_HEAD, strlen(HTTP_HEAD), 0);
	send(sock, ptmp, strlen(ptmp), 0);
	closesocket(sock);
	return 0;
}


int main(int argc,char ** argv)
{
	char perror[1024];
	SOCKET s,rs;
	sockaddr_in sin,rsin;
	socklen_t slen;
	bool result=false;
	perror[0]=0;
	CPO_Init(g_cpo);

	g_hits=0;
	if(!bsocket_init())
	{
		strcpy(perror, "Can't init socket!");
		goto error_out;
	}
	s=socket(PF_INET,SOCK_STREAM,0);
	if(s==INVALID_SOCKET)
	{
		strcpy(perror, "Can't create socket!");
		goto error_out;
	}
	sin.sin_family=AF_INET;
	sin.sin_port=htons(WEB_SERVER_PORT);
	sin.sin_addr.s_addr=inet_addr(WEB_SERVER_IP);
	slen=sizeof(sin);
	if(bind(s,(sockaddr *)&sin,slen)==SOCKET_ERROR)
	{
		strcpy(perror, "Can't bind socket!");
		goto error_out;
	}
	if(listen(s,5)==SOCKET_ERROR)
	{
		strcpy(perror, "Can't listen on this socket!");
		goto error_out;
	}
	printf("web server running......\n");
	slen=sizeof(rsin);
	while(true)
	{
		rs=accept(s,(sockaddr *)&rsin,&slen);
		if(rs==INVALID_SOCKET)
		{
			strcpy(perror, "accept() a INVALID_SOCKET!");
			break;
		}
		bcreate_thread(do_accept, &rs);
	}
	result=true;

error_out:
	if(s!=INVALID_SOCKET) closesocket(s);
	if(rs!=INVALID_SOCKET) closesocket(rs);
	if(!result) { printf(perror); printf("\n"); }
	CPO_Dele(g_cpo);
	bsocket_cleanup();

	return 0;
} 
