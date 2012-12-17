#include "stdio.h"
#include <stdlib.h>
#include <iostream>
#include "CXMLReaderImpl.h"
#include "CReadFileStream.h"
#include "CWriteFileStream.h"
#include "CLogger.h"
#include "dae2y3d.h"
#include "Importer.h"
using namespace std;

using namespace yon;
using namespace yon::io;



namespace yon{
	namespace debug{
		ILogger* Logger=NULL;
	}
}


io::path getAbsolutePath(const io::path& filename){
	if(filename.findFirst(':')!=-1)
		return filename;
	fschar *p=0;
	fschar fpath[_MAX_PATH];
	p = _fullpath(fpath, filename.c_str(), _MAX_PATH);
	core::stringc tmp(p);
	tmp.replace('\\', '/');
	return tmp;
}

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits>

#include "postprocess.h"
#include "version.h"
#include "scene.h"
#include "Importer.hpp"
#include "DefaultLogger.hpp"
#include "AssimpPCH.h"

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	if(argc<3)
	{
		cout<<"No param!Please execute command as:"<<endl;
		cout<<"dae2y3d [inputfile] [outputfile] [loglevel]"<<endl;
		system("pause");
		return 0;
	}

	io::path input=getAbsolutePath(io::path(argv[1]));
	io::path output=getAbsolutePath(io::path(argv[2]));
	FILE* file;
	errno_t ir=fopen_s(&file,input.c_str(),"rb+");
	if(ir!=0)
	{
		cout<<"No input file,error:"<<ir<<"!"<<endl;
		system("pause");
		return 0;
	}
	fclose(file);
	/*errno_t or=fopen_s(&file,output.c_str(),"w");
	if(or==0)
	{
		cout<<"Already has output file,error:"<<or<<"!"<<endl;
		fclose(file);
		system("pause");
		return 0;
	}
	if(or)
	{
		cout<<"open output file,error:"<<or<<"!"<<endl;
		system("pause");
		return 0;
	}*/

	Logger=new debug::CLogger();
	Logger->setFormat(debug::MASK_FORMAT_LOG);
	/*IReadStream* in= new CReadFileStream(io::path(input),io::ENUM_ENDIAN_MODE_LITTLE);
	IWriteStream* out= new CWriteFileStream(io::path(output),false,io::ENUM_ENDIAN_MODE_LITTLE);
	XMLReader* reader=new CXMLReaderImpl<c8,core::IReferencable>(in);
	dae2y3d(reader,out);
	reader->drop();
	in->drop();
	out->drop();*/

	Importer imp;
	imp.ReadFile("../media/box_translate.dae",aiProcess_ValidateDataStructure);
	const aiScene* scene=imp.GetScene();

	Logger->drop();

	return 0;
}