#include "COGLES1ExtensionHandler.h"
#include "COGLES1Driver.h"

#include "ILogger.h"

namespace yon{
namespace video{
namespace ogles1{

#ifdef YON_COMPILE_WITH_WIN32
	PFNGLISRENDERBUFFEROESPROC pglIsRenderbufferOES;
	PFNGLBINDRENDERBUFFEROESPROC pglBindRenderbufferOES;
	PFNGLDELETERENDERBUFFERSOESPROC pglDeleteRenderbuffersOES;
	PFNGLGENRENDERBUFFERSOESPROC pglGenRenderbuffersOES;
	PFNGLRENDERBUFFERSTORAGEOESPROC pglRenderbufferStorageOES;
	PFNGLGETRENDERBUFFERPARAMETERIVOESPROC pglGetRenderbufferParameterivOES;
	PFNGLISFRAMEBUFFEROESPROC pglIsFramebufferOES;
	PFNGLBINDFRAMEBUFFEROESPROC pglBindFramebufferOES;
	PFNGLDELETEFRAMEBUFFERSOESPROC pglDeleteFramebuffersOES;
	PFNGLGENFRAMEBUFFERSOESPROC pglGenFramebuffersOES;
	PFNGLCHECKFRAMEBUFFERSTATUSOESPROC pglCheckFramebufferStatusOES;
	PFNGLFRAMEBUFFERRENDERBUFFEROESPROC pglFramebufferRenderbufferOES;
	PFNGLFRAMEBUFFERTEXTURE2DOESPROC pglFramebufferTexture2DOES;
	PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOESPROC pglGetFramebufferAttachmentParameterivOES;
	PFNGLGENERATEMIPMAPOESPROC pglGenerateMipmapOES;
#endif

	static const c8* const OGLESFeatureStrings[] ={
		"GL_OES_framebuffer_object"
	};

	COGLES1ExtensionHandler::COGLES1ExtensionHandler(){

		for(u32 i=0;i<ENUM_VIDEO_FEATURE_COUNT;++i)
			m_featureAvailables[i]=false;
	}

	void COGLES1ExtensionHandler::initExtensionHandler(){
		m_extensions=(const c8*)glGetString(GL_EXTENSIONS);
		Logger->info("GL_EXTENSIONS:\n");
		core::array<core::stringc> arr;
		m_extensions.split(arr," ");
		for(u32 i=0;i<arr.size();++i)
			Logger->info("%s\n",arr[i].c_str());

		for(u32 i=0;i<ENUM_VIDEO_FEATURE_COUNT;++i){
			m_featureAvailables[i]=m_extensions.find(OGLESFeatureStrings[i])!=-1;
		}

#ifdef YON_COMPILE_WITH_WIN32
		if(m_featureAvailables[ENUM_VIDEO_FEATURE_FBO])
		{
			pglIsRenderbufferOES = (PFNGLISRENDERBUFFEROESPROC) eglGetProcAddress("glIsRenderbufferOES");
			pglBindRenderbufferOES = (PFNGLBINDRENDERBUFFEROESPROC) eglGetProcAddress("glBindRenderbufferOES");
			pglDeleteRenderbuffersOES = (PFNGLDELETERENDERBUFFERSOESPROC) eglGetProcAddress("glDeleteRenderbuffersOES");
			pglGenRenderbuffersOES = (PFNGLGENRENDERBUFFERSOESPROC) eglGetProcAddress("glGenRenderbuffersOES");
			pglRenderbufferStorageOES = (PFNGLRENDERBUFFERSTORAGEOESPROC) eglGetProcAddress("glRenderbufferStorageOES");
			pglGetRenderbufferParameterivOES = (PFNGLGETRENDERBUFFERPARAMETERIVOESPROC) eglGetProcAddress("glGetRenderbufferParameterivOES");
			pglIsFramebufferOES = (PFNGLISFRAMEBUFFEROESPROC) eglGetProcAddress("glIsFramebufferOES");
			pglBindFramebufferOES = (PFNGLBINDFRAMEBUFFEROESPROC) eglGetProcAddress("glBindFramebufferOES");
			pglDeleteFramebuffersOES = (PFNGLDELETEFRAMEBUFFERSOESPROC) eglGetProcAddress("glDeleteFramebuffersOES");
			pglGenFramebuffersOES = (PFNGLGENFRAMEBUFFERSOESPROC) eglGetProcAddress("glGenFramebuffersOES");
			pglCheckFramebufferStatusOES = (PFNGLCHECKFRAMEBUFFERSTATUSOESPROC) eglGetProcAddress("glCheckFramebufferStatusOES");
			pglFramebufferRenderbufferOES = (PFNGLFRAMEBUFFERRENDERBUFFEROESPROC) eglGetProcAddress("glFramebufferRenderbufferOES");
			pglFramebufferTexture2DOES = (PFNGLFRAMEBUFFERTEXTURE2DOESPROC) eglGetProcAddress("glFramebufferTexture2DOES");
			pglGetFramebufferAttachmentParameterivOES = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVOESPROC) eglGetProcAddress("glGetFramebufferAttachmentParameterivOES");
			pglGenerateMipmapOES = (PFNGLGENERATEMIPMAPOESPROC) eglGetProcAddress("glGenerateMipmapOES");
		}
#endif
	}

}
}
}