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

#include "RenderingAPIs/Yon/SPK_YONBuffer.h"

namespace SPK
{
namespace YON
{
	bool YONBuffer::useVBO = false;

	YONBuffer::YONBuffer(yon::IYonEngine* device,size_t nbParticles,size_t particleVertexSize,size_t particleIndexSize,size_t indexType) :
		Buffer(),
		device(device),
		meshBuffer(),
		nbParticles(nbParticles),
		particleVertexSize(particleVertexSize),
		particleIndexSize(particleIndexSize),
		VBOInitialized(false)
	{
		initInnerBuffers();
	}

	YONBuffer::YONBuffer(const YONBuffer& buffer) :
		Buffer(buffer),
		device(buffer.device),
		meshBuffer(),
		nbParticles(buffer.nbParticles),
		particleVertexSize(buffer.particleVertexSize),
		particleIndexSize(buffer.particleIndexSize),
		VBOInitialized(false)
	{
		initInnerBuffers();
	}

	YONBuffer::~YONBuffer()
	{
		//TODO
		//device->getVideoDriver()->removeHardwareBuffer(&meshBuffer);
	}

	void YONBuffer::initInnerBuffers()
	{
		if (useVBO) // VBOs are at the moment only usable with triangles in Yon
		{
			//meshBuffer.setHardwareMappingHint(yon::scene::EHM_STREAM,yon::scene::EBT_VERTEX);
			//meshBuffer.setHardwareMappingHint(yon::scene::EHM_STATIC,yon::scene::EBT_INDEX);
			meshBuffer.setVertexHardwareBufferUsageType(yon::video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);
			meshBuffer.setIndexHardwareBufferUsageType(yon::video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
		}
		else
		{
			//meshBuffer.setHardwareMappingHint(yon::scene::EHM_NEVER,yon::scene::EBT_VERTEX_AND_INDEX);
			meshBuffer.setVertexHardwareBufferUsageType(yon::video::ENUM_HARDWARDBUFFER_USAGE_TYPE_NONE);
			meshBuffer.setIndexHardwareBufferUsageType(yon::video::ENUM_HARDWARDBUFFER_USAGE_TYPE_NONE);
		}

		// Allocates enough space
		getVertexBuffer().set_used(nbParticles * particleVertexSize);
		getIndexBuffer().set_used(nbParticles * particleIndexSize);
	}

	void YONBuffer::swap(size_t index0,size_t index1)
	{
		// Not useful but implemented for consistency
		size_t startIndex0 = index0 * particleVertexSize;
		size_t startIndex1 = index1 * particleVertexSize;
		for (size_t i = 0; i < particleVertexSize; ++i)
			std::swap(getVertexBuffer()[startIndex0 + i],getVertexBuffer()[startIndex1 + i]);
		//meshBuffer.setDirty(yon::scene::EBT_VERTEX);
		meshBuffer.setVerticesDirty();
	}

	YONBufferCreator::YONBufferCreator(yon::IYonEngine* device,size_t particleVertexSize,size_t particleIndexSize,E_YONSPK_INDEXMODE indexType) :
		BufferCreator(),
		device(device),
		particleVertexSize(particleVertexSize),
		particleIndexSize(particleIndexSize),
		indexType(indexType)
	{}

	YONBuffer* YONBufferCreator::createBuffer(size_t nbParticles, const Group& group) const
	{
		YONBuffer* buffer = NULL;

		/*
		switch(indexType)
		{
			case EII_32BITS :
				 return new YONBuffer(device,nbParticles,particleVertexSize,particleIndexSize,yon::video::EIT_32BIT);

			case EII_16BITS :
				if(nbParticles * particleVertexSize > 65536)
				{
					device->getLogger()->log(L"Unable to create particle buffer using 16bits indices : too many particles\n",yon::ELL_WARNING);
					return NULL;
				}
				else
					return new YONBuffer(device,nbParticles,particleVertexSize,particleIndexSize,yon::video::EIT_16BIT);

			case EII_AUTO :
				if (nbParticles * particleVertexSize > 65536)
					return new YONBuffer(device,nbParticles,particleVertexSize,particleIndexSize,yon::video::EIT_32BIT);
				else
					return new YONBuffer(device,nbParticles,particleVertexSize,particleIndexSize,yon::video::EIT_16BIT);

			default :
			{
				device->getLogger()->log(L"Unknown index mode !\n",yon::ELL_INFORMATION);
				return NULL;
			}
		}*/
		return new YONBuffer(device,nbParticles,particleVertexSize,particleIndexSize,16);
	}

	void YONBuffer::activateVBOHint(bool activate)
	{
		useVBO = activate;
	}
}}


























