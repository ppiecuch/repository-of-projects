#include <windows.h>
#include <stdio.h>
#include <MMSystem.h>
#include <shellapi.h>
#include <AFXCOM_.H> 
#include <string>
#include <iostream>
#pragma comment(lib,"winmm.lib")

using namespace std;

/*
��������
mciSendString("play music",buf,sizeof(buf),NULL); 
ȫ�����ţ� 
mciSendString("play movie fullscreen",buf,sizeof(buf),NULL); 
��ͣ���š� 
mciSendString("pause movie",buf,sizeof(buf),NULL); 
ֹͣ���š� 
mciSendString("close movie",buf,sizeof(buf),NULL);
*/

int play(string &music_file)   //���Ĳ��ź���
{
	char returnstr[200]={0};
	unsigned long time=0;
	string Path(music_file);        
	string::size_type pos=Path.find_first_of('\\',0);
	while(pos!=string::npos)
	{
		Path.at(pos)='/';
		pos=Path.find_first_of('\\',pos);
	}
	string status="status "+Path+" length";
	string play="play "+Path+" wait";
	mciSendStringA(status.c_str(), returnstr,200, NULL);
	if (strlen(returnstr)==0)
		exit(1);
	cout<<"���ڲ���\t"<<music_file<<endl;
	mciSendStringA(play.c_str(),returnstr,200,NULL);
	return 0;
}

int main(int argc, char* argv[]){

	printf("play:%d\n",play(string("bg.mp3")));
	getchar();
}