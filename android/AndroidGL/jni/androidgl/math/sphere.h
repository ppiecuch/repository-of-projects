#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "point3f.h"
#include "point2f.h"
#include "vector3f.h"
#include <math.h>

class Sphere
{
private:
	float radius;
	static const float DTOR=M_PI/180;
	static const float M_2PI=M_PI+M_PI;
	int horizonalSteps,portraitSteps;
	//Point3f*** vertex;
	Point3f* vertexs;
	Point2f* texCoords;
	Point3f* normals;
	/*将point复制到vector并进行规范化*/
	void normalizeVertexToVector(const Point3f& point,Vector3f& vector);
public:
	Sphere(float radius,int horizonalSteps,int portraitSteps);
	void toString() const;
	//Point3f*** getVertex() const;
	Point3f*  getVertexs() const;
	Point2f* getTexCoords() const;
	Point3f* getNormals() const;
};
#endif
