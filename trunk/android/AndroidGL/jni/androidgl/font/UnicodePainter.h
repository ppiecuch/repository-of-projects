#ifndef _UNICODEPAINTER_H_
#define _UNICODEPAINTER_H_
#include <stdio.h>
#include <GLES/gl.h>
#include <ext/hash_map>
#include <iostream>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftmodapi.h>
#include "../log.h"
#include "../util.h"
#include "Font.h"

using namespace std;
using namespace __gnu_cxx;

#define HASH hash_map<unsigned short,Font>
#define CODE_SPACE 0x0020
#define CODE_NEW_LINE 0x000A

/**
 * Unicode码绘制器
 * 支持：
 * 1、设置字体颜色（暂不支持同编码多颜色，并且字符创建后颜色再改变无效）
 * 2、设置行高，字距
 * 待优化：
 * 1、字体模糊问题
 */
class UnicodePainter{
private:
	FT_Byte* dataBase;		 //字体文件数据
	//unsigned long dataSize;//字体文件数据长度
	FT_Library library;
	FT_Face face;
	HASH fonts;//字符对象表
	GLfloat lineSpacing;	//行距（默认值为face的ascender）
	unsigned char spaceWidth;//空格宽度
	GLfloat charSpacing;	//字符间距(默认为0)
	unsigned int size;		//字符大小
	long color;	//颜色，RGBA格式(默认为黑色)

	UnicodePainter &operator =(const UnicodePainter &other);    //赋值函数
	UnicodePainter(const UnicodePainter &other);    // 拷贝构造函数
public:
	UnicodePainter(FT_Byte* &dataBase,const unsigned long dataSize,const unsigned int size);
	~UnicodePainter();
	/**
	 * 在屏幕上绘制文字,xy表示字母基线的开始坐标，若字符纹理不存在以空格计
	 * @param str为unicode编码的字符串
	 */
	void draw(unsigned short* (&str),int len,float x,float y);
	/**
	 * 设置字符间距
	 */
	void setCharSpacing(float distance);
	/**
	 * 设置行距
	 */
	void setLineSpacing(float distance);
	/**
	 * 设置颜色，16位RGBA格式
	 */
	void setColor(long color);
	/**
	 * 设置字符大小
	 */
	void setSize(int size);
};
#endif
