#include "CQuadtreeTerrain.h"
#include "CImage.h"
#include "SUnit.h"
#include "ISceneManager.h"
#include "yonMath.h"

#include "ILogger.h"

namespace yon{
namespace scene{
namespace terrain{

	//fan iFanCode table
	c8 g_cQTFanCode[] = { 10, 8, 8, 12, 8, 0, 12, 14, 8, 12, 0, 14, 12, 14, 14, 0 };
	c8 g_cQTFanStart[]= { 3,  3, 0,  3, 1, 0,  0,  3, 2,  2, 0,  2,  1,  1,  0, 0 };

	CQuadtreeTerrain::CQuadtreeTerrain(IModel* parent,const core::vector3df& pos,
		const core::vector3df& rot,const core::vector3df& scale,
		f32 desiredResolution,f32 minResolution)
		:ITerrainModel(parent,pos,rot,scale),m_pMatrix(NULL),m_pUnit(NULL),
		m_iSizePerSide(0),m_iImageSizePerSide(0),m_bDrawLine(false),
		m_fDesiredResolution(desiredResolution),m_fMinResolution(minResolution)
	{
		m_pUnit=new Unit3D2T();
		//m_pUnit->setVertexHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
		//m_pUnit->setIndexHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);
		//m_pUnit->getMaterial()->setFrontFace(video::ENUM_FRONT_FACE_CW);
		m_pUnit->getMaterial()->setPolygonMode(video::ENUM_POLYGON_MODE_LINE);
		//m_pUnit->setShap(&m_shap);
	}
	CQuadtreeTerrain::~CQuadtreeTerrain(){
		m_pUnit->drop();
		if(m_pMatrix)
			delete[] m_pMatrix;
	}

	void CQuadtreeTerrain::loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize)
	{
		if(image==NULL)
			return;
		YON_DEBUG_BREAK_IF(image->getDimension().w!=image->getDimension().h);

		//TODO release old data
		//TODO use temp buffer to load, for swapping heightmap

		m_iImageSizePerSide=image->getDimension().w;
		m_iSizePerSide=(core::nearestPowerOf2(m_iImageSizePerSide)>>1)+1;

		Logger->debug("image.w:%d,sizePerSide:%d\r\n",m_iImageSizePerSide,m_iSizePerSide);

		//initialize the quadtree matrix to 'true' (for all possible nodes)
		//m_pMatrix=new bool[m_iSizePerSide*m_iSizePerSide];
		m_pMatrix=new u8[m_iSizePerSide*m_iSizePerSide];
		for(s32 x=0;x<m_iSizePerSide;++x)
		{
			for(s32 z=0;z<m_iSizePerSide;++z)
			{
				//m_pMatrix[x*m_iSizePerSide+z]=true;
				m_pMatrix[x*m_iSizePerSide+z]=1;
			}
		}

		const u32 numVertices=m_iSizePerSide*m_iSizePerSide;
		//m_shap.getVertexArray().set_used(numVertices);
		m_vertices.set_used(numVertices);

		const f32 texcoordDelta = 1.0f/(f32)(m_iImageSizePerSide-1);
		const f32 texcoordDelta2 = 1.0f/(f32)(patchSize-1);
		s32 index=0;
		f32 fx=0.f;
		f32 fv=0.f;
		f32 fv2=0.f;
		for(s32 x = 0; x<m_iSizePerSide; ++x)
		{
			f32 fz=0.f;
			f32 fu=0.f;
			f32 fu2=0.f;
			for(s32 z = 0; z<m_iSizePerSide; ++z)
			{
				SVertex2TCoords& v=m_vertices[index++];
				v.pos.x=fx*m_scale.x;
				v.pos.y=(f32)image->getValue(z,x)*m_scale.y;
				v.pos.z=fz*m_scale.z;

				v.texcoords0.x=fu;
				v.texcoords0.y=fv;

				v.texcoords1.x=fu2;
				v.texcoords1.y=fv2;

				//Logger->debug("vertex[%d]:%.2f,%.2f,%.2f\r\n",index-1,v.pos.x,v.pos.y,v.pos.z);

				++fz;
				fu+=texcoordDelta;
				fu2+=texcoordDelta2;
			}
			++fx;
			fv+=texcoordDelta;
			fv2+=texcoordDelta2;
		}

		propagateRoughness();

		/*for(s32 x=m_iSizePerSide-1;x>=0;--x)
		{
			core::stringc str;
			for(s32 z=0;z<m_iSizePerSide;++z)
			{
				str.append(core::stringc("%d,",m_pMatrix[x*m_iSizePerSide+z]));
			}
			Logger->debug("%s\r\n",str.c_str());
		}*/
	}

