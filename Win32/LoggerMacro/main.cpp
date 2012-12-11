#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
const static unsigned int BUFFER_SIZE=1024;
char m_buffer[BUFFER_SIZE];
void Log(const char* format,...){
	va_list arg;
	va_start(arg,format);
	memset(m_buffer,0x0,BUFFER_SIZE);
	vsprintf_s(m_buffer,BUFFER_SIZE,format,arg);
	printf("%s",m_buffer);
	va_end(arg);
}

#define YON_DEBUG(format, ...) Log(format,##__VA_ARGS__)

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	Log("%s\r\n","helloworld");
	YON_DEBUG("helloworld\r\n");
	YON_DEBUG("%s\r\n","helloworld");

	system("pause");
	return 0;
}