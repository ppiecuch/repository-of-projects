#ifndef _VECTOR3F_H_
#define _VECTOR3F_H_
//#include <iostream>
#include <GLES/gl.h>
#include <math.h>

class Vector3f{
public:
	GLfloat x,y,z;
	Vector3f();
	Vector3f(GLfloat,GLfloat,GLfloat);
	/*����ƽ��*/
	GLfloat lengthSquared() const;
	/*����*/
	GLfloat length() const;
	/*�淶��*/
	void normalize();
	void toString() const;
	//friend std::ostream & operator <<(std::ostream&,const Point3f&);
};
#endif
