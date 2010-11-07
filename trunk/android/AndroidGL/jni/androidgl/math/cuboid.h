/**
 * ������
 * ע��:����ʹ�ö�������,��������������������޷���ȷӳ��,����ֻ�����������4�������ӳ������
 */
#ifndef _CUBOID_H_
#define _BUBOID_H_
#include <GLES/gl.h>
#include <math.h>
#include "point3f.h"

class Cuboid
{
private:
	GLfloat length,width,height;
	Point3f* vertexs;
	static const int bases[72];
public:
	static const short normals[][3];
	static const short texCoords[][2];
	static const unsigned char vertexIndices[][4];
	Cuboid(GLfloat length,GLfloat width,GLfloat height);
	Point3f* getVertexs() const;
	void toString() const;
};
#endif
