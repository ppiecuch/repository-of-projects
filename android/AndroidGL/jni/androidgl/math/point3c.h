#ifndef _POINT3C_H_
#define _POINT3C_H_
//#include <iostream>
#include <GLES/gl.h>

class Point3c{
public:
	GLbyte x,y,z;
	Point3c();
	Point3c(GLbyte,GLbyte,GLbyte);
	void toString() const;
	//friend std::ostream & operator <<(std::ostream&,const Point3f&);
};
#endif
