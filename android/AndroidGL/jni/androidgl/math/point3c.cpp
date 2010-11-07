#include "point3c.h"
#include "../log.h"
//#include <iostream>
//using namespace std;
Point3c::Point3c():x(0),y(0),z(0)
{

}
Point3c::Point3c(GLbyte x,GLbyte y,GLbyte z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}
void Point3c::toString() const
{
	LOGI("Point3c","{%d,%d,%d}",this->x,this->y,this->z);
	//LOGI("{%.2f,%.2f,%.2f}",this->x,this->y,this->z);
}
/*ostream & operator <<(ostream &stream,const Point3f &p){
	stream<<"{"<<p.x<<","<<p.y<<","<<p.z<<"}";
	return stream;
}*/
