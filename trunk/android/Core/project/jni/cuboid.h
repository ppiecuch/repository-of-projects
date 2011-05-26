/**
 * 长方体
 * 注意:由于使用顶点索引,而有两个面的纹理坐标无法正确映射,所以只能另外添加这4个顶点的映射坐标
 */
#ifndef _CUBOID_H_
#define _CUBOID_H_
#include <math.h>
#include "point3f.h"
#include "object3d.h"

class Cuboid:Object3D
{
private:
	float length,width,height;
	Point3f* vertexs;
	static const int bases[24];
public:
	static const short normals[][3];
	static const unsigned char vertexIndices[][4];
	Cuboid(float length,float width,float height);
	Point3f* getVertexs() const;
	float getLength() const;
	float getWidth() const;
	float getHeight() const;
};
#endif
