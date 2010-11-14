#include "point2s.h"
#include "../log.h"
//#include <iostream>
//using namespace std;
Point2s::Point2s():x(0),y(0)
{

}
Point2s::Point2s(GLshort x,GLshort y)
{
	this->x=x;
	this->y=y;
}
void Point2s::toString() const
{
	LOGI("Point2s","{%d,%d}",this->x,this->y);
	//LOGI("{%.2f,%.2f,%.2f}",this->x,this->y,this->z);
}
/*ostream & operator <<(ostream &stream,const Point3f &p){
	stream<<"{"<<p.x<<","<<p.y<<","<<p.z<<"}";
	return stream;
}*/
