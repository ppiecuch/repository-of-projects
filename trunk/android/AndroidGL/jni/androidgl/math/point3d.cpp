#include "point3d.h"
#include "../log.h"
//#include <iostream>
//using namespace std;
Point3d::Point3d():x(0),y(0),z(0)
{

}
Point3d::Point3d(GLint x,GLint y,GLint z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}
void Point3d::toString() const
{
	LOGI("Point3d","{%d,%d,%d}",this->x,this->y,this->z);
	//LOGI("{%.2f,%.2f,%.2f}",this->x,this->y,this->z);
}
/*ostream & operator <<(ostream &stream,const Point3f &p){
	stream<<"{"<<p.x<<","<<p.y<<","<<p.z<<"}";
	return stream;
}*/
