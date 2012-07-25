#ifndef _YON_VIDEO_OGLES1_COGLES1MATERIALRENDERER_H_
#define _YON_VIDEO_OGLES1_COGLES1MATERIALRENDERER_H_

#include "IMaterialRenderer.h"
#include "COGLES1Driver.h"
#include "ILogger.h"

namespace yon{
namespace video{
namespace ogles1{

	const static u32 MATERIAL_GLSTATES[]=
	{
		//GL_ALPHA_TEST 0
		//GL_BLEND 0
		GL_COLOR_MATERIAL,// 0
		GL_CULL_FACE,// 0
		//GL_DEPTH_TEST,// 0
		GL_DITHER,// 1
		GL_FOG,// 0
		GL_LIGHTING,// 0
		GL_LINE_SMOOTH,// 0
		GL_NORMALIZE,// 0
		GL_RESCALE_NORMAL,// 0
		GL_SCISSOR_TEST,// 0
		GL_STENCIL_TEST
	};

	const static u32 MATERIAL_GLBLENDS[]=
	{
		GL_ZERO,
		GL_ONE,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_SRC_ALPHA_SATURATE
	};

	class COGLES1MaterialRenderer : public IMaterialRenderer{
	public:
		COGLES1MaterialRenderer(COGLES1Driver* driver):m_pDriver(driver){}
	protected:
		COGLES1Driver* m_pDriver;

