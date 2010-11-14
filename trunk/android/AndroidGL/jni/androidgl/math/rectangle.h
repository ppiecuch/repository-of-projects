/**
 * ³¤·½ÐÎ
 */
#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_
#include <GLES/gl.h>
#include <math.h>
#include "point2f.h"

class Rectangle
{
private:
	GLfloat length,height;
	Point2f* vertexs;
	static const int bases[8];
public:
	static const short normals[][3];
	static const short texCoords[][2];
	static const unsigned char vertexIndices[][4];
	Rectangle(GLfloat length,GLfloat height);
	Point2f* getVertexs() const;
	void toString() const;
};
#endif
