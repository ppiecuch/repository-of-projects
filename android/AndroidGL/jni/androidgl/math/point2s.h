#ifndef _POINT2S_H_
#define _POINT2S_H_
//#include <iostream>
#include <GLES/gl.h>

class Point2s{
public:
	GLshort x,y;
	Point2s();
	Point2s(GLshort,GLshort);
	void toString() const;
	//friend std::ostream & operator <<(std::ostream&,const Point3f&);
};
#endif
