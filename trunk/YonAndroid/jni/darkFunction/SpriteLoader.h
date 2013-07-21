#ifndef _SPRITELOADER_H_
#define _SPRITELOADER_H_

#include "IFileSystem.h"
#include "IVideoDriver.h"
#include "Sprite.h"

class SpriteLoader{
private:
	static io::IFileSystem* s_pFileSystem;
	static video::IVideoDriver* s_pVideoDriver;

	static void readImg(io::XMLReader* xml,Spritesheet& sheet);
	static void readDir(io::XMLReader* xml,io::path dir,Spritesheet& sheet);
	static void readSpr(io::XMLReader* xml,io::path dir,Spritesheet& sheet);
	static void skipSection(io::XMLReader* xml);
public:
	static void setFileSystem(io::IFileSystem* fs){
		s_pFileSystem=fs;
	}
	static void setVideoDriver(video::IVideoDriver* driver){
		s_pVideoDriver=driver;
	}

	static bool load(const io::path& p,Spritesheet& sheet);
};
#endif