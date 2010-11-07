/**
 * 长方体
 * 注意:由于使用顶点索引,而有两个面的纹理坐标无法正确映射,所以只能另外添加这4个顶点的映射坐标
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
