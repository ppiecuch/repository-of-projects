#include "stdio.h"


#include "yon.h"
using namespace yon;

#pragma comment(lib, "Yon.lib")

int main(int argc, char* argv[])
{
	SYonEngineParameters params;
	YonEngine* pYE=CreateEngine(params);

	core::stringc str;
	printf("%d,%02x\n",str.length(),str[0]);

	getchar();
	return 0;
}