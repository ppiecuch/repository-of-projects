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
 * Unicode�������
 * ֧�֣�
 * 1������������ɫ���ݲ�֧��ͬ�������ɫ�������ַ���������ɫ�ٸı���Ч��
 * 2�������иߣ��־�
 * ���Ż���
 * 1������ģ������
 */
class UnicodePainter{
private:
	FT_Byte* dataBase;		 //�����ļ�����
	//unsigned long dataSize;//�����ļ����ݳ���
	FT_Library library;
	FT_Face face;
	HASH fonts;//�ַ������
	GLfloat lineSpacing;	//�оࣨĬ��ֵΪface��ascender��
	unsigned char spaceWidth;//�ո���
	GLfloat charSpacing;	//�ַ����(Ĭ��Ϊ0)
	unsigned int size;		//�ַ���С
	long color;	//��ɫ��RGBA��ʽ(Ĭ��Ϊ��ɫ)

	UnicodePainter &operator =(const UnicodePainter &other);    //��ֵ����
	UnicodePainter(const UnicodePainter &other);    // �������캯��
public:
	UnicodePainter(FT_Byte* &dataBase,const unsigned long dataSize,const unsigned int size);
	~UnicodePainter();
	/**
	 * ����Ļ�ϻ�������,xy��ʾ��ĸ���ߵĿ�ʼ���꣬���ַ����������Կո��
	 * @param strΪunicode������ַ���
	 */
	void draw(unsigned short* (&str),int len,float x,float y);
	/**
	 * �����ַ����
	 */
	void setCharSpacing(float distance);
	/**
	 * �����о�
	 */
	void setLineSpacing(float distance);
	/**
	 * ������ɫ��16λRGBA��ʽ
	 */
	void setColor(long color);
	/**
	 * �����ַ���С
	 */
	void setSize(int size);
};
#endif
