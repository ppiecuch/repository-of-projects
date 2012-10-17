#include <stdio.h>
#include <windows.h>
#include "CI18N.h"
#include <string>
using namespace std;


inline char* createUTF8(char* gb2312)
{
	int buffLen = 0;
	WCHAR wbuff[5120];
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wbuff, 5120);
	buffLen = WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, NULL, 0, 0, 0);
	char* m_utf8 = new char[buffLen+1];
	WideCharToMultiByte(CP_UTF8, 0, wbuff, -1, (LPSTR)m_utf8, buffLen, 0, 0);
	return m_utf8;
}




int main(int argc, char* argv[])
{
	//char* temp=new char[10];
	//ZeroMemory(temp,10);

	//CI18N::UnicodeToUTF_8(temp,L"我");
	//printf("%s\r\n","我");

	//string str;
	//CI18N::UTF_8ToGB2312(str,"我",2);
	//printf("%s\r\n",str.c_str());

	char* temp=createUTF8("中国人");
	printf("%s\r\n",temp);

	delete[] temp;
	getchar();
}