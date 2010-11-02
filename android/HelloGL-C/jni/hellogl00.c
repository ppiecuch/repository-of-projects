#include <hellogl.h>
#include <GLES/gl.h>
#include <android/log.h>


static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;

void appInit()
{
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
