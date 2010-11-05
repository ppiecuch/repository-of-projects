#include "point3f.h"
#include <stl_pair.h>
using namespace std;
Point3f::Point3f()
{

}
Point3f::Point3f(float x,float y,float z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}
ostream & operator <<(ostream &stream,const Point3f &p){
	stream<<"{"<<p.x<<","<<p.y<<","<<p.z<<"}";
	return stream;
}
