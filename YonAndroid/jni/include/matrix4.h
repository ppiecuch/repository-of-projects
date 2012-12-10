#ifndef _YON_CORE_MATRIX4_H_
#define _YON_CORE_MATRIX4_H_

#include "yonConfig.h"
#include "yonTypes.h"
#include "yonMath.h"
#include "vector3d.h"
#include <memory.h>
#ifdef YON_COMPILE_WITH_ANDROID
#include <string.h>
#endif

#include "ILogger.h"

namespace yon{
namespace core{

	class quaternion;

	template<class T>
	class matrix4{
	private:
		//无论dx还是opengl，所表示的矢量和矩阵都是依据线性代数中的标准定义的：
		//“矩阵A与B的乘积矩阵C的第i行第j列的元素c(ij)等于A的第i行于B的第j列的对应元素乘积的和。”（实用数学手册，科学出版社，第二版）
		//a00 a10 a20 a30    b00 b10 b20 b30   c00 c10 c20 c30
		//a01 a11 a21 a31 \/ b01 b11 b21 b31 _ c01 c11 c21 c31
		//a02 a12 a22 a32 /\ b02 b12 b22 b32 - c02 c12 c22 c32
		//a03 a13 a23 a33    b03 b13 b23 b33   c03 c13 c23 c33
		//如下，取i=0,j=1，则有c10=a的第0行*b的第1列
		//refer to:http://www.cnblogs.com/cgwolver/archive/2009/07/29/1533570.html
		//T m[4][4];
		inline void setByProduct(const matrix4& a,const matrix4& b){
			//TODO 4阶矩阵快速乘法[[C++]GLDemo04_1]
			//矩阵快速乘法http://blog.csdn.net/byhuang/article/details/1480695
			//for(int i=0;i<4;++i){
			//	for(int j=0;j<4;++j){
			//		m[j][i]=a.m[0][i]*b.m[j][0]+a.m[1][i]*b.m[j][1]+a.m[2][i]*b.m[j][2]+a.m[3][i]*b.m[j][3];
			//	}
			//}
			const T *m1 = a.M;
			const T *m2 = b.M;

			M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
			M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
			M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
			M[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

			M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
			M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
			M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
			M[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

			M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
			M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
			M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
			M[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

			M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
			M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
			M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
			M[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
		}
		inline void checkIdentity() const{
			/*m_bIsChanged=false;
			m_bIsIdentity=true;
			if (!core::equals( m[0][0], (T)1 ) ||
				!core::equals( m[1][1], (T)1 ) ||
				!core::equals( m[2][2], (T)1 ) ||
				!core::equals( m[3][3], (T)1 )){
				m_bIsIdentity=false;
				return;
			}
				

			for (s32 i=0; i<4; ++i)
				for (s32 j=0; j<4; ++j)
					if ((j != i) && (!iszero(m[i][j]))){
						m_bIsIdentity=false;
						return;
					}
			*/
			if (!core::equals( M[ 0], (T)1 ) ||
				!core::equals( M[ 5], (T)1 ) ||
				!core::equals( M[10], (T)1 ) ||
				!core::equals( M[15], (T)1 ))
			{
				m_bIsIdentity=false;
				return;
			}

			for(s32 i=0;i<16;++i)
				//TODO 0,5,10,15在位运算有共同点没？
				if(i!=0&&i!=5&&i!=10&&i!=15&&!iszero(M[i]))
				{
					m_bIsIdentity=false;
					return;
				}
		}
		mutable bool m_bIsIdentity : 1;
		mutable bool m_bIsChanged : 1;
		T M[16];

		friend class quaternion;
	public:
		//T m[4][4];
		matrix4(bool identity=false)
			:m_bIsIdentity(identity),m_bIsChanged(!identity){
			if(identity==true){
				makeIdentity();
			}
		}
		matrix4(const T t[16])
			:m_bIsIdentity(false),m_bIsChanged(true){
			/*for(int i=0;i<4;++i){
				for(int j=0;j<4;++j){
					m[i][j]=t[(i<<2)+j];
				}
			}*/
			//TODO memcpy
			for(s32 i=0;i<16;++i)
				M[i]=t[i];
		}
		matrix4(const matrix4<T>& other)
			:m_bIsIdentity(false),m_bIsChanged(true){
			*this=other;
		}

		matrix4(const T n11, const T n12, const T n13, const T n14,
			const T n21, const T n22, const T n23, const T n24,
			const T n31, const T n32, const T n33, const T n34,
			const T n41, const T n42, const T n43, const T n44)
			:m_bIsIdentity(false),m_bIsChanged(true){
				M[0] = n11;
				M[1] = n12;
				M[2] = n13;
				M[3] = n14;
				M[4] = n21;
				M[5] = n22;
				M[6] = n23;
				M[7] = n24;
				M[8] = n31;
				M[9] = n32;
				M[10] = n33;
				M[11] = n34;
				M[12] = n41;
				M[13] = n42;
				M[14] = n43;
				M[15] = n44;
		}

		inline void print() const{
			if(Logger){
				//for(u32 i=0;i<4;++i)
				//	YON_DEBUG("%.3f,%.3f,%.3f,%.3f\n",m[0][i],m[1][i],m[2][i],m[3][i]);
				YON_DEBUG("%.3f,%.3f,%.3f,%.3f\r\n%.3f,%.3f,%.3f,%.3f\r\n%.3f,%.3f,%.3f,%.3f\r\n%.3f,%.3f,%.3f,%.3f\r\n",M[0],M[4],M[8],M[12],M[1],M[5],M[9],M[13],M[2],M[6],M[10],M[14],M[3],M[7],M[11],M[15]);
			}else{
				//for(u32 i=0;i<4;++i)
				//	printf("%.3f,%.3f,%.3f,%.3f\n\n",m[0][i],m[1][i],m[2][i],m[3][i]);
				printf("%.3f,%.3f,%.3f,%.3f\r\n%.3f,%.3f,%.3f,%.3f\r\n%.3f,%.3f,%.3f,%.3f\r\n%.3f,%.3f,%.3f,%.3f\r\n",M[0],M[4],M[8],M[12],M[1],M[5],M[9],M[13],M[2],M[6],M[10],M[14],M[3],M[7],M[11],M[15]);
			}
		}

		const T* pointer() const { 
			return &M[0];//return &m[0][0]; 
		}
		T* pointer() { 
			return &M[0];
			//return &m[0][0];
		}

		inline bool isIdentity() const
		{
			if(m_bIsChanged)
				checkIdentity();
			return m_bIsIdentity;
		}

		inline void makeIdentity(){
			memset(M, 0x0, sizeof(M));
			//m[0][0] = 1.0f;
			//m[1][1] = 1.0f;
			//m[2][2] = 1.0f;
			//m[3][3] = 1.0f;
			M[0]=M[5]=M[10]=M[15]=(T)1;

			m_bIsChanged = false;
			m_bIsIdentity = true;
		}
		inline void makeTranspose(){
			/*T temp;
			for(int i=0;i<4;++i){
				for(int j=0;j<i;++j){
					temp=m[i][j];
					m[i][j]=m[j][i];
					m[j][i]=temp;
				}
			}*/
			matrix4 tmp(M[0], M[4], M[8], M[12],
						M[1], M[5], M[9], M[13],
						M[2], M[6], M[10], M[14],
						M[3], M[7], M[11], M[15]);
			tmp.m_bIsChanged=m_bIsChanged;
			tmp.m_bIsIdentity=m_bIsIdentity;
			*this = tmp;
			//不用调整，转置行为不影响是否单位变化
			//m_bIsChanged = true;
		}
		//不存在逆矩阵则返回false
		inline bool makeInverse(){
			
			int i, j, k;
			T temp;
			matrix4 tempMatrix(*this);
			matrix4 result;
			result.makeIdentity();
			for (i = 0; i<4; i++)
			{
				//寻找主元
				k = i;
				for (j=i+1; j<4; j++)
				{
					if (fabs(tempMatrix.M[(j<<2)+i]) > fabs(tempMatrix.M[(i<<2)+i]))
						k = j;
				}
				//如果主元所在行不是第i行，进行行交换
				if (k != i)
				{
					for (j=0; j<4; j++)
					{
						temp = tempMatrix.M[(i<<2)+j];
						tempMatrix.M[(i<<2)+j] = tempMatrix.M[(k<<2)+j];
						tempMatrix.M[(k<<2)+j] = temp;
						//result伴随交换
						temp = result.M[(i<<2)+j];
						result.M[(i<<2)+j]= result.M[(k<<2)+j];
						result.M[(k<<2)+j] = temp;
					}
				}
				//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵
				if (tempMatrix.M[(i<<2)+i] == 0)
					return false;
				//消去src的第i列除去i行以外的各行元素
				temp = tempMatrix.M[(i<<2)+i];
				for (j=0; j<4; j++)
				{
					tempMatrix.M[(i<<2)+j] /=temp;    //主对角线上的元素变为1
					result.M[(i<<2)+j] /= temp;       //伴随计算
				}
				for (j=0; j<4; j++)	
				{
					if (j != i)						//不是第i行
					{
						temp = tempMatrix.M[(j<<2)+i];
						for (k=0; k<4; k++)        //第j行元素 - i行元素*j列i行元素
						{
							tempMatrix.M[(j<<2)+k] -= tempMatrix.M[(i<<2)+k]*temp;
							result.M[(j<<2)+k] -= result.M[(i<<2)+k]*temp;
						}
					}
				}
			}
			*this=result;

			//不用调整，取逆行为不影响是否单位变化
			//m_bIsChanged = true;
			return true;
			

			//TODO *4改为移位
			/*matrix4 matA(*this);
			matrix4 matB(true);

			u32 uiRow, uiColumn;
			u32 uiRowMax; // Points to max abs value row in this column
			f32 tmp;

			// Go through columns
			for(uiColumn = 0; uiColumn < 4; uiColumn++)
			{

				// Find the row with max value in this column
				uiRowMax = uiColumn;
				for(uiRow = uiColumn+1; uiRow < 4; uiRow++)
				{
					if(core::abs_(matA[4 * uiRow + uiColumn]) > core::abs_(matA[4 * uiRowMax + uiColumn]))
					{
						uiRowMax = uiRow;
					}
				}

				// If the max value here is 0, we can't invert.  .
				if(matA[4 * uiColumn + uiRowMax] == 0.0F)
				{
					makeIdentity();
					YON_WARN(YON_LOG_WARN_FORMAT,"Not a valid matrix for inverse!");
					return false;
				}

				// Swap row "rowMax" with row "c"
				for(s32 cc = 0; cc < 4; cc++)
				{
					tmp = matA[4 * uiColumn + cc];
					matA[4 * uiColumn + cc] = matA[4 * uiRowMax + cc];
					matA[4 * uiRowMax + cc] = tmp;

					tmp = matB[4 * uiColumn + cc];
					matB[4 * uiColumn + cc] = matB[4 * uiRowMax + cc];
					matB[4 * uiRowMax + cc] = tmp;
				}

				// Now everything we do is on row "c".
				// Set the max cell to 1 by dividing the entire row by that value
				tmp = matA[4 * uiColumn + uiColumn];
				for(s32 cc = 0; cc < 4; cc++)
				{
					matA[4 * uiColumn + cc] /= tmp;
					matB[4 * uiColumn + cc] /= tmp;
				}

				// Now do the other rows, so that this column only has a 1 and 0's
				for(uiRow = 0; uiRow < 4; uiRow++)
				{
					if (uiRow != uiColumn)
					{
						tmp = matA[4 * uiRow + uiColumn];
						for(s32 cc = 0; cc < 4; cc++)
						{
							matA[4 * uiRow + cc] -= matA[4 * uiColumn + cc] * tmp;
							matB[4 * uiRow + cc] -= matB[4 * uiColumn + cc] * tmp;
						}
					}
				}
			}

			*this = matB;
			return true;*/
		}
		inline void translate(T x,T y,T z){
			//m[3][0]+=m[0][0]*x+m[1][0]*y+m[2][0]*z;
			//m[3][1]+=m[0][1]*x+m[1][1]*y+m[2][1]*z;
			//m[3][2]+=m[0][2]*x+m[1][2]*y+m[2][2]*z;
			//m[3][3]+=m[0][3]*x+m[1][3]*y+m[2][3]*z;
			M[12]+=M[0]*x+M[4]*y+M[8]*z;
			M[13]+=M[1]*x+M[5]*y+M[9]*z;
			M[14]+=M[2]*x+M[6]*y+M[10]*z;
			M[15]+=M[3]*x+M[7]*y+M[11]*z;

			m_bIsChanged=true;
		}
		inline void setTranslation(T x,T y,T z){
			//m[3][0]=x;
			//m[3][1]=y;
			//m[3][2]=z;
			M[12] = x;
			M[13] = y;
			M[14] = z;

			m_bIsChanged=true;
		}
		inline void setTranslation( const vector3d<T>& v){
			setTranslation(v.x,v.y,v.z);
		}
		inline vector3d<T> getTranslation() const{
			//return vector3d<T>(m[3][0],m[3][1],m[3][2]);
			return vector3d<T>(M[12],M[13],M[14]);
		}
		/*inline void setTranslation(const vector3d<T>& translation){
			m[3][0]=translation.x;
			m[3][1]=translation.y;
			m[3][2]=translation.z;
		}*/
		inline void rotate(f32 degree,f32 x,f32 y,f32 z){
			matrix4 rot;
			rot.makeIdentity();
			rot.setRotation(degree,x,y,z);
			*this*=rot;

			m_bIsChanged=true;
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
				/*m[0][0] = (T)(oneMinusCos * xx) + cosAngle;
				m[1][0] = (T)(oneMinusCos * xy) - zs;
				m[2][0] = (T)(oneMinusCos * zx) + ys;

				m[0][1] = (T)(oneMinusCos * xy) + zs;
				m[1][1] = (T)(oneMinusCos * yy) + cosAngle;
				m[2][1] = (T)(oneMinusCos * yz) - xs;

				m[0][2] = (T)(oneMinusCos * zx) - ys;
				m[1][2] = (T)(oneMinusCos * yz) + xs;
				m[2][2] = (T)(oneMinusCos * zz) + cosAngle;*/
				M[0] = (T)(oneMinusCos * xx) + cosAngle;
				M[4] = (T)(oneMinusCos * xy) - zs;
				M[8] = (T)(oneMinusCos * zx) + ys;

				M[1] = (T)(oneMinusCos * xy) + zs;
				M[5] = (T)(oneMinusCos * yy) + cosAngle;
				M[9] = (T)(oneMinusCos * yz) - xs;

				M[2] = (T)(oneMinusCos * zx) - ys;
				M[6] = (T)(oneMinusCos * yz) + xs;
				M[10] = (T)(oneMinusCos * zz) + cosAngle;

				m_bIsChanged=true;
			}
		}
		inline void scale(f32 x,f32 y,f32 z){
			matrix4 temp;
			temp.makeIdentity();
			temp.setScale(x,y,z);
			*this*=temp;

			m_bIsChanged=true;
		}
		inline void scale(const vector3d<T>& v){
			scale(v.x,v.y,v.z);
		}
		inline void setScale(const vector3df& v){
			setScale(v.x,v.y,v.z);
		}
		inline void setScale(f32 x,f32 y,f32 z){
			//m[0][0] = (T)x;
			//m[1][1] = (T)y;
			//m[2][2] = (T)z;
			//m[3][3] = (T)1.0f;
			M[0] = (T)x;
			M[5] = (T)y;
			M[10] = (T)z;
			M[15] = (T)1.0f;

			m_bIsChanged=true;
		}

		//TODO 待推导
		inline vector3d<T> getScale() const
		{
			/*
			// Deal with the 0 rotation case first
			if(core::iszero(m[0][1]) && core::iszero(m[0][2]) &&
				core::iszero(m[1][0]) && core::iszero(m[1][2]) &&
				core::iszero(m[2][0]) && core::iszero(m[2][1]))
				return vector3d<T>(m[0][0], m[1][1], m[2][2]);

			// We have to do the full calculation.
			return vector3d<T>(sqrtf(m[0][0] * m[0][0] + m[0][1] * m[0][1] + m[0][2] * m[0][2]),
				sqrtf(m[1][0] * m[1][0] + m[1][1] * m[1][1] + m[1][2] * m[1][2]),
				sqrtf(m[2][0] * m[2][0] + m[2][1] * m[2][1] + m[2][2] * m[2][2]));
			*/

			// See http://www.robertblum.com/articles/2005/02/14/decomposing-matrices
			// Deal with the 0 rotation case first
			// Prior to Irrlicht 1.6, we always returned this value.
			if(core::iszero(M[1]) && core::iszero(M[2]) &&
				core::iszero(M[4]) && core::iszero(M[6]) &&
				core::iszero(M[8]) && core::iszero(M[9]))
				return vector3d<T>(M[0], M[5], M[10]);

			// We have to do the full calculation.
			return vector3d<T>(sqrtf(M[0] * M[0] + M[1] * M[1] + M[2] * M[2]),
				sqrtf(M[4] * M[4] + M[5] * M[5] + M[6] * M[6]),
				sqrtf(M[8] * M[8] + M[9] * M[9] + M[10] * M[10]));
		}


		inline matrix4<T> operator*(const matrix4<T>& other) const{
			 matrix4<T> r;
			 r.setByProduct(*this,other);
			 return r;
		}
		inline matrix4<T>& operator=(const matrix4<T>& other){
			if(this==&other)
				return *this;
			memcpy(M,other.M,sizeof(M));
			m_bIsChanged=other.m_bIsChanged;
			m_bIsIdentity=other.m_bIsIdentity;
			return *this;
		}
		inline matrix4<T>& operator*=(const matrix4<T>& other){
			matrix4 temp(*this);
			setByProduct(temp,other);
			m_bIsChanged=true;
			return *this;
		}
		inline bool operator==(const matrix4<T> &other) const
		{
			if(isIdentity()&&other.isIdentity())
				return true;
			//for (u32 i = 0; i < 4; ++i)
			//	for (u32 j = 0; j < 4; ++j)
			//		if (core::equals(m[i][j],other.m[i][j])==false)
			//			return false;
			for(u32 i=0;i<16;++i)
				if(core::equals(M[i],other.M[i])==false)
					return false;
			return true;
		}
		const T& operator[](u32 index) const { return M[index]; }

		T& operator[](u32 index) { 
			m_bIsChanged=true;
			m_bIsIdentity=false;
			return M[index]; 
		}

		inline bool operator!=(const matrix4<T> &other) const{return !(*this==other);}
		/*const T& operator[](u32 index) const{
			u32 quotient=index>>2;
			u32 remainder=index-quotient;
			return m[quotient][remainder];
		}*/
		inline void rotateVector( vector3df& vect ) const{
			vector3df tmp = vect;
			vect.x = tmp.x*M[0] + tmp.y*M[4] + tmp.z*M[8];
			vect.y = tmp.x*M[1] + tmp.y*M[5] + tmp.z*M[9];
			vect.z = tmp.x*M[2] + tmp.y*M[6] + tmp.z*M[10];
			//TODO 与ms3d_demo.InverseRotateVec不一致，使用ms3d_demo (本方法还未有用到)
			/*vector3df tmp = vect;
			vect.x = tmp.x*M[0] + tmp.y*M[1] + tmp.z*M[2];
			vect.y = tmp.x*M[4] + tmp.y*M[5] + tmp.z*M[6];
			vect.z = tmp.x*M[8] + tmp.y*M[9] + tmp.z*M[10];*/
		}

		inline matrix4<T>& setRotationDegrees( const vector3d<T>& rotation )
		{
			//if(core::equals(rotation.x,0)&&core::equals(rotation.y,0)&&core::equals(rotation.z,0))
			//	return *this;
			return setRotationRadians( rotation * core::DEGTORAD );
		}

		inline matrix4<T>& setRotationRadians( const vector3d<T>& rotation )
		{
			if(core::equals(rotation.x,0)&&core::equals(rotation.y,0)&&core::equals(rotation.z,0))
				return *this;
			const f64 cr = cos( rotation.x );
			const f64 sr = sin( rotation.x );
			const f64 cp = cos( rotation.y );
			const f64 sp = sin( rotation.y );
			const f64 cy = cos( rotation.z );
			const f64 sy = sin( rotation.z );

			//zyx
			M[0] = (T)( cp*cy );
			M[1] = (T)( cp*sy );
			M[2] = (T)( -sp );

			const f64 srsp = sr*sp;
			const f64 crsp = cr*sp;

			M[4] = (T)( srsp*cy-cr*sy );
			M[5] = (T)( srsp*sy+cr*cy );
			M[6] = (T)( sr*cp );

			M[8] = (T)( crsp*cy+sr*sy );
			M[9] = (T)( crsp*sy-sr*cy );
			M[10] = (T)( cr*cp );
			//yxz
			//cp*cy+sp*sr*sy,	cp*-sy+sp*sr*cy,	sp*cr
			//cr*sy,			cr*cy,				-sr
			//-sp*cy+cp*sr*sy,	-sp*-sy+cp*sr*cy,	cp*cr
			/*const f64 srsp = sr*sp;
			const f64 srcp = sr*cp;

			M[0] = (T)( cp*cy+srsp*sy );
			M[1] = (T)( cr*sy );
			M[2] = (T)( -sp*cy+srcp*sy );

			M[4] = (T)( srsp*cy-cp*sy );
			M[5] = (T)( cr*cy );
			M[6] = (T)( srcp*cy+sy*sp );

			M[8] = (T)( cr*sp );
			M[9] = (T)( -sr );
			M[10] = (T)( cr*cp );*/

			m_bIsChanged=true;


			return *this;
		}

		//! Returns a rotation that is equivalent to that set by setRotationDegrees().
		/**  Note that it does not necessarily return the *same* Euler angles as those set by setRotationDegrees(),
		but the rotation will be equivalent, i.e. will have the same result when used to rotate a vector or node. */
		inline vector3d<T> getRotationDegrees() const
		{
			//yxz
			/*
			const core::vector3d<T> scale = getScale();
			const core::vector3d<f64> invScale(core::reciprocal(scale.x),core::reciprocal(scale.y),core::reciprocal(scale.z));

			//fix bug:1.#QNAN of X exceed 1/-1
			f64 temp=core::clamp(M[9]*invScale.z,-1.0,1.0);
			f64 X = -asin(temp);
			const f64 C = cos(X);
			X *= RADTODEG64;

			f64 a, b, Y, Z;
			if (!core::iszero(C))
			{
				const f64 invC = core::reciprocal(C);
				b = M[10] * invC * invScale.z;
				a = M[8] * invC * invScale.z;
				Y = atan2( a, b ) * RADTODEG64;
				a = M[1] * invC * invScale.x;
				b = M[5] * invC * invScale.y;
				Z = atan2( a, b ) * RADTODEG64;
			}
			else
			{
				Y = 0.0;
				a = -M[4] * invScale.y;
				b = M[0] * invScale.x;
				Z = atan2( a, b ) * RADTODEG64;
			}

			// fix values that get below zero
			// before it would set (!) values to 360
			// that were above 360:
			if (X < 0.0) X += 360.0;
			if (Y < 0.0) Y += 360.0;
			if (Z < 0.0) Z += 360.0;

			return vector3d<T>((T)X,(T)Y,(T)Z);
			*/
			//zyx
			const core::vector3d<T> scale = getScale();
			const core::vector3d<f64> invScale(core::reciprocal(scale.x),core::reciprocal(scale.y),core::reciprocal(scale.z));

			//fix bug:1.#QNAN of X exceed 1/-1
			f64 temp=core::clamp(M[2]*invScale.x,-1.0,1.0);
			f64 Y = -asin(temp);
			const f64 C = cos(Y);
			Y *= RADTODEG64;

			f64 rotx, roty, X, Z;

			if (!core::iszero(C))
			{
				const f64 invC = core::reciprocal(C);
				rotx = M[10] * invC * invScale.z;
				roty = M[6] * invC * invScale.y;
				X = atan2( roty, rotx ) * RADTODEG64;
				rotx = M[0] * invC * invScale.x;
				roty = M[1] * invC * invScale.x;
				Z = atan2( roty, rotx ) * RADTODEG64;
			}
			else
			{
				X = 0.0;
				rotx = M[5] * invScale.y;
				roty = -M[4] * invScale.y;
				Z = atan2( roty, rotx ) * RADTODEG64;
				//Z = 0.0;
				//rotx = M[8] * invScale.z;
				//roty = M[4] * invScale.y;
				//X = atan2(roty , rotx) * RADTODEG64;
			}

			// fix values that get below zero
			// before it would set (!) values to 360
			// that were above 360:
			if (X < 0.0) X += 360.0;
			if (Y < 0.0) Y += 360.0;
			if (Z < 0.0) Z += 360.0;

			return vector3d<T>((T)X,(T)Y,(T)Z);
		}

		/*inline void upsidedown(){
			//m[1][0]=-m[1][0];
			m[1][1]=-m[1][1];
			//m[1][2]=-m[1][2];
			//m[1][3]=-m[1][3];
		}*/

		inline void frustum(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ){
			f32	deltaX = right - left;
			f32	deltaY = top - bottom;
			f32	deltaZ = farZ - nearZ;
			matrix4	frust;

			if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
				(deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
				return;

			/*frust.m[0][0] = (T)2.0f * nearZ / deltaX;
			frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = (T)0.0f;

			frust.m[1][1] = (T)2.0f * nearZ / deltaY;
			frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = (T)0.0f;

			frust.m[2][0] = (T)(right + left) / deltaX;
			frust.m[2][1] = (T)(top + bottom) / deltaY;
			frust.m[2][2] = (T)-(nearZ + farZ) / deltaZ;
			frust.m[2][3] = (T)-1.0f;

			frust.m[3][2] = (T)-2.0f * nearZ * farZ / deltaZ;
			frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = (T)0.0f;*/
			frust.M[0] = (T)2.0f * nearZ / deltaX;
			frust.M[1] = frust.M[2] = frust.M[3] = (T)0.0f;

			frust.M[5] = (T)2.0f * nearZ / deltaY;
			frust.M[4] = frust.M[6] = frust.M[7] = (T)0.0f;

			frust.M[8] = (T)(right + left) / deltaX;
			frust.M[9] = (T)(top + bottom) / deltaY;
			frust.M[10] = (T)-(nearZ + farZ) / deltaZ;
			frust.M[11] = (T)-1.0f;

			frust.M[14] = (T)-2.0f * nearZ * farZ / deltaZ;
			frust.M[12] = frust.M[13] = frust.M[15] = (T)0.0f;

			*this*=frust;

			m_bIsChanged=true;
		}
		inline void ortho(f32 left, f32 right, float bottom, float top, float nearZ, float farZ)
		{
			f32 deltaX = right - left;
			f32 deltaY = top - bottom;
			f32 deltaZ = farZ - nearZ;
			matrix4  ort;

			if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
				(deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
				return;

			/*ort.m[0][0] = (T)2.0f / deltaX;
			ort.m[0][1] = ort.m[0][2] = ort.m[0][3] = (T)0.0f;
			ort.m[1][1] = (T)2.0f / deltaY;
			ort.m[1][0] = ort.m[1][2] = ort.m[1][3] = (T)0.0f;
			ort.m[2][2] = (T)-2.0f / deltaZ;
			ort.m[2][0] = ort.m[2][1] = ort.m[2][3] = (T)0.0f;
			ort.m[3][0] = (T)-(right + left) / deltaX;
			ort.m[3][1] = (T)-(top + bottom) / deltaY;
			ort.m[3][2] = (T)-(nearZ + farZ) / deltaZ;
			ort.m[3][3] = (T)1.0f;*/
			ort.M[0] = (T)2.0f / deltaX;
			ort.M[1] = ort.M[2] = ort.M[3] = (T)0.0f;
			ort.M[5] = (T)2.0f / deltaY;
			ort.M[4] = ort.M[6] = ort.M[7] = (T)0.0f;
			ort.M[10] = (T)-2.0f / deltaZ;
			ort.M[8] = ort.M[9] = ort.M[11] = (T)0.0f;
			ort.M[12] = (T)-(right + left) / deltaX;
			ort.M[13] = (T)-(top + bottom) / deltaY;
			ort.M[14] = (T)-(nearZ + farZ) / deltaZ;
			ort.M[15] = (T)1.0f;

			*this*=ort;

			m_bIsChanged=true;
		}
		inline void perspective(f32 fovy, f32 aspect, f32 nearZ, f32 farZ)
		{
			f32 frustumW, frustumH;

			frustumH = tanf(fovy/2) * nearZ;
			frustumW = frustumH * aspect;

			frustum(-frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
		}
		inline void lookAt(f32 eyex, f32 eyey, f32 eyez,f32 centerx, f32 centery, f32 centerz,f32 upx, f32 upy, f32 upz)
		{
			f32 x[3], y[3], z[3];
			f32 mag;

			// Z vector
			z[0] = eyex - centerx;
			z[1] = eyey - centery;
			z[2] = eyez - centerz;
			mag = (f32)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
			if (mag) {
				z[0] /= mag;
				z[1] /= mag;
				z[2] /= mag;
			}

			// Y vector
			y[0] = upx;
			y[1] = upy;
			y[2] = upz;

			// X vector = Y cross Z 
			x[0] = y[1] * z[2] - y[2] * z[1];
			x[1] = -y[0] * z[2] + y[2] * z[0];
			x[2] = y[0] * z[1] - y[1] * z[0];

			// Recompute Y = Z cross X 
			//似乎多余，但可能是为了防止用户提供的数据有误时处理
			y[0] = z[1] * x[2] - z[2] * x[1];
			y[1] = -z[0] * x[2] + z[2] * x[0];
			y[2] = z[0] * x[1] - z[1] * x[0];

			// cross product gives area of parallelogram, which is < 1.0 for
			// non-perpendicular unit-length vectors; so normalize x, y here

			mag = (f32)sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
			if (mag) {
				x[0] /= mag;
				x[1] /= mag;
				x[2] /= mag;
			}

			mag = (f32)sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
			if (mag) {
				y[0] /= mag;
				y[1] /= mag;
				y[2] /= mag;
			}

			matrix4 mat;

			/*mat.m[0][0]=(T)x[0];
			mat.m[1][0]=(T)x[1];
			mat.m[2][0]=(T)x[2];
			mat.m[3][0]=(T)0;

			mat.m[0][1]=(T)y[0];
			mat.m[1][1]=(T)y[1];
			mat.m[2][1]=(T)y[2];
			mat.m[3][1]=(T)0;

			mat.m[0][2]=(T)z[0];
			mat.m[1][2]=(T)z[1];
			mat.m[2][2]=(T)z[2];
			mat.m[3][2]=(T)0;

			mat.m[0][3]=(T)0.0f;
			mat.m[1][3]=(T)0.0f;
			mat.m[2][3]=(T)0.0f;
			mat.m[3][3]=(T)1.0f;*/
			mat.M[0]=(T)x[0];
			mat.M[4]=(T)x[1];
			mat.M[8]=(T)x[2];
			mat.M[12]=(T)0;

			mat.M[1]=(T)y[0];
			mat.M[5]=(T)y[1];
			mat.M[9]=(T)y[2];
			mat.M[13]=(T)0;

			mat.M[2]=(T)z[0];
			mat.M[6]=(T)z[1];
			mat.M[10]=(T)z[2];
			mat.M[14]=(T)0;

			mat.M[3]=(T)0.0f;
			mat.M[7]=(T)0.0f;
			mat.M[11]=(T)0.0f;
			mat.M[15]=(T)1.0f;

			*this*=mat;
			translate(-eyex,-eyey,-eyez);

			m_bIsChanged=true;
		}

		inline void transformVect( vector3df& vect) const
		{
			/*f32 vector[3];

			vector[0] = vect.x*m[0][0] + vect.y*m[1][0] + vect.z*m[2][0] + m[3][0];
			vector[1] = vect.x*m[0][1] + vect.y*m[1][1] + vect.z*m[2][1] + m[3][1];
			vector[2] = vect.x*m[0][2] + vect.y*m[1][2] + vect.z*m[2][2] + m[3][2];

			vect.x = vector[0];
			vect.y = vector[1];
			vect.z = vector[2];*/

			f32 vector[3];

			vector[0] = vect.x*M[0] + vect.y*M[4] + vect.z*M[8] + M[12];
			vector[1] = vect.x*M[1] + vect.y*M[5] + vect.z*M[9] + M[13];
			vector[2] = vect.x*M[2] + vect.y*M[6] + vect.z*M[10] + M[14];

			vect.x = vector[0];
			vect.y = vector[1];
			vect.z = vector[2];
		}

		inline void transformVect( vector3df& out, const vector3df& in) const
		{
			//out.x = in.x*m[0][0] + in.y*m[1][0] + in.z*m[2][0] + m[3][0];
			//out.y = in.x*m[0][1] + in.y*m[1][1] + in.z*m[2][1] + m[3][1];
			//out.z = in.x*m[0][2] + in.y*m[1][2] + in.z*m[2][2] + m[3][2];

			out.x = in.x*M[0] + in.y*M[4] + in.z*M[8] + M[12];
			out.y = in.x*M[1] + in.y*M[5] + in.z*M[9] + M[13];
			out.z = in.x*M[2] + in.y*M[6] + in.z*M[10] + M[14];
		}
	};

	typedef matrix4<f32> matrix4f;

	YON_API extern const matrix4f IDENTITY_MATRIX;
	YON_API extern const matrix4f SWAP_MATRIX;
}
}
#endif