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
	GLubyte* buffer;		//�ҶȻ���������glyph-bitmap->buffer���ݣ����ڴ���������ÿ���������������������Դ�deltas�л��
	/**
	 * ��������Ԫ
	 */
	void generateTexture();
	bool operator=(const Font &font);
public:
	GLuint texture;
	Point2d base;		//�ַ��ĳ���
	Point2d offset;		//�ַ���ƫ�ƣ���Ϊÿ���ַ��Ŀ�߲�ͬ��
	int size;			//�ַ���С
	long color;			//��ɫ��RGBA��ʽ(Ĭ��Ϊ��ɫ)
	Point2f vertexs[4];	//�������꣬�Ի��ߵ�Ϊԭ��
	Point2f texCoords[4];	//�ַ�����������,ע����Щ�ַ��ĳ�����������������һ���ַ�����Ϊ4���ڶ��ַ��ĳ���Ϊ��������2���������Ϊ8��
	static const unsigned char vertexIndices[];
	Font(GLubyte* (&buffer),const int size,const long color,const int width,const int height,const int left,const int top);

	/**
	 * ����Ļ�ϻ�������,xy��ʾ��ĸ���ߵĿ�ʼ���꣬���ַ����������Կո��
	 */
	void draw(float x,float y);

//	inline bool operator=(const Font &font);

};

/**
 * ��Ȳ���������
 */
//inline bool operator==(Font &leftFont,Font &rightFont);

/**
 * ���Ȳ���������
 */
//bool operator!=(Font &leftFont,const Font &rightFont);
#endif
