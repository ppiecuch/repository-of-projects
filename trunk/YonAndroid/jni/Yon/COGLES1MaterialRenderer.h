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

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		}
	};

	class COGLES1MaterialRendererLighten : public COGLES1MaterialRenderer
	{
	public:

		COGLES1MaterialRendererLighten(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* material)
		{
			m_pDriver->setTexture(0, material->getTexture(0));

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		}

		virtual void onUnsetMaterial(){
			glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	};
	class COGLES1MaterialRendererTransparent : public COGLES1MaterialRenderer
	{
	public: 

		COGLES1MaterialRendererTransparent(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* material)
		{
			m_pDriver->setTexture(0, material->getTexture(0));

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.f);
		}

		virtual void onUnsetMaterial(){
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
		}
	};

	class COGLES1MaterialRendererTransparentBlendColor : public COGLES1MaterialRenderer
	{
	public: 

		COGLES1MaterialRendererTransparentBlendColor(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* material)
		{
			m_pDriver->setTexture(0, material->getTexture(0));

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			/*glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);

			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
			glTexEnvf(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);*/

			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.f);
		}

		virtual void onUnsetMaterial(){
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
		}
	};

	class COGLES1MaterialRendererCoat : public COGLES1MaterialRenderer
	{
	public:

		COGLES1MaterialRendererCoat(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* material)
		{
			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.f);

			glActiveTexture(GL_TEXTURE0);
			m_pDriver->setTexture(0, material->getTexture(0));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			

			glActiveTexture(GL_TEXTURE1);
			m_pDriver->setTexture(1, material->getTexture(1));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC2_RGB, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);

			//m_pDriver->checkGLError(__FILE__,__LINE__);

		}
		virtual void onUnsetMaterial(){
			glActiveTexture(GL_TEXTURE1);
			m_pDriver->setTexture(1, NULL);
			glActiveTexture(GL_TEXTURE0);

			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
		}
	};
}//ogles1
	IMaterialRenderer* createMaterialRendererSolid(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererSolid((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererLighten(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererLighten((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererTransparent(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererTransparent((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererTransparentBlendColor(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererTransparentBlendColor((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererCoat(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererCoat((ogles1::COGLES1Driver*)driver);
		//return new ogles1::COGLES1MaterialRendererSolid((ogles1::COGLES1Driver*)driver);
	}
}//video
}//yon
#endif