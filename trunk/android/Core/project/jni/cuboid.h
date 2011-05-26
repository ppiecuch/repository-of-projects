/**
 * ������
 * ע��:����ʹ�ö�������,��������������������޷���ȷӳ��,����ֻ�����������4�������ӳ������
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
