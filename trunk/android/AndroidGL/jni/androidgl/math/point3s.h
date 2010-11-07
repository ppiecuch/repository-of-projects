#ifndef _POINT3S_H_
#define _POINT3S_H_
//#include <iostream>
#include <GLES/gl.h>

class Point3s{
public:
	GLshort x,y,z;
	Point3s();
	Point3s(GLshort,GLshort,GLshort);
	void toString() const;
	//friend std::ostream & operator <<(std::ostream&,const Point3f&);
};
#endif
