#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#ifdef TEXTURE_EXPORTS
#define Cpp_API __declspec(dllexport)
#else
#define Cpp_API __declspec(dllimport)
#endif

//#include "vector3f.h"
#include <memory>
#include <cmath>
#include <iostream>
using namespace std;

#define PI 3.141592654
#define DEGTORAD(degree) ((degree/180.0f)*PI)
#define RADTODEG(radian) ((radian/PI)*180.0f)

class Cpp_API Matrix4x4f
{
	public:
		Matrix4x4f(){identity();}
		Matrix4x4f(float m0, float m1, float m2, float m3,
					float m4, float m5, float m6, float m7,
					float m8, float m9, float m10, float m11,
					float m12, float m13, float m14, float m15);

		void identity();
		Matrix4x4f transpose();
		float *getMatrix(){return m_matrix;}
		void setMatrix(float *matrix);
		static Matrix4x4f invertMatrix( const Matrix4x4f *matIn );
		static Matrix4x4f invertMatrix( const Matrix4x4f &matIn );
//		void translate(const vector3f &translate);
//		void translateX(const float &x_distance);
//		void translateY(const float &y_distance);
//		void translateZ(const float &z_distance);
//		void rotate(const float &angle, vector3f &axis);
//		void rotateX(const float &angle);
//		void rotateY(const float &angle);
//		void rotateZ(const float &angle);
//		void scale(const vector3f &scale);
//		void transformPoint(vector3f &v1);
//		void transformVector(vector3f &v1);

		Matrix4x4f operator + (const Matrix4x4f &matrix);
		Matrix4x4f operator - (const Matrix4x4f &matrix);
		Matrix4x4f operator * (const Matrix4x4f &matrix);
		Matrix4x4f operator * (const float scalar);
		Matrix4x4f operator = (const Matrix4x4f &matrix);

	private:
		float m_matrix[16];
};

#endif