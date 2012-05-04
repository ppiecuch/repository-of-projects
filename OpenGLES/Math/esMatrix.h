#ifndef _ES_MATRIX_H_
#define _ES_MATRIX_H_

#include <stdio.h>
typedef struct
{
	GLfloat   m[4][4];

	void print(){
		for(int i=0;i<4;++i)
			printf("%.2f\t%.2f\t%.2f\t%.2f\n",m[0][i],m[1][i],m[2][i],m[3][i]);
		printf("\n");
	}

} ESMatrix;


#include <math.h>
#define PI 3.1415926535897932384626433832795f

inline void esMatrixLoadIdentity(ESMatrix *result)
{
	memset(result, 0x0, sizeof(ESMatrix));
	result->m[0][0] = 1.0f;
	result->m[1][1] = 1.0f;
	result->m[2][2] = 1.0f;
	result->m[3][3] = 1.0f;
}

inline void esMatrixMultiply(ESMatrix *result, ESMatrix *srcB, ESMatrix *srcA)
{
	ESMatrix    tmp;
	int         i;
	for (i=0; i<4; i++)
	{
		tmp.m[i][0] =	(srcB->m[i][0] * srcA->m[0][0]) +
			(srcB->m[i][1] * srcA->m[1][0]) +
			(srcB->m[i][2] * srcA->m[2][0]) +
			(srcB->m[i][3] * srcA->m[3][0]) ;

		tmp.m[i][1] =	(srcB->m[i][0] * srcA->m[0][1]) + 
			(srcB->m[i][1] * srcA->m[1][1]) +
			(srcB->m[i][2] * srcA->m[2][1]) +
			(srcB->m[i][3] * srcA->m[3][1]) ;

		tmp.m[i][2] =	(srcB->m[i][0] * srcA->m[0][2]) + 
			(srcB->m[i][1] * srcA->m[1][2]) +
			(srcB->m[i][2] * srcA->m[2][2]) +
			(srcB->m[i][3] * srcA->m[3][2]) ;

		tmp.m[i][3] =	(srcB->m[i][0] * srcA->m[0][3]) + 
			(srcB->m[i][1] * srcA->m[1][3]) +
			(srcB->m[i][2] * srcA->m[2][3]) +
			(srcB->m[i][3] * srcA->m[3][3]) ;
	}
	memcpy(result, &tmp, sizeof(ESMatrix));
}

inline void esMatrixTranspose(ESMatrix *matIn,ESMatrix* result)
{
	if(matIn!=result)
	{
		for(int i=0;i<4;++i)
		{
			for(int j=0;j<4;++j)
			{
				result->m[i][j]=matIn->m[j][i];
			}
		}
	}
	else
	{
		ESMatrix temp;
		for(int i=0;i<4;++i)
		{
			for(int j=0;j<4;++j)
			{
				temp.m[i][j]=matIn->m[j][i];
			}
		}
		for(int i=0;i<4;++i)
		{
			for(int j=0;j<4;++j)
			{
				result->m[i][j]=temp.m[i][j];
			}
		}
	}
}

