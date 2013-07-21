#include "AnimationLoader.h"

const core::stringc ELEM_ANIMATIONS = "animations";
const core::stringc ELEM_ANIM = "anim";
const core::stringc ELEM_CELL = "cell";
const core::stringc ELEM_SPR = "spr";

const core::stringc ATTR_SPRITE_SHEET = "spriteSheet";
const core::stringc ATTR_NAME = "name";
const core::stringc ATTR_LOOPS = "loops";
const core::stringc ATTR_INDEX = "index";
const core::stringc ATTR_DELAY = "delay";
const core::stringc ATTR_X = "x";
const core::stringc ATTR_Y = "y";
const core::stringc ATTR_Z = "z";
const core::stringc ATTR_ANGLE = "angle";
const core::stringc ATTR_FLIP_H = "flipH";
const core::stringc ATTR_FLIP_V = "flipV";

io::IFileSystem* AnimationLoader::s_pFileSystem=NULL;


bool AnimationLoader::load(const io::path& p,AnimationGroup& group){
	YON_DEBUG_BREAK_IF(s_pFileSystem==NULL);

	io::IReadStream* rs=s_pFileSystem->createAndOpenReadFileStream(p);

	if(rs==NULL)
		return false;

	io::XMLReader* xml=s_pFileSystem->createXMLReader(rs);

	while(xml&&xml->read())
	{
		switch(xml->getNodeType())
		{
		case io::ENUM_XML_NODE_ELEMENT:
			{
				const c8* nodeName=xml->getNodeName();
				if(ELEM_ANIMATIONS==nodeName)
				{
					group.SpriteSheet=xml->getAttributeValue(ATTR_SPRITE_SHEET.c_str());
				}
				else if(ELEM_ANIM==nodeName)
				{
					readAnim(xml,group);
				}
				else if(ELEM_CELL==nodeName)
				{
					Animation& anim=group.Animiations.getLast();
					readCell(xml,anim);
				}
				else if(ELEM_SPR==nodeName)
				{
					Animation::Cell& cell=group.Animiations.getLast().Cells.getLast();
					readSpr(xml,cell);
				}
				else
				{
					YON_WARN(YON_LOG_WARN_FORMAT,"Unexpected content in AnimationLoader::load!");
					skipSection(xml);
				}
			}
			break;
		case io::ENUM_XML_NODE_ELEMENT_END:
			break;
		case io::ENUM_XML_NODE_COMMENT:
			break;
		default:
			skipSection(xml);
		}
	}

	xml->drop();

	rs->drop();

	return true;
}

void AnimationLoader::readAnim(io::XMLReader* xml,AnimationGroup& group)
{
	Animation anim;
	anim.Name=xml->getAttributeValue(ATTR_NAME.c_str());
	anim.Loop=xml->getAttributeValueAsInt(ATTR_LOOPS.c_str());

	group.Animiations.push_back(anim);
}


void AnimationLoader::readCell(io::XMLReader* xml,Animation& anim)
{
	Animation::Cell cell;
	cell.Index=xml->getAttributeValueAsInt(ATTR_INDEX.c_str());
	cell.Delay=xml->getAttributeValueAsInt(ATTR_DELAY.c_str());

	anim.Cells.push_back(cell);
}


void AnimationLoader::readSpr(io::XMLReader* xml,Animation::Cell& cell)
{
	Animation::SpriteExt ext;
	ext.Name=xml->getAttributeValue(ATTR_NAME.c_str());
	ext.X=xml->getAttributeValueAsInt(ATTR_X.c_str());
	ext.Y=xml->getAttributeValueAsInt(ATTR_Y.c_str());
	ext.ZOrder=xml->getAttributeValueAsInt(ATTR_Z.c_str());
	ext.Angle=xml->getAttributeValueAsInt(ATTR_ANGLE.c_str());
	ext.FlipH=xml->getAttributeValueAsInt(ATTR_FLIP_H.c_str())!=0;
	ext.FlipV=xml->getAttributeValueAsInt(ATTR_FLIP_V.c_str())!=0;

	cell.Sprites.push_back(ext);
}


void AnimationLoader::skipSection(io::XMLReader* xml)
{
	if(xml==NULL||xml->isEmptyElement())
		return;
	const char* nodeName=xml->getNodeName();

	// read until we've reached the last element in this section
	int tagCounter = 1;

	while(tagCounter && xml->read())
	{
		if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT &&!xml->isEmptyElement())
		{
			++tagCounter;
		}
		else
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
				--tagCounter;
	}
}