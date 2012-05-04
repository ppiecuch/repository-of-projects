
#include "PngFile.h"

#include <crtdbg.h>
#include <typeinfo>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	unsigned char *dataBuf = NULL;
	unsigned int width, height;
	int components;
	if(CPngFile::load("./pngtest.png",&dataBuf, &width, &height, &components))
	{
		printf("success\n");
	}
	delete[] dataBuf;
	getchar();
	return 0;
}
