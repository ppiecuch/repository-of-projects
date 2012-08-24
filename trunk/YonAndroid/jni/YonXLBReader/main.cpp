#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yon.h"
using namespace yon;
using namespace yon::core;
using namespace yon::event;
using namespace yon::debug;
using namespace yon::io;
using namespace yon::video;
using namespace yon::scene;
using namespace yon::scene::camera;
using namespace yon::scene::terrain;
using namespace yon::scene::water;
using namespace yon::scene::animator;
using namespace yon::audio;

#ifdef YON_COMPILE_WITH_WIN32
#pragma comment(lib, "Yon.lib")
#endif
int hex2int(const char* str)
{
	return (int)strtol( str, NULL, 16);
}
int main(int argc, char* argv[])
{
#if 0
	//V1.0
	IFileSystem* fs=createFileSystem();
	fs->addWorkingDirectory("d:/");
	IReadStream* stream=fs->createAndOpenReadFileStream("Item.xlb",io::ENUM_ENDIAN_MODE_BIG);
	core::stringc header=stream->readString();
	s32 versionCode=stream->readInt();
	core::stringc versionName=stream->readString();
	Logger->debug("header:%s,versionCode:%d,versionName:%s\r\n",header.c_str(),versionCode,versionName.c_str());

	s32 rowCount=stream->readInt();
	s32 colCount=stream->readInt();

	Logger->debug("rowCount:%d,colCount:%d\r\n",rowCount,colCount);

	for(s32 r=0;r<rowCount;++r)
	{
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%s\r\n",stream->readString());
		Logger->debug("%s\r\n",stream->readString());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());

		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());

		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());

		Logger->debug("%s\r\n",stream->readString());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());

		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",hex2int(stream->readString().c_str()));
	}
	stream->drop();

	fs->drop();
	return 0;
#else
	IFileSystem* fs=createFileSystem();
	fs->addWorkingDirectory("d:/xlb/");
	IReadStream* stream=fs->createAndOpenReadFileStream("consume.xlb",io::ENUM_ENDIAN_MODE_BIG);
	core::stringc header=stream->readString();
	s32 versionCode=stream->readInt();
	core::stringc versionName=stream->readString();
	Logger->debug("header:%s,versionCode:%d,versionName:%s\r\n",header.c_str(),versionCode,versionName.c_str());

	s32 rowCount=stream->readInt();

	Logger->debug("rowCount:%d\r\n",rowCount);

	
	//consume
	for(s32 r=0;r<rowCount;++r)
	{
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%s\r\n",stream->readString());
		Logger->debug("%s\r\n",stream->readString());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readInt());

		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readShort());

		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());

		Logger->debug("%s\r\n",stream->readString());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readBool());

		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readInt());
	}
	
	/*for(s32 r=0;r<rowCount;++r)
	{
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%s\r\n",stream->readString());
		Logger->debug("%d\r\n",stream->readBool());
	}*/
	stream->drop();

	fs->drop();
	return 0;
#endif
}