inline void esMatrixInverse(ESMatrix *matIn,ESMatrix* result)
{	
	// OpenGL matrices are column major and can be quite confusing to access 
	// when stored in the typical, one-dimensional array often used by the API.
	// Here are some shorthand conversion macros, which convert a row/column 
	// combination into an array index.

	#define MAT(matIn,r,c) matIn->m[c][r]

	#define m11 MAT(matIn,0,0)
	#define m12 MAT(matIn,0,1)
	#define m13 MAT(matIn,0,2)
	#define m14 MAT(matIn,0,3)
	#define m21 MAT(matIn,1,0)
	#define m22 MAT(matIn,1,1)
	#define m23 MAT(matIn,1,2)
	#define m24 MAT(matIn,1,3)
	#define m31 MAT(matIn,2,0)
	#define m32 MAT(matIn,2,1)
	#define m33 MAT(matIn,2,2)
	#define m34 MAT(matIn,2,3)
	#define m41 MAT(matIn,3,0)
	#define m42 MAT(matIn,3,1)
	#define m43 MAT(matIn,3,2)
	#define m44 MAT(matIn,3,3)

	// Inverse = adjoint / det. (See linear algebra texts.)

	// pre-compute 2x2 dets for last two rows when computing
	// cofactors of first two rows.
	float d12 = (m31 * m42 - m41 * m32);
	float d13 = (m31 * m43 - m41 * m33);
	float d23 = (m32 * m43 - m42 * m33);
	float d24 = (m32 * m44 - m42 * m34);
	float d34 = (m33 * m44 - m43 * m34);
	float d41 = (m34 * m41 - m44 * m31);

	float tmp[16];

	tmp[0] =  (m22 * d34 - m23 * d24 + m24 * d23);
	tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
	tmp[2] =  (m21 * d24 + m22 * d41 + m24 * d12);
	tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

	// Compute determinant as early as possible using these cofactors.
	float det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

	// Run singularity test.
	if( det == 0.0 )
	{
		printf("Warning: Call to invertMatrix produced a Singular matrix.\n");

		float identity[16] = 
		{
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		};

		memcpy( result->m, identity, 16*sizeof(float) );
	}
	else
	{
		float invDet = 1.0f / det;

		// Compute rest of inverse.
		tmp[0] *= invDet;
		tmp[1] *= invDet;
		tmp[2] *= invDet;
		tmp[3] *= invDet;

		tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
		tmp[5] =  (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
		tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
		tmp[7] =  (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

		// Pre-compute 2x2 dets for first two rows when computing cofactors 
		// of last two rows.
		d12 = m11 * m22 - m21 * m12;
		d13 = m11 * m23 - m21 * m13;
		d23 = m12 * m23 - m22 * m13;
		d24 = m12 * m24 - m22 * m14;
		d34 = m13 * m24 - m23 * m14;
		d41 = m14 * m21 - m24 * m11;

		tmp[8]  =  (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
		tmp[9]  = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
		tmp[10] =  (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
		tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
		tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
		tmp[13] =  (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
		tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
		tmp[15] =  (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

		memcpy( result->m, tmp, 16*sizeof(float) );
	}

	#undef m11
	#undef m12
	#undef m13
	#undef m14
	#undef m21
	#undef m22
	#undef m23
	#undef m24
	#undef m31
	#undef m32
	#undef m33
	#undef m34
	#undef m41
	#undef m42
	#undef m43
	#undef m44
	#undef MAT
}

inline void esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
	float       deltaX = right - left;
	float       deltaY = top - bottom;
	float       deltaZ = farZ - nearZ;
	ESMatrix    frust;

	if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
		(deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
		return;

	frust.m[0][0] = 2.0f * nearZ / deltaX;
	frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

	frust.m[1][1] = 2.0f * nearZ / deltaY;
	frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

	frust.m[2][0] = (right + left) / deltaX;
	frust.m[2][1] = (top + bottom) / deltaY;
	frust.m[2][2] = -(nearZ + farZ) / deltaZ;
	frust.m[2][3] = -1.0f;

	frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
	frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

	esMatrixMultiply(result, &frust, result);
}

inline void esOrtho(ESMatrix* result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
	float   deltaX = right - left;
	float   deltaY = top - bottom;
	float   deltaZ = farZ - nearZ;
	ESMatrix  ortho;

	if ( (nearZ <= 0.0f) || (farZ <= 0.0f) ||
		(deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f) )
		return;

	ortho.m[0][0] = 2.0f / deltaX;
	ortho.m[0][1] = ortho.m[0][2] = ortho.m[0][3] = 0.0f;
	ortho.m[1][1] = 2.0f / deltaY;
	ortho.m[1][0] = ortho.m[1][2] = ortho.m[1][3] = 0.0f;
	ortho.m[2][2] = -2.0f / deltaZ;
	ortho.m[2][0] = ortho.m[2][1] = ortho.m[2][3] = 0.0f;
	ortho.m[3][0] = -(right + left) / deltaX;
	ortho.m[3][1] = -(top + bottom) / deltaY;
	ortho.m[3][2] = -(nearZ + farZ) / deltaZ;
	ortho.m[3][3] = 1.0f;

	esMatrixMultiply(result, &ortho, result);
}


inline void esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ)
{
	GLfloat frustumW, frustumH;

	frustumH = tanf( fovy / 360.0f * PI ) * nearZ;
	frustumW = frustumH * aspect;

	esFrustum( result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
}

inline void esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz)
{
	result->m[3][0] += (result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
	result->m[3][1] += (result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
	result->m[3][2] += (result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
	result->m[3][3] += (result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

inline void esScale(ESMatrix* result, GLfloat tx, GLfloat ty, GLfloat tz)
{
	ESMatrix scaleMat;
	memset(&scaleMat, 0x0, sizeof(ESMatrix));
	scaleMat.m[0][0] = tx;
	scaleMat.m[1][1] = ty;
	scaleMat.m[2][2] = tz;
	scaleMat.m[3][3] = 1.0f;
	esMatrixMultiply(result,&scaleMat,result);
}

inline void esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat sinAngle, cosAngle;
	GLfloat mag = sqrtf(x * x + y * y + z * z);

	sinAngle = sinf ( angle * PI / 180.0f );
	cosAngle = cosf ( angle * PI / 180.0f );
	if ( mag > 0.0f )
	{
		GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
		GLfloat oneMinusCos;
		ESMatrix rotMat;

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

		rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
		rotMat.m[0][1] = (oneMinusCos * xy) - zs;
		rotMat.m[0][2] = (oneMinusCos * zx) + ys;
		rotMat.m[0][3] = 0.0F; 

		rotMat.m[1][0] = (oneMinusCos * xy) + zs;
		rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
		rotMat.m[1][2] = (oneMinusCos * yz) - xs;
		rotMat.m[1][3] = 0.0F;

		rotMat.m[2][0] = (oneMinusCos * zx) - ys;
		rotMat.m[2][1] = (oneMinusCos * yz) + xs;
		rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
		rotMat.m[2][3] = 0.0F; 

		rotMat.m[3][0] = 0.0F;
		rotMat.m[3][1] = 0.0F;
		rotMat.m[3][2] = 0.0F;
		rotMat.m[3][3] = 1.0F;

		esMatrixMultiply( result, &rotMat, result );
	}
}
inline void esLookAt(ESMatrix *result, GLfloat eyex, GLfloat eyey, GLfloat eyez,GLfloat centerx, GLfloat centery, GLfloat centerz,GLfloat upx, GLfloat upy, GLfloat upz)
{
	GLfloat x[3], y[3], z[3];
	GLfloat mag;

	// Z vector
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;
	mag = (float)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
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

	mag = (float)sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
	if (mag) {
		x[0] /= mag;
		x[1] /= mag;
		x[2] /= mag;
	}

	mag = (float)sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
	if (mag) {
		y[0] /= mag;
		y[1] /= mag;
		y[2] /= mag;
	}

	ESMatrix mat;

	mat.m[0][0]=x[0];
	mat.m[1][0]=x[1];
	mat.m[2][0]=x[2];
	mat.m[3][0]=0;

	mat.m[0][1]=y[0];
	mat.m[1][1]=y[1];
	mat.m[2][1]=y[2];
	mat.m[3][1]=0;

	mat.m[0][2]=z[0];
	mat.m[1][2]=z[1];
	mat.m[2][2]=z[2];
	mat.m[3][2]=0;

	mat.m[0][3]=0.0f;
	mat.m[1][3]=0.0f;
	mat.m[2][3]=0.0f;
	mat.m[3][3]=1.0f;

	esMatrixMultiply( result , &mat, result);
	esTranslate(result,-eyex,-eyey,-eyez);
}
#endif