#include <stdlib.h>
#include <iostream>
using namespace std;
#include <crtdbg.h>
#include <typeinfo>

#include "decrypt.h"


inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) |  _CRTDBG_LEAK_CHECK_DF);
}

#include <locale.h>

#ifdef XC_HIDE_CONSOLE
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif//XC_HIDE_CONSOLE

#include "xccore.h"

#pragma comment(lib, "../../dependencies/xccore/lib/win32/xccore.lib")

using namespace xc;
using namespace xc::core;
using namespace xc::io;

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

int htoi(core::stringc& str)
{
	str.makeUpper();
	const c8* s=str.c_str();

	if( !s )return 0;
	if( *s == '0' )
	{
		s++;
		if(*s == 'X')s++;
	}
	int n = 0;
	while( *s )
	{
		n <<= 4;
		if( *s <= '9' )
			n |= ( *s & 0xf );
		else
			n |= ( (*s & 0xf) + 9 );
		s++;
	}
	return n;
}

u8 encrypt(u8 &key,u8 c,u32 & pos)
{
	if (pos&0x01)
	{
		//奇
		key ^= pos<<1 + 1;
	}else
	{
		//偶
		key ^= pos<<2 + 1;
	}
	return key^c;
}


/************************************************************************/
/* 将定点型的R8G8B8转为float型的R8G8B8                                  */
/************************************************************************/

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();

	setlocale(LC_CTYPE,"UTF-8");

	if(argc<4)
	{
		cout<<"No param!Please execute command as:"<<endl;
		cout<<"filecrypt [key] [inputfile] [outputfile]"<<endl;
		system("pause");
		return 0;
	}

	core::stringc keyStr;
	keyStr.build(argv[1],strlen(argv[1]));
	if(keyStr.find("0x")!=0)
	{
		cout<<"the key param should start with 0x:"<<endl;
		system("pause");
		return 0;
	}

	if(keyStr.length()!=4)
	{
		cout<<"the key param should be one byte number"<<endl;
		system("pause");
		return 0;
	}

	u8 key=(u8)htoi(keyStr);
	io::path input=getAbsolutePath(io::path(argv[2]));
	io::path output=getAbsolutePath(io::path(argv[3]));
	//core::stringc keyStr;
	//keyStr.build("0x7E",strlen("0x7E"));
	//u8 key=(u8)htoi(keyStr);
	//io::path input = "E:\\syjt\\trunk\\tools\\product\\filecrypt\\src\\const_define.lua";
	//io::path output = "E:\\syjt\\trunk\\tools\\product\\filecrypt\\dest\\const_define.lua";
	xc::xccoreInit();

	IFileSystem* fs=createFileSystem(".");

	IReadStream* rs=fs->createAndOpenReadFileStream(input);
	if(rs){
		IWriteStream* ws=fs->createAndOpenWriteFileStream(output);
		if(ws){
			//版本号
			ws->writeInt(VERSION1);
			//key
			ws->writeUnsignedByte(key);
			while(rs->getPos() < rs->getSize())
			{
				u8 c=rs->readUnsignedByte();
				u32 pos = ws->getPos();
				u8 r=encrypt(key,c,pos);
				ws->writeUnsignedByte(r);
			}

			ws->drop();
			rs->drop();
		}
	}

	fs->drop();

	xc::xccoreDestroy();
	return 1;
}