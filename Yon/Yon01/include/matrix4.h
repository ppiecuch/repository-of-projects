#ifndef _YON_CORE_MATRIX4_H_
#define _YON_CORE_MATRIX4_H_

#include "yonTypes.h"
#include "yonMath.h"
#include "vector3d.h"
#include <memory.h>
namespace yon{
	namespace core{

		template<class T>
		class matrix4{
		private:
			//T m[4][4];
		public:
			T m[4][4];
			matrix4(){}
			matrix4(const T t[16]){
				for(int i=0;i<4;++i){
					for(int j=0;j<4;++j){
						m[i][j]=t[(i<<2)+j];
					}
				}
			}
			matrix4(const matrix4<T>& other){
				*this=other;
			}
			inline void makeIdentity(){
				memset(m, 0x0, sizeof(m));
				m[0][0] = 1.0f;
				m[1][1] = 1.0f;
				m[2][2] = 1.0f;
				m[3][3] = 1.0f;
			}
			inline void makeTranspose(){
				T temp;
				for(int i=0;i<4;++i){
					for(int j=0;j<i;++j){
						temp=m[i][j];
						m[i][j]=m[j][i];
						m[j][i]=temp;
					}
				}
			}
			inline void translate(T x,T y,T z){
				m[3][0]+=m[0][0]*x+m[1][0]*y+m[2][0]*z;
				m[3][1]+=m[0][1]*x+m[1][1]*y+m[2][1]*z;
				m[3][2]+=m[0][2]*x+m[1][2]*y+m[2][2]*z;
				m[3][3]+=m[0][3]*x+m[1][3]*y+m[2][3]*z;
			}
			inline void setTranslation(const vector3d<T>& translation){
				m[3][0]=translation.x;
				m[3][1]=translation.y;
				m[3][2]=translation.z;
			}
			inline void rotate(f32 degree,f32 x,f32 y,f32 z){
				matrix4 rot;
				rot.makeIdentity();
				rot.setRotation(degree,x,y,z);
				*this*=rot;
			}
			inline void setRotation(f32 degree,f32 x,f32 y,f32 z){
				f32 mag=sqrt(x*x+y*y+z*z);
				f32 sinAngle = sinf (degree * DEGTORAD);
				f32 cosAngle = cosf (degree * DEGTORAD);
				if (mag > 0.0f)
				{
					f32 xx, yy, zz, xy, yz, zx, xs, ys, zs;
					f32 oneMinusCos;

					x /= mag;
					y /= mag;
					z /= mag;

					xx = x * x;
					yy = y * y;
					zz = z * z;
					xy = x * y;
					yz = y * z;
					zx = z * x;
					xs = x * sinAngle;
					ys = y * sinAngle;
					zs = z * sinAngle;
					oneMinusCos = 1.0f - cosAngle;

					m[0][0] = (T)(oneMinusCos * xx) + cosAngle;
					m[0][1] = (T)(oneMinusCos * xy) - zs;
					m[0][2] = (T)(oneMinusCos * zx) + ys;

					m[1][0] = (T)(oneMinusCos * xy) + zs;
					m[1][1] = (T)(oneMinusCos * yy) + cosAngle;
					m[1][2] = (T)(oneMinusCos * yz) - xs;

					m[2][0] = (T)(oneMinusCos * zx) - ys;
					m[2][1] = (T)(oneMinusCos * yz) + xs;
					m[2][2] = (T)(oneMinusCos * zz) + cosAngle;
				}
			}
			inline matrix4<T> operator*(const matrix4<T>& other) const{
				 matrix4<T> r;
				 for(int i=0;i<4;++i){
					 for(int j=0;j<4;++j){
						 r.m[j][i]=m[0][i]*other.m[j][0]+m[1][i]*other.m[j][1]+m[2][i]*other.m[j][2]+m[3][i]*other.m[j][3];
					 }
				 }
				 return r;
			}
			inline matrix4<T>& operator=(const matrix4<T>& other){
				if(this==&other)
					return *this;
				memcpy(m,other.m,sizeof(m));
				return *this;
			}
			inline matrix4<T>& operator*=(const matrix4<T>& other){
				//TODO setByProduct(temp,other)
				matrix4 temp;
				temp=*this*other;
				*this=temp;
				return *this;
			}
			/*const T& operator[](u32 index) const{
				u32 quotient=index>>2;
				u32 remainder=index-quotient;
				return m[quotient][remainder];
			}*/
		};

		typedef matrix4<f32> matrix4f;
	}
}
#endif