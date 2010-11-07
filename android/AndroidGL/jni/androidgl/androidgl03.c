#include "androidgl.h"
#include <math.h>
#include <GLES/gl.h>
#include <android/log.h>

/***************************************/
/*           正方体(索引数组)           */
/***************************************/

#define PI 3.1415926535897932f
#define ONE 1
#define VERTEX_COUNT_PER_FACE 4
#define FACE_COUNT 6

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static float angle=0;

static GLfloat vertexArray[] = {
-1.0, -1.0,  1.0,//前左下0
 1.0, -1.0,  1.0,//前右下1
 1.0,  1.0,  1.0,//前右上2
-1.0,  1.0,  1.0,//前左上3
-1.0, -1.0, -1.0,//后左下4
-1.0,  1.0, -1.0,//后左上5
 1.0,  1.0, -1.0,//后右上6
 1.0, -1.0, -1.0 //后右下7
};

static GLubyte indiceArray[][VERTEX_COUNT_PER_FACE]={
{0,1,3,2},//Front
{4,5,7,6},//Back
{3,2,5,6},//Top
{1,0,7,4},//Bottom
{2,1,6,7},//Right
{0,3,4,5} //Left
};

static GLubyte colorArray[] = {
  255, 0, 0, 255,     // Front face
  255, 255, 0, 255,     // Back face
  0, 255, 0, 255,     // Top face
  255, 128, 128, 255,     // Bottom face
  255, 0, 255, 255,     // Right face
  0, 0, 255, 255     // Left face
};

static void gluPerspective(GLfloat fovy, GLfloat aspect,
                           GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;

    ymax = zNear * (GLfloat)tan(fovy * PI / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustumx((GLfixed)(xmin * 65536), (GLfixed)(xmax * 65536),
               (GLfixed)(ymin * 65536), (GLfixed)(ymax * 65536),
               (GLfixed)(zNear * 65536), (GLfixed)(zFar * 65536));
}

/* Following gluLookAt implementation is adapted from the
 * Mesa 3D Graphics library. http://www.mesa3d.org
 */
static void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
	              GLfloat centerx, GLfloat centery, GLfloat centerz,
	              GLfloat upx, GLfloat upy, GLfloat upz)
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

#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    {
        int a;
        GLfixed fixedM[16];
        for (a = 0; a < 16; ++a)
            fixedM[a] = (GLfixed)(m[a] * 65536);
        glMultMatrixx(fixedM);
    }

    /* Translate Eye to Origin */
    glTranslatex((GLfixed)(-eyex * 65536),
                 (GLfixed)(-eyey * 65536),
                 (GLfixed)(-eyez * 65536));
}

// Capped conversion from float to fixed.
static long floatToFixed(float value)
{
    if (value < -32768) value = -32768;
    if (value > 32767) value = 32767;
    return (long)(value * 65536);
}

void appInit()
{
	glClearColorx((GLfixed)(0.1f * 65536),(GLfixed)(0.2f * 65536),(GLfixed)(0.3f * 65536), 0x10000);
}

void appResize(int width,int height)
{
	/*视口变换*/
	glViewport(0, 0, width, height);
	/*投影变换*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/(float)height,0.1f,200);
	gluLookAt(0,0,10,0,0,-50,0,1,0);
	/*模型矩阵*/
	glMatrixMode(GL_MODELVIEW);
	/*启用剔除功能*/
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

void appRender(long tick, int width, int height)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(angle,1,1,0);
	angle+=0.5f;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,vertexArray);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4,GL_UNSIGNED_BYTE,0,colorArray);
	int i;
	for(i=0;i<FACE_COUNT;i++)
		glDrawElements(GL_TRIANGLE_STRIP,VERTEX_COUNT_PER_FACE,GL_UNSIGNED_BYTE,indiceArray[i]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}


/* Call to initialize the graphics state */
void
Java_org_tinder_studio_gl_AndroidGLView_nativeInit( JNIEnv*  env, jobject thiz, jstring apkPath )
{
    appInit();
}


/* Call to finalize the graphics state */
void
Java_org_tinder_studio_gl_AndroidGLView_nativeDone( JNIEnv*  env, jobject thiz )
{
}

/* Call to render the next GL frame */
void
Java_org_tinder_studio_gl_AndroidGLView_nativeRender( JNIEnv*  env, jobject thiz )
{
    appRender(0, sWindowWidth, sWindowHeight);
}

void
Java_org_tinder_studio_gl_AndroidGLView_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    appResize(w,h);
    __android_log_print(ANDROID_LOG_INFO, "AndroidGL", "resize w=%d h=%d", w, h);
}
void
Java_org_tinder_studio_gl_AndroidGLView_nativePause(JNIEnv *env, jobject thiz)
{

}