	void CQuadtreeTerrain::fillData(){
		// Reset the Terrains Bounding Box for re-calculation
		m_boundingBox = core::aabbox3df(true);
	}

	void CQuadtreeTerrain::refine(s32 x,s32 z,s32 edgeLength,bool center,core::vector3df& cameraPos,camera::IViewFrustum* viewFrustum){
		//calculate the distance between current node and camera
		s32 index=x*m_iSizePerSide+z;

		//test the node's bounding box against the view frustm
		if(viewFrustum->intersectWithBox(m_vertices[index].pos,edgeLength*m_scale.x)==false)
		{
			//disable this node, and return (since the parent node is disabled, we don't need
			//to waste any CPU cycles by traversing down the tree even farther
			m_pMatrix[index]= 0;
			return;
		}

		f32 distance=calculateL1Norm(m_vertices[index].pos,cameraPos);

		//bool subdivide;
		u8 blend;
		
		//compute the 'f' value (as stated in Roettger's whitepaper of this algorithm)
		//f32 d2=calculateD2(index,edgeLength);
		//f32 f=calculateF(distance,edgeLength,d2);

		//why should "index-1" or "index+1" but can not be "index"?
		//f32 d2=m_fDesiredResolution*m_pMatrix[index]/3;
		//f32 f=distance/(edgeLength*m_fMinResolution*core::max_(d2,1.0f));

		f32 d2=m_fDesiredResolution*(f32)m_pMatrix[index-1]/12;
		f32 f=distance/(edgeLength*m_fMinResolution*core::max_(d2,1.0f));

		//if( f<1.0f )
		//	subdivide= true;		
		//else
		//	subdivide= false;
		//
		//if(center)
		//{
		//	subdivide=true;
		//}
		if( f<1.0f )
			blend= 255;		
		else
			blend= 0;

		if(center)
		{
			blend=255;
		}

		//store whether or not the current node gets subdivided
		//m_pMatrix[index]=subdivide;
		m_pMatrix[index]=blend;

		//Logger->debug("refine|x:%d,z:%d,edgeLength:%d,index:%d,distance:%.2f,d2:%.2f,f:%.2f,subdivide:%s\r\n",x,z,edgeLength,index,distance,d2,f,subdivide?"true":"false");
		//Logger->debug("refine|x:%d,z:%d,edgeLength:%d,index:%d,distance:%.2f,f:%.2f,subdivide:%s\r\n",x,z,edgeLength,index,distance,f,subdivide?"true":"false");

		//if(subdivide)
		if(blend!=0)
		{
			if(edgeLength>3)
			{
				s32 childOffset=(edgeLength-1)>>2;
				s32 childEdgeLength=(edgeLength+1)>>1;

				//refine the children nodes
				refine(x-childOffset,z-childOffset,childEdgeLength,false,cameraPos,viewFrustum);
				refine(x-childOffset,z+childOffset,childEdgeLength,false,cameraPos,viewFrustum);
				refine(x+childOffset,z-childOffset,childEdgeLength,false,cameraPos,viewFrustum);
				refine(x+childOffset,z+childOffset,childEdgeLength,false,cameraPos,viewFrustum);
			}
		}
	}

