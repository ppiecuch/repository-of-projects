#include "stdio.h"
#include <iostream>
using namespace std;


int main(int argc, char* argv[])
{
	if(argc<3)
	{
		cout<<"No param!Please execute command as:"<<endl;
		cout<<"dae2y3d [inputfile] [outputfile]"<<endl;
		return 0;
	}

	const char* input=argv[1];
	const char* output=argv[2];
	FILE* file;
	errno_t ir=fopen_s(&file,input,"rb+");
	if(ir!=0)
	{
		cout<<"No input file,error:"<<ir<<"!"<<endl;
		return 0;
	}
	fclose(file);
	errno_t or=fopen_s(&file,output,"rb+");
	if(or==0)
	{
		cout<<"Already has output file,error:"<<or<<"!"<<endl;
		fclose(file);
		return 0;
	}
}