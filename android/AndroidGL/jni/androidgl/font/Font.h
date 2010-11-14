#ifndef _FONT_H_
#define _FONT_H_

#include <stdio.h>
#include <GLES/gl.h>
#include "../log.h"
#include "../util.h"
#include "../math/point2d.h"
#include "../math/point2f.h"

class Font{
private:
	GLubyte* buffer;		//灰度缓冲区（即glyph-bitmap->buffer数据，用于创建纹理），每个缓冲区的行数列数可以从deltas中获得
	/**
	 * 生成纹理单元
	 */
	void generateTexture();
	bool operator=(const Font &font);
public:
	GLuint texture;
	Point2d base;		//字符的长宽
	Point2d offset;		//字符的偏移（因为每个字符的宽高不同）
	int size;			//字符大小
	long color;			//颜色，RGBA格式(默认为黑色)
	Point2f vertexs[4];	//顶点坐标，以基线点为原点
	Point2f texCoords[4];	//字符的纹理坐标,注意有些字符的长宽过大的情况，比如第一个字符长度为4，第二字符的长度为５（经过2的幂扩大后为8）
	static const unsigned char vertexIndices[];
	Font(GLubyte* (&buffer),const int size,const long color,const int width,const int height,const int left,const int top);

	/**
	 * 在屏幕上绘制文字,xy表示字母基线的开始坐标，若字符纹理不存在以空格计
	 */
	void draw(float x,float y);

//	inline bool operator=(const Font &font);

};

/**
 * 相等操作符重载
 */
//inline bool operator==(Font &leftFont,Font &rightFont);

/**
 * 不等操作符重载
 */
//bool operator!=(Font &leftFont,const Font &rightFont);
#endif