	void CQuadtreeTerrain::render(video::IVideoDriver* driver)
	{
		driver->setTransform(video::ENUM_TRANSFORM_WORLD, core::IDENTITY_MATRIX);
		driver->setMaterial(getMaterial(0));
		s32 center=(m_iSizePerSide-1)>>1;
		renderNode(center,center,m_iSizePerSide,driver);
	}
	void CQuadtreeTerrain::_pre(){
		if(m_indices.size()>=2){
			if(m_bDrawLine)
				m_indices.push_back(m_indices[m_indices.size()-1]);
		}
	}
	void CQuadtreeTerrain::_post(){
		if(m_indices.size()>=3){
			if(m_bDrawLine)
			{
				m_indices.push_back(m_indices[0]);
				m_indices.push_back(m_indices[m_indices.size()-2]);
			}
		}
	}
#define _QUADTREE_PRE_PUSH_ \
	_pre();
#define _QUADTREE_POST_PUSH_ \
	_post();
	void CQuadtreeTerrain::printIndices(const char* name)
	{
		core::stringc str;
		for(u32 i=0;i<m_indices.size();++i)
			str.append(core::stringc("%d,",m_indices[i]));
		Logger->debug("%s:%s\r\n",name,str.c_str());
	}
	
	void CQuadtreeTerrain::renderNode(s32 x,s32 z,s32 edgeLength,video::IVideoDriver* driver)
	{
		s32 index=x*m_iSizePerSide+z;
		//bool subdivide=m_pMatrix[index];
		u8 blend=m_pMatrix[index];

		s32 offset=(edgeLength-1)>>1;
		s32 neighbourOffset=edgeLength-1;

		//Logger->debug("render|x:%d,z:%d,edgeLength:%d,index:%d,subdivide:%s,offset:%d,neighbourOffset:%d\r\n",x,z,edgeLength,index,subdivide?"true":"false",offset,neighbourOffset);

		//if(subdivide)
		if(blend!=0)
		{
			//is this the smallest node?
			if(edgeLength<=3)
			{
				m_indices.set_used(0);
				m_indices.push_back(getIndex(x,z));
				m_indices.push_back(getIndex(x-offset,z-offset));
				//if((x-neighbourOffset)<0||m_pMatrix[(x-neighbourOffset)*m_iSizePerSide+z]==true)
				if((x-neighbourOffset)<0||m_pMatrix[(x-neighbourOffset)*m_iSizePerSide+z]!=0)
				{
					_QUADTREE_PRE_PUSH_
					m_indices.push_back(getIndex(x-offset,z));
					_QUADTREE_POST_PUSH_
				}
				_QUADTREE_PRE_PUSH_
				m_indices.push_back(getIndex(x-offset,z+offset));
				_QUADTREE_POST_PUSH_
				//if((z+neighbourOffset)>=m_iSizePerSide||m_pMatrix[x*m_iSizePerSide+z+neighbourOffset]==true)
				if((z+neighbourOffset)>=m_iSizePerSide||m_pMatrix[x*m_iSizePerSide+z+neighbourOffset]!=0)
				{
					_QUADTREE_PRE_PUSH_
					m_indices.push_back(getIndex(x,z+offset));
					_QUADTREE_POST_PUSH_
				}
				_QUADTREE_PRE_PUSH_
				m_indices.push_back(getIndex(x+offset,z+offset));
				_QUADTREE_POST_PUSH_
				//if((x+neighbourOffset)>=m_iSizePerSide||m_pMatrix[(x+neighbourOffset)*m_iSizePerSide+z]==true)
				if((x+neighbourOffset)>=m_iSizePerSide||m_pMatrix[(x+neighbourOffset)*m_iSizePerSide+z]!=0)
				{
					_QUADTREE_PRE_PUSH_
					m_indices.push_back(getIndex(x+offset,z));
					_QUADTREE_POST_PUSH_
				}
				_QUADTREE_PRE_PUSH_
				m_indices.push_back(getIndex(x+offset,z-offset));
				_QUADTREE_POST_PUSH_
				//if((z-neighbourOffset)<0||m_pMatrix[x*m_iSizePerSide+z-neighbourOffset]==true)
				if((z-neighbourOffset)<0||m_pMatrix[x*m_iSizePerSide+z-neighbourOffset]!=0)
				{
					_QUADTREE_PRE_PUSH_
					m_indices.push_back(getIndex(x,z-offset));
					_QUADTREE_POST_PUSH_
				}
				_QUADTREE_PRE_PUSH_
				m_indices.push_back(getIndex(x-offset,z-offset));
				_QUADTREE_POST_PUSH_
				//printIndices("BASE");
				driver->drawVertexPrimitiveList(m_vertices.const_pointer(),m_vertices.size(),m_indices.const_pointer(),m_indices.size(),m_bDrawLine?video::ENUM_PRIMITIVE_TYPE_LINES:video::ENUM_PRIMITIVE_TYPE_TRIANGLE_FAN,m_pUnit->getVertexType());
			}
			else
			{
				s32 childOffset=(edgeLength-1)>>2;
				s32 childEdgeLength=(edgeLength+1)>>1;

				//calculate the mask code, which specifies which fans need to be rendered.
				s32 mask=0;
				mask|=m_pMatrix[(x+childOffset)*m_iSizePerSide+z+childOffset]?0x8:0;//TR
				mask|=m_pMatrix[(x+childOffset)*m_iSizePerSide+z-childOffset]?0x4:0;//TL
				mask|=m_pMatrix[(x-childOffset)*m_iSizePerSide+z-childOffset]?0x2:0;//BL
				mask|=m_pMatrix[(x-childOffset)*m_iSizePerSide+z+childOffset]?0x1:0;//BR

				//Logger->debug("mask:%d,childOffset:%d,childEdgeLength:%d\r\n",mask,childOffset,childEdgeLength);

				switch(mask)
				{
				case 15://no fans
					{
						renderNode(x-childOffset,z-childOffset,childEdgeLength,driver);
						renderNode(x-childOffset,z+childOffset,childEdgeLength,driver);
						renderNode(x+childOffset,z-childOffset,childEdgeLength,driver);
						renderNode(x+childOffset,z+childOffset,childEdgeLength,driver);
					}
					return;
				case 10://tr,bl
					{
						renderNode(x+childOffset,z+childOffset,childEdgeLength,driver);
						renderNode(x-childOffset,z-childOffset,childEdgeLength,driver);

						m_indices.set_used(0);
						m_indices.push_back(getIndex(x,z));
						m_indices.push_back(getIndex(x+offset,z));
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x+offset,z-offset));
						_QUADTREE_POST_PUSH_
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x,z-offset));
						_QUADTREE_POST_PUSH_
						driver->drawVertexPrimitiveList(m_vertices.const_pointer(),m_vertices.size(),m_indices.const_pointer(),m_indices.size(),m_bDrawLine?video::ENUM_PRIMITIVE_TYPE_LINES:video::ENUM_PRIMITIVE_TYPE_TRIANGLE_FAN,m_pUnit->getVertexType());

						m_indices.set_used(0);
						m_indices.push_back(getIndex(x,z));
						m_indices.push_back(getIndex(x-offset,z));
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x-offset,z+offset));
						_QUADTREE_POST_PUSH_
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x,z+offset));
						_QUADTREE_POST_PUSH_
						//printIndices("TR-BL");
						driver->drawVertexPrimitiveList(m_vertices.const_pointer(),m_vertices.size(),m_indices.const_pointer(),m_indices.size(),m_bDrawLine?video::ENUM_PRIMITIVE_TYPE_LINES:video::ENUM_PRIMITIVE_TYPE_TRIANGLE_FAN,m_pUnit->getVertexType());
					}
					return;
				case 5:	//tl,br
					{
						renderNode(x+childOffset,z-childOffset,childEdgeLength,driver);
						renderNode(x-childOffset,z+childOffset,childEdgeLength,driver);

						m_indices.set_used(0);
						m_indices.push_back(getIndex(x,z));
						m_indices.push_back(getIndex(x,z+offset));
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x+offset,z+offset));
						_QUADTREE_POST_PUSH_
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x+offset,z));
						_QUADTREE_POST_PUSH_
						driver->drawVertexPrimitiveList(m_vertices.const_pointer(),m_vertices.size(),m_indices.const_pointer(),m_indices.size(),m_bDrawLine?video::ENUM_PRIMITIVE_TYPE_LINES:video::ENUM_PRIMITIVE_TYPE_TRIANGLE_FAN,m_pUnit->getVertexType());

						m_indices.set_used(0);
						m_indices.push_back(getIndex(x,z));
						m_indices.push_back(getIndex(x,z-offset));
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x-offset,z-offset));
						_QUADTREE_POST_PUSH_
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x-offset,z));
						_QUADTREE_POST_PUSH_
						//printIndices("TL-BR");
						driver->drawVertexPrimitiveList(m_vertices.const_pointer(),m_vertices.size(),m_indices.const_pointer(),m_indices.size(),m_bDrawLine?video::ENUM_PRIMITIVE_TYPE_LINES:video::ENUM_PRIMITIVE_TYPE_TRIANGLE_FAN,m_pUnit->getVertexType());
					}
					return;
				case 0:
					{
						m_indices.set_used(0);
						m_indices.push_back(getIndex(x,z));
						m_indices.push_back(getIndex(x-offset,z-offset));
						//if((x-neighbourOffset)<0||m_pMatrix[(x-neighbourOffset)*m_iSizePerSide+z]==true)
						if((x-neighbourOffset)<0||m_pMatrix[(x-neighbourOffset)*m_iSizePerSide+z]!=0)
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x-offset,z));
							_QUADTREE_POST_PUSH_
						}
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x-offset,z+offset));
						_QUADTREE_POST_PUSH_
						//if((z+neighbourOffset)>=m_iSizePerSide||m_pMatrix[x*m_iSizePerSide+z+neighbourOffset]==true)
						if((z+neighbourOffset)>=m_iSizePerSide||m_pMatrix[x*m_iSizePerSide+z+neighbourOffset]!=0)
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x,z+offset));
							_QUADTREE_POST_PUSH_
						}
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x+offset,z+offset));
						_QUADTREE_POST_PUSH_
						//if((x+neighbourOffset)>=m_iSizePerSide||m_pMatrix[(x+neighbourOffset)*m_iSizePerSide+z]==true)
						if((x+neighbourOffset)>=m_iSizePerSide||m_pMatrix[(x+neighbourOffset)*m_iSizePerSide+z]!=0)
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x+offset,z));
							_QUADTREE_POST_PUSH_
						}
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x+offset,z-offset));
						_QUADTREE_POST_PUSH_
						//if((z-neighbourOffset)<0||m_pMatrix[x*m_iSizePerSide+z-neighbourOffset]==true)
						if((z-neighbourOffset)<0||m_pMatrix[x*m_iSizePerSide+z-neighbourOffset]!=0)
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x,z-offset));
							_QUADTREE_POST_PUSH_
						}
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x-offset,z-offset));
						_QUADTREE_POST_PUSH_
						//printIndices("ALL");
						driver->drawVertexPrimitiveList(m_vertices.const_pointer(),m_vertices.size(),m_indices.const_pointer(),m_indices.size(),m_bDrawLine?video::ENUM_PRIMITIVE_TYPE_LINES:video::ENUM_PRIMITIVE_TYPE_TRIANGLE_FAN,m_pUnit->getVertexType());
					}
					return;
				//default:
					//Logger->warn(YON_LOG_WARN_FORMAT,"other case!");
				}

				//the remaining cases are only partial fans, so we need to figure out what to render
				//(thanks to Chris Cookson for this idea)
				s32 iFanCode=mask;
				s32 iStart= g_cQTFanStart[iFanCode];
				s32 iFanLength= 0;
				//calculate the fan length by computing the index of the first non-zero bit in g_cQTFanCode
				while( !( ( (s64)g_cQTFanCode[iFanCode] )&((u64)( 1<<iFanLength) ) ) && iFanLength<8 )
					iFanLength++;

				m_indices.set_used(0);
				m_indices.push_back(getIndex(x,z));
				//render a triangle fan
				for(s32 iFanPosition=iFanLength; iFanPosition>0; iFanPosition-- )
				{
					switch( iStart )
					{
					case 0:
						//if((x-neighbourOffset)<0||m_pMatrix[(x-neighbourOffset)*m_iSizePerSide+z]==true||iFanPosition==iFanLength)
						if((x-neighbourOffset)<0||m_pMatrix[(x-neighbourOffset)*m_iSizePerSide+z]!=0||iFanPosition==iFanLength)
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x-offset,z));
							_QUADTREE_POST_PUSH_
						}
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x-offset,z+offset));
						_QUADTREE_POST_PUSH_
						if( iFanPosition==1 )
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x,z+offset));
							_QUADTREE_POST_PUSH_
						}
						break;
					case 1:
						//if((z-neighbourOffset)<0||m_pMatrix[x*m_iSizePerSide+z-neighbourOffset]==true||iFanPosition==iFanLength)
						if((z-neighbourOffset)<0||m_pMatrix[x*m_iSizePerSide+z-neighbourOffset]!=0||iFanPosition==iFanLength)
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x,z-offset));
							_QUADTREE_POST_PUSH_
						}
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x-offset,z-offset));
						_QUADTREE_POST_PUSH_
						if( iFanPosition==1 )
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x-offset,z));
							_QUADTREE_POST_PUSH_
						}
						break;
					case 2:
						//if((x+neighbourOffset)>=m_iSizePerSide||m_pMatrix[(x+neighbourOffset)*m_iSizePerSide+z]==true||iFanPosition==iFanLength)
						if((x+neighbourOffset)>=m_iSizePerSide||m_pMatrix[(x+neighbourOffset)*m_iSizePerSide+z]!=0||iFanPosition==iFanLength)
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x+offset,z));
							_QUADTREE_POST_PUSH_
						}
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x+offset,z-offset));
						_QUADTREE_POST_PUSH_
						if( iFanPosition==1 )
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x,z-offset));
							_QUADTREE_POST_PUSH_
						}
						break;
					case 3:
						//if((z+neighbourOffset)>=m_iSizePerSide||m_pMatrix[x*m_iSizePerSide+z+neighbourOffset]==true||iFanPosition==iFanLength)
						if((z+neighbourOffset)>=m_iSizePerSide||m_pMatrix[x*m_iSizePerSide+z+neighbourOffset]!=0||iFanPosition==iFanLength)
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x,z+offset));
							_QUADTREE_POST_PUSH_
						}
						_QUADTREE_PRE_PUSH_
						m_indices.push_back(getIndex(x+offset,z+offset));
						_QUADTREE_POST_PUSH_
						if( iFanPosition==1 )
						{
							_QUADTREE_PRE_PUSH_
							m_indices.push_back(getIndex(x+offset,z));
							_QUADTREE_POST_PUSH_
						}
						break;
					}

					iStart--;
					iStart&= 3;
				}
				//printIndices("ONE");
				driver->drawVertexPrimitiveList(m_vertices.const_pointer(),m_vertices.size(),m_indices.const_pointer(),m_indices.size(),m_bDrawLine?video::ENUM_PRIMITIVE_TYPE_LINES:video::ENUM_PRIMITIVE_TYPE_TRIANGLE_FAN,m_pUnit->getVertexType());

				//now, recurse down to children (special cases that weren't handled earlier)
				for(s32 iFanPosition=( 4-iFanLength ); iFanPosition>0; iFanPosition-- )
				{
					switch( iStart )
					{
						//lower right node
					case 0:
						renderNode(x-childOffset,z+childOffset,childEdgeLength,driver);
						break;
						//lower left node
					case 1:
						renderNode(x-childOffset,z-childOffset,childEdgeLength,driver);
						break;
						//upper left node
					case 2:
						renderNode(x+childOffset,z-childOffset,childEdgeLength,driver);
						break;
						//upper right node
					case 3:
						renderNode(x+childOffset,z+childOffset,childEdgeLength,driver);
						break;
					}

					iStart--;
					iStart&= 3;
				}

				return;
			}
		}
	}
