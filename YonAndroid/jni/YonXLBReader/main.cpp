#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yon.h"
using namespace yon;
using namespace yon::core;
using namespace yon::event;
using namespace yon::debug;
using namespace yon::io;
using namespace yon::i18n;
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
char* createGB(const char* utf8)
{
	int buffLen = 0;
	WCHAR wbuff[5120];
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wbuff, 5120);
	buffLen = WideCharToMultiByte(CP_ACP, 0, wbuff, -1, NULL, 0, 0, 0);
	char* m_gb2312 = new char[buffLen+1];
	WideCharToMultiByte(CP_ACP, 0, wbuff, -1, (LPSTR)m_gb2312, buffLen, 0, 0);
	return m_gb2312;
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
#elif 0
	//V2.0
	IFileSystem* fs=createFileSystem();
	fs->addWorkingDirectory("D:/Development/Tools/xls2xlbV2.0/output");
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
		core::stringc str=stream->readString();
		//char* temp=createGB(str.c_str());
		Logger->debug("%s\r\n",UTF8ToGB18030(str.c_str()).c_str());
		//delete[] temp;
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
#elif 0
	//V2.1
	IFileSystem* fs=createFileSystem();
	II18NManager* i18nMgr=createI18NManager();
	fs->addWorkingDirectory("D:/xlb");
	IReadStream* stream=fs->createAndOpenReadFileStream("composite.xlb",io::ENUM_ENDIAN_MODE_BIG);
	core::stringc header=stream->readString();
	s32 versionCode=stream->readInt();
	core::stringc versionName=stream->readString();
	Logger->debug("header:%s,versionCode:%d,versionName:%s\r\n",header.c_str(),versionCode,versionName.c_str());

	s32 rowCount=stream->readInt();

	Logger->debug("rowCount:%d\r\n",rowCount);


	for(s32 r=0;r<rowCount;++r)
	{
		Logger->debug("%s\r\n",i18nMgr->convert(stream->readString().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
		Logger->debug("%s\r\n",i18nMgr->convert(stream->readString().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readShort());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readShort());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readInt());
	}


	stream->drop();
	i18nMgr->drop();
	fs->drop();
#else
	//V2.1
	IFileSystem* fs=createFileSystem();
	II18NManager* i18nMgr=createI18NManager();
	fs->addWorkingDirectory("D:/Development/Tools/[Java]XLS转换工具/excel/xml");
	fs->addWorkingDirectory("D:/Development/Tools/[Java]XLS转换工具/excel");
	fs->addWorkingDirectory("D:/Development/Tools/xls2xlbV2.1/output");
	IReadStream* stream=fs->createAndOpenReadFileStream("skill(1).xc",io::ENUM_ENDIAN_MODE_BIG);

	/*core::stringc header=stream->readString();
	s32 versionCode=stream->readInt();
	core::stringc versionName=stream->readString();
	Logger->debug("header:%s,versionCode:%d,versionName:%s\r\n",header.c_str(),versionCode,versionName.c_str());

	s32 rowCount=stream->readInt();*/
	s32 rowCount=stream->readShort();

	Logger->debug("rowCount:%d\r\n",rowCount);


	for(s32 r=0;r<rowCount;++r)
	{
		//Logger->debug("%d\r\n",stream->readInt());
		Logger->debug("%d\r\n",stream->readShort());
		Logger->debug("%s\r\n",i18nMgr->convert(stream->readString().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
		Logger->debug("%s\r\n",i18nMgr->convert(stream->readString().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
		//Logger->debug("%s\r\n",stream->readString().c_str());
		//Logger->debug("%s\r\n",stream->readString().c_str());
		Logger->debug("%d\r\n",stream->readShort());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readByte());

		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%d\r\n",stream->readBool());
		Logger->debug("%s\r\n",i18nMgr->convert(stream->readString().c_str(),ENUM_ENCODING_UTF8,ENUM_ENCODING_GB18030).c_str());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readShort());
		Logger->debug("%d\r\n",stream->readByte());
		Logger->debug("%d\r\n",stream->readBool());
	}


	stream->drop();
	i18nMgr->drop();
	fs->drop();
	return 0;
#endif
}