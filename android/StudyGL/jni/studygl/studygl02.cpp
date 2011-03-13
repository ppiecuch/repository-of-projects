#include "studygl.h"
#include <GLES/gl.h>
#include <android/log.h>
#include "../core/point3d.h"
#include "../core/printer.h"

/**
1、实现等边三角形的绘制
**/

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;

GLshort vertexArray[9]={240,408,0,  120,200,0,  360,200,0};
GLubyte colorArray[12]={255,0,0,0,  0,255,0,0,  0,0,255,0};


/* Call to initialize the graphics state */
void
Java_org_tinder_studio_AndroidGLView_nativeInit( JNIEnv*  env, jobject thiz, jstring apkPath )
{
	Point3d p;
	Printer::print(p);
	glClearColorx((GLfixed)(0.1f * 65536),(GLfixed)(0.2f * 65536),(GLfixed)(0.3f * 65536), 0x10000);
}


/* Call to finalize the graphics state */
void
Java_org_tinder_studio_AndroidGLView_nativeDone( JNIEnv*  env, jobject thiz )
{
}

/* Call to render the next GL frame */
void
Java_org_tinder_studio_AndroidGLView_nativeRender( JNIEnv*  env, jobject thiz )
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_SHORT,0,vertexArray);
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4,GL_UNSIGNED_BYTE,0,colorArray);
	glDrawArrays(GL_TRIANGLES,0,3);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void
Java_org_tinder_studio_AndroidGLView_nativeResize( JNIEnv*  env, jobject  thiz, jint width, jint height )
{
    sWindowWidth  = width;
    sWindowHeight = height;
    glViewport(0, 0, width, height);
    __android_log_print(ANDROID_LOG_INFO, "StudyGL", "resize w=%d h=%d", width, height);
    
    glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//使用正交投影
	glOrthof(0, width, 0, height, 0, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void
Java_org_tinder_studio_AndroidGLView_nativePause(JNIEnv *env, jobject thiz)
{

}

void 
Java_org_tinder_studio_AndroidGLView_nativePress(JNIEnv* env, jobject thiz, jfloat x, jfloat y)
{
	__android_log_print(ANDROID_LOG_INFO, "StudyGL", "press x=%.2f y=%.2f", x, y);
}

void 
Java_org_tinder_studio_AndroidGLView_nativeRelease(JNIEnv* env, jobject thiz, jfloat x, jfloat y)
{
	__android_log_print(ANDROID_LOG_INFO, "StudyGL", "release x=%.2f y=%.2f", x, y);
}

jboolean 
Java_org_tinder_studio_AndroidGLView_nativeOnBackDown(JNIEnv* env, jobject thiz)
{
	__android_log_print(ANDROID_LOG_INFO, "StudyGL", "backDown");
	return false;
}