		void setBasicRenderStates(const IMaterial* current,const IMaterial* last){
			//设置基本状态
			s32 mask=1;
			s32 cmask=current->states.getInt();
			s32 xorValue=~0;
			bool stateChanged=false;
			if(last){
				if(last->states!=current->states){
					xorValue=last->states^(current->states);
					stateChanged=true;
				}
				if(last->getCullingMode()!=current->getCullingMode())
					glCullFace(current->getCullingMode());
				if(last->getFrontFace()!=current->getFrontFace())
					glFrontFace(current->getFrontFace());

			}else{
				stateChanged=true;
				glCullFace(current->getCullingMode());
				glFrontFace(current->getFrontFace());
			}
			if(stateChanged)
			{
				for(int i=0;i<ENUM_MATERIAL_STATE_COUNT;++i)
				{
					if(xorValue&mask)
					{
						if(cmask&mask){
							//Logger->debug("%s change to true\n",MATERIAL_STATE_NAMES[i]);
							glEnable(MATERIAL_GLSTATES[i]);
						}else{
							//Logger->debug("%s change to false\n",MATERIAL_STATE_NAMES[i]);
							glDisable(MATERIAL_GLSTATES[i]);
						}
					}
					mask<<=1;
				}
			}

			if(isTransparent(current->getMaterialType()))
				glDepthMask(false);
			else
				glDepthMask(true);

			//设置过滤方式和环绕方式
			GLint mag_filter;
			GLint min_filter;
			for(u32 i=0;i<MATERIAL_MAX_TEXTURES;++i)
			{
				mag_filter=current->getFilterMode(i)>=ENUM_FILTER_MODE_BILINEAR ? GL_LINEAR : GL_NEAREST;
				min_filter=current->getFilterMode(i)>=ENUM_FILTER_MODE_TRILINEAR?GL_LINEAR_MIPMAP_NEAREST:current->getFilterMode(i)>=ENUM_FILTER_MODE_BILINEAR ? GL_LINEAR : GL_NEAREST;
				//mag_filter=current->getFilterMode(i)>=ENUM_FILTER_MODE_BILINEAR ? GL_LINEAR : GL_NEAREST;
				//min_filter=current->getFilterMode(i)>=ENUM_FILTER_MODE_BILINEAR ? GL_LINEAR : GL_NEAREST;

				glActiveTexture(GL_TEXTURE0 + i);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,mag_filter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,min_filter);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, current->getWrapModeU(i));
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, current->getWrapModeV(i));
			}
			glActiveTexture(GL_TEXTURE0);
		}
	};

	class COGLES1MaterialRendererSolid : public COGLES1MaterialRenderer
	{
	public:

		COGLES1MaterialRendererSolid(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last)
		{
			m_pDriver->setTexture(0, current->getTexture(0));
			setBasicRenderStates(current,last);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		}
	};

	class COGLES1MaterialRendererBlend : public COGLES1MaterialRenderer
	{
	public:

		COGLES1MaterialRendererBlend(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last)
		{
			m_pDriver->setTexture(0, current->getTexture(0));
			setBasicRenderStates(current,last);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);	
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, (f32)current->getModulate());

			glBlendFunc(MATERIAL_GLBLENDS[current->getBlendSrcFactor()], MATERIAL_GLBLENDS[current->getBlendDstFactor()]);
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);

			if(blendFactorHasAlpha(current->getBlendSrcFactor())||blendFactorHasAlpha(current->getBlendDstFactor()))
			{
				switch(current->getAlphaSource())
				{
				case ENUM_ALPHA_SOURCE_VERTEX:
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_PRIMARY_COLOR);
					break;
				case ENUM_ALPHA_SOURCE_TEXTURE:
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
					break;
				default:
					glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);
					glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_ALPHA, GL_PRIMARY_COLOR);
				}
				glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);
			}
		}

		virtual void onUnsetMaterial(){
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 1.f );
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
		}
	};

	class COGLES1MaterialRendererLighten : public COGLES1MaterialRenderer
	{
	public:

		COGLES1MaterialRendererLighten(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last)
		{
			m_pDriver->setTexture(0, current->getTexture(0));
			setBasicRenderStates(current,last);

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

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

		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last)
		{
			m_pDriver->setTexture(0, current->getTexture(0));
			setBasicRenderStates(current,last);

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.f);
		}

		virtual void onUnsetMaterial(){
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);

			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
	};

	class COGLES1MaterialRendererTransparentRef : public COGLES1MaterialRenderer
	{
	public: 

		COGLES1MaterialRendererTransparentRef(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last)
		{
			m_pDriver->setTexture(0, current->getTexture(0));
			setBasicRenderStates(current,last);


			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			//glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			//glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
			//glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PRIMARY_COLOR);

			//glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.5f);
		}

		virtual void onUnsetMaterial(){
			//glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
		}
	};

	class COGLES1MaterialRendererCompositePass1 : public COGLES1MaterialRenderer
	{
	public: 

		COGLES1MaterialRendererCompositePass1(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last)
		{
			m_pDriver->setTexture(0, current->getTexture(0));
			setBasicRenderStates(current,last);


			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_EQUAL, 1.0f);
		}

		virtual void onUnsetMaterial(){
			glDisable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
	};

	class COGLES1MaterialRendererCompositePass2 : public COGLES1MaterialRenderer
	{
	public: 

		COGLES1MaterialRendererCompositePass2(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last)
		{
			m_pDriver->setTexture(0, current->getTexture(0));
			setBasicRenderStates(current,last);

			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_LESS, 1.0f);
		}

		virtual void onUnsetMaterial(){
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
		}
	};

	/*
	class COGLES1MaterialRendererTransparentBlendColor : public COGLES1MaterialRenderer
	{
	public: 

		COGLES1MaterialRendererTransparentBlendColor(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last)
		{
			m_pDriver->setTexture(0, current->getTexture(0));
			setBasicRenderStates(current,last);

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.f);
		}

		virtual void onUnsetMaterial(){
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
		}
	};
	*/

	class COGLES1MaterialRendererMask : public COGLES1MaterialRenderer
	{
	public:

		COGLES1MaterialRendererMask(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last)
		{
			glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

			m_pDriver->setTexture(0, current->getTexture(0));
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			

			m_pDriver->setTexture(1, current->getTexture(1));
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			setBasicRenderStates(current,last);

			//m_pDriver->checkGLError(__FILE__,__LINE__);

		}
		virtual void onUnsetMaterial(){
			m_pDriver->setTexture(1, NULL);
			m_pDriver->setTexture(0, NULL);
			glDisable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	};

	class COGLES1MaterialRendererDetailMap : public COGLES1MaterialRenderer
	{
	public: 

		COGLES1MaterialRendererDetailMap(COGLES1Driver* driver)
			: COGLES1MaterialRenderer(driver) {}

		virtual void onSetMaterial(const IMaterial* current,const IMaterial* last)
		{
			m_pDriver->setTexture(0, current->getTexture(0));
			m_pDriver->setTexture(1, current->getTexture(1));

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD_SIGNED);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_TEXTURE);

			setBasicRenderStates(current,last);
		}

		virtual void onUnsetMaterial(){
			m_pDriver->setTexture(1, NULL);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			m_pDriver->setTexture(0, NULL);
		}
	};
}//ogles1
// 	IMaterialRenderer* createMaterialRendererNone(IVideoDriver* driver){
// 		return new ogles1::COGLES1MaterialRenderer((ogles1::COGLES1Driver*)driver);
// 	}
	IMaterialRenderer* createMaterialRendererSolid(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererSolid((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererBlend(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererBlend((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererLighten(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererLighten((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererTransparent(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererTransparent((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererTransparentRef(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererTransparentRef((ogles1::COGLES1Driver*)driver);
	}
	//IMaterialRenderer* createMaterialRendererTransparentBlendColor(IVideoDriver* driver){
	//	return new ogles1::COGLES1MaterialRendererTransparentBlendColor((ogles1::COGLES1Driver*)driver);
	//}
	IMaterialRenderer* createMaterialRendererMask(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererMask((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererDetailMap(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererDetailMap((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererCompositePass1(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererCompositePass1((ogles1::COGLES1Driver*)driver);
	}
	IMaterialRenderer* createMaterialRendererCompositePass2(IVideoDriver* driver){
		return new ogles1::COGLES1MaterialRendererCompositePass2((ogles1::COGLES1Driver*)driver);
	}
}
}
#endif