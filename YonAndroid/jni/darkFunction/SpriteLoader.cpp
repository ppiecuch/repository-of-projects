#include "SpriteLoader.h"
#include "ILogger.h"

const core::stringc ELEM_IMG = "img";
const core::stringc ELEM_DEFINITIONS = "definitions";
const core::stringc ELEM_DIR = "dir";
const core::stringc ELEM_SPR = "spr";

const core::stringc ATTR_NAME = "name";
const core::stringc ATTR_W = "w";
const core::stringc ATTR_H = "h";
const core::stringc ATTR_X = "x";
const core::stringc ATTR_Y = "y";


io::IFileSystem* SpriteLoader::s_pFileSystem=NULL;
video::IVideoDriver* SpriteLoader::s_pVideoDriver=NULL;

bool SpriteLoader::load(const io::path& p,Spritesheet& sheet){
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
				if(ELEM_IMG==nodeName)
				{
					readImg(xml,sheet);
				}
				else if(ELEM_DEFINITIONS==nodeName)
				{
					io::path dir;
					xml->read();
					readDir(xml,dir,sheet);
				}
				else
				{
					YON_WARN(YON_LOG_WARN_FORMAT,"Unexpected content!");
					skipSection(xml);
				}
			}
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

void SpriteLoader::readImg(io::XMLReader* xml,Spritesheet& sheet){
	core::stringc name=xml->getAttributeValue(ATTR_NAME.c_str());
	core::stringc ws=xml->getAttributeValue(ATTR_W.c_str());
	core::stringc hs=xml->getAttributeValue(ATTR_H.c_str());

	u32 w=(u32)ws.superatof();
	u32 h=(u32)hs.superatof();

	YON_DEBUG_BREAK_IF(s_pVideoDriver==NULL);

	sheet.Texture=s_pVideoDriver->getTexture(name);

	YON_DEBUG_BREAK_IF(sheet.Texture->getSize()!=core::dimension2du(w,h));
}

void SpriteLoader::readDir(io::XMLReader* xml,io::path dir,Spritesheet& sheet){
	core::stringc name=xml->getAttributeValue(ATTR_NAME.c_str());
	io::path real=dir+name;
	while(xml&&xml->read())
	{
		switch(xml->getNodeType())
		{
		case io::ENUM_XML_NODE_ELEMENT:
			{
				const c8* nodeName=xml->getNodeName();
				if(ELEM_DIR==nodeName)
				{
					readDir(xml,real,sheet);
				}
				else if(ELEM_SPR==nodeName)
				{
					readSpr(xml,real,sheet);
				}
				else
				{
					YON_WARN(YON_LOG_WARN_FORMAT,"Unexpected content in dir!");
					skipSection(xml);
				}

			}
			break;
		case io::ENUM_XML_NODE_ELEMENT_END:
			return;
		default:
			skipSection(xml);
		}
	}
}
void SpriteLoader::readSpr(io::XMLReader* xml,io::path dir,Spritesheet& sheet){
	core::stringc name=xml->getAttributeValue(ATTR_NAME.c_str());
	core::stringc xs=xml->getAttributeValue(ATTR_X.c_str());
	core::stringc ys=xml->getAttributeValue(ATTR_Y.c_str());
	core::stringc ws=xml->getAttributeValue(ATTR_W.c_str());
	core::stringc hs=xml->getAttributeValue(ATTR_H.c_str());

	s32 x=(s32)xs.superatof();
	s32 y=(s32)ys.superatof();
	u32 w=(u32)ws.superatof();
	u32 h=(u32)hs.superatof();

	const core::dimension2du& size=sheet.Texture->getSize();

	Sprite spr;
	spr.Name=name;
	spr.Path=dir;
	if(dir.findLast('/')==dir.length()-1)
		spr.FullName=dir+name;
	else
		spr.FullName=dir+"/"+name;
	spr.Rect.set((f32)x/size.w,(f32)y/size.h,(f32)(x+w)/size.w,(f32)(y+h)/size.h);
	spr.Size.set(w,h);
	sheet.Sprites.push_back(spr);
}

void SpriteLoader::skipSection(io::XMLReader* xml)
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