#undef _QUADTREE_PRE_PUSH_
#undef _QUADTREE_POST_PUSH_
	void CQuadtreeTerrain::onRegisterForRender()
	{
		if(m_bVisible)
		{
			m_pSceneManager->registerForRender(this);

			camera::ICamera* camera=m_pSceneManager->getLogisticCamera();
			if(camera==NULL)
				camera=m_pSceneManager->getViewingCamera();
			core::vector3df cameraPosition = camera->getAbsolutePosition();
			camera::IViewFrustum* viewFrustum=camera->getViewFrustum();

			s32 center=(m_iSizePerSide-1)>>1;
			refine(center,center,m_iSizePerSide,true,cameraPosition,viewFrustum);

			/*
			bool temp[]={
				1,1,1,1,1,1,1,1,1,
				1,0,1,1,1,1,1,0,1,
				1,1,0,1,1,1,0,1,1,
				1,1,1,1,1,1,1,1,1,
				1,1,1,1,1,1,1,1,1,
				1,1,1,1,1,1,1,1,1,
				1,1,1,1,1,1,1,1,1,
				1,1,1,1,1,1,1,1,1,
				1,1,1,1,1,1,1,1,1};
			for(s32 x=0;x<m_iSizePerSide;++x)
			{
				for(s32 z=0;z<m_iSizePerSide;++z)
					m_pMatrix[(m_iSizePerSide-1-x)*m_iSizePerSide+z]=temp[x*m_iSizePerSide+z];
			}
			

			for(s32 x=m_iSizePerSide-1;x>=0;--x)
			{
				core::stringc str;
				for(s32 z=0;z<m_iSizePerSide;++z)
				{
					str.append(core::stringc("%d,",m_pMatrix[x*m_iSizePerSide+z]));
				}
				Logger->debug("%s\r\n",str.c_str());
			}*/

			m_bDrawLine=getMaterial(0)->getPolygonMode()==video::ENUM_POLYGON_MODE_LINE;

			ITerrainModel::onRegisterForRender();
		}
	}

	void CQuadtreeTerrain::propagateRoughness(){
		f32 fKUpperBound;
		s32 iDH, iLocalD2;
		u8 iD2;
		f32 iLocalH;
		s32 tileCount, iEdgeLength, iEdgeOffset;
		s32 iChildOffset;
		s32 x, z;

		//set the iEdgeLength to 3 (lowest length possible)
		iEdgeLength= 3;

		//start off at the lowest level of detail, and traverse up to the highest node (lowest detail)
		while( iEdgeLength<=m_iSizePerSide )
		{
			//offset of node edges (since all edges are the same length
			tileCount=iEdgeLength-1;
			iEdgeOffset=tileCount>>1;
			//offset of the node's children's edges
			iChildOffset = iEdgeOffset>>1;

			for( x=iEdgeOffset; x<m_iSizePerSide; x+=tileCount)
			{
				for( z=iEdgeOffset; z<m_iSizePerSide; z+=tileCount)
				{
					//compute "iLocalD2" values for this node
					//upper-mid
					iLocalD2= core::ceil32(core::abs_(((m_vertices[(x+iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset].pos.y+m_vertices[(x+iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset].pos.y)/2)-m_vertices[(x+iEdgeOffset)*m_iSizePerSide+z].pos.y));

					//right-mid
					iDH= core::ceil32(core::abs_(((m_vertices[(x+iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset].pos.y+m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset].pos.y)/2)-m_vertices[x*m_iSizePerSide+z+iEdgeOffset].pos.y));
					iLocalD2= core::max_( iLocalD2, iDH );

					//bottom-mid
					iDH= core::ceil32(core::abs_(((m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset].pos.y+m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset].pos.y)/2)-m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z].pos.y));
					iLocalD2= core::max_( iLocalD2, iDH );

					//left-mid
					iDH= core::ceil32(core::abs_(((m_vertices[(x+iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset].pos.y+m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset].pos.y)/2)-m_vertices[x*m_iSizePerSide+z-iEdgeOffset].pos.y));
					iLocalD2= core::max_( iLocalD2, iDH );

					//bottom-left to top-right diagonal
					iDH= core::ceil32(core::abs_(((m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset].pos.y+m_vertices[(x+iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset].pos.y)/2)-m_vertices[x*m_iSizePerSide+z].pos.y));
					iLocalD2= core::max_( iLocalD2, iDH );

					//bottom-right to top-left diagonal
					iDH= core::ceil32(core::abs_(((m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset].pos.y+m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset].pos.y)/2)-m_vertices[x*m_iSizePerSide+z].pos.y));
					iLocalD2= core::max_( iLocalD2, iDH );

					// iLocalD2 will be a value from 0 - 255.
					// Dividing by d:
					// 	d ranges from 3 to m_iSize.
					// 	Upper bound: 255 / 3 = 85
					//	Lower bound: 0
					//	Extra precision if iD2 multiplied by 3.
					iLocalD2= core::ceil32((iLocalD2*3.0f )/iEdgeLength);

					//test minimally sized block
					if( iEdgeLength==3 )
					{
						iD2= iLocalD2;

						//compute the "iLocalH" value
						//upper right
						iLocalH= m_vertices[(x+iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset].pos.y;

						//right mid
						iLocalH= core::max_(iLocalH, m_vertices[x*m_iSizePerSide+z+iEdgeOffset].pos.y);

						//lower right
						iLocalH= core::max_(iLocalH, m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset].pos.y);

						//bottom mid
						iLocalH= core::max_(iLocalH, m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z].pos.y);

						//lower left
						iLocalH= core::max_(iLocalH, m_vertices[(x-iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset].pos.y);

						//left mid
						iLocalH= core::max_(iLocalH, m_vertices[x*m_iSizePerSide+z-iEdgeOffset].pos.y);

						//upper left
						iLocalH= core::max_(iLocalH, m_vertices[(x+iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset].pos.y);

						//upper mid
						iLocalH= core::max_(iLocalH, m_vertices[(x+iEdgeOffset)*m_iSizePerSide+z].pos.y);

						//center
						iLocalH= core::max_(iLocalH, m_vertices[x*m_iSizePerSide+z].pos.y);

						//store the maximum iLocalH value in the matrix
						m_pMatrix[x*m_iSizePerSide+z+1]=(u8)(iLocalH/m_scale.y);
					}
					else
					{
						fKUpperBound= 1.0f*m_fMinResolution/( 2.0f*( m_fMinResolution-1.0f ) );

						//use d2 values from farther up on the quadtree
						iD2=(u8)core::ceil32(core::max_(fKUpperBound*(f32)m_pMatrix[x*m_iSizePerSide+z],(f32)iLocalD2));
						iD2=(u8)core::ceil32(core::max_(fKUpperBound*(f32)m_pMatrix[x*m_iSizePerSide+z-iEdgeOffset],(f32)iD2));
						iD2=(u8)core::ceil32(core::max_(fKUpperBound*(f32)m_pMatrix[x*m_iSizePerSide+z+iEdgeOffset],(f32)iD2));
						iD2=(u8)core::ceil32(core::max_(fKUpperBound*(f32)m_pMatrix[(x+iEdgeOffset)*m_iSizePerSide+z],(f32)iD2));
						iD2=(u8)core::ceil32(core::max_(fKUpperBound*(f32)m_pMatrix[(x-iEdgeOffset)*m_iSizePerSide+z],(f32)iD2));

						//get the max local height values of the 4 nodes (LL, LR, UL, UR)
						iLocalH=m_vertices[(x+iChildOffset)*m_iSizePerSide+z+iChildOffset].pos.y;
						iLocalH=core::max_(iLocalH,m_vertices[(x-iChildOffset)*m_iSizePerSide+z+iChildOffset].pos.y);
						iLocalH=core::max_(iLocalH,m_vertices[(x-iChildOffset)*m_iSizePerSide+z-iChildOffset].pos.y);
						iLocalH=core::max_(iLocalH,m_vertices[(x+iChildOffset)*m_iSizePerSide+z-iChildOffset].pos.y);

						//store the max value in the quadtree matrix
						m_pMatrix[x*m_iSizePerSide+z+1]=(u8)(iLocalH/m_scale.y);
					}

					//store the values we calculated for iD2 into the quadtree matrix
					m_pMatrix[x*m_iSizePerSide+z-1]=iD2;
					m_pMatrix[x*m_iSizePerSide+z+1]=iD2;

					//propogate the value up the quadtree
					m_pMatrix[(x-iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset]=core::max_(m_pMatrix[(x-iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset],iD2);
					m_pMatrix[(x-iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset]=core::max_(m_pMatrix[(x-iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset],iD2);
					m_pMatrix[(x+iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset]=core::max_(m_pMatrix[(x+iEdgeOffset)*m_iSizePerSide+z-iEdgeOffset],iD2);
					m_pMatrix[(x+iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset]=core::max_(m_pMatrix[(x+iEdgeOffset)*m_iSizePerSide+z+iEdgeOffset],iD2);
				}
			}

			//move up to the next quadtree level (lower level of detail)
			iEdgeLength= ( iEdgeLength<<1 )-1;
		}
	}

	f32 CQuadtreeTerrain::calculateL1Norm(const core::vector3df& a,const core::vector3df& b){
		return core::abs_(a.x-b.x)+core::abs_(a.y-b.y)+core::abs_(a.z-b.z);
	}
}
}
}