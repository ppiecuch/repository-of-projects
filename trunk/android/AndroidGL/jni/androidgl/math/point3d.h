#ifndef _POINT3D_H_
#define _POINT3D_H_
//#include <iostream>
#include <GLES/gl.h>

class Point3d{
public:
	GLint x,y,z;
	Point3d();
	Point3d(GLint,GLint,GLint);
	void toString() const;
	//friend std::ostream & operator <<(std::ostream&,const Point3f&);
};
#endif
