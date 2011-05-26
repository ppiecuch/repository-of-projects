#ifndef _ASCII_BRUSH_H_
#define _ASCII_BRUSH_H_

/*#include <gl\gl.h>
#include <gl\glu.h>
#include "freetype.h"
#include <stdio.h>*/
#include <GLES/gl.h>
#include <freetype/ft2build.h>
#include <freetype/freetype.h>


#include "point2.h"
#include "hashmap.h"

class HashMapFontKey:HashMapKeyInterface{
protected:
	int color;
	int size;
	unsigned char c;
public:
	HashMapFontKey(){}
	int hashCode()
	{
		return this->c;
	}
	bool equal(const HashMapKeyInterface* const &i)
	{
		try
		{
			HashMapFontKey* p = dynamic_cast<HashMapFontKey*>(const_cast<HashMapKeyInterface*>(i));
			return this->color==p->hashCode()&&this->size==p->size&&this->c==p->c;
		}
		catch(...)
		{
			return false;
		}
		
	}
	friend class AsciiBrush;
};

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
		glGenTextures(1, &this->texture);
	}
	~AsciiFont(){
		if(this->texture!=-1)
			glDeleteTextures(1,&this->texture);
	}
public:
	friend class AsciiBrush;

};

class AsciiBrush{
protected:
	static const unsigned char vertexIndices[];
	FT_Library library;
	FT_Face face;
	HashMapFontKey tempKey;
	Point2<int> vertex[4];
	HashMap<HashMapFontKey,AsciiFont> map;
	AsciiFont& generateFont(const HashMapFontKey& key);
	void drawChar(const AsciiFont &font,int x,int y);
public:
	AsciiBrush(const char* const &fontPath);
	AsciiBrush(FT_Byte* &dataBase,unsigned long dataSize);
	~AsciiBrush();
	//����Ļ�ϻ�������,color��ʾ�ַ���ɫ,R8G8B8A8,size��ʾ�����С,xy��ʾ��ĸ���ߵĿ�ʼ���꣬���ַ����������Կո�ƣ�������
	void drawInLine(const unsigned char* const& str,int color,int size,int x,int y);

};

#endif