	//YONBuffer::YONBuffer(yon::video::IVideoDriver* d, int it) :
	//	yon::scene::SMeshBuffer(),
	//	Purpose(0),
	//	driver(d)
 //   {
	//	if(it == 32)
	//		IndexType = yon::video::EIT_32BIT;
	//	else
	//		IndexType = yon::video::EIT_16BIT;
	//}

 //   void YONBuffer::constructIndicesForPointRendering()
 //   {
 //       Purpose=0;

 //       if(IndexType==yon::video::EIT_32BIT)
 //       {
 //           Indices_32.set_used(Vertices.size());
 //           for(unsigned int t=0; t<Indices_32.size(); t++)
 //               Indices_32[t] = t;
 //       }
 //       else
 //       {
 //           Indices.set_used(Vertices.size());
 //           for(unsigned int t=0; t<Indices.size(); t++)
 //               Indices[t] = t;
 //       }

 //       setDirty(yon::scene::EBT_INDEX);
 //   }

 //   void YONBuffer::constructIndicesForLineRendering()
 //   {
 //       Purpose=1;

 //       if(IndexType==yon::video::EIT_32BIT)
 //       {
 //           Indices_32.set_used(Vertices.size());
 //           for(unsigned int t=0; t<Indices_32.size(); t++)
 //               Indices_32[t] = t;
 //       }
 //       else
 //       {
 //               Indices.set_used(Vertices.size());
 //           for(unsigned int t=0; t<Indices.size(); t++)
 //               Indices[t] = t;
 //       }

 //       setDirty(yon::scene::EBT_INDEX);
 //   }

 //   void YONBuffer::constructIndicesForQuadRendering(unsigned int nbP)
 //   {
 //       Purpose=2;

 //       if(IndexType==yon::video::EIT_32BIT)
 //       {
 //           Indices_32.set_used(nbP*6);
 //           for(unsigned int t=0; t<nbP; t++)
 //           {
 //               Indices_32[6*t+0] = 4*t+0;
 //               Indices_32[6*t+1] = 4*t+1;
 //               Indices_32[6*t+2] = 4*t+2;
 //               Indices_32[6*t+3] = 4*t+0;
 //               Indices_32[6*t+4] = 4*t+2;
 //               Indices_32[6*t+5] = 4*t+3;
 //           }
 //       }
 //       else
 //       {
 //           Indices.set_used(nbP*6);
 //           for(unsigned int t=0; t<nbP; t++)
 //           {
 //               Indices[6*t+0] = 4*t+0;
 //               Indices[6*t+1] = 4*t+1;
 //               Indices[6*t+2] = 4*t+2;
 //               Indices[6*t+3] = 4*t+0;
 //               Indices[6*t+4] = 4*t+2;
 //               Indices[6*t+5] = 4*t+3;
 //           }
 //       }

 //       setDirty(yon::scene::EBT_INDEX);
 //   }

