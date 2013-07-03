#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char ** argv)
{
	const char* str1="";
	printf("%d\r\n",strlen(str1));

	const char str2[]="\0";
	printf("%d\r\n",strlen(str2));

	const char str3[]={'\0'};
	printf("%d\r\n",strlen(str3));

	system("pause");
	return 0;
}