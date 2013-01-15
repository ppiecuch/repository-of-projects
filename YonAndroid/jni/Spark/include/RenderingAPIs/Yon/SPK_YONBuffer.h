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

#ifndef H_SPK_YONBUFFER
#define H_SPK_YONBUFFER

#include "RenderingAPIs/Yon/SPK_YON_DEF.h"
#include "Core/SPK_Buffer.h"

namespace SPK
{
namespace YON
{
	/**
	* @enum E_YONSPK_INDEXMODE
	* @brief Constant defining the way Yon index buffer are stored
	* @since 1.04.00
	*/
    enum E_YONSPK_INDEXMODE
    {
		//EII_16BITS = yon::video::EIT_16BIT,	/**< Always use 16bits indices. Any attempt to create a buffer with more particles will fail */
     	//EII_32BITS = yon::video::EIT_32BIT,	/**< Try to always use 32bits indices. If not available (old graphic cards), setting this indexmode will result in an undefined behavior */		
		EII_AUTO,							/**< Choose the best index mode according to the group size */
    };

	/**
	* @brief A buffer to store particle mesh data to be rendered with Yon
	*
	* The buffer can hold an index buffer of either 16 bits or 32 bits indices.<br>
	* Moreover, it can be used as VBO and store on the GPU side in an optimized way.
	*
	* @since 1.04.00
	*/
    class SPK_YON_PREFIX YONBuffer : public Buffer
    {
	friend class YONBufferCreator;

    public:

		/////////////
		// Setters //
		/////////////

		/**
		* @brief Tells whether VBOs for this buffer are initialized or not
		* 
		* This is used internally by the renderers and may not be called by the user.
		*
		* @param init : true if VBOs are initialized, false of not
		*/
		void setVBOInitialized(bool init);

		/**
		* @brief Tells the amount of data that are used by the buffer
		*
		* This is used internally by the renderers and may not be called by the user.
		*
		* @param nb : the amount of data that are used by the buffer
		*/
		void setUsed(size_t nb);


		/////////////
		// Getters //
		/////////////

		/**
		* @brief Gets the inner vertex buffer
		* @return the inner vertex buffer
		*/
		//yon::scene::IVertexBuffer& getVertexBuffer();
		yon::core::array<yon::scene::SVertex>& getVertexBuffer();

		/**
		* @brief Gets the inner index buffer
		* @return the inner index buffer
		*/
		//yon::scene::IIndexBuffer& getIndexBuffer();
		yon::core::array<yon::u16>& getIndexBuffer();

		/**
		* @brief Gets the inner mesh buffer
		* @return the inner mesh buffer
		*/
		//yon::scene::IDynamicMeshBuffer& getMeshBuffer();
		yon::scene::SDynamicShap3D& getMeshBuffer();

		/**
		* @brief Tells whether VBOs are initialized for this buffer
		* 
		* This is used internally by the renderers and may not be called by the user.
		*
		* @return true if VBOs are initialized, false of not
		*/
		bool areVBOInitialized() const;


		////////////
		// static //
		////////////
	
		/**
		* @brief Sets whether to try to store YONBuffers on GPU or not
		*
		* When creating an Yon buffer, the system checks whether the VBO is activated or not.<br>
		* If yes, VBOs may be used.<br>
		* <br>
		* Not that activating VBOs is only a hint, the Yon engine may not use them depending
		* on the buffer size, the renderer used and the graphic card.<br>
		* <br<
		* By default, VBOs are not used (the hint is deactivated)
		*
		* @param activate : true to try to use VBO when creating buffers, false not to
		*/
		static void activateVBOHint(bool activate);

		/**
		* @brief Tells whether VBO hint is activated or not
		* @return true if VBO hint is activated, alse if not
		* @since 1.05.03
		*/
		static bool isVBOHintActivated();

    private:

		yon::scene::SDynamicShap3D meshBuffer;

		yon::IYonEngine* device;

		size_t nbParticles;
		size_t particleVertexSize;
		size_t particleIndexSize;

		bool VBOInitialized;
		static bool useVBO;

		YONBuffer(yon::IYonEngine* device,size_t nbParticles,size_t particleVertexSize,size_t particleIndexSize,size_t indexType);
		YONBuffer(const YONBuffer& buffer);
		virtual ~YONBuffer();

		virtual void swap(size_t index0,size_t index1);

		void initInnerBuffers();
    };


	/**
	* @brief A buffer creator to create YONBuffers
	* @since 1.04.00
	*/
    class SPK_YON_PREFIX YONBufferCreator : public BufferCreator
    {
    public:

		/**
		* @brief Constructor of YONBufferCreator
		* @param device : A pointer to the Yon Device 
		* @param particleVertexSize : the number of vertices per particle
		* @param particleIndexSize : the number of indices per particle
		* @param indexType : the index type
		*/
		YONBufferCreator(yon::IYonEngine* device,size_t particleVertexSize,size_t particleIndexSize,E_YONSPK_INDEXMODE indexType = EII_AUTO);

    private:
	
		yon::IYonEngine* device;

		size_t particleVertexSize;
		size_t particleIndexSize;
        E_YONSPK_INDEXMODE indexType;

        virtual YONBuffer* createBuffer(size_t nbParticles, const Group& group) const;
    };


	//inline yon::scene::IVertexBuffer& YONBuffer::getVertexBuffer()
	inline yon::core::array<yon::scene::SVertex>& YONBuffer::getVertexBuffer()
	{
		//return meshBuffer.getVertexBuffer();
		return meshBuffer.getVertexArray();
	}

	//inline yon::scene::IIndexBuffer& YONBuffer::getIndexBuffer()
	inline yon::core::array<yon::u16>& YONBuffer::getIndexBuffer()
	{
		//return meshBuffer.getIndexBuffer();
		return meshBuffer.getIndexArray();
	}

	//inline yon::scene::IDynamicMeshBuffer& YONBuffer::getMeshBuffer()
	inline yon::scene::SDynamicShap3D& YONBuffer::getMeshBuffer()
	{
		return meshBuffer;
	}

	inline void YONBuffer::setUsed(size_t nb)
	{
		if (nb > nbParticles) // Prevents the buffers from growing
			nb = nbParticles;

		getVertexBuffer().set_used(nb * particleVertexSize);
		getIndexBuffer().set_used(nb * particleIndexSize);
	}

	inline bool YONBuffer::areVBOInitialized() const
	{
		return VBOInitialized;
	}

	inline void YONBuffer::setVBOInitialized(bool init)
	{
		VBOInitialized = init;
	}

	inline bool YONBuffer::isVBOHintActivated()
	{
		return useVBO;
	}
}}

#endif
