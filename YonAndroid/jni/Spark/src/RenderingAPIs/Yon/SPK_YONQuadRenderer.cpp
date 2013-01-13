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

#include "RenderingAPIs/Yon/SPK_YONQuadRenderer.h"
#include "RenderingAPIs/Yon/SPK_YONBuffer.h"

namespace SPK
{
namespace YON
{
	const std::string YONQuadRenderer::YON_BUFFER_NAME("SPK_YONQuadRenderer_Buffer");
	void (YONQuadRenderer::*YONQuadRenderer::renderParticle)(const Particle&) const = NULL;

	YONQuadRenderer::YONQuadRenderer(yon::YonDevice* d,float scaleX,float scaleY) :
		YONRenderer(d),
		QuadRendererInterface(scaleX,scaleY),
		Oriented3DRendererInterface()
	{}

	void YONQuadRenderer::createBuffers(const Group& group)
	{
		currentBuffer = dynamic_cast<YONBuffer*>(group.createBuffer(getBufferName(),
																	YONBufferCreator(device,
																		NB_VERTICES_PER_QUAD,
																		NB_INDICES_PER_QUAD),
																	getVBOFlag(),
																	false));
		size_t nbTotalParticles = group.getParticles().getNbReserved();

		yon::scene::IIndexBuffer& indexBuffer = currentBuffer->getIndexBuffer();
		if (indexBuffer.getType() == yon::video::EIT_32BIT)
		{
			yon::u32* indices = reinterpret_cast<yon::u32*>(indexBuffer.pointer());
			for (size_t t = 0; t < nbTotalParticles; ++t)
			{
				indices[NB_INDICES_PER_QUAD*t+0] = NB_VERTICES_PER_QUAD*t+0;
                indices[NB_INDICES_PER_QUAD*t+1] = NB_VERTICES_PER_QUAD*t+1;
                indices[NB_INDICES_PER_QUAD*t+2] = NB_VERTICES_PER_QUAD*t+2;
                indices[NB_INDICES_PER_QUAD*t+3] = NB_VERTICES_PER_QUAD*t+0;
                indices[NB_INDICES_PER_QUAD*t+4] = NB_VERTICES_PER_QUAD*t+2;
                indices[NB_INDICES_PER_QUAD*t+5] = NB_VERTICES_PER_QUAD*t+3;
			}
		}
		else if (indexBuffer.getType() == yon::video::EIT_16BIT)
		{
			yon::u16* indices = reinterpret_cast<yon::u16*>(indexBuffer.pointer());
			for (size_t t = 0; t < nbTotalParticles; ++t)
			{
				indices[NB_INDICES_PER_QUAD*t+0] = NB_VERTICES_PER_QUAD*t+0;
                indices[NB_INDICES_PER_QUAD*t+1] = NB_VERTICES_PER_QUAD*t+1;
                indices[NB_INDICES_PER_QUAD*t+2] = NB_VERTICES_PER_QUAD*t+2;
                indices[NB_INDICES_PER_QUAD*t+3] = NB_VERTICES_PER_QUAD*t+0;
                indices[NB_INDICES_PER_QUAD*t+4] = NB_VERTICES_PER_QUAD*t+2;
				indices[NB_INDICES_PER_QUAD*t+5] = NB_VERTICES_PER_QUAD*t+3;
			}	
		}

		yon::video::S3DVertex* vertices = currentBuffer->getVertexBuffer().pointer();
		for (size_t t = 0; t < nbTotalParticles; t++)
		{
			vertices[NB_VERTICES_PER_QUAD*t+0].TCoords = yon::core::vector2df(0.0f,0.0f);
			vertices[NB_VERTICES_PER_QUAD*t+1].TCoords = yon::core::vector2df(1.0f,0.0f);
			vertices[NB_VERTICES_PER_QUAD*t+2].TCoords = yon::core::vector2df(1.0f,1.0f);
			vertices[NB_VERTICES_PER_QUAD*t+3].TCoords = yon::core::vector2df(0.0f,1.0f);
		}

		if (YONBuffer::isVBOHintActivated())
		{
			for (size_t t = 0; t < nbTotalParticles * NB_VERTICES_PER_QUAD; t++)
			{
				vertices[t].Pos = yon::core::vector3df(0.0f,0.0f,0.0f);
				vertices[t].Color = yon::video::SColor(0x00000000);
			}
		}
		else
			currentBuffer->setVBOInitialized(true);

		currentBuffer->getMeshBuffer().setDirty(yon::scene::EBT_VERTEX_AND_INDEX);	 
	}

