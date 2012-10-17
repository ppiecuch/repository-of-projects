#include <stdlib.h>
//include <sys/ipc.h>
#include <stdio.h>
#include <string.h>
//#include <fcntl.h>
//include <unistd.h>
#include <iconv.h>

#pragma comment(lib, "IconvLib-1.11.1.lib")

int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;
	size_t iconv_ret;
	cd = iconv_open(to_charset,from_charset);
	
	if (cd==(iconv_t)-1)
	{
		perror("iconv_open():");
		exit(0);
	}
	
	memset(outbuf,0,outlen);
	iconv_ret = iconv(cd,pin,&inlen,pout,&outlen);

	if (iconv_ret == (size_t)-1)
	{
		perror("iconv():");
		exit(0);
	}

	iconv_close(cd);
	return 0;
}

int u2g(char *inbuf,int inlen,char *outbuf,int outlen)   //UNICODE码转为GB2312码
{
	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
}

int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)  //GB2312码转为UNICODE码
{
	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}



/*
这是一个iconv的测试例程：
“纵海杯”东南大学第三届嵌入式系统设计邀请赛
*/

int main(void)
{

#if 0
	unsigned char in_utf8[] = 
	{0x20,0xe8,0xbf,0x99,0xe6,0x98,0xaf,0xe4,0xb8,0x80,0xe4,0xb8,0xaa,0x69,0x63,0x6f,
	 0x6e,0x76,0xe7,0x9a,0x84,0xe6,0xb5,0x8b,0xe8,0xaf,0x95,0xe4,0xbe,0x8b,0xe7,0xa8,
	 0x8b,0xef,0xbc,0x9a,0x0a,0xe2,0x80,0x9c,0xe7,0xba,0xb5,0xe6,0xb5,0xb7,0xe6,0x9d,
	 0xaf,0xe2,0x80,0x9d,0xe4,0xb8,0x9c,0xe5,0x8d,0x97,0xe5,0xa4,0xa7,0xe5,0xad,0xa6,
	 0xe7,0xac,0xac,0xe4,0xb8,0x89,0xe5,0xb1,0x8a,0xe5,0xb5,0x8c,0xe5,0x85,0xa5,0xe5,
	 0xbc,0x8f,0xe7,0xb3,0xbb,0xe7,0xbb,0x9f,0xe8,0xae,0xbe,0xe8,0xae,0xa1,0xe9,0x82,
	 0x80,0xe8,0xaf,0xb7,0xe8,0xb5,0x9b,0x00};    //UTF-8编码
	char in_gb2312[] =" 这是一个iconv的测试例程：\n\
“纵海杯”东南大学第三届嵌入式系统设计邀请赛"; //GB2312编码
	char out[256];
	int rc;

	printf("\r\n------iconv test------\r\n");


	printf("\r\n---Start UTF8 to GB2312 convert...\r\n");
	rc = u2g(in_utf8,strlen(in_utf8),out,256);
	printf("%s\n",out);
	
	rc = strcmp(in_gb2312,out);
	if(rc==0)
	{
		printf("---End UTF8 to GB2312 convert OK\r\n");
	}
	else
	{
		printf("---End UTF8 to GB2312 convert FAIL\r\n");
	}


	printf("\r\n---Start GB2312 to UTF8 convert...\n");
	rc = g2u(in_gb2312,strlen(in_gb2312),out,256);
	printf("%s\n",out);

	rc = strcmp(in_utf8,out);
	if(rc==0)
	{
		printf("---End GB2312 to UTF8 convert OK\r\n");
	}
	else
	{
		printf("---End GB2312 to UTF8 convert FAIL\r\n");
	}

	printf("\r\n------iconv test over------\r\n");
#else
	char* test="中华人民共和国";
	char buff1[256];
	char buff2[256];
	g2u(test,strlen(test),buff1,256);
	u2g(buff1,strlen(buff1),buff2,256);
	printf("%d,%d,%d,%s\r\n",strlen(test),strlen(buff1),strlen(buff2),buff2);
#endif
	system("pause");;
}