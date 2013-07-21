#include "DarkManager.h"
#include "SpriteLoader.h"
#include "AnimationLoader.h"

DarkManager::~DarkManager(){
	SpritesheetMap::Iterator it=m_spritesheets.getIterator();
	for(;!it.atEnd();++it)
	{
		it->getValue()->drop();
	}
	m_spritesheets.clear();

	AnimationGroupMap::Iterator it2=m_animationGroups.getIterator();
	for(;!it2.atEnd();++it2)
	{
		it2->getValue()->drop();
	}
	m_animationGroups.clear();
}

Spritesheet* DarkManager::getSpritesheet(const io::path& p){
	SpritesheetMap::Node* node=m_spritesheets.find(p);
	if(node)
		return node->getValue();

	Spritesheet* sheet=new Spritesheet;
	SpriteLoader::load(p,*sheet);

	m_spritesheets.set(p,sheet);

	return sheet;
}

AnimationGroup* DarkManager::getAnimationGroup(const io::path& p){
	AnimationGroupMap::Node* node=m_animationGroups.find(p);
	if(node)
		return node->getValue();

	AnimationGroup* group=new AnimationGroup;
	AnimationLoader::load(p,*group);

	m_animationGroups.set(p,group);

	return group;
}