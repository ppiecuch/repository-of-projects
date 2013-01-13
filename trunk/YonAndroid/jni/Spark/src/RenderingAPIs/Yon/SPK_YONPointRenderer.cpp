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

#include "RenderingAPIs/Yon/SPK_YONPointRenderer.h"
#include "RenderingAPIs/Yon/SPK_YONBuffer.h"

namespace SPK
{
namespace YON
{
	const std::string YONPointRenderer::YON_BUFFER_NAME("SPK_YONPointRenderer_Buffer");

	YONPointRenderer::YONPointRenderer(yon::YonDevice* d,float size) :
		YONRenderer(d),
		PointRendererInterface(POINT_SQUARE,size)
	{
		material.Thickness = size;
	}

	bool YONPointRenderer::setType(PointType type)
	{
		if (type == POINT_CIRCLE)
			return false;

		this->type = type;
		return true;
	}

	void YONPointRenderer::createBuffers(const Group& group)
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
                indices[t] = t;
        }
        else
        {
            yon::u16* indices = reinterpret_cast<yon::u16*>(indexBuffer.pointer());
            for (size_t t = 0; t < nbTotalParticles; ++t)
                indices[t] = t;
        }

		currentBuffer->getMeshBuffer().setDirty(yon::scene::EBT_INDEX);
		currentBuffer->setVBOInitialized(true);
	}

	void YONPointRenderer::render(const Group& group)
	{
		if (!prepareBuffers(group))
			return;

		for (size_t t = 0; t < group.getNbParticles(); ++t)
		{
			const Particle& p = group.getParticle(t);
			currentBuffer->getVertexBuffer()[t].Pos = spk2yon(p.position());
			currentBuffer->getVertexBuffer()[t].Color = spk2yon(p.getParamCurrentValue(PARAM_ALPHA),p.getR(),p.getG(),p.getB());
		}
		currentBuffer->getMeshBuffer().setDirty(yon::scene::EBT_VERTEX);

		yon::video::IVideoDriver* driver = device->getVideoDriver();
        driver->setMaterial(material);
        driver->drawVertexPrimitiveList(
			currentBuffer->getVertexBuffer().pointer(),
			group.getNbParticles(),
			currentBuffer->getIndexBuffer().pointer(),
			group.getNbParticles(),
			yon::video::EVT_STANDARD,
			type == POINT_SPRITE ? yon::scene::EPT_POINT_SPRITES : yon::scene::EPT_POINTS,
			currentBuffer->getIndexBuffer().getType());
	}
}}
