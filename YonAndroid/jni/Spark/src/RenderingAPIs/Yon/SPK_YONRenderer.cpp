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

#include "RenderingAPIs/Yon/SPK_YONRenderer.h"

namespace SPK
{
namespace YON
{
	YONRenderer::YONRenderer(yon::IYonEngine* d) :
		device(d),
		currentBuffer(NULL)
	{
		//material.GouraudShading = true;									// fix 1.05.00 for ATI cards
		//material.Lighting = false;										// No lights per default
		//material.BackfaceCulling = false;								// Deactivates backface culling
		material.CullingMode=yon::video::ENUM_CULLING_MODE_NONE;
		material.MaterialType = yon::video::ENUM_MATERIAL_TYPE_BLEND;	// To allow complex blending functions
		setBlending(BLENDING_NONE);										// Blending is disabled per default
	}

		void YONRenderer::setBlending(yon::video::ENUM_BLEND_FACTOR srcFunc,yon::video::ENUM_BLEND_FACTOR destFunc,yon::video::ENUM_ALPHA_SOURCE alphaSrc)
	{
		blendSrcFunc = srcFunc;
		blendDestFunc = destFunc;
		alphaSource = alphaSrc;
		updateMaterialBlendingMode();
	}

	void YONRenderer::setBlending(BlendingMode blendMode)
	{
		switch(blendMode)
		{
		case BLENDING_NONE :
			blendSrcFunc = yon::video::ENUM_BLEND_FACTOR_ONE;
			blendDestFunc = yon::video::ENUM_BLEND_FACTOR_ZERO;
			alphaSource = yon::video::ENUM_ALPHA_SOURCE_NONE;
			break;

		case BLENDING_ADD :
			blendSrcFunc = yon::video::ENUM_BLEND_FACTOR_SRC_ALPHA;
			blendDestFunc = yon::video::ENUM_BLEND_FACTOR_ONE;
			//alphaSource = yon::video::ENUM_ALPHA_SOURCE_VERTEX | yon::video::ENUM_ALPHA_SOURCE_TEXTURE;
			alphaSource = yon::video::ENUM_ALPHA_SOURCE_NONE;
			break;

		case BLENDING_ALPHA :
			blendSrcFunc = yon::video::ENUM_BLEND_FACTOR_SRC_ALPHA;
			blendDestFunc = yon::video::ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			//alphaSource = yon::video::ENUM_ALPHA_SOURCE_VERTEX | yon::video::ENUM_ALPHA_SOURCE_TEXTURE;
			alphaSource = yon::video::ENUM_ALPHA_SOURCE_NONE;
			break;
		}
		updateMaterialBlendingMode();
	}

	void YONRenderer::enableRenderingHint(RenderingHint renderingHint,bool enable)
	{
		switch(renderingHint)
		{
		case DEPTH_TEST :
			//TODO
			//material.ZBuffer = (enable ? 1 : 0);
			break;

		case DEPTH_WRITE :
			//TODO
			//material.ZWriteEnable = enable;
			break;
		}
	}

	bool YONRenderer::isRenderingHintEnabled(RenderingHint renderingHint) const
	{
		switch(renderingHint)
		{
		case DEPTH_TEST :
			//TODO
			//return material.ZBuffer != 0;

		case DEPTH_WRITE :
			//TODO
			//return material.ZWriteEnable;

		case ALPHA_TEST :
			return true; // always enabled in the yonlicht material
		}

		return false;
	}
}}
