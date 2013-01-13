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

#ifndef SPK_YONLINERENDERER
#define SPK_YONLINERENDERER

#include "RenderingAPIs/Yon/SPK_YONRenderer.h"
#include "Extensions/Renderers/SPK_LineRendererInterface.h"

namespace SPK
{
namespace YON
{
	/**
	* @class YONLineRenderer
	* @brief A Renderer drawing particles as lines with Yon
	*
	* The length of the lines is function of the Particle velocity and is defined in the universe space
	* while the width is fixed and defines in the screen space (in pixels).<br>
	* Note that the width only works when using Yon with OpenGL. With Direct3D, the parameter is ignored and 1 is used instead.
	* <br>
	* Below are the parameters of Particle that are used in this Renderer (others have no effects) :
	* <ul>
	* <li>SPK::PARAM_RED</li>
	* <li>SPK::PARAM_GREEN</li>
	* <li>SPK::PARAM_BLUE</li>
	* <li>SPK::PARAM_ALPHA (only if blending is enabled)</li>
	* </ul>
	* @since 1.04.00
	*/
	class SPK_YON_PREFIX YONLineRenderer : public YONRenderer,public LineRendererInterface
	{
		SPK_IMPLEMENT_REGISTERABLE(YONLineRenderer)

	public :

		/////////////////
		// Constructor //
		/////////////////

		/**
		* @brief Constructor of YONLineRenderer
		* @param d : the Yon device
		* @param length : the length multiplier of this YONLineRenderer
		* @param width : the width of this YONLineRenderer in pixels
		*/
		YONLineRenderer(yon::YonDevice* d,float length = 1.0f,float width = 1.0f);

		/**
		* @brief Creates and registers a new YONLineRenderer
		* @param d : the Yon device
		* @param length : the length multiplier of this YONLineRenderer
		* @param width : the width of this YONLineRenderer in pixels
		* @return A new registered YONLineRenderer
		*/
		static YONLineRenderer* create(yon::YonDevice* d,float length = 1.0f,float width = 1.0f);

		/////////////
		// Setters //
		/////////////

		virtual void setWidth(float width);

		///////////////
		// Interface //
		///////////////

		virtual void render(const Group& group);

		void createBuffers(const Group& group);

	private :

		static const size_t NB_INDICES_PER_QUAD = 2;
		static const size_t NB_VERTICES_PER_QUAD = 2;

		// buffer name
		static const std::string YON_BUFFER_NAME;

		virtual const std::string& getBufferName() const;
	};


	inline YONLineRenderer* YONLineRenderer::create(yon::YonDevice* d,float length,float width)
	{
		YONLineRenderer* obj = new YONLineRenderer(d,length,width);
		registerObject(obj);
		return obj;
	}

	inline void YONLineRenderer::setWidth(float width)
	{
		material.Thickness = this->width = width;
	}

	inline const std::string& YONLineRenderer::getBufferName() const
	{
		return YON_BUFFER_NAME;
	}
}}

#endif
