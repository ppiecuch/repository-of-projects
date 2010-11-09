/**
 * 长方形
 */
#include "rectangle.h"
#include "log.h"
const int Rectangle::bases[8]={
	-1, 1,//前左
	 1, 1,//前右
	-1,-1,//后左
	 1,-1 //后右
};
const short Rectangle::texCoords[][2]={
		{0,0},//0
		{1,0},//1
		{0,1},//2
		{1,1},//3
};
const short Rectangle::normals[][3]={
		{ 0, 1, 0},
		{ 0, 1, 0},
		{ 0, 1, 0},
		{ 0, 1, 0}
};
const unsigned char Rectangle::vertexIndices[][4]={
		{0,1,2,3}
};
Rectangle::Rectangle(GLfloat length,GLfloat width)
{
	this->length=length;
	this->width=width;
	float halfLength=length/2;
	float halfWidth=width/2;
	this->vertexs=new Point2f[4];


	for(int i=0;i<4;i++)
	{
		this->vertexs[i].x=(GLfloat)bases[i*2]*halfLength;
		this->vertexs[i].y=(GLfloat)bases[i*2+1]*halfWidth;
		//this->vertexs[i].toString();
	}
}
Point2f* Rectangle::getVertexs() const
{
	return this->vertexs;
}
void Rectangle::toString() const
{
	LOGI("Rectangle","{length:%.2f,width:%.2f}",this->length,this->width);
}
