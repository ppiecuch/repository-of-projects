#include "point2f.h"
#include <stdio.h>
//#include <iostream>
//using namespace std;
Point2f::Point2f():x(0),y(0)
{

}
Point2f::Point2f(GLfloat x,GLfloat y)
{
	this->x=x;
	this->y=y;
}
void Point2f::toString() const
{
	printf("{%.2f,%.2f}",this->x,this->y);
	//LOGI("{%.2f,%.2f,%.2f}",this->x,this->y,this->z);
}
/*ostream & operator <<(ostream &stream,const Point3f &p){
	stream<<"{"<<p.x<<","<<p.y<<","<<p.z<<"}";
	return stream;
}*/
