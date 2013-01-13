//////////////////////////////////////////////////////////////////////////////////
// SPARK Yon Rendering library												//
// Copyright (C) 2009															//
// Thibault Lescoat -  info-tibo <at> orange <dot> fr							//
// Julien Fryer - julienfryer@gmail.com											//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to anyone to use this software for any purpose,		//
// including commercial applications, and to alter it and redistribute it		//
// freely, subject to the following restrictions:								//
//																				//
// 1. The origin of this software must not be misrepresented; you must not		//
//    claim that you wrote the original software. If you use this software		//
//    in a product, an acknowledgment in the product documentation would be		//
//    appreciated but is not required.											//
// 2. Altered source versions must be plainly marked as such, and must not be	//
//    misrepresented as being the original software.							//
// 3. This notice may not be removed or altered from any source distribution.	//
//////////////////////////////////////////////////////////////////////////////////

#ifndef SPK_YONRENDERER
#define SPK_YONRENDERER

#include "RenderingAPIs/Yon/SPK_YON_DEF.h"
#include "RenderingAPIs/Yon/SPK_YONBuffer.h"
#include "RenderingAPIs/Yon/SPK_YONMaterialProxy.h"
#include "Core/SPK_Renderer.h"
#include "Core/SPK_Group.h"


namespace SPK
{
namespace YON
{
	/**
	* @brief The base renderer for all Yon renderers
	* 
	* This class presents a convenient interface to set some parameters common to all Yon renderers (blending mode...).<br>
	* <br>
	* Note that rendering hints work with Yon renderers except the SPK::ALPHA_TEST
	* which is always enabled with a threshold of 0. (meaning alpha values of 0 are never rendered).
	*
	* @since 1.04.00
	*/
	class SPK_YON_PREFIX YONRenderer : public Renderer
	{
	public :

		/////////////////
		// Constructor //
		/////////////////

		/**
		* @brief Constructor of YONRenderer
		* @param d : the Yon device
		*/
		YONRenderer(yon::YonDevice* d);

		////////////////
		// Destructor //
		////////////////

		/** @brief Destructor of YONRenderer */
		virtual ~YONRenderer(){};

		/////////////
		// Setters //
		/////////////

		/**
		* @brief Sets the blending mode in a very accurate way
		*
		* This method allows to set any blending mode supported by Yon.<br>
		* Note that a simpler helper method exist to set the most common blending modes :<br>
		* <i>setBlending(BlendingMode)</i>
		*
		* @param srcFunc : the blending source function
		* @param destFunc : the blending destination function
		* @param alphaSrc : the alpha source
		*/
		void setBlending(yon::video::E_BLEND_FACTOR srcFunc,yon::video::E_BLEND_FACTOR destFunc,unsigned int alphaSrc);
		virtual void setBlending(BlendingMode blendMode);

		virtual void enableRenderingHint(RenderingHint renderingHint,bool enable);
		virtual void setAlphaTestThreshold(float alphaThreshold);

		/////////////
		// Getters //
		/////////////

		/**
		* @brief Gets the Yon device of this renderer
		* @return the device of this renderer
		*/
        yon::YonDevice* getDevice() const;

		/**
		* @brief Gets the source blending funtion of this renderer
		* @return the source blending funtion of this renderer
		*/
		yon::video::E_BLEND_FACTOR getBlendSrcFunc() const;

		/**
		* @brief Gets the destination blending funtion of this renderer
		* @return the destination blending funtion of this renderer
		*/
		yon::video::E_BLEND_FACTOR getBlendDestFunc() const;

		/**
		* @brief Gets the alpha source of this renderer
		* @return the alpha source of this renderer
		*/
		unsigned int getAlphaSource() const;

		/**
		* @brief Gets the material of this renderer
		*
		* Note that the material is constant and therefore cannot be modified directly
		*
		* @return the material of this renderer
		*/
		const yon::video::SMaterial& getMaterial() const;

		/**
		* @brief Gets a proxy for the material of this renderer
		*
		* The material proxy allows to modify some parameters of the underlying Yon material
		*
		* @return a material proxy
		* @since 1.5.6
		*/
		YONMaterialProxy getMaterialProxy() { return YONMaterialProxy(material); }

		virtual bool isRenderingHintEnabled(RenderingHint renderingHint) const;

		///////////////
		// Interface //
		///////////////

		virtual void destroyBuffers(const Group& group);

	protected :

		yon::YonDevice* device;	// the device
		yon::video::SMaterial material;	// the material

		mutable YONBuffer* currentBuffer;

		virtual bool checkBuffers(const Group& group);
		static unsigned int getVBOFlag();

	private :

		yon::video::E_BLEND_FACTOR blendSrcFunc;
		yon::video::E_BLEND_FACTOR blendDestFunc;
		unsigned int alphaSource;

		/**
		* @brief Gets the name of the YONBuffer used by the renderer
		*
		* This method must be implemented in all children renderers
		*
		* @return the name of the YONBuffer
		*/
		virtual const std::string& getBufferName() const = 0;

		void updateMaterialBlendingMode();
	};

	
	inline void YONRenderer::setAlphaTestThreshold(float alphaThreshold)
	{
		Renderer::setAlphaTestThreshold(0.0f); // the alpha threshold of the yonlicht material is always 0
	}
	
	inline yon::YonDevice* YONRenderer::getDevice() const
	{
		return device;
	}

	inline yon::video::E_BLEND_FACTOR YONRenderer::getBlendSrcFunc() const
	{
		return blendSrcFunc;
	}

	inline yon::video::E_BLEND_FACTOR YONRenderer::getBlendDestFunc() const
	{
		return blendDestFunc;
	}

	inline unsigned int YONRenderer::getAlphaSource() const
	{
		return alphaSource;
	}

	inline const yon::video::SMaterial& YONRenderer::getMaterial() const
	{
		return material;
	}

	inline void YONRenderer::destroyBuffers(const Group& group)
    {
		group.destroyBuffer(getBufferName());
    }

	inline bool YONRenderer::checkBuffers(const Group& group)
	{
		currentBuffer = dynamic_cast<YONBuffer*>(group.getBuffer(getBufferName(),getVBOFlag()));
		return currentBuffer != NULL;
	}

	inline unsigned int YONRenderer::getVBOFlag()
	{
		return YONBuffer::isVBOHintActivated() ? 1 : 0;	
	}

	inline void YONRenderer::updateMaterialBlendingMode()
	{
		material.MaterialTypeParam = yon::video::pack_texureBlendFunc(
			blendSrcFunc,
			blendDestFunc,
			yon::video::EMFN_MODULATE_1X,
			alphaSource);
	}
}}

#endif
