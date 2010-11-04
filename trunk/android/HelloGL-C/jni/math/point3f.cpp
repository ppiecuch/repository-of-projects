#include "point3f.h"
#include <iostream>
using namespace std;

Point3f::Point3f()
{
	cout<<"Create Point3f"<<endl;
}
Point3f::Point3f(float x,float y,float z)
{
	this.x=x;
	this.y=y;
	this.z=z;
}

