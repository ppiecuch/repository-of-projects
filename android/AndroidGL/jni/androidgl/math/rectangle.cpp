/**
 * 长方形
 */
#include "rectangle.h"
#include "log.h"
const int Rectangle::bases[8]={
	-1, -1, //左下0
	 1, -1, //右下1
	-1,  1, //左上2
	 1,  1  //右上3
};
const short Rectangle::texCoords[][2]={
		{0,0},//0
		{1,0},//1
		{0,1},//2
		{1,1},//3
};
const short Rectangle::normals[][3]={
		{ 0, 0, 1},
		{ 0, 0, 1},
		{ 0, 0, 1},
		{ 0, 0, 1}
};
const unsigned char Rectangle::vertexIndices[][4]={
		{0,1,2,3}
};
Rectangle::Rectangle(GLfloat length,GLfloat height)
{
	this->length=length;
	this->height=height;
	float halfLength=length/2;
	float halfHeight=height/2;
	this->vertexs=new Point2f[4];


	for(int i=0;i<4;i++)
	{
		this->vertexs[i].x=(GLfloat)bases[i*2]*halfLength;
		this->vertexs[i].y=(GLfloat)bases[i*2+1]*halfHeight;
		//this->vertexs[i].toString();
	}
}
Point2f* Rectangle::getVertexs() const
{
	return this->vertexs;
}
void Rectangle::toString() const
{
	LOGI("Rectangle","{length:%.2f,height:%.2f}",this->length,this->height);
}
