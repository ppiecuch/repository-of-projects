#ifndef _POINT3F_H_
#define _POINT3F_H_
//#include <iostream>
#include <GLES/gl.h>

class Point3f{
public:
	GLfloat x,y,z;
	Point3f();
	Point3f(GLfloat,GLfloat,GLfloat);
	void toString() const;
	//friend std::ostream & operator <<(std::ostream&,const Point3f&);
};
#endif
