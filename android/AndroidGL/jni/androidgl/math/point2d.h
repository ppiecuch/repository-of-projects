#ifndef _POINT2D_H_
#define _POINT2D_H_
//#include <iostream>
#include <GLES/gl.h>

class Point2d{
public:
	GLint x,y;
	Point2d();
	Point2d(GLint,GLint);
	void toString() const;
	//friend std::ostream & operator <<(std::ostream&,const Point3f&);
};
#endif
