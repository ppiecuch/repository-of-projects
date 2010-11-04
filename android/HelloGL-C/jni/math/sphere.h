#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "point3f.h"

class Sphere
{
private:
	float radius;
	const static float DTOR=M_PI/180;
	int horizonalSteps,portraitSteps;
	Point3f*** vertex;
public:
	Sphere(float radius,int horizonalSteps,int portraitSteps);
};
#endif