 //   void YONBuffer::set(unsigned int nbParticles, int purpose)
 //   {
 //       if(purpose>=0 && purpose<3)
 //       {
 //           Purpose=purpose;
 //           switch(Purpose)
 //           {
 //               case 0:
 //               {
 //                   Vertices.set_used(nbParticles);
 //                   for(unsigned int t = 0; t < Vertices.size(); t++)
 //                       Vertices[t] = yon::video::S3DVertex();
 //                   constructIndicesForPointRendering();
 //                   break;
 //               }
 //               case 1:
 //               {
 //                   Vertices.set_used(nbParticles*2);
 //                   for(unsigned int t = 0; t < Vertices.size(); t++)
 //                       Vertices[t] = yon::video::S3DVertex();
 //                   constructIndicesForLineRendering();
 //                   break;
 //               }
 //               case 2:
 //               {
 //                   Vertices.set_used(nbParticles*4);
 //                   for(unsigned int t=0; t<nbParticles; t++)
 //                   {
 //                       Vertices[4*t+0] = Vertices[4*t+1] = Vertices[4*t+2] = Vertices[4*t+3] = yon::video::S3DVertex();
 //                       Vertices[4*t+0].TCoords = yon::core::vector2df(0,0);
 //                       Vertices[4*t+1].TCoords = yon::core::vector2df(1,0);
 //                       Vertices[4*t+2].TCoords = yon::core::vector2df(1,1);
 //                       Vertices[4*t+3].TCoords = yon::core::vector2df(0,1);
 //                   }
 //                   constructIndicesForQuadRendering(nbParticles);
 //                   break;
 //               }
 //           }
 //       }
 //   }

	//yon::u16* YONBuffer::getIndices()
 //   {
 //       if(IndexType==yon::video::EIT_32BIT)
 //           return (yon::u16*)Indices_32.pointer();
 //       else
 //           return Indices.pointer();
 //   }

 //   const yon::u16* YONBuffer::getIndices() const
 //   {
 //       if(IndexType==yon::video::EIT_32BIT)
 //           return (yon::u16*)Indices_32.const_pointer();
 //       else
 //           return Indices.const_pointer();
 //   }

 //   yon::u32 YONBuffer::getIndexCount() const
 //   {
 //       if(IndexType==yon::video::EIT_32BIT)
 //           return Indices_32.size();
 //       else
 //           return Indices.size();
 //   }

 //   unsigned int YONBuffer::getPrimCount() const
 //   {
 //       switch(Purpose)
 //       {
 //           case 0: return Vertices.size();
 //           case 1: return Vertices.size()/2;
 //           case 2: return Vertices.size()/2;
 //           default: return 0;
 //       }
 //   }

 //   yon::scene::E_PRIMITIVE_TYPE YONBuffer::getPrimType() const
 //   {
 //       switch(Purpose)
 //       {
 //           case 0: return yon::scene::EPT_POINTS;
 //           case 1: return yon::scene::EPT_LINES;
 //           case 2: return yon::scene::EPT_TRIANGLES;
 //           default: return yon::scene::EPT_POINTS;
 //       }
 //   }

 //   void YONBuffer::swap(size_t index0, size_t index1)
 //   {
 //       switch(Purpose)
 //       {
 //           case 0: { swapPoints(index0,index1); break; }
 //           case 1: { swapLines(index0,index1); break; }
 //           case 2: { swapQuads(index0,index1); break; }
 //       }
 //   }

	//YONBufferCreator::YONBufferCreator(yon::IYonEngine* d,int purpose,E_YONSPK_INDEXMODE im) :
	//	p(purpose),
	//	m(im),
	//	device(d)
	//{}

	//YONBuffer* YONBufferCreator::createBuffer(size_t nbParticles, const Group& group) const
 //   {
 //       YONBuffer* buff = 0;
 //       if(m == EII_32BITS)
 //           buff = new YONBuffer(device->getVideoDriver(),32);
 //       else if(m == EII_16BITS)
 //       {
 //           if(nbParticles > (unsigned int)65535/(p==0?1:2*p))
 //           {
 //               device->getLogger()->log(L"Unable to create particle buffer using 16bits indices : too much particles\n",yon::ELL_WARNING);
 //               return 0;
 //           }
 //           else
 //               buff = new YONBuffer(device->getVideoDriver(),16);
 //       }
 //       else
 //       {
 //           if(m != EII_AUTO)
 //           {
 //               wchar_t temp[12];
 //               _snwprintf_s(temp,12,L"%i",(int)m);
 //               device->getLogger()->log(L"Unknown index mode, switching to EII_AUTO; specified value\n",temp,yon::ELL_INFORMATION);
 //           }
 //           if(nbParticles > (unsigned int)65535/(p==0?1:2*p))
 //               buff = new YONBuffer(device->getVideoDriver(),32);
 //           else
 //               buff = new YONBuffer(device->getVideoDriver(),16);
 //       }
 //       buff->set(nbParticles,p);
 //       return buff;
 //   }
