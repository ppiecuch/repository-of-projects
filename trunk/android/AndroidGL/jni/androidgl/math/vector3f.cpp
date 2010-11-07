#include "vector3f.h"
#include "../log.h"
//#include <iostream>
//using namespace std;
Vector3f::Vector3f():x(0),y(0),z(0)
{

}
Vector3f::Vector3f(GLfloat x,GLfloat y,GLfloat z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}
/**
 * 长度平方
 */
GLfloat Vector3f::lengthSquared() const
{
	return this->x*this->x+this->y*this->y+this->z*this->z;
}
/**
 * 长度
 */
GLfloat Vector3f::length() const
{
	return sqrt(this->lengthSquared());
}
/**
 * 规范化
 */
void Vector3f::normalize()
{
	GLfloat length=this->length();
	this->x/=length;
	this->y/=length;
	this->z/=length;
}
void Vector3f::toString() const
{
	LOGI("Vector3f","{%.2f,%.2f,%.2f}",this->x,this->y,this->z);
	//LOGI("{%.2f,%.2f,%.2f}",this->x,this->y,this->z);
}
/*ostream & operator <<(ostream &stream,const Point3f &p){
	stream<<"{"<<p.x<<","<<p.y<<","<<p.z<<"}";
	return stream;
}*/
