#ifndef _ANIMATIONLOADER_H_
#define _ANIMATIONLOADER_H_

#include "IFileSystem.h"
#include "Sprite.h"

class  AnimationLoader
{
private:
	static io::IFileSystem* s_pFileSystem;

	static void readAnim(io::XMLReader* xml,AnimationGroup& group);
	static void readCell(io::XMLReader* xml,Animation& anim);
	static void readSpr(io::XMLReader* xml,Animation::Cell& cell);
	static void skipSection(io::XMLReader* xml);
public:
	static void setFileSystem(io::IFileSystem* fs){
		s_pFileSystem=fs;
	}
	static bool load(const io::path& p,AnimationGroup& group);
};
#endif