#include "CQuadtreeTerrain.h"
#include "CImage.h"
#include "SUnit.h"
#include "ISceneManager.h"
#include "yonMath.h"

namespace yon{
namespace scene{
namespace terrain{

	CQuadtreeTerrain::CQuadtreeTerrain(IModel* parent,const core::vector3df& pos,
		const core::vector3df& rot,const core::vector3df& scale,
		f32 desiredResolution,f32 minResolution)
		:ITerrainModel(parent,pos,rot,scale),m_pMatrix(NULL),m_pUnit(NULL),
		m_iSizePerSide(0),
		m_fDesiredResolution(desiredResolution),m_fMinResolution(minResolution)
	{
		m_pUnit=new Unit3D2T();
		//m_pUnit->setVertexHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
		//m_pUnit->setIndexHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);
		//m_pUnit->getMaterial()->setFrontFace(video::ENUM_FRONT_FACE_CW);
		//m_pUnit->getMaterial()->setPolygonMode(video::ENUM_POLYGON_MODE_LINE);
		m_pUnit->setShap(&m_shap);
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

		m_iSizePerSide=image->getDimension().w;

		//initialize the quadtree matrix to 'true' (for all possible nodes)
		m_pMatrix=new bool[m_iSizePerSide];
		for(s32 x=0;x<m_iSizePerSide;++x)
		{
			for(s32 z=0;z<m_iSizePerSide;++z)
			{
				m_pMatrix[x*m_iSizePerSide+z]=true;
			}
		}

		const u32 numVertices=m_iSizePerSide*m_iSizePerSide;
		//m_shap.getVertexArray().set_used(numVertices);
		m_vertices.set_used(numVertices);


		const f32 texcoordDelta = 1.0f/(f32)(m_iSizePerSide-1);
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
	}

	void CQuadtreeTerrain::refine(s32 x,s32 z,f32 edgeLength,core::vector3df& cameraPos){
		//calculate the distance between current node and camera
		s32 index=x*m_iSizePerSide+z;
		f32 distance=calculateL1Norm(m_vertices[index].pos,cameraPos);

		//compute the 'f' value (as stated in Roettger's whitepaper of this algorithm)
		f32 d2=calculateD2(index,edgeLength);
		f32 f=calculateF(distance,edgeLength,d2);

		bool subdivide;
		if( f<1.0f )
			subdivide= true;		
		else
			subdivide= false;

		//store whether or not the current node gets subdivided
		m_pMatrix[index]=subdivide;

		if(subdivide)
		{
			if(edgeLength<=3)
			{
				s32 childOffset=(edgeLength-1)>>2;
				s32 childEdgeLength=(edgeLength+1)>>1;

				//refine the children nodes
				refine(x-childOffset,z-childOffset,childEdgeLength,cameraPos);
				refine(x-childOffset,z+childOffset,childEdgeLength,cameraPos);
				refine(x+childOffset,z-childOffset,childEdgeLength,cameraPos);
				refine(x+childOffset,z+childOffset,childEdgeLength,cameraPos);
			}
		}
	}

	void CQuadtreeTerrain::render(video::IVideoDriver* driver)
	{

	}

	void CQuadtreeTerrain::renderNode(s32 x,s32 z,s32 edgeLength,video::IVideoDriver* driver)
	{
		s32 index=x*m_iSizePerSide+z;
		bool subdivide=m_pMatrix[index];

		s32 offset=(edgeLength-1)>>1;
		s32 neighbourOffset=edgeLength-1;

		m_indices.set_used(0);

		if(subdivide)
		{
			if(edgeLength<=3)
			{
				m_indices.push_back(getIndex(x,z));
				m_indices.push_back(getIndex(x-offset,z-offset));
				if((x-neighbourOffset)<0||m_pMatrix[(x-neighbourOffset)*m_iSizePerSide+z]==true)
					m_indices.push_back(getIndex(x-neighbourOffset,z));
				m_indices.push_back(getIndex(x-offset,z+offset));
				if((z+neighbourOffset)>=m_iSizePerSide||m_pMatrix[x*m_iSizePerSide+z+neighbourOffset]==true)
					m_indices.push_back(getIndex(x,z+neighbourOffset));
				m_indices.push_back(getIndex(x+offset,z+offset));
				if((x+neighbourOffset)>=m_iSizePerSide||m_pMatrix[(x+neighbourOffset)*m_iSizePerSide+z]==true)
					m_indices.push_back(getIndex(x+neighbourOffset,z));
				m_indices.push_back(getIndex(x+offset,z-offset));
				if((z-neighbourOffset)<0||m_pMatrix[x*m_iSizePerSide+z-neighbourOffset]==true)
					m_indices.push_back(getIndex(x,z-neighbourOffset));
				driver->drawVertexPrimitiveList(m_vertices.const_pointer(),m_vertices.size(),m_indices.const_pointer(),m_indices.size(),video::ENUM_PRIMITIVE_TYPE_TRIANGLE_FAN);
			}
			else
			{
				s32 childOffset=(edgeLength-1)>>2;
				s32 childEdgeLength=(edgeLength+1)>>1;

				//calculate the mask code, which specifies which fans need to be rendered.
				s32 mask=0;
				mask|=m_pMatrix[(x+childOffset)*m_iSizePerSide+z+childOffset]?0x8:0;//TR
				mask|=m_pMatrix[(x+childOffset)*m_iSizePerSide+z-childOffset]?0x4:0;//TL
				mask|=m_pMatrix[(x-childOffset)*m_iSizePerSide+z-childOffset]?0x2:0;//BR
				mask|=m_pMatrix[(x-childOffset)*m_iSizePerSide+z+childOffset]?0x1:0;//BL

				switch(mask)
				{
				case 15://no fans
					{
						renderNode(x-childOffset,z-childOffset,childEdgeLength,driver);
						renderNode(x-childOffset,z+childOffset,childEdgeLength,driver);
						renderNode(x+childOffset,z-childOffset,childEdgeLength,driver);
						renderNode(x+childOffset,z+childOffset,childEdgeLength,driver);
						break;
					}
				case 10://tr,bl
					{
						renderNode(x-childOffset,z+childOffset,childEdgeLength,driver);
						renderNode(x+childOffset,z-childOffset,childEdgeLength,driver);
					}
				case 5:	//tl,br
				case 3:
				case 2:
				case 1:
				case 0:
				}
			}
		}
	}

	void CQuadtreeTerrain::onRegisterForRender()
	{

	}

	f32 CQuadtreeTerrain::calculateD2(s32 index,const f32 d){
		//TODO
		return 1/d;
		//return 1/(d*maxValue);
	}

	f32 CQuadtreeTerrain::calculateF(const f32 l,const f32 d,const f32 d2){
		return l/(d*m_fMinResolution*core::max_(m_fDesiredResolution*d2,1.0f));
	}

	f32 CQuadtreeTerrain::calculateL1Norm(const core::vector3df& a,const core::vector3df& b){
		return core::abs_(a.x-b.x)+core::abs_(a.y-b.y)+core::abs_(a.z-b.z);
	}
}
}
}