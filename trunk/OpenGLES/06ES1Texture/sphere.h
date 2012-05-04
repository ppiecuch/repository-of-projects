#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "point3f.h"
#include "point2f.h"
#include "vector3f.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

class Sphere
{
private:
	float radius;
	static const float DTOR;
	static const float M_2PI;
	int horizonalSteps,portraitSteps;
	//Point3f*** vertex;
	Point3f* vertexs;
	Point2f* texCoords;
	Point3f* normals;
	/*将point复制到vector并进行规范化*/
	void normalizeVertexToVector(const Point3f& point,Vector3f& vector);
public:
	Sphere(float radius,int horizonalSteps,int portraitSteps);
	~Sphere();
	void toString() const;
	//Point3f*** getVertex() const;
	Point3f*  getVertexs() const;
	Point2f* getTexCoords() const;
	Point3f* getNormals() const;
	int getHorizonalSteps() const;
	int getPortraitSteps() const;
};
#endif
