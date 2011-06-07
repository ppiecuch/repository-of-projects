#ifndef _ASCII_BRUSH_H_
#define _ASCII_BRUSH_H_

/*
#include <gl\gl.h>
#include <gl\glu.h>
#include "freetype.h"
#include <stdio.h>
#include <hash_map>
using namespace std;
using namespace stdext;*/


#include "point2.h"

/**/
#include <GLES/gl.h>
#include <freetype/ft2build.h>
#include <freetype/freetype.h>
#include <ext/hash_set>
#include <stdexcept>
using namespace  std;
using namespace  __gnu_cxx;//��Ҫ����


class AsciiBrush;
struct HashAsciiFont;

class AsciiFont{
private:
	Point2<int> delta;			//�ַ��ĳ���
	Point2<float> texCoord[4];	//�ַ�����������
	Point2<int> offset;			//�ַ���ƫ��
	Point2<int> advance;		//�ַ��Ĳ���
	int color;					//�ַ�����ɫ
	int size;					//�ַ��Ĵ�С
	unsigned char c;			//�ַ�����
	GLuint texture;				//����ID
	AsciiFont():texture(-1),color(0x000000FF){
		
	}
	~AsciiFont(){
		if(this->texture!=-1)
			glDeleteTextures(1,&this->texture);
	}
public:
	friend class AsciiBrush;
	friend struct HashAsciiFont;
	friend struct EqualAsciiFont;

};
struct HashAsciiFont{
	size_t operator()(const AsciiFont& f) const {
			return f.c;   
	}
};
struct EqualAsciiFont{
	bool operator()(const AsciiFont& f1,const AsciiFont& f2) const{
        return f1.color==f2.color&&f1.size==f2.size&&f1.c==f2.c;
    }
};




class AsciiBrush{
protected:
	static const unsigned char vertexIndices[];
	FT_Library library;
	FT_Face face;
	AsciiFont tempKey; 
	Point2<int> vertex[4];
	hash_set<AsciiFont,HashAsciiFont,EqualAsciiFont> set;
	AsciiFont& generateFont(const AsciiFont& key);
	void drawChar(const AsciiFont &font,int x,int y);
public:
	AsciiBrush(const char* const &fontPath);
	AsciiBrush(FT_Byte* &dataBase,unsigned long dataSize);
	~AsciiBrush();
	//����Ļ�ϻ�������,color��ʾ�ַ���ɫ,R8G8B8A8,size��ʾ�����С,xy��ʾ��ĸ���ߵĿ�ʼ���꣬���ַ����������Կո�ƣ�������
	void drawInLine(const unsigned char* const& str,int color,int size,int x,int y);

};

#endif
