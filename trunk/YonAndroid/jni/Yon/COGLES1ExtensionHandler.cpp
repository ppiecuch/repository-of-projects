#include "COGLES1ExtensionHandler.h"
#include "COGLES1Driver.h"
#include "fastatof.h"

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
		m_version=(const c8*)glGetString(GL_VERSION);
		f32 version=m_version.superatof();
		Logger->info("GL_VERSION:%s(%.2f)\r\n",m_version.c_str(),version);
		m_extensions=(const c8*)glGetString(GL_EXTENSIONS);
		Logger->info("GL_EXTENSIONS:\r\n");
		core::array<core::stringc> arr;
		m_extensions.split(arr," ");
		for(u32 i=0;i<arr.size();++i)
			Logger->info("%s\n",arr[i].c_str());

		glGetIntegerv(GL_MAX_TEXTURE_UNITS,&m_iMaxTextureUnits);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE,&m_iMaxTextureSize);
		//glGetIntegerv(GL_MAX_ELEMENTS_VERTICES,&m_iMaxElementsVertices);
		//glGetIntegerv(GL_MAX_ELEMENTS_INDICES,&m_iMaxElementsIndices);
		glGetIntegerv(GL_MAX_LIGHTS,&m_iMaxLights);

		Logger->info("GL_MAX_TEXTURE_UNITS:%d\r\n",m_iMaxTextureUnits);
		Logger->info("GL_MAX_TEXTURE_SIZE:%d\r\n",m_iMaxTextureSize);
		//Logger->info("GL_MAX_ELEMENTS_VERTICES:%d\r\n",m_iMaxElementsVertices);
		//Logger->info("GL_MAX_ELEMENTS_INDICES:%d\r\n",m_iMaxElementsIndices);
		Logger->info("GL_MAX_LIGHTS:%d\r\n",m_iMaxLights);
		

		for(u32 i=0;i<ENUM_VIDEO_FEATURE_COUNT;++i){
			switch(i)
			{
			case ENUM_VIDEO_FEATURE_GENERATE_MIPMAP:
				m_featureAvailables[i]=version>=1.1f;//GL_GENERATE_MIPMAP从OpenGL1.4开始受支持
				break;
			default:
				m_featureAvailables[i]=m_extensions.find(OGLESFeatureStrings[i])!=-1;
			}	
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