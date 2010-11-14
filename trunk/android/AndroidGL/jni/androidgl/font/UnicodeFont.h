
/**
 * UNICODE 字体
 */
#ifndef _UNICODEFONT_H_
#define _UNICODEFONT_H_

#include <stdio.h>
#include <GLES/gl.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftmodapi.h>
#include "../log.h"
#include "../util.h"
#include "../math/point2d.h"
#include "../math/point2f.h"
#include "../encode/GBnUnicode.h"

/**
 * UNICODE码字符
 * 1、支持256个字符的纹理缓冲（部分字符可能不支持，具体参考cpp文件）
 * 2、支持字符串绘制
 * 3、支持颜色、字间距、行距设置
 * 4、支持换行
 */
class UnicodeFont{
private:
	GLubyte** buffers;		//灰度缓冲区（即glyph-bitmap->buffer数据，用于创建纹理），每个缓冲区的行数列数可以从deltas中获得
	GLuint textures[256];
	GLfloat lineSpacing;	//行距（默认值为face的ascender）
	unsigned char spaceWidth;//空格宽度
	GLfloat charSpacing;	//字符间距(默认为0)
	Point2d deltas[256];	//字符的长宽
	Point2d offsets[256];	//字符的偏移（因为每个字符的宽高不同）
	unsigned char color[4];//颜色，RGBA格式(默认为黑色)
	Point2f** texCoords;	//字符的纹理坐标,注意有些字符的长宽过大的情况，比如第一个字符长度为4，第二字符的长度为５（经过2的幂扩大后为8）
	static const unsigned char vertexIndices[];
	/**
	 * 生成纹理
	 */
	void generateTexture(const int& ch,const unsigned char (&color)[4]);
	/**
	 * 生成字符的绘制坐标
	 */
	void generateChar(Point2f* &vertex,const int& index,const float& x,const float& y);
	/**
	 * 统计字符串的字符个数，用于创建字符顶点数组
	 */
	int size(const char* (&str));
public:
	UnicodeFont(FT_Byte* &dataBase,const unsigned long& dataSize,const FT_F26Dot6& charWidth,const FT_F26Dot6& charHeight);

	/**
	 * 在屏幕上绘制文字,xy表示字母基线的开始坐标，若字符纹理不存在以空格计
	 */
	void draw(const char* (&str),float x,float y);
	/**
	 * 设置字符间距
	 */
	void setCharSpacing(const float& distance);
	/**
	 * 设置行距
	 */
	void setLineSpacing(const float& distance);
	/**
	 * 设置颜色，16位RGBA格式
	 */
	void setColor(const unsigned char (&color)[4]);
};
#endif
