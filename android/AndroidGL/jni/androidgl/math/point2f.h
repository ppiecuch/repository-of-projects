#ifndef _POINT2F_H_
#define _POINT2F_H_
//#include <iostream>
#include <GLES/gl.h>

class Point2f{
public:
	GLfloat x,y;
	Point2f();
	Point2f(GLfloat,GLfloat);
	void toString() const;
	//friend std::ostream & operator <<(std::ostream&,const Point3f&);
};
#endif
