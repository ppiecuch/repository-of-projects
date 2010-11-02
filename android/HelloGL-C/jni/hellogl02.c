#include <hellogl.h>
#include <math.h>
#include <GLES/gl.h>
#include <android/log.h>

#define PI 3.1415926535897932f
#define ONE 1

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;
static float angle=0;

static GLfloat vertexArray[] = {
// Front face
-1.0, -1.0,  1.0,
 1.0, -1.0,  1.0,
 1.0,  1.0,  1.0,
-1.0,  1.0,  1.0,

// Back face
-1.0, -1.0, -1.0,
-1.0,  1.0, -1.0,
 1.0,  1.0, -1.0,
 1.0, -1.0, -1.0,

// Top face
-1.0,  1.0, -1.0,
-1.0,  1.0,  1.0,
 1.0,  1.0,  1.0,
 1.0,  1.0, -1.0,

// Bottom face
-1.0, -1.0, -1.0,
 1.0, -1.0, -1.0,
 1.0, -1.0,  1.0,
-1.0, -1.0,  1.0,

// Right face
 1.0, -1.0, -1.0,
 1.0,  1.0, -1.0,
 1.0,  1.0,  1.0,
 1.0, -1.0,  1.0,

// Left face
-1.0, -1.0, -1.0,
-1.0, -1.0,  1.0,
-1.0,  1.0,  1.0,
-1.0,  1.0, -1.0
};

static GLfixed colorArray[] = {
  1.0, 0.0, 0.0, 1.0,     // Front face
  1.0, 1.0, 0.0, 1.0,     // Back face
  0.0, 1.0, 0.0, 1.0,     // Top face
  1.0, 0.5, 0.5, 1.0,     // Bottom face
  1.0, 0.0, 1.0, 1.0,     // Right face
  0.0, 0.0, 1.0, 1.0     // Left face
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
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,width/height,0,500);
	gluLookAt(0,0,100,0,0,-20,0,1,0);
	glMatrixMode(GL_MODELVIEW);
}

void appRender(long tick, int width, int height)
{
	/*glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(angle,0,1,0);
	angle+=0.2f;
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,vertexArray);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4,GL_FIXED,0,colorArray);
	glDrawArrays(GL_TRIANGLES,0,3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);*/
	//旋转的角度
	 static int rotation = 0;

	 //正方体六个面对应顶点的数组索引
	 static GLubyte front[]  = {2,1,3,0}; //front face
	 static GLubyte back[]   = {5,6,4,7}; //back face
	 static GLubyte top[]    = {6,2,7,3}; //top face
	 static GLubyte bottom[] = {1,5,0,4}; //bottom face
	 static GLubyte left[]   = {3,0,7,4}; //left face
	 static GLubyte right[]  = {6,5,2,1}; //right face

	 //正方体顶点坐标数组
	 static GLshort vertices[] = {-5,-5,-5,  5,-5,-5,  5,5,-5, -5,5,-5,
	          -5,-5,5,   5,-5,5,   5,5,5,  -5,5,5};

	 //清除缓冲区
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 //重置当前的模型观察矩阵
	 glLoadIdentity();

	 //旋转正方体
	 glRotatex(angle, ONE, ONE, 0);
	 angle+=0.2f;

	 //启用顶点数组
	 glEnableClientState(GL_VERTEX_ARRAY);
	 glVertexPointer(3, GL_SHORT, 0, vertices);

	 //给六个面配置颜色并绘制
	 glColor4x(ONE,0,0,0);
	 glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, front);

	 glColor4x(0,ONE,0,0);
	 glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, back);

	 glColor4x(0,0,ONE,0);
	 glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, top);

	 glColor4x(ONE,ONE,0,0);
	 glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, bottom);

	 glColor4x(0,ONE,ONE,0);
	 glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, left);

	 glColor4x(ONE,0,ONE,0);
	 glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, right);

	 //关闭顶点数组
	 glDisableClientState(GL_VERTEX_ARRAY);
}


/* Call to initialize the graphics state */
void
Java_org_tinder_studio_gl_HelloGLView_nativeInit( JNIEnv*  env, jobject thiz )
{
    appInit();
}


/* Call to finalize the graphics state */
void
Java_org_tinder_studio_gl_HelloGLView_nativeDone( JNIEnv*  env, jobject thiz )
{
}

/* Call to render the next GL frame */
void
Java_org_tinder_studio_gl_HelloGLView_nativeRender( JNIEnv*  env, jobject thiz )
{
    appRender(0, sWindowWidth, sWindowHeight);
}

void
Java_org_tinder_studio_gl_HelloGLView_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    appResize(w,h);
    __android_log_print(ANDROID_LOG_INFO, "HelloGL", "resize w=%d h=%d", w, h);
}
