#include<windows.h>
#include <stdio.h>
#include <locale>
#include <iostream>
using namespace std;

void print(int line)
{
	printf("test:%i\n",line);
}

int main(int argc, char* argv[])
{
	::setlocale(LC_ALL, "chs"); 
	printf("%s","你好");
	wprintf(L"%s",L"你好");

	cout<<"你好"<<endl;
	MessageBox(NULL,TEXT("你好Hello World!"),TEXT("C图形程序"),MB_OK);
	MessageBox(NULL,L"你好Hello World!",L"C图形程序",MB_OK);

	print(__LINE__);
	getchar();
	return 0;
}
