#include "stdio.h"
#include <iostream>
#include "CXMLReaderImpl.h"
#include "CReadFileStream.h"
#include "CWriteFileStream.h"
#include "CLogger.h"
#include "dae2y3d.h"
using namespace std;

using namespace yon;
using namespace yon::io;



namespace yon{
	namespace debug{
		ILogger* Logger=NULL;
	}
}

int main(int argc, char* argv[])
{
	if(argc<3)
	{
		cout<<"No param!Please execute command as:"<<endl;
		cout<<"dae2y3d [inputfile] [outputfile] [loglevel]"<<endl;
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

	Logger=new debug::CLogger();

	IReadStream* in= new CReadFileStream(io::path(input),io::ENUM_ENDIAN_MODE_LITTLE);
	IWriteStream* out= new CWriteFileStream(io::path(output),false,io::ENUM_ENDIAN_MODE_LITTLE);
	XMLReader* reader=new CXMLReaderImpl<c8,core::IReferencable>(in);
	dae2y3d(reader,out);
	reader->drop();
	in->drop();
	out->drop();

	Logger->drop();
}