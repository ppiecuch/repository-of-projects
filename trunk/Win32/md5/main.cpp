#include<stdlib.h>
#include <stdio.h>
#include<string>
#include "MD5.h"
using namespace std;

const static char* input="ver.txt";
const static char* output="res.ver";

typedef unsigned char u8;

string readVersion(){
	FILE* file=NULL;
	int nLen=0;
	char chBuffer[1024];
	if(fopen_s(&file,input, "rb"))   
	{   
		printf("open %s failed!",input);
	}
	else
	{
		nLen = fread (chBuffer, 1, 1024, file);
		fclose (file);   
	}
	chBuffer[nLen]=0x0;
	string str=chBuffer;
	return str;
}

void bigEndian(int& num){
	u8 temp[4];
	for(int i=0;i<4;++i){
		temp[3-i]=((u8*)&num)[i];
	}
	memcpy(&num,temp,4);
}

void bigEndian(short& num){
	u8 temp[2];
	for(int i=0;i<2;++i){
		temp[1-i]=((u8*)&num)[i];
	}
	memcpy(&num,temp,2);
}

void generateOutput(int ver,string& md5Str){
	FILE* file=NULL;
	int nLen=0;
	if(fopen_s(&file,output, "w"))   
	{   
		printf("open %s failed!",input);
	}
	else
	{
		bigEndian(ver);
		fwrite(&ver,1,4,file);
		short len=md5Str.length();
		bigEndian(len);
		fwrite(&len,1,2,file);
		fwrite(md5Str.c_str(),1,md5Str.length(),file);
		fclose (file);   
	}
}

int main(int argc,char ** argv)
{
	string version=readVersion();
	int ver=atoi(version.c_str());
	int len = version.length();
	char* buffer=new char[len+1];
	memset(buffer,0x0,len+1);
	memcpy(buffer,version.c_str(),len);
	printf("%s,%d(%d)\n",buffer,ver,len);
	string md=MD5String(buffer);
	int i=0;
	len = md.length();
	while (i != len)
	{
		md[i] = toupper(md[i]);
		++i;
	}
	printf("md5:%s\n",md.c_str());

	generateOutput(ver,md);

	delete[] buffer;
	system("pause");
	return 0;
}