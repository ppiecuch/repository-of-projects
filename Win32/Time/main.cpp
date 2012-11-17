#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <windows.h>

int main(int argc, char* argv[])
{
	static struct _timeb start;
	_ftime64_s(&start); 
	printf("%lld,%u\r\n",start.time,start.millitm);
	Sleep(100);
	static struct _timeb end;
	_ftime64_s(&end); 
	printf("%lld,%u\r\n",end.time,end.millitm);
	system("pause");
	return 0;
}