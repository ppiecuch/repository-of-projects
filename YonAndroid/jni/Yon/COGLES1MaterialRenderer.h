#ifndef _YON_VIDEO_OGLES1_COGLES1MATERIALRENDERER_H_
#define _YON_VIDEO_OGLES1_COGLES1MATERIALRENDERER_H_

#include "IMaterialRenderer.h"
#include "COGLES1Driver.h"

namespace yon{
namespace video{
namespace ogles1{

	const static u32 MATERIAL_GLSTATES[]=
	{
		//GL_ALPHA_TEST 0
		//GL_BLEND 0
		GL_COLOR_MATERIAL,// 0
		GL_CULL_FACE,// 0
		GL_DEPTH_TEST,// 0
		GL_DITHER,// 1
		GL_FOG,// 0
		GL_LIGHTING,// 0
		GL_LINE_SMOOTH,// 0
		GL_NORMALIZE,// 0
		GL_RESCALE_NORMAL,// 0
		GL_SCISSOR_TEST,// 0
		GL_STENCIL_TEST
	};

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
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
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

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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

	class COGLES1MaterialRendererMask : public COGLES1MaterialRenderer
	{
	public:

		COGLES1MaterialRendererMask(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* material)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			m_pDriver->setTexture(0, material->getTexture(0));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			

			m_pDriver->setTexture(1, material->getTexture(1));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			//m_pDriver->checkGLError(__FILE__,__LINE__);

		}
		virtual void onUnsetMaterial(){
			m_pDriver->setTexture(1, NULL);
			m_pDriver->setTexture(0, NULL);
			glDisable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	};
}//ogles1
	IMaterialRenderer* createMaterialRendererNone(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRenderer((ogles1::COGLES1Driver*)driver);
	}
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
	IMaterialRenderer* createMaterialRendererMask(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererMask((ogles1::COGLES1Driver*)driver);
	}
}//video
}//yon
#endif