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

#ifndef SPK_YONQUADRENDERER
#define SPK_YONQUADRENDERER

#include "RenderingAPIs/Yon/SPK_YONRenderer.h"
#include "Extensions/Renderers/SPK_QuadRendererInterface.h"
#include "Extensions/Renderers/SPK_Oriented3DRendererInterface.h"

namespace SPK
{
namespace YON
{
	/**
	* @class YONQuadRenderer
	* @brief A Renderer drawing particles as quads with Yon
	*
	* The orientation of the quads depends on the orientation parameters set.
	* This orientation is computed during rendering by the CPU (further improvement of SPARK will allow to make the computation on GPU side).<br>
	* <br>
	* Below are the parameters of Particle that are used in this Renderer (others have no effects) :
	* <ul>
	* <li>SPK::PARAM_SIZE</li>
	* <li>SPK::PARAM_RED</li>
	* <li>SPK::PARAM_GREEN</li>
	* <li>SPK::PARAM_BLUE</li>
	* <li>SPK::PARAM_ALPHA (only if blending is enabled)</li>
	* <li>SPK::PARAM_ANGLE</li>
	* <li>SPK::PARAM_TEXTURE_INDEX (only if not in TEXTURE_NONE mode)</li>
	* </ul>
	* @since 1.04.00
	*/
	class SPK_YON_PREFIX YONQuadRenderer :	public YONRenderer,
											public QuadRendererInterface,
											public Oriented3DRendererInterface
	{
		SPK_IMPLEMENT_REGISTERABLE(YONQuadRenderer)

	public :

		//////////////////
		// Constructors //
		//////////////////

		/**
		* @brief Constructor of YONQuadRenderer
		* @param d : the Yon device
		* @param scaleX the scale of the width of the quad
		* @param scaleY the scale of the height of the quad
		*/
		YONQuadRenderer(yon::IYonEngine* d,float scaleX = 1.0f,float scaleY = 1.0f);

		/**
		* @brief Creates and registers a new YONQuadRenderer
		* @param d : the Yon device
		* @param scaleX the scale of the width of the quad
		* @param scaleY the scale of the height of the quad
		* @return A new registered YONQuadRenderer
		* @since 1.04.00
		*/
		static YONQuadRenderer* create(yon::IYonEngine* d,float scaleX = 1.0f,float scaleY = 1.0f);

		/////////////
		// Setters //
		/////////////

		/**
		* @brief Sets the texture to map on particles
		* 
		* Note that this only works with points being rendered as SPK::POINT_SPRITE
		*
		* @param texture : the texture to set
		*/
		void setTexture(yon::video::ITexture* texture);

		/**
		* @brief Sets the atlas dimension of the texture in an Yon way
		*
		* see setQuadAtlasDimension(unsigned int,unsigned int) for more information
		*
		* @param dim : the atlas dimension of the texture
		*/
		void setAtlasDimensions(yon::core::dimension2du dim);
		using QuadRendererInterface::setAtlasDimensions;

		/////////////
		// Getters //
		/////////////

		/**
		* @brief Gets the texture of this renderer
		* @return the texture of this renderer
		*/
		yon::video::ITexture* getTexture() const;

		/**
		* @brief Gets the material texture layer
		* @return the material texture layer
		*/
		yon::video::SMaterialLayer& getMaterialLayer();

		/**
		* @brief Gets the material texture layer in a constant way
		* @return the material texture layer
		*/
		const yon::video::SMaterialLayer& getMaterialLayer() const;

		/**
		* @brief Gets the atlas dimension of the texture in an Yon way
		* @return the atlas dimension of the texture
		*/
        yon::core::dimension2du getAtlasDimensions() const;

		///////////////
		// Interface //
		///////////////

		virtual void render(const Group& group);

		void createBuffers(const Group& group);

	private :

		static const size_t NB_INDICES_PER_QUAD = 6;
		static const size_t NB_VERTICES_PER_QUAD = 4;

		// buffer name
		static const std::string YON_BUFFER_NAME;

		virtual const std::string& getBufferName() const;

		void FillBufferColorAndVertex(const Particle& particle) const;	// Fills Yon buffer with color and position
		void FillBufferTextureAtlas(const Particle& particle) const;		// Fills Yon buffer with atlas texture coordinates

		static void (YONQuadRenderer::*renderParticle)(const Particle&)  const;	// pointer to the right render method

		void renderBasic(const Particle& particle) const;		// Rendering for particles with texture or no texture
		void renderRot(const Particle& particle) const;			// Rendering for particles with texture or no texture and rotation
		void renderAtlas(const Particle& particle) const;		// Rendering for particles with texture atlas
		void renderAtlasRot(const Particle& particle) const;	// Rendering for particles with texture atlas and rotation
	};


