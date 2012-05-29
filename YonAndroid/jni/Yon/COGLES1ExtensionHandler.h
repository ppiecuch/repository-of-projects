#ifndef _YON_VIDEO_OGLES1_COGLES1EXTENSIONHANDLER_H_
#define _YON_VIDEO_OGLES1_COGLES1EXTENSIONHANDLER_H_

#include "IVideoDriver.h"
#include "yonArray.h"

#ifdef YON_COMPILE_WITH_WIN32
//加载OpenGL ES1需要的库及头文件
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv1_CM.lib")
#include <EGL/egl.h>
#include <GLES/gl.h>
typedef char  GLchar;
#include "glesext.h"
#elif defined(YON_COMPILE_WITH_ANDROID)
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif//YON_COMPILE_WITH_WIN32


namespace yon{
namespace video{
namespace ogles1{

#ifndef GL_GLEXT_PROTOTYPES
/* GL_OES_framebuffer_object */
typedef GLboolean (GL_APIENTRYP PFNGLISRENDERBUFFEROESPROC) (GLuint renderbuffer);
typedef void (GL_APIENTRYP PFNGLBINDRENDERBUFFEROESPROC) (GLenum target, GLuint renderbuffer);
typedef void (GL_APIENTRYP PFNGLDELETERENDERBUFFERSOESPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (GL_APIENTRYP PFNGLGENRENDERBUFFERSOESPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (GL_APIENTRYP PFNGLRENDERBUFFERSTORAGEOESPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GL_APIENTRYP PFNGLGETRENDERBUFFERPARAMETERIVOESPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GL_APIENTRYP PFNGLISFRAMEBUFFEROESPROC) (GLuint framebuffer);
typedef void (GL_APIENTRYP PFNGLBINDFRAMEBUFFEROESPROC) (GLenum target, GLuint framebuffer);
typedef void (GL_APIENTRYP PFNGLDELETEFRAMEBUFFERSOESPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (GL_APIENTRYP PFNGLGENFRAMEBUFFERSOESPROC) (GLsizei n, GLuint* framebuffers);
typedef GLenum (GL_APIENTRYP PFNGLCHECKFRAMEBUFFERSTATUSOESPROC) (GLenum target);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERRENDERBUFFEROESPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERTEXTURE2DOESPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GL_APIENTRYP PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOESPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void (GL_APIENTRYP PFNGLGENERATEMIPMAPOESPROC) (GLenum target);
extern PFNGLISRENDERBUFFEROESPROC pglIsRenderbufferOES;
extern PFNGLBINDRENDERBUFFEROESPROC pglBindRenderbufferOES;
extern PFNGLDELETERENDERBUFFERSOESPROC pglDeleteRenderbuffersOES;
extern PFNGLGENRENDERBUFFERSOESPROC pglGenRenderbuffersOES;
extern PFNGLRENDERBUFFERSTORAGEOESPROC pglRenderbufferStorageOES;
extern PFNGLGETRENDERBUFFERPARAMETERIVOESPROC pglGetRenderbufferParameterivOES;
extern PFNGLISFRAMEBUFFEROESPROC pglIsFramebufferOES;
extern PFNGLBINDFRAMEBUFFEROESPROC pglBindFramebufferOES;
extern PFNGLDELETEFRAMEBUFFERSOESPROC pglDeleteFramebuffersOES;
extern PFNGLGENFRAMEBUFFERSOESPROC pglGenFramebuffersOES;
extern PFNGLCHECKFRAMEBUFFERSTATUSOESPROC pglCheckFramebufferStatusOES;
extern PFNGLFRAMEBUFFERRENDERBUFFEROESPROC pglFramebufferRenderbufferOES;
extern PFNGLFRAMEBUFFERTEXTURE2DOESPROC pglFramebufferTexture2DOES;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOESPROC pglGetFramebufferAttachmentParameterivOES;
extern PFNGLGENERATEMIPMAPOESPROC pglGenerateMipmapOES;

#define glIsRenderbufferOES pglIsRenderbufferOES
#define glBindRenderbufferOES pglBindRenderbufferOES
#define glDeleteRenderbuffersOES pglDeleteRenderbuffersOES
#define glGenRenderbuffersOES pglGenRenderbuffersOES
#define glRenderbufferStorageOES pglRenderbufferStorageOES
#define glGetRenderbufferParameterivOES pglGetRenderbufferParameterivOES
#define glIsFramebufferOES pglIsFramebufferOES
#define glBindFramebufferOES pglBindFramebufferOES
#define glDeleteFramebuffersOES pglDeleteFramebuffersOES
#define glGenFramebuffersOES pglGenFramebuffersOES
#define glCheckFramebufferStatusOES pglCheckFramebufferStatusOES
#define glFramebufferRenderbufferOES pglFramebufferRenderbufferOES
#define glFramebufferTexture2DOES pglFramebufferTexture2DOES
#define glGetFramebufferAttachmentParameterivOES pglGetFramebufferAttachmentParameterivOES
#define glGenerateMipmapOES pglGenerateMipmapOES
#endif

	class COGLES1ExtensionHandler{
	private:
		core::stringc m_extensions;
		bool m_featureAvailables[ENUM_VIDEO_FEATURE_COUNT];
	public:
		COGLES1ExtensionHandler();
		void initExtensionHandler();
		bool isFeatureAvailable(ENUM_VIDEO_FEATURE feature) const{
			return m_featureAvailables[feature];
		}
	};
}
}
}
#endif