#include "COGLES1ExtensionHandler.h"
#include "COGLES1Driver.h"

#include "ILogger.h"

namespace yon{
namespace video{
namespace ogles1{

	static const c8* const OGLESFeatureStrings[] ={
		"GL_OES_framebuffer_object"
	};

	COGLES1ExtensionHandler::COGLES1ExtensionHandler(){

		for(u32 i=0;i<ENUM_VIDEO_FEATURE_COUNT;++i)
			m_featureAvailables[i]=false;
	}

	void COGLES1ExtensionHandler::initExtensionHandler(){
		m_extensions=(const c8*)glGetString(GL_EXTENSIONS);
		Logger->info("GL_EXTENSIONS:\n%s\n",m_extensions.c_str());

		for(u32 i=0;i<ENUM_VIDEO_FEATURE_COUNT;++i){
			m_featureAvailables[i]=m_extensions.find(OGLESFeatureStrings[i])!=-1;
		}
	}

}
}
}