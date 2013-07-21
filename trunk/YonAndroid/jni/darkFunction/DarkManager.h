#ifndef _DARKMANAGER_H_
#define _DARKMANAGER_H_

#include "Sprite.h"

class  DarkManager
{
private:
	typedef core::map<io::path,Spritesheet*> SpritesheetMap;
	SpritesheetMap m_spritesheets;

	typedef core::map<io::path,AnimationGroup*> AnimationGroupMap;
	AnimationGroupMap m_animationGroups;

public:
	~DarkManager();
	Spritesheet* getSpritesheet(const io::path& p);

	AnimationGroup* getAnimationGroup(const io::path& p);
	
};
#endif