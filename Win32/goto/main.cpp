#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
	for(int i=0;i<5;++i)
	{
		for(int j=0;j<5;++j)
		{
			printf("%d,%d\r\n",i,j);
			if(j==2)
				goto LABEL;
		}
	}

LABEL:

	system("pause");
	return 0;
}