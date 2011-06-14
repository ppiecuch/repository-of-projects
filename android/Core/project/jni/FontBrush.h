#ifndef _FONT_BRUSH_H_
#define _FONT_BRUSH_H_

/*
#include <gl\gl.h>
#include <gl\glu.h>
#include "freetype.h"
#include <stdio.h>
#include <hash_set>
using namespace std;
using namespace stdext;*/


#include "point2.h"
#include "common/mathUtil.h"
#include "common/log.h"

/**/
#include <GLES/gl.h>
#include <freetype/ftglyph.h>
#include <freetype/freetype.h>
#include <ext/hash_set>
#include <stdexcept>
using namespace  __gnu_cxx;//需要引入
using namespace  std;



class FontBrush;
struct HashFont;

class Font{
private:
	Point2<int> delta;			//字符的长宽
	Point2<float> texCoord[4];	//字符的纹理坐标
	Point2<int> offset;			//字符的偏移
	Point2<int> advance;		//字符的步进
	int color;					//字符的颜色
	int size;					//字符的大小
	unsigned short c;			//字符代码
	GLuint texture;				//纹理ID
	Font():texture(-1),color(0x000000FF){
		
	}
public:
	Font(const Font& font):texture(-1),color(0x000000FF){
		this->delta.x=font.delta.x;
		this->delta.y=font.delta.y;
		for(int i=0;i<4;i++)
		{
			this->texCoord[i].x=font.texCoord[i].x;
			this->texCoord[i].y=font.texCoord[i].y;
		}
		this->offset.x=font.offset.x;
		this->offset.y=font.offset.y;
		this->advance.x=font.advance.x;
		this->advance.y=font.advance.y;
		this->color=font.color;
		this->size=font.size;
		this->c=font.c;
		this->texture=font.texture;
	}
	~Font(){
		if(this->texture!=-1)
			glDeleteTextures(1,&this->texture);
	}
	friend class FontBrush;
	friend struct HashFont;
	friend struct EqualFont;

};
struct HashFont{
	size_t operator()(const Font* const&f) const {
			return f->c;   
	}
};
struct EqualFont{
	bool operator()(const Font* const &f1,const Font* const &f2) const{
        return f1->color==f2->color&&f1->size==f2->size&&f1->c==f2->c;
    }
};



typedef hash_set<Font*,HashFont,EqualFont> FontSet;
class FontBrush{
protected:
	static const unsigned char vertexIndices[];
	FT_Byte* dataBase;
	FT_Library library;
	FT_Face face;
	Font tempKey; 
	Point2<float> vertex[4];
	FontSet set;
	//生成一个指定的Font并将它保存进FontSet中
	void generateFont(const Font& key);
	void drawChar(const FontSet::iterator &iterator,int x,int y);
public:
	FontBrush(const char* const &fontPath);
	FontBrush(FT_Byte* &dataBase,unsigned long dataSize);
	~FontBrush();
	//在屏幕上绘制文字,color表示字符颜色,R8G8B8A8,size表示字体大小,xy表示字母基线的开始坐标，若字符纹理不存在以空格计，不换行
	void drawInLine(const char* const& str,int color,int size,int x,int y);
	void drawInLine(const unsigned short* const& str,int len,int color,int size,int x,int y);

};

#endif
