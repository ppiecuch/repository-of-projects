#ifndef _CONFIG_H_
#define _CONFIG_H_

//加载OpenGL ES2需要的库及头文件
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <crtdbg.h>
#include <typeinfo>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include <stdlib.h>
#define GL_CHECK \
		{ \
		GLenum glError = glGetError(); \
		if(glError != GL_NO_ERROR) { \
		printf("glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__); \
		}else{ \
		printf("gl process ok at line %i\n",  __LINE__); \
		} \
		}

//单位矩阵
float pfIdentity[]={
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f
};


//
/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores 
///        the results in the arrays.  Generate index list for a TRIANGLES
/// \param scale The size of the cube, use 1.0 for a unit cube.
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
int genCube( float scale, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices )
{
	int i;
	int numVertices = 24;
	int numIndices = 36;

	GLfloat cubeVerts[] =
	{
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
	};

	GLfloat cubeNormals[] =
	{
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	GLfloat cubeTex[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	// Allocate memory for buffers
	if ( vertices != NULL )
	{
		*vertices = (GLfloat*)malloc(sizeof(GLfloat)*3*numVertices );
		memcpy( *vertices, cubeVerts, sizeof( cubeVerts ) );
		for ( i = 0; i < numVertices * 3; i++ )
		{
			(*vertices)[i] *= scale;
		}
	}

	if ( normals != NULL )
	{
		*normals = (GLfloat*)malloc ( sizeof(GLfloat) * 3 * numVertices );
		memcpy( *normals, cubeNormals, sizeof( cubeNormals ) );
	}

	if ( texCoords != NULL )
	{
		*texCoords = (GLfloat*)malloc ( sizeof(GLfloat) * 2 * numVertices );
		memcpy( *texCoords, cubeTex, sizeof( cubeTex ) ) ;
	}


	// Generate the indices
	if ( indices != NULL )
	{
		GLushort cubeIndices[] =
		{
			0, 2, 1,
			0, 3, 2, 
			4, 5, 6,
			4, 6, 7,
			8, 9, 10,
			8, 10, 11, 
			12, 15, 14,
			12, 14, 13, 
			16, 17, 18,
			16, 18, 19, 
			20, 23, 22,
			20, 22, 21
		};

		*indices = (GLushort*)malloc ( sizeof(GLushort) * numIndices );
		memcpy( *indices, cubeIndices, sizeof( cubeIndices ) );
	}

	return numIndices;
}



typedef struct
{
	GLfloat   m[4][4];
} ESMatrix;

#include <math.h>
#define PI 3.1415926535897932384626433832795f

//矩阵左乘，行*列=行列(列优先)
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

void esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ)
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


void esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ)
{
	GLfloat frustumW, frustumH;

	frustumH = tanf( fovy / 360.0f * PI ) * nearZ;
	frustumW = frustumH * aspect;

	esFrustum( result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
}

void esMatrixLoadIdentity(ESMatrix *result)
{
	memset(result, 0x0, sizeof(ESMatrix));
	result->m[0][0] = 1.0f;
	result->m[1][1] = 1.0f;
	result->m[2][2] = 1.0f;
	result->m[3][3] = 1.0f;
}

//实际是使用一个偏移矩阵对result矩阵进行变换，OpenGL使用矩阵左乘，即M[偏移]*M[result]（行*列=行列）
//| 1 0 0 x |  | 0 0 0 0 |
//| 0 1 0 y |\/| 0 0 0 0 |
//| 0 0 1 z |/\| 0 0 0 0 |
//| 0 0 0 1 |  | 0 0 0 0 |
void esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz)
{
	result->m[3][0] += (result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
	result->m[3][1] += (result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
	result->m[3][2] += (result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
	result->m[3][3] += (result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

void esScale(ESMatrix* result, GLfloat tx, GLfloat ty, GLfloat tz)
{
	ESMatrix scaleMat;
	memset(&scaleMat, 0x0, sizeof(ESMatrix));
	scaleMat.m[0][0] = tx;
	scaleMat.m[1][1] = ty;
	scaleMat.m[2][2] = tz;
	scaleMat.m[3][3] = 1.0f;
	esMatrixMultiply(result,&scaleMat,result);
}

void esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
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
