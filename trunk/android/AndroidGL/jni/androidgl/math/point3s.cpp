#include "point3s.h"
#include "../log.h"
//#include <iostream>
//using namespace std;
Point3s::Point3s():x(0),y(0),z(0)
{

}
Point3s::Point3s(GLshort x,GLshort y,GLshort z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}
void Point3s::toString() const
{
	LOGI("Point3s","{%d,%d,%d}",this->x,this->y,this->z);
	//LOGI("{%.2f,%.2f,%.2f}",this->x,this->y,this->z);
}
/*ostream & operator <<(ostream &stream,const Point3f &p){
	stream<<"{"<<p.x<<","<<p.y<<","<<p.z<<"}";
	return stream;
}*/
