#include "stdio.h"


#include "yon.h"
using namespace yon;

#pragma comment(lib, "Yon.lib")

int main(int argc, char* argv[])
{
	SYonEngineParameters params;
	YonEngine* pYE=CreateEngine(params);

	getchar();
	return 0;
}