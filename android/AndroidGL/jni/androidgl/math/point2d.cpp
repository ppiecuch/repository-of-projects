#include "point2d.h"
#include "../log.h"
//#include <iostream>
//using namespace std;
Point2d::Point2d():x(0),y(0)
{

}
Point2d::Point2d(GLint x,GLint y)
{
	this->x=x;
	this->y=y;
}
void Point2d::toString() const
{
	LOGI("Point2d","{%d,%d}",this->x,this->y);
	//LOGI("{%.2f,%.2f,%.2f}",this->x,this->y,this->z);
}
/*ostream & operator <<(ostream &stream,const Point3f &p){
	stream<<"{"<<p.x<<","<<p.y<<","<<p.z<<"}";
	return stream;
}*/
