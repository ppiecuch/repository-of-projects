#include <string.h>
#include <jni.h>
#include <jni_md.h>
#include <stdio.h>
#include <GLES/gl.h>
#include <android/log.h>
#include <math.h>
#include <stl_pair.h>
#include "hellogl.h"
#include "point3f.h"

using namespace std;

/***************************************/
/*             球体(索引数组)           */
/***************************************/

#define ONE 1
#define RADIUS 10.0f
#define HORIZONAL_STEPS 12
#define PORTRAIT_STEPS 12

static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;


void appInit()
{
	glClearColorx((GLfixed)(0.1f * 65536),(GLfixed)(0.2f * 65536),(GLfixed)(0.3f * 65536), 0x10000);
	Point3f *point=new Point3f();
	cout<<*point<<endl;
}

void appResize(int width,int height)
{
	glViewport(0, 0, width, height);
}

void appRender(long tick, int width, int height)
{

	glClearColorx((GLfixed)(0.1f * 65536),
	              (GLfixed)(0.2f * 65536),
	              (GLfixed)(0.3f * 65536), 0x10000);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
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

static JNINativeMethod methods[] = {

  {"nativeInit", "()", (void*)Java_org_tinder_studio_gl_HelloGLView_nativeInit },
  {"nativeDone", "()", (void*)Java_org_tinder_studio_gl_HelloGLView_nativeDone },
  {"nativeRender", "()", (void*)Java_org_tinder_studio_gl_HelloGLView_nativeRender },
  {"nativeResize", "()", (void*)Java_org_tinder_studio_gl_HelloGLView_nativeResize }

};

static const char *classPathName = "org/tinder/studio/gl/HelloGLView";

/*
* Register several native methods for one class.
*/

static int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    fprintf(stderr, "RegisterNatives start for '%s'", className);
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        fprintf(stderr, "Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        fprintf(stderr, "RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/*
 * Register native methods for all classes we know about.
 *
 * returns JNI_TRUE on success.
 */
static int registerNatives(JNIEnv* env)
{
  if (!registerNativeMethods(env, classPathName,
                 methods, sizeof(methods) / sizeof(methods[0]))) {
    return JNI_FALSE;
  }
  return JNI_TRUE;
}


// ----------------------------------------------------------------------------
/*
 * This is called by the VM when the shared library is first loaded.
 */
typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

