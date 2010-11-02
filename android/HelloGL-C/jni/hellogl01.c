#include <hellogl.h>
#include <GLES/gl.h>
#include <android/log.h>


static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;

GLshort vertexArray[9]={160,368,0,  100,265,0,  220,265,0};
GLubyte colorArray[12]={255,0,0,0,  0,255,0,0,  0,0,255,0};

void appInit()
{
	glClearColorx((GLfixed)(0.1f * 65536),(GLfixed)(0.2f * 65536),(GLfixed)(0.3f * 65536), 0x10000);
}

void appResize(int width,int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, width, 0, height, 0, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void appRender(long tick, int width, int height)
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
