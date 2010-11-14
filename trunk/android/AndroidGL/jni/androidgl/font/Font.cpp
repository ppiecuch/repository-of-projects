#include "Font.h"

const unsigned char Font::vertexIndices[]={0,1,2,3};

Font::Font(GLubyte* (&buffer),const int size,const long color,const int width,const int height,const int left,const int top)
{
	this->buffer=buffer;
	this->size=size;
	this->color=color;
	this->base.x=width;
	this->base.y=height;
	this->offset.x=left;
	this->offset.y=top;
//	for(int i=0;i<4;i++)
//	{
//		texCoords[i]=new Point2f(0,0);
//		vertexs[i]=new Point2f(0,0);
//	}
	generateTexture();
}

//inline bool Font::operator=(const Font &font)
//{
//	this->size=font.size;
//	this->color=font.color;
//	this->base.x=font.base.x;
//	this->base.y=font.base.y;
//	this->offset.x=font.offset.x;
//	this->offset.y=font.offset.y;
//	GLubyte* buffer = new GLubyte[base.x * base.y];
//	memcpy(buffer,font.buffer,base.x * base.y);
//	this->buffer=buffer;
//	this->generateTexture();
//}


/**
 * 相等操作符重载
 */
//inline bool operator==(Font &leftFont,Font &rightFont){
//	if(leftFont.size==NULL&&rightFont.size==NULL)
//		return true;
//	if(leftFont.size==NULL||rightFont.size==NULL)
//		return false;
//	//TODO
//	return true;
//}

///**
// * 不等操作符重载
// */
//inline bool operator!=(const Font &leftFont,const Font &rightFont) const{
//	return !(leftFont==rightFont);
//}

void Font::generateTexture()
{
	if(this->buffer==NULL)
	{
		LOGW("Font","generateTexture() fail,for buffer==NULL",NULL);
		return;
	}
	//获取颜色分量
//	int red=0xFF&color[0];
//	int green=0xFF&color[1];
//	int blue=0xFF&color[2];
//	int alpha=0xFF&color[3];
//	unsigned char red=(unsigned char)((t&RED_IN_LONG)>>24);
//	unsigned char green=(unsigned char)((t&GREEN_IN_LONG)>>16);
//	unsigned char blue=(unsigned char)((t&BLUE_IN_LONG)>>8);
//	unsigned char alpha=(unsigned char)(t&ALPHA_IN_LONG);

	int width=this->base.x;
	int height=this->base.y;
	int newWidth=ceilPower(width);
	int newHeight=ceilPower(height);
	GLubyte* data=new GLubyte[newWidth*newHeight];
	int bufferIndex=0;
	int value;
	int row;
	int column;
	for(int i=0; i<newHeight;i++) {
		row=(newHeight-i-1)*newWidth;
		for(int j=0; j<newWidth; j++){
			column=row+j;
			//因为width跟height是经过对bitmap.width,bitmap.rows进行ceilPower扩大后得到的，
			//所以对于扩出的部分应该用0表示
			value=(i>=height||j>=width)?0:buffer[bufferIndex++];
			data[column]=value;
//			LOGI("UnicodePainter","%d-%d:%d",i,j,value);
		}
	}

	//申请1个纹理的名称
	glGenTextures(1, &texture);
	//设置字体纹理的纹理过滤器
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D( GL_TEXTURE_2D,0,GL_LUMINANCE,newWidth,newHeight,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,data);

	delete[] data;
}
void Font::draw(float x,float y)
{
	/*
	 * -----------------------------<---基线与此线距离为top，即offsets[index].y
	 * -----------------------------
	 * 	a...x...z
	 * -----------------------------<---基线
	 * -----------------------------
	 *
	 *	|  |
	 *	| a|
	 *	|  |
	 *	+
	 *	|
	 *	|
	 *	left 为字符与此线的距离，字符在右为负数，在左为正数
	 */

	vertexs[0].x=x+this->offset.x;
	vertexs[0].y=y+this->offset.y-this->base.y;

	vertexs[1].x=x+this->offset.x+this->base.x;
	vertexs[1].y=y+this->offset.y-this->base.y;

	vertexs[2].x=x+this->offset.x;
	vertexs[2].y=y+this->offset.y;

	vertexs[3].x=x+this->offset.x+this->base.x;
	vertexs[3].y=y+this->offset.y;

	glVertexPointer(2,GL_FLOAT,0,this->vertexs);
	glTexCoordPointer(2, GL_FLOAT, 0, this->texCoords);
	glBindTexture(GL_TEXTURE_2D,this->texture);
	glDrawElements(GL_TRIANGLE_STRIP,4,GL_UNSIGNED_BYTE,vertexIndices);
}
