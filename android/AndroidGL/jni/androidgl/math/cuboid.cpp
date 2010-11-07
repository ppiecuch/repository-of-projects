/**
 * 长方体
 */
#include "cuboid.h"
#include "log.h"
const int Cuboid::bases[72]={
	/*前*/
	-1, -1,  1,//前左下0
	 1, -1,  1,//前右下1
	-1,  1,  1,//前左上2
	 1,  1,  1,//前右上3
	 /*后*/
	 1, -1, -1,//后右下4
	-1, -1, -1,//后左下5
	 1,  1, -1,//后右上6
	-1,  1, -1,//后左上7
	 /*上*/
	-1,  1,  1,//前左上8
	 1,  1,  1,//前右上9
	-1,  1, -1,//后左上10
	 1,  1, -1,//后右上11
	 /*下*/
	-1, -1, -1,//后左下12
	 1, -1, -1,//后右下13
	-1, -1,  1,//前左下14
	 1, -1,  1,//前右下15
	 /*左*/
	-1, -1, -1,//后左下16
	-1, -1,  1,//前左下17
	-1,  1, -1,//后左上18
	-1,  1,  1,//前左上19
	 /*右*/
	 1, -1,  1,//前右下20
	 1, -1, -1,//后右下21
	 1,  1,  1,//前右上22
	 1,  1, -1 //后右上23
};
const short Cuboid::texCoords[][2]={
		{0,0},//0
		{1,0},//1
		{0,1},//2
		{1,1},//3

		{0,0},//0
		{1,0},//1
		{0,1},//2
		{1,1},//3

		{0,0},//0
		{1,0},//1
		{0,1},//2
		{1,1},//3

		{0,0},//0
		{1,0},//1
		{0,1},//2
		{1,1},//3

		{0,0},//0
		{1,0},//1
		{0,1},//2
		{1,1},//3

		{0,0},//0
		{1,0},//1
		{0,1},//2
		{1,1},//3
};
const short Cuboid::normals[][3]={
		{ 0, 0, 1},//0
		{ 0, 0, 1},//0
		{ 0, 0, 1},//0
		{ 0, 0, 1},//0

		{ 0, 0,-1},//1
		{ 0, 0,-1},//1
		{ 0, 0,-1},//1
		{ 0, 0,-1},//1

		{ 0, 1, 0},//2
		{ 0, 1, 0},//2
		{ 0, 1, 0},//2
		{ 0, 1, 0},//2

		{ 0,-1, 0},//3
		{ 0,-1, 0},//3
		{ 0,-1, 0},//3
		{ 0,-1, 0},//3

		{-1, 0, 0},//4
		{-1, 0, 0},//4
		{-1, 0, 0},//4
		{-1, 0, 0},//4

		{ 1, 0, 0},//5
		{ 1, 0, 0},//5
		{ 1, 0, 0},//5
		{ 1, 0, 0}//5
};
const unsigned char Cuboid::vertexIndices[][4]={
		{0,1,2,3},//Front
		{4,5,6,7},//Back
		{8,9,10,11},//Top
		{12,13,14,15},//Bottom
		{16,17,18,19},//Right
		{20,21,22,23} //Left
};
Cuboid::Cuboid(GLfloat length,GLfloat width,GLfloat height)
{
	this->length=length;
	this->width=width;
	this->height=height;
	float halfLength=length/2;
	float halfWidth=width/2;
	float halfHeight=height/2;
	this->vertexs=new Point3f[24];


	for(int i=0;i<24;i++)
	{
		this->vertexs[i].x=(GLfloat)bases[i*3]*halfLength;
		this->vertexs[i].y=(GLfloat)bases[i*3+1]*halfWidth;
		this->vertexs[i].z=(GLfloat)bases[i*3+2]*halfHeight;
		//this->vertexs[i].toString();
	}
}
/*
const int Cuboid::bases[36]={
	-1, -1,  1,//前左下0
	 1, -1,  1,//前右下1
	 1,  1,  1,//前右上2
	-1,  1,  1,//前左上3
	-1, -1, -1,//后左下4
	-1,  1, -1,//后左上5
	 1,  1, -1,//后右上6
	 1, -1, -1,//后右下7
	-1, -1, -1,//后左下8=4
	-1,  1, -1,//后左上9=5
	 1,  1, -1,//后右上10=6
	 1, -1, -1 //后右下11=7
};
const short Cuboid::texCoords[][2]={
		{0,0},//0
		{1,0},//1
		{1,1},//2
		{0,1},//3
		{0,1},//4
		{0,0},//5
		{1,0},//6
		{1,1},//7
		{1,0},//8=1
		{1,1},//9=2
		{0,1},//10=3
		{0,0} //11=0
};
const short Cuboid::normals[][3]={
		{ 0, 0, 1},//0
		{ 0, 0,-1},//1
		{ 0, 1, 0},//2
		{ 0,-1, 0},//3
		{ 1, 0, 0},//4
		{-1, 0, 0},//5
};
const unsigned char Cuboid::vertexIndices[][4]={
		{0,1,3,2},//Front
		{7,4,6,5},//Back
		{3,2,5,6},//Top
		{1,0,7,4},//Bottom
		{2,1,10,11},//Right
		{0,3,8,9} //Left
};
Cuboid::Cuboid(GLfloat length,GLfloat width,GLfloat height)
{
	this->length=length;
	this->width=width;
	this->height=height;
	float halfLength=length/2;
	float halfWidth=width/2;
	float halfHeight=height/2;
	this->vertexs=new Point3f[12];


	for(int i=0;i<12;i++)
	{
		this->vertexs[i].x=(GLfloat)bases[i*3]*halfLength;
		this->vertexs[i].y=(GLfloat)bases[i*3+1]*halfWidth;
		this->vertexs[i].z=(GLfloat)bases[i*3+2]*halfHeight;
		this->vertexs[i].toString();
	}
}
*/
Point3f* Cuboid::getVertexs() const
{
	return this->vertexs;
}
void Cuboid::toString() const
{
	LOGI("Cuboid","{length:%.2f,width:%.2f,height:%.2f}",this->length,this->width,this->height);
}
