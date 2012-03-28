#ifndef _YON_VIDEO_OGLES1_COGLES1MATERIALRENDERER_H_
#define _YON_VIDEO_OGLES1_COGLES1MATERIALRENDERER_H_

#include "IMaterialRenderer.h"
#include "COGLES1Driver.h"

namespace yon{
namespace video{
namespace ogles1{

	class COGLES1MaterialRenderer : public IMaterialRenderer{
	public:
		COGLES1MaterialRenderer(COGLES1Driver* driver):m_pDriver(driver){}
	protected:
		COGLES1Driver* m_pDriver;
	};

	class COGLES1MaterialRendererSolid : public COGLES1MaterialRenderer
	{
	public:

		COGLES1MaterialRendererSolid(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* material)
		{
			m_pDriver->setTexture(0, material->getTexture(0));
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		}
	};
}//ogles1
	IMaterialRenderer* createMaterialRendererSolid(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererSolid((ogles1::COGLES1Driver*)driver);
	}
}//video
}//yon
#endif