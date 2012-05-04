
#include "sphere.h"
#include <stdio.h>
const float Sphere::DTOR=M_PI/180;
const float Sphere::M_2PI=M_PI+M_PI;
Sphere::Sphere(float radius,int horizonalSteps,int portraitSteps)
{
	this->radius=radius;
	this->horizonalSteps=horizonalSteps;
	this->portraitSteps=portraitSteps;
	float dtheta=(float)360/horizonalSteps;	//水平方向步增
	float dphi=(float)180/portraitSteps;		//垂直方向步增

	this->vertexs=new Point3f[portraitSteps*horizonalSteps<<2];
	this->texCoords=new Point2f[portraitSteps*horizonalSteps<<2];
	this->normals=new Point3f[portraitSteps*horizonalSteps<<2];
	Vector3f* tempVector=new Vector3f();
	int index=0;
	for(int i=0,phi=0;i<portraitSteps;i++,phi+=dphi)
	{
		for(int j=0,theta=0;j<horizonalSteps;j++,theta+=dtheta)
		{
			vertexs[index].z = (GLfloat) (radius * sinf(phi*DTOR) * cosf(DTOR*theta));
			vertexs[index].x = (GLfloat) (radius * sinf(phi*DTOR) * sinf(DTOR*theta));
			vertexs[index].y = (GLfloat) (radius * cosf(phi*DTOR));
			normalizeVertexToVector(vertexs[index],*tempVector);
			texCoords[index].x=(GLfloat)j/horizonalSteps;
			texCoords[index].y=(GLfloat)asinf(tempVector->y)/M_PI+0.5f;
			normals[index].x=tempVector->x;
			normals[index].y=tempVector->y;
			normals[index].z=tempVector->z;
			index++;
			vertexs[index].z = (GLfloat) (radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*theta));
			vertexs[index].x = (GLfloat) (radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*theta));
			vertexs[index].y = (GLfloat) (radius * cosf((phi+dphi)*DTOR));
			normalizeVertexToVector(vertexs[index],*tempVector);
			texCoords[index].x=(GLfloat)j/horizonalSteps;
			texCoords[index].y=(GLfloat)asinf(tempVector->y)/M_PI+0.5f;
			normals[index].x=tempVector->x;
			normals[index].y=tempVector->y;
			normals[index].z=tempVector->z;
			index++;
			vertexs[index].z = (GLfloat) (radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*(theta+dtheta)));
			vertexs[index].x = (GLfloat) (radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*(theta+dtheta)));
			vertexs[index].y = (GLfloat) (radius * cosf((phi+dphi)*DTOR));
			normalizeVertexToVector(vertexs[index],*tempVector);
			texCoords[index].x=(GLfloat)(j+1)/horizonalSteps;
			texCoords[index].y=(GLfloat)asinf(tempVector->y)/M_PI+0.5f;
			normals[index].x=tempVector->x;
			normals[index].y=tempVector->y;
			normals[index].z=tempVector->z;
			index++;
			vertexs[index].z = (GLfloat) (radius * sinf(phi*DTOR) * cosf(DTOR*(theta+dtheta)));
			vertexs[index].x = (GLfloat) (radius * sinf(phi*DTOR) * sinf(DTOR*(theta+dtheta)));
			vertexs[index].y = (GLfloat) (radius * cosf(phi*DTOR));
			normalizeVertexToVector(vertexs[index],*tempVector);
			texCoords[index].x=(GLfloat)(j+1)/horizonalSteps;
			texCoords[index].y=(GLfloat)asinf(tempVector->y)/M_PI+0.5f;
			normals[index].x=tempVector->x;
			normals[index].y=tempVector->y;
			normals[index].z=tempVector->z;
			index++;
		}
	}

	delete tempVector;

	/*
	this->vertexs=(Point3f***) new Point3f**[portraitSteps];
	for(int i=0,phi=0;i<portraitSteps;i++,phi+=dphi)
	{
		vertexs[i]=(Point3f**)new Point3f*[horizonalSteps];
		for(int j=0,theta=0;j<horizonalSteps;j++,theta+=dtheta)
		{
			vertexs[i][j]=new Point3f[4];
			Point3f* point=new Point3f();
			//Point2f uv=new Point2f();
			point->y = (GLfloat) (radius * sinf(phi*DTOR) * cosf(DTOR*theta));
			point->x = (GLfloat) (radius * sinf(phi*DTOR) * sinf(DTOR*theta));
			point->z = (GLfloat) (radius * cosf(phi*DTOR));
			vertexs[i][j][0]=*point;
			//Vector3f vector=new Vector3f(point.x, point.y, point.z);
			//vector.normalize();
			//uv.x=(GLfloat)j/horizonalSteps;
			//uv.y=(GLfloat) (Math.asinf(vector.z)/Math.PI*2);
			//texCoords[i][j][0]=uv;

			point=new Point3f();
			//uv=new Point2f();
			point->y = (GLfloat) (radius * sinf(phi*DTOR) * cosf(DTOR*(theta+dtheta)));
			point->x = (GLfloat) (radius * sinf(phi*DTOR) * sinf(DTOR*(theta+dtheta)));
			point->z = (GLfloat) (radius * cosf(phi*DTOR));
			vertexs[i][j][1]=*point;
			//vector=new Vector3f(point.x, point.y, point.z);
			//vector.normalize();
			//uv.x=(GLfloat)(j+1)/horizonalSteps;
			//uv.y=(GLfloat) (Math.asinf(vector.z)/Math.PI*2);
			//texCoords[i][j][1]=uv;

			point=new Point3f();
			//uv=new Point2f();
			point->y = (GLfloat) (radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*(theta+dtheta)));
			point->x = (GLfloat) (radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*(theta+dtheta)));
			point->z = (GLfloat) (radius * cosf((phi+dphi)*DTOR));
			vertexs[i][j][2]=*point;
			//vector=new Vector3f(point.x, point.y, point.z);
			//vector.normalize();
			//uv.x=(GLfloat)(j+1)/horizonalSteps;
			//uv.y=(GLfloat) (Math.asinf(vector.z)/Math.PI*2);
			//texCoords[i][j][2]=uv;

			point=new Point3f();
			//uv=new Point2f();
			point->y = (GLfloat) (radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*theta));
			point->x = (GLfloat) (radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*theta));
			point->z = (GLfloat) (radius * cosf((phi+dphi)*DTOR));
			vertexs[i][j][3]=*point;
			//vector=new Vector3f(point.x, point.y, point.z);
			//vector.normalize();
			//uv.x=(GLfloat)j/horizonalSteps;
			//uv.y=(GLfloat) (Math.asinf(vector.z)/Math.PI*2);
			//texCoords[i][j][3]=uv;
		}
	}
	*/
}
Sphere::~Sphere()
{
	delete[] vertexs;
	delete[] texCoords;
	delete[] normals;
}
/**
 * 将point复制到vector并进行规范化
 */
void Sphere::normalizeVertexToVector(const Point3f& point,Vector3f& vector)
{
	vector.x=point.x;
	vector.y=point.y;
	vector.z=point.z;
	vector.normalize();
}
Point3f* Sphere::getVertexs() const
{
	return this->vertexs;
}
Point2f* Sphere::getTexCoords() const
{
	return this->texCoords;
}
Point3f* Sphere::getNormals() const
{
	return this->normals;
}
/*
Point3f*** Sphere::getvertexs() const
{
	return this->vertexs;
}
*/
void Sphere::toString() const
{
	printf("radius:%.2f,horizonalSteps:%d,portraitSteps:%d",radius,horizonalSteps,portraitSteps);
	//LOGI("radius:%.2f,horizonalSteps:%d,portraitSteps:%d",this->radius,this->horizonalSteps,this->portraitSteps);
}
int Sphere::getHorizonalSteps() const{
	return horizonalSteps;
}
int Sphere::getPortraitSteps() const{
	return portraitSteps;
}