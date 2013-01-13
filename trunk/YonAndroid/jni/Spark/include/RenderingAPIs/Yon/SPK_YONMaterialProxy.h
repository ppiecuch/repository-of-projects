//////////////////////////////////////////////////////////////////////////////////
// SPARK Yon Rendering library												//
// Copyright (C) 2011															//
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

#ifndef SPK_YONMATERIALPROXY
#define SPK_YONMATERIALPROXY

#include "RenderingAPIs/Yon/SPK_YON_DEF.h"

namespace SPK
{
namespace YON
{
	/**
	* @brief A Proxy for Yon material
	* As some properties of the material are controlled by SPARK and must not be altered by user, a proxy is used to access the Yon material
	* @since 1.5.6
	*/
	class YONMaterialProxy
	{
	public :

		YONMaterialProxy(yon::video::SMaterial& material) : material(material) {}

		const yon::video::SMaterial& getMaterial() const { return material; }

		void setAntiAliasing(yon::u8 aa)		{ material.AntiAliasing = aa; }
		yon::u8 getAntiAliasing() const			{ return material.AntiAliasing; }

		void setFogEnable(bool enable)			{ material.FogEnable = enable; }
		bool getFogEnable() const				{ return material.FogEnable; }

		void setPointCloud(bool enable)			{ material.PointCloud = enable; }
		bool getPointCloud() const				{ return material.PointCloud; }

		void setWireframe(bool enable)			{ material.Wireframe = enable; }
		bool getWireframe() const				{ return material.Wireframe; }

	private :

		yon::video::SMaterial& material;
	};
}}

#endif
