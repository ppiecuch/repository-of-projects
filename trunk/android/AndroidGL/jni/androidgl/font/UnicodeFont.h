
/**
 * UNICODE ����
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
 * UNICODE���ַ�
 * 1��֧��256���ַ��������壨�����ַ����ܲ�֧�֣�����ο�cpp�ļ���
 * 2��֧���ַ�������
 * 3��֧����ɫ���ּ�ࡢ�о�����
 * 4��֧�ֻ���
 */
class UnicodeFont{
private:
	GLubyte** buffers;		//�ҶȻ���������glyph-bitmap->buffer���ݣ����ڴ���������ÿ���������������������Դ�deltas�л��
	GLuint textures[256];
	GLfloat lineSpacing;	//�оࣨĬ��ֵΪface��ascender��
	unsigned char spaceWidth;//�ո���
	GLfloat charSpacing;	//�ַ����(Ĭ��Ϊ0)
	Point2d deltas[256];	//�ַ��ĳ���
	Point2d offsets[256];	//�ַ���ƫ�ƣ���Ϊÿ���ַ��Ŀ�߲�ͬ��
	unsigned char color[4];//��ɫ��RGBA��ʽ(Ĭ��Ϊ��ɫ)
	Point2f** texCoords;	//�ַ�����������,ע����Щ�ַ��ĳ�����������������һ���ַ�����Ϊ4���ڶ��ַ��ĳ���Ϊ��������2���������Ϊ8��
	static const unsigned char vertexIndices[];
	/**
	 * ��������
	 */
	void generateTexture(const int& ch,const unsigned char (&color)[4]);
	/**
	 * �����ַ��Ļ�������
	 */
	void generateChar(Point2f* &vertex,const int& index,const float& x,const float& y);
	/**
	 * ͳ���ַ������ַ����������ڴ����ַ���������
	 */
	int size(const char* (&str));
public:
	UnicodeFont(FT_Byte* &dataBase,const unsigned long& dataSize,const FT_F26Dot6& charWidth,const FT_F26Dot6& charHeight);

	/**
	 * ����Ļ�ϻ�������,xy��ʾ��ĸ���ߵĿ�ʼ���꣬���ַ����������Կո��
	 */
	void draw(const char* (&str),float x,float y);
	/**
	 * �����ַ����
	 */
	void setCharSpacing(const float& distance);
	/**
	 * �����о�
	 */
	void setLineSpacing(const float& distance);
	/**
	 * ������ɫ��16λRGBA��ʽ
	 */
	void setColor(const unsigned char (&color)[4]);
};
#endif
