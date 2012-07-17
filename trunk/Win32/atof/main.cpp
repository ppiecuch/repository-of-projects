#include <stdio.h>
#include <list>
using namespace std;

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

double superAtof(char* str,int len)
{
	int start=-1,end=-1;
	for(int i=0;i<len;++i)
	{
		if((str[i]<'0'||str[i]>'9')&&str[i]!='.')
		{
			if(start==-1)
				continue;
			else
			{
				end=i;
				break;
			}
		}
		else if(start==-1)
		{
			start=i;
		}
	}
	return atof(str+start);
}

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();


	char* str="OpenGL ES-CM 1.1";
	printf("%.2f\n",superAtof(str,strlen(str)));
	getchar();
	return 0;
}