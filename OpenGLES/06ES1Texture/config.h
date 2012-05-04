#ifndef _CONFIG_H_
#define _CONFIG_H_

//加载OpenGL ES1需要的库及头文件
#if 1
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv1_CM.lib")
#include <EGL/egl.h>
#include <GLES/gl.h>
#else
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"Opengl32.lib")
#pragma comment(lib,"glut32.lib")
#include <EGL/egl.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <math.h>
#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

#include "PngFile.h"
#include "Texture.h"

#define GL_CHECK \
		{ \
			GLenum glError = glGetError(); \
			if(glError != GL_NO_ERROR) { \
				printf("glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__); \
			}else{ \
				printf("gl process ok at line %i\n",  __LINE__); \
			} \
		}

//如果没有引入<glu.h>
#ifndef __glu_h__
inline void gluPerspective(double fovy, double aspect, double zNear, double zFar)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double xmin, xmax, ymin, ymax;
	ymax = zNear * tan(fovy * PI / 360);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
	glFrustumf((GLfloat)xmin, (GLfloat)xmax, (GLfloat)ymin, (GLfloat)ymax, (GLfloat)zNear, (GLfloat)zFar);
}

/* Following gluLookAt implementation is adapted from the
* Mesa 3D Graphics library. http://www.mesa3d.org
*/
inline void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,GLfloat centerx, GLfloat centery, GLfloat centerz,GLfloat upx, GLfloat upy, GLfloat upz)
{
	GLfloat m[16];
	GLfloat x[3], y[3], z[3];
	GLfloat mag;

	/* Make rotation matrix */

	/* Z vector */
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;
	mag = (float)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
	if (mag) {			/* mpichler, 19950515 */
		z[0] /= mag;
		z[1] /= mag;
		z[2] /= mag;
	}

	/* Y vector */
	y[0] = upx;
	y[1] = upy;
	y[2] = upz;

	/* X vector = Y cross Z */
	x[0] = y[1] * z[2] - y[2] * z[1];
	x[1] = -y[0] * z[2] + y[2] * z[0];
	x[2] = y[0] * z[1] - y[1] * z[0];

	/* Recompute Y = Z cross X */
	//似乎多余，但可能是为了防止用户提供的数据有误时处理
	y[0] = z[1] * x[2] - z[2] * x[1];
	y[1] = -z[0] * x[2] + z[2] * x[0];
	y[2] = z[0] * x[1] - z[1] * x[0];

	/* mpichler, 19950515 */
	/* cross product gives area of parallelogram, which is < 1.0 for
	* non-perpendicular unit-length vectors; so normalize x, y here
	*/

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

	GLfloat mat[16];
	mat[0]=x[0];
	mat[4]=x[1];
	mat[8]=x[2];
	mat[12]=-eyex;

	mat[1]=y[0];
	mat[5]=y[1];
	mat[9]=y[2];
	mat[13]=-eyey;

	mat[2]=z[0];
	mat[6]=z[1];
	mat[10]=z[2];
	mat[14]=-eyez;

	mat[3]=0.0;
	mat[7]=0.0;
	mat[11]=0.0;
	mat[15]=1.0;
	glMultMatrixf(mat);

// #define M(row,col)  m[col*4+row]
// 	M(0, 0) = x[0];
// 	M(0, 1) = x[1];
// 	M(0, 2) = x[2];
// 	M(0, 3) = -eyex;
// 	M(1, 0) = y[0];
// 	M(1, 1) = y[1];
// 	M(1, 2) = y[2];
// 	M(1, 3) = -eyey;
// 	M(2, 0) = z[0];
// 	M(2, 1) = z[1];
// 	M(2, 2) = z[2];
// 	M(2, 3) = -eyez;
// 	M(3, 0) = 0.0;
// 	M(3, 1) = 0.0;
// 	M(3, 2) = 0.0;
// 	M(3, 3) = 1.0;
// #undef M
// 	if(false)
// 	{
// 		int a;
// 		GLfixed fixedM[16];
// 		for (a = 0; a < 16; ++a)
// 			fixedM[a] = (GLfixed)(m[a] * 65536);
// 		glMultMatrixx(fixedM);
// 	}
// 	else
// 	{
// 		GLfloat mat[16];
// 		for(int i=0;i<16;++i)
// 			mat[i]=m[i];
// 		glMultMatrixf(mat);
// 	}
}
#endif

#endif
