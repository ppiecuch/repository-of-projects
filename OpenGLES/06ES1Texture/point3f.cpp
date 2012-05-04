#include "point3f.h"
#include <stdio.h>
//#include <iostream>
//using namespace std;
Point3f::Point3f():x(0),y(0),z(0)
{

}
Point3f::Point3f(GLfloat x,GLfloat y,GLfloat z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}
void Point3f::toString() const
{
	printf("{%.2f,%.2f,%.2f}",this->x,this->y,this->z);
	//LOGI("{%.2f,%.2f,%.2f}",this->x,this->y,this->z);
}
/*ostream & operator <<(ostream &stream,const Point3f &p){
	stream<<"{"<<p.x<<","<<p.y<<","<<p.z<<"}";
	return stream;
}*/
