#include <stdio.h>
#include <string.h>
int main(int argc, char* argv[])
{
	//char    str1[20];
	char    str2[15]="Hello,World!";
	//sprintf(str1,"%-s%0*d",str2,sizeof(str1)-strlen(str2),0);
	//printf("str1:[%s][%d]\n",str1,strlen(str1));

	printf("%s%0*d\r\n",str2,5,0);

	getchar();
	return 0;
}