	void YONQuadRenderer::render(const Group& group)
	{
		if (!prepareBuffers(group))
			return;

		yon::video::IVideoDriver* driver = device->getVideoDriver();

		// Computes the inverse model view
		yon::core::matrix4 invModelView;
		{
			yon::core::matrix4 modelView(driver->getTransform(yon::video::ETS_VIEW));
			modelView *= driver->getTransform(yon::video::ETS_WORLD);
			modelView.getInversePrimitive(invModelView); // wont work for odd modelview matrices (but should happen in very special cases)
		}

		// Saves the renderer texture
		yon::video::ITexture* savedTexture = material.TextureLayer[0].Texture;
		if (texturingMode == TEXTURE_NONE)
			material.TextureLayer[0].Texture = NULL;

		if ((texturingMode == TEXTURE_2D)&&(group.getModel()->isEnabled(PARAM_TEXTURE_INDEX)))
		{
			if (group.getModel()->isEnabled(PARAM_ANGLE))
				renderParticle = &YONQuadRenderer::renderAtlasRot;
			else
				renderParticle = &YONQuadRenderer::renderAtlas;
		}
		else
		{
			if (group.getModel()->isEnabled(PARAM_ANGLE))
				renderParticle = &YONQuadRenderer::renderRot;
			else
				renderParticle = &YONQuadRenderer::renderBasic;
		}

		// At the first frame we pass the full buffer so that VBOs are correctly initialised
		// Then at next frames we pass only what is needed to be rendered
		if (currentBuffer->areVBOInitialized())
			currentBuffer->setUsed(group.getNbParticles());
		else
			currentBuffer->setUsed(group.getParticles().getNbReserved());

		bool globalOrientation = precomputeOrientation3D(
			group,
			Vector3D(invModelView[8],invModelView[9],invModelView[10]),
			Vector3D(invModelView[4],invModelView[5],invModelView[6]),
			Vector3D(invModelView[12],invModelView[13],invModelView[14]));

		if (globalOrientation)
		{
			computeGlobalOrientation3D();

			for (size_t t = 0; t < group.getNbParticles(); ++t)
				(this->*renderParticle)(group.getParticle(t));
		}
		else
		{
			for (size_t t = 0; t < group.getNbParticles(); ++t)
			{
				const Particle& particle = group.getParticle(t);
				computeSingleOrientation3D(particle);
				(this->*renderParticle)(particle);
			}
		}
		currentBuffer->getMeshBuffer().setDirty(yon::scene::EBT_VERTEX);

		driver->setMaterial(material);
		driver->drawMeshBuffer(&currentBuffer->getMeshBuffer()); // this draw call is used in order to be able to use VBOs

		currentBuffer->setVBOInitialized(true);
		material.TextureLayer[0].Texture = savedTexture; // Restores the texture
	}

	void YONQuadRenderer::renderBasic(const Particle& particle) const
	{
		scaleQuadVectors(particle,scaleX,scaleY);
		FillBufferColorAndVertex(particle);
	}

	void YONQuadRenderer::renderRot(const Particle& particle) const
	{
		rotateAndScaleQuadVectors(particle,scaleX,scaleY);
		FillBufferColorAndVertex(particle);
	}

	void YONQuadRenderer::renderAtlas(const Particle& particle) const
	{
		scaleQuadVectors(particle,scaleX,scaleY);
		FillBufferColorAndVertex(particle);
		FillBufferTextureAtlas(particle);
	}

	void YONQuadRenderer::renderAtlasRot(const Particle& particle) const
	{
		rotateAndScaleQuadVectors(particle,scaleX,scaleY);
		FillBufferColorAndVertex(particle);
		FillBufferTextureAtlas(particle);
	}
}}
