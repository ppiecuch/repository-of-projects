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
	printf("%s","���");
	wprintf(L"%s",L"���");

	cout<<"���"<<endl;
	MessageBox(NULL,TEXT("���Hello World!"),TEXT("Cͼ�γ���"),MB_OK);
	MessageBox(NULL,L"���Hello World!",L"Cͼ�γ���",MB_OK);

	print(__LINE__);
	getchar();
	return 0;
}
