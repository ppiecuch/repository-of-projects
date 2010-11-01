#include <jni.h>
#include <GLES/gl.h>
#include <android/log.h>

#undef WIN32
#undef LINUX
#ifdef _MSC_VER
// Desktop or mobile Win32 environment:
#define WIN32
#else
// Linux environment:
#define LINUX
#endif

#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
static HMODULE sGLESDLL = NULL;
#endif // WIN32

#ifndef IMPORTGL_API
#define IMPORTGL_API extern
#endif
#ifndef IMPORTGL_FNPTRINIT
#define IMPORTGL_FNPTRINIT
#endif

#define FNDEF(retType, funcName, args) IMPORTGL_API retType (*funcPtr_##funcName) args IMPORTGL_FNPTRINIT
#define FNPTR(name) funcPtr_##name
FNDEF(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height));
#define glViewport              FNPTR(glViewport)
#ifdef WIN32
    sGLESDLL = LoadLibrary(_T("libGLES_CM.dll"));
    if (sGLESDLL == NULL)
        sGLESDLL = LoadLibrary(_T("libGLES_CL.dll"));
    if (sGLESDLL == NULL)
        return 0;   // Cannot find OpenGL ES Common or Common Lite DLL.

    /* The following fetches address to each egl & gl function call
     * and stores it to the related function pointer. Casting through
     * void * results in warnings with VC warning level 4, which
     * could be fixed by casting to the true type for each fetch.
     */
#define IMPORT_FUNC(funcName) do { \
        void *procAddress = (void *)GetProcAddress(sGLESDLL, _T(#funcName)); \
        if (procAddress == NULL) result = 0; \
        *((void **)&FNPTR(funcName)) = procAddress; } while (0)
#endif // WIN32

IMPORT_FUNC(glViewport);


static int  sWindowWidth  = 320;
static int  sWindowHeight = 480;

/* Call to initialize the graphics state */
void
Java_org_tinder_studio_gl_HelloGLView_HelloGLRender_nativeInit( JNIEnv*  env )
{
    appInit();
}


/* Call to finalize the graphics state */
void
Java_org_tinder_studio_gl_HelloGLView_HelloGLRender_nativeDone( JNIEnv*  env )
{
}

/* Call to render the next GL frame */
void
Java_org_tinder_studio_gl_HelloGLView_HelloGLRender_nativeRender( JNIEnv*  env )
{
    appRender(0, sWindowWidth, sWindowHeight);
}

void
Java_org_tinder_studio_gl_HelloGLView_HelloGLRender_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
    sWindowWidth  = w;
    sWindowHeight = h;
    __android_log_print(ANDROID_LOG_INFO, "SanAngeles", "resize w=%d h=%d", w, h);
}


void appRender(long tick, int width, int height)
{

	glViewport(0, 0, width, height);
	glClearColorx((GLfixed)(0.1f * 65536),
	              (GLfixed)(0.2f * 65536),
	              (GLfixed)(0.3f * 65536), 0x10000);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}