	inline YONQuadRenderer* YONQuadRenderer::create(yon::IYonEngine* d,float scaleX,float scaleY)
	{
		YONQuadRenderer* obj = new YONQuadRenderer(d,scaleX,scaleY);
		registerObject(obj);
		return obj;
	}

	inline void YONQuadRenderer::setTexture(yon::video::ITexture* texture)
	{
		material.TextureLayers[0].texture = texture;
	}

	inline void YONQuadRenderer::setAtlasDimensions(yon::core::dimension2du dim)
	{
		setAtlasDimensions(dim.w,dim.h);
	}

	inline yon::video::ITexture* YONQuadRenderer::getTexture() const
	{
		return material.TextureLayers[0].texture;
	}

	inline yon::video::SMaterialLayer& YONQuadRenderer::getMaterialLayer()
	{
		return material.TextureLayers[0];
	}
		
	inline const yon::video::SMaterialLayer& YONQuadRenderer::getMaterialLayer() const
	{
		return material.TextureLayers[0];
	}

	inline yon::core::dimension2du YONQuadRenderer::getAtlasDimensions() const
	{
		return yon::core::dimension2du(textureAtlasNbX,textureAtlasNbY);
	}

	inline const std::string& YONQuadRenderer::getBufferName() const
	{
		return YON_BUFFER_NAME;
	}

	inline void YONQuadRenderer::FillBufferColorAndVertex(const Particle& particle) const
	{
		size_t offset = particle.getIndex() << 2;

		float x = particle.position().x;
		float y = particle.position().y;
		float z = particle.position().z;

		yon::video::SColor color(spk2yon(
			 particle.getParamCurrentValue(PARAM_ALPHA),
			 particle.getR(),
			 particle.getG(),
			 particle.getB()));

		// According to Yon coordinates system, quads are drawn in clockwise order
		// Note that the quad side points towards the left as it is a left handed system
		// top left vertex
		currentBuffer->getVertexBuffer()[offset].pos.set(
			x + quadSide().x + quadUp().x,
			y + quadSide().y + quadUp().y,
			z + quadSide().z + quadUp().z);
		currentBuffer->getVertexBuffer()[offset].color = color;

		// top right vertex
		currentBuffer->getVertexBuffer()[++offset].pos.set(
			x - quadSide().x + quadUp().x,
			y - quadSide().y + quadUp().y,
			z - quadSide().z + quadUp().z);
		currentBuffer->getVertexBuffer()[offset].color = color;

		// bottom right vertex
		currentBuffer->getVertexBuffer()[++offset].pos.set(
			x - quadSide().x - quadUp().x,
			y - quadSide().y - quadUp().y,
			z - quadSide().z - quadUp().z);
		currentBuffer->getVertexBuffer()[offset].color = color;

		// bottom left vertex
		currentBuffer->getVertexBuffer()[++offset].pos.set(
			x + quadSide().x - quadUp().x,
			y + quadSide().y - quadUp().y,
			z + quadSide().z - quadUp().z);
		currentBuffer->getVertexBuffer()[offset].color = color;
	}

	inline void YONQuadRenderer::FillBufferTextureAtlas(const Particle& particle) const
	{
		size_t offset = particle.getIndex() << 2;
		computeAtlasCoordinates(particle);
		currentBuffer->getVertexBuffer()[offset].texcoords.set(textureAtlasU0(),textureAtlasV0());
        currentBuffer->getVertexBuffer()[++offset].texcoords.set(textureAtlasU1(),textureAtlasV0());
        currentBuffer->getVertexBuffer()[++offset].texcoords.set(textureAtlasU1(),textureAtlasV1());
        currentBuffer->getVertexBuffer()[++offset].texcoords.set(textureAtlasU0(),textureAtlasV1());
	}
}}

#endif
