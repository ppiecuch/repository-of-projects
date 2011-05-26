/**
 * ������
 */
#include "cuboid.h"
const int Cuboid::bases[24]={
	-1, -1,  1,//ǰ����0
	 1, -1,  1,//ǰ����1
	 1,  1,  1,//ǰ����2
	-1,  1,  1,//ǰ����3
	-1, -1, -1,//������4
	-1,  1, -1,//������5
	 1,  1, -1,//������6
	 1, -1, -1 //������7
};
const short Cuboid::normals[][3]={
		{ 0, 0, 1},//0
		{ 0, 0,-1},//1
		{ 0, 1, 0},//2
		{ 0,-1, 0},//3
		{ 1, 0, 0},//4
		{-1, 0, 0} //5
};
const unsigned char Cuboid::vertexIndices[][4]={
		{0,1,3,2},//Front
		{7,4,6,5},//Back
		{3,2,5,6},//Top
		{1,0,7,4},//Bottom
		{2,1,6,7},//Right
		{0,3,4,5} //Left
};
Cuboid::Cuboid(float length,float width,float height)
{
	this->length=length;
	this->width=width;
	this->height=height;
	float halfLength=length/2;
	float halfWidth=width/2;
	float halfHeight=height/2;
	this->vertexs=new Point3f[8];


	for(int i=0;i<8;i++)
	{
		this->vertexs[i].x=bases[i*3]*halfLength;
		this->vertexs[i].y=bases[i*3+1]*halfWidth;
		this->vertexs[i].z=bases[i*3+2]*halfHeight;
	}
}
Point3f* Cuboid::getVertexs() const
{
	return this->vertexs;
}
float Cuboid::getLength() const
{
	return this->length;
}
float Cuboid::getWidth() const
{
	return this->width;
}
float Cuboid::getHeight() const
{
	return this->height;
}
