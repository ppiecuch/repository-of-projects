#include "studygl.h"
#include <GLES/gl.h>
#include <android/log.h>

/**
1、实现环境的搭建
**/

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;


/* Call to initialize the graphics state */
void
Java_org_tinder_studio_AndroidGLView_nativeInit( JNIEnv*  env, jobject thiz, jstring apkPath )
{
	
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
    glClearColorx((GLfixed)(0.1f * 65536),
	              (GLfixed)(0.2f * 65536),
	              (GLfixed)(0.3f * 65536), 0x10000);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void
Java_org_tinder_studio_AndroidGLView_nativeResize( JNIEnv*  env, jobject  thiz, jint width, jint height )
{
    sWindowWidth  = width;
    sWindowHeight = height;
    glViewport(0, 0, width, height);
    __android_log_print(ANDROID_LOG_INFO, "StudyGL", "resize w=%d h=%d", width, height);
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
