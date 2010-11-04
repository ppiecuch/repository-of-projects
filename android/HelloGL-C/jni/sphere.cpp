#include <string>
#include <iostream>
#include <math.h>
using¡¡namespace¡¡std;

class Sphere
{
private:
	double radius;
	const static double DTOR=M_PI/180;
	int horizonalSteps,portraitSteps;
public:
	Sphere(double radius,int horizonalSteps,int portraitSteps)
	{
		this.radius=radius;
		this.horizonalSteps=horizonalSteps;
		this.portraitSteps=portraitSteps;
	}

};
