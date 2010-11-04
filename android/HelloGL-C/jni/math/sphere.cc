#include <iostream>
#include <math.h>
#include "sphere.h"
#include "point3f.h"

Sphere::Sphere(float radius,int horizonalSteps,int portraitSteps)
{
	this.radius=radius;
	this.horizonalSteps=horizonalSteps;
	this.portraitSteps=portraitSteps;
	float dtheta=(float)360/horizonalSteps;	//水平方向步增
	float dphi=(float)180/portraitSteps;		//垂直方向步增

	this.vertex=(Point3f***)new Point3f**[portraitSteps];
	for(int i=0,phi=0;i<portraitSteps;i++,phi+=dphi)
	{
		vertex[i]=(Point3f**)new Point3f*[horizonalSteps];
		for(int j=0,theta=0;j<horizonalSteps;j++,theta+=dtheta)
		{
			vertex[i][j]=new Point3f[4];
			Point3f point=new Point3f();
			Point2f uv=new Point2f();
			point.y = (float) (radius * sin(phi*DTOR) * cos(DTOR*theta));
			point.x = (float) (radius * sin(phi*DTOR) * sin(DTOR*theta));
			point.z = (float) (radius * cos(phi*DTOR));
			vertex[i][j][0]=point;
			//Vector3f vector=new Vector3f(point.x, point.y, point.z);
			//vector.normalize();
			//uv.x=(float)j/horizonalSteps;
			//uv.y=(float) (Math.asin(vector.z)/Math.PI*2);
			//texCoords[i][j][0]=uv;

			point=new Point3f();
			uv=new Point2f();
			point.y = (float) (radius * sin(phi*DTOR) * cos(DTOR*(theta+dtheta)));
			point.x = (float) (radius * sin(phi*DTOR) * sin(DTOR*(theta+dtheta)));
			point.z = (float) (radius * cos(phi*DTOR));
			vertex[i][j][1]=point;
			//vector=new Vector3f(point.x, point.y, point.z);
			//vector.normalize();
			//uv.x=(float)(j+1)/horizonalSteps;
			//uv.y=(float) (Math.asin(vector.z)/Math.PI*2);
			//texCoords[i][j][1]=uv;

			point=new Point3f();
			uv=new Point2f();
			point.y = (float) (radius * sin((phi+dphi)*DTOR) * cos(DTOR*(theta+dtheta)));
			point.x = (float) (radius * sin((phi+dphi)*DTOR) * sin(DTOR*(theta+dtheta)));
			point.z = (float) (radius * cos((phi+dphi)*DTOR));
			vertex[i][j][2]=point;
			//vector=new Vector3f(point.x, point.y, point.z);
			//vector.normalize();
			//uv.x=(float)(j+1)/horizonalSteps;
			//uv.y=(float) (Math.asin(vector.z)/Math.PI*2);
			//texCoords[i][j][2]=uv;

			point=new Point3f();
			uv=new Point2f();
			point.y = (float) (radius * sin((phi+dphi)*DTOR) * cos(DTOR*theta));
			point.x = (float) (radius * sin((phi+dphi)*DTOR) * sin(DTOR*theta));
			point.z = (float) (radius * cos((phi+dphi)*DTOR));
			vertex[i][j][3]=point;
			//vector=new Vector3f(point.x, point.y, point.z);
			//vector.normalize();
			//uv.x=(float)j/horizonalSteps;
			//uv.y=(float) (Math.asin(vector.z)/Math.PI*2);
			//texCoords[i][j][3]=uv;
		}
	}
}
