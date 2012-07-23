#include "CGeomipmapTerrain2.h"
#include "CImage.h"
#include "SUnit.h"
#include "ISceneManager.h"

#include "ILogger.h"

namespace yon{
namespace scene{
namespace terrain{

	CGeomipmapTerrain2::CGeomipmapTerrain2(IModel* parent,const core::vector3df& pos,
		const core::vector3df& rot,const core::vector3df& scale)
		:ITerrainModel(parent,pos,rot,scale),m_pPatchs(NULL),m_pUnit(NULL){
			m_pUnit=new Unit3D2T();
			//m_pUnit->getMaterial()->setFrontFace(video::ENUM_FRONT_FACE_CW);
			m_pUnit->getMaterial()->setPolygonMode(video::ENUM_POLYGON_MODE_LINE);
			m_pUnit->setShap(&m_shap);
	}

	CGeomipmapTerrain2::~CGeomipmapTerrain2()
	{
		m_pUnit->drop();
		if(m_pPatchs)
			delete[] m_pPatchs;
	}

	void CGeomipmapTerrain2::loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize)
	{
		if(image==NULL)
			return;
		YON_DEBUG_BREAK_IF(image->getDimension().w!=image->getDimension().h);

		m_iSizePerSide=image->getDimension().w;
		m_iPatchSize=patchSize-1;
		m_iPatchCountPerSide=(m_iSizePerSide-1)/m_iPatchSize;

		if(m_iPatchCountPerSide==0)
		{
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("patchSize:%d must be smaller than width of the image:%d!",patchSize,m_iSizePerSide).c_str());
			return;
		}

		switch(patchSize)
		{
		case ENUM_PATCH_SIZE_3:
			m_iMaxLOD=1;
			break;
		case ENUM_PATCH_SIZE_5:
			m_iMaxLOD=2;
			break;
		case ENUM_PATCH_SIZE_9:
			m_iMaxLOD=3;
			break;
		case ENUM_PATCH_SIZE_17:
			m_iMaxLOD=4;
			break;
		case ENUM_PATCH_SIZE_33:
			m_iMaxLOD=5;
			break;
		case ENUM_PATCH_SIZE_65:
			m_iMaxLOD=6;
			break;
		case ENUM_PATCH_SIZE_129:
			m_iMaxLOD=7;
			break;
		default:
			m_iMaxLOD=0;
		}

		const u32 numVertices=m_iSizePerSide*m_iSizePerSide;
		m_shap.getVertexArray().set_used(numVertices);

		//TODO texcoords
		const f32 texcoordDelta = 1.0f/(f32)(m_iSizePerSide-1);
		s32 index=0;
		f32 fx=0.f;
		for(s32 x = 0; x<m_iSizePerSide; ++x)
		{
			f32 fz=0.f;
			for(s32 z = 0; z<m_iSizePerSide; ++z)
			{
				SVertex2TCoords& v=m_shap.getVertexArray()[index++];
				v.pos.x=fx*m_scale.x;
				v.pos.y=(f32)image->getValue(z,x)*m_scale.y;
				v.pos.z=fz*m_scale.z;
				//v.pos.x=fx;
				//v.pos.y=(f32)image->getValue(z,x);
				//v.pos.z=fz;

				++fz;
			}
			++fx;
		}

		//calculate all the necessary data for the patches and the terrain
		calculateDistanceThresholds();
		createPatches();
		calculatePatchData();
	}

	void CGeomipmapTerrain2::calculateDistanceThresholds()
	{
		m_distanceThresholds.set_used(0);
		m_distanceThresholds.reallocate(m_iMaxLOD);
		const f64 size = m_iPatchSize*m_iPatchSize*m_scale.x*m_scale.z;
		for(s32 i=0;i<m_iMaxLOD;++i)
		{
			//m_distanceThresholds.push_back(size*((i+1+i/2)*(i+1+i/2)));//but why i+1+i/2?
			m_distanceThresholds.push_back(size*2);//but why i+1+i/2?
			Logger->debug("m_distanceThresholds[%d]:%.2f\r\n",m_distanceThresholds.size()-1,m_distanceThresholds.getLast());
		}
	}

	void CGeomipmapTerrain2::createPatches()
	{
		if(m_pPatchs)
			delete[] m_pPatchs;
		m_pPatchs=new SPatch[m_iPatchCountPerSide*m_iPatchCountPerSide];
	}

	void CGeomipmapTerrain2::calculatePatchData()
	{
		// Reset the Terrains Bounding Box for re-calculation
		m_boundingBox = core::aabbox3df(true);

		s32 index=0;
		for(s32 x=0;x<m_iPatchCountPerSide;++x)
		{
			for(s32 z=0;z<m_iPatchCountPerSide;++z)
			{
				index=x*m_iPatchCountPerSide+z;

				m_pPatchs[index].m_iLOD=0;
				m_pPatchs[index].m_centerPos=core::ORIGIN_VECTOR3DF;
				m_pPatchs[index].m_boundingBox=core::aabbox3df(true);

				for (s32 xx = x*m_iPatchSize; xx <= (x + 1) * m_iPatchSize; ++xx)
				{
					for (s32 zz = z*m_iPatchSize; zz <= (z + 1) * m_iPatchSize; ++zz)
					{
						m_pPatchs[index].m_boundingBox.addInternalPoint(m_shap.getVertexArray()[xx*m_iSizePerSide+zz].pos);
					}
				}
				m_pPatchs[index].m_centerPos=m_pPatchs[index].m_boundingBox.getCenter();
				Logger->debug("center[%d]:%.2f,%.2f,%.2f\r\n",index,m_pPatchs[index].m_centerPos.x,m_pPatchs[index].m_centerPos.y,m_pPatchs[index].m_centerPos.z);

				// Reconfigure the bounding box of the terrain as a whole
				m_boundingBox.addInternalBox(m_pPatchs[index].m_boundingBox);
				

				//assign neighbours
				/*if(x<m_iPatchCountPerSide-1)
					m_pPatchs[index].left=&m_pPatchs[index+1];
				else
					m_pPatchs[index].left=NULL;

				if(x>0)
					m_pPatchs[index].right=&m_pPatchs[index-1];
				else
					m_pPatchs[index].right=NULL;

				if(z>0)
					m_pPatchs[index].bottom=&m_pPatchs[index-m_iPatchCountPerSide];
				else
					m_pPatchs[index].bottom=NULL;

				if(z<m_iPatchCountPerSide-1)
					m_pPatchs[index].top=&m_pPatchs[index+m_iPatchCountPerSide];
				else
					m_pPatchs[index].top=NULL;*/
				if(x>0)
					m_pPatchs[index].bottom=&m_pPatchs[index-m_iPatchCountPerSide];
				else
					m_pPatchs[index].bottom=NULL;

				if(x<m_iPatchCountPerSide-1)
					m_pPatchs[index].top=&m_pPatchs[index+m_iPatchCountPerSide];
				else
					m_pPatchs[index].top=NULL;

				if(z>0)
					m_pPatchs[index].left=&m_pPatchs[index-1];
				else
					m_pPatchs[index].left=NULL;

				if(z<m_iPatchCountPerSide-1)
					m_pPatchs[index].right=&m_pPatchs[index+1];
				else
					m_pPatchs[index].right=NULL;
				/*if(x>0)
					m_pPatchs[index].top=&m_pPatchs[index-m_iPatchCountPerSide];
				else
					m_pPatchs[index].top=NULL;

				if(x<m_iPatchCountPerSide-1)
					m_pPatchs[index].bottom=&m_pPatchs[index+m_iPatchCountPerSide];
				else
					m_pPatchs[index].bottom=NULL;

				if(z>0)
					m_pPatchs[index].left=&m_pPatchs[index-1];
				else
					m_pPatchs[index].left=NULL;

				if(z<m_iPatchCountPerSide-1)
					m_pPatchs[index].right=&m_pPatchs[index+1];
				else
					m_pPatchs[index].right=NULL;*/
			}
		}
	}

	bool CGeomipmapTerrain2::preRenderLODCalculations()
	{
		bool change=false;

		camera::ICamera* camera=m_pSceneManager->getActiveCamera();
		if(camera==NULL)
			return change;

		// Determine the camera rotation, based on the camera direction.
		core::vector3df cameraPosition = camera->getAbsolutePosition();

		// Determine each patches LOD based on distance from camera (and whether or not they are in
		// the view frustum).
		const s32 count = m_iPatchCountPerSide*m_iPatchCountPerSide;
		f32 distance;
		for (s32 j = 0; j < count; ++j)
		{
			//TODO check frustum
			for (s32 i = m_iMaxLOD - 1; i >= 0; --i)
			{
				distance = cameraPosition.getDistanceFromSQ(m_pPatchs[j].m_centerPos);
				if (distance >= m_distanceThresholds[i])
				{
					if(m_pPatchs[j].m_iLOD!=i)
						change=true;
					m_pPatchs[j].m_iLOD = i;
					break;
				}
				//else if (i == 0)
				{
					if(m_pPatchs[j].m_iLOD!=0)
						change=true;
					// If we've turned off a patch from viewing, because of the frustum, and now we turn around and it's
					// too close, we need to turn it back on, at the highest LOD. The if above doesn't catch this.
					m_pPatchs[j].m_iLOD = 0;
				}
			}
		}

		return change;
	}

	void CGeomipmapTerrain2::preRenderIndicesCalculations()
	{
		core::array<u16>& indices=m_shap.getIndexArray();
		indices.set_used(0);

		bool wireframe=m_pUnit->getMaterial()->getPolygonMode()==video::ENUM_POLYGON_MODE_LINE;

		Logger->debug("\nm_iPatchCountPerSide:%d\n",m_iPatchCountPerSide);

		s32 index=0;
		s32 step=0;
		for(s32 px=0;px<m_iPatchCountPerSide;++px)
		{
			for(s32 pz=0;pz<m_iPatchCountPerSide;++pz)
			{
				if(m_pPatchs[index].m_iLOD>=0)
				{
					// calculate the step we take this patch, based on the patches current LOD
					step = 1<<m_pPatchs[index].m_iLOD;
					SPatch* top=m_pPatchs[index].top;
					SPatch* bottom=m_pPatchs[index].bottom;
					SPatch* left=m_pPatchs[index].left;
					SPatch* right=m_pPatchs[index].right;
					Logger->debug("px:%d,pz:%d,m_pPatchs[%d].m_iLOD:%d,top:%d,bottom:%d,left:%d,right:%d,step:%d\n",px,pz,index,m_pPatchs[index].m_iLOD,top?top->m_iLOD:-1,bottom?bottom->m_iLOD:-1,left?left->m_iLOD:-1,right?right->m_iLOD:-1,step);
					// Loop through patch and generate indices
					for(s32 x=0;x<m_iPatchSize;x+=step)
					{
						for(s32 z=0;z<m_iPatchSize;z+=step)
						{
							const s32 index11 = getIndex(px, pz, index, x, z);
							const s32 index21 = getIndex(px, pz, index, x + step, z);
							const s32 index12 = getIndex(px, pz, index, x, z + step);
							const s32 index22 = getIndex(px, pz, index, x + step, z + step);

							Logger->debug("x:%d,z:%d---->%d,%d,%d,%d->(%d,%d,%d),(%d,%d,%d)\n",x,z,index11,index21,index12,index22,index12,index11,index22,index22,index11,index21);

							if(wireframe)
							{
								indices.push_back(index12);
								indices.push_back(index11);
								indices.push_back(index11);
								indices.push_back(index22);
								indices.push_back(index22);
								indices.push_back(index12);
								indices.push_back(index21);
								indices.push_back(index22);
								indices.push_back(index11);
								indices.push_back(index21);
							}
							else
							{
								indices.push_back(index12);
								indices.push_back(index11);
								indices.push_back(index22);
								indices.push_back(index22);
								indices.push_back(index11);
								indices.push_back(index21);
							}
						}
					}
					/*while (z<m_iPatchSize)
					{
						

						// increment index position horizontally
						x += step;

						// we've hit an edge
						if (x>=m_iPatchSize)
						{
							x = 0;
							z += step;
						}
					}*/
				}
				++index;
			}
		}
	}

	u32 CGeomipmapTerrain2::getIndex(const s32 PatchX, const s32 PatchZ,const s32 PatchIndex, u32 vX, u32 vZ) const
	{
		// left border
		if (vZ == 0)
		{
			if (m_pPatchs[PatchIndex].left &&m_pPatchs[PatchIndex].m_iLOD < m_pPatchs[PatchIndex].left->m_iLOD &&
				(vX % (1 <<m_pPatchs[PatchIndex].left->m_iLOD)) != 0 )
			{
				vX -= vX % (1 <<m_pPatchs[PatchIndex].left->m_iLOD);
			}
		}
		 // right border
		else if (vZ == (u32)m_iPatchSize)
		{
			if (m_pPatchs[PatchIndex].right &&m_pPatchs[PatchIndex].m_iLOD < m_pPatchs[PatchIndex].right->m_iLOD &&
				(vX % (1 << m_pPatchs[PatchIndex].right->m_iLOD)) != 0)
			{
				vX -= vX % (1 <<m_pPatchs[PatchIndex].right->m_iLOD);
			}
		}

		// bottom border
		if (vX == 0)
		{
			if (m_pPatchs[PatchIndex].bottom &&m_pPatchs[PatchIndex].m_iLOD <m_pPatchs[PatchIndex].bottom->m_iLOD &&
				(vZ % (1 <<m_pPatchs[PatchIndex].bottom->m_iLOD)) != 0)
			{
				vZ -= vZ % (1 << m_pPatchs[PatchIndex].bottom->m_iLOD);
			}
		}
		// top border
		else if (vX == (u32)m_iPatchSize) 
		{
			if (m_pPatchs[PatchIndex].top &&m_pPatchs[PatchIndex].m_iLOD <m_pPatchs[PatchIndex].top->m_iLOD &&
				(vZ % (1 <<m_pPatchs[PatchIndex].top->m_iLOD)) != 0)
			{
				vZ -= vZ % (1 <<m_pPatchs[PatchIndex].top->m_iLOD);
			}
		}

		if (vZ >= (u32)m_iPatchSize+1)
			vZ = m_iPatchSize;

		if (vX >= (u32)m_iPatchSize+1)
			vX = m_iPatchSize;

		return (vX + ((m_iPatchSize) * PatchX)) * m_iSizePerSide + (vZ + ((m_iPatchSize) * PatchZ));
		/*
		// top border
		if (vZ == 0)
		{
			if (m_pPatchs[PatchIndex].top &&m_pPatchs[PatchIndex].m_iLOD < m_pPatchs[PatchIndex].top->m_iLOD &&
				(vX % (1 <<m_pPatchs[PatchIndex].top->m_iLOD)) != 0 )
			{
				vX -= vX % (1 <<m_pPatchs[PatchIndex].top->m_iLOD);
			}
		}
		else if (vZ == (u32)m_iPatchSize) // bottom border
		{
			if (m_pPatchs[PatchIndex].bottom &&m_pPatchs[PatchIndex].m_iLOD < m_pPatchs[PatchIndex].bottom->m_iLOD &&
				(vX % (1 << m_pPatchs[PatchIndex].bottom->m_iLOD)) != 0)
			{
				vX -= vX % (1 <<m_pPatchs[PatchIndex].bottom->m_iLOD);
			}
		}

		// left border
		if (vX == 0)
		{
			if (m_pPatchs[PatchIndex].left &&m_pPatchs[PatchIndex].m_iLOD <m_pPatchs[PatchIndex].left->m_iLOD &&
				(vZ % (1 <<m_pPatchs[PatchIndex].left->m_iLOD)) != 0)
			{
				vZ -= vZ % (1 << m_pPatchs[PatchIndex].left->m_iLOD);
			}
		}
		else if (vX == (u32)m_iPatchSize) // right border
		{
			if (m_pPatchs[PatchIndex].right &&m_pPatchs[PatchIndex].m_iLOD <m_pPatchs[PatchIndex].right->m_iLOD &&
				(vZ % (1 <<m_pPatchs[PatchIndex].right->m_iLOD)) != 0)
			{
				vZ -= vZ % (1 <<m_pPatchs[PatchIndex].right->m_iLOD);
			}
		}

		if (vZ >= (u32)m_iPatchSize+1)
			vZ = m_iPatchSize;

		if (vX >= (u32)m_iPatchSize+1)
			vX = m_iPatchSize;

		return (vZ + ((m_iPatchSize) * PatchZ)) * m_iSizePerSide + (vX + ((m_iPatchSize) * PatchX));
		*/
	}

	void CGeomipmapTerrain2::render(video::IVideoDriver* driver)
	{
		if(m_bVisible==false)
			return;

		driver->setTransform(video::ENUM_TRANSFORM_WORLD, getAbsoluteTransformation());
		driver->setMaterial(getMaterial(0));
		driver->drawUnit(m_pUnit);
	}

	void CGeomipmapTerrain2::onRegisterForRender(ISceneManager* manager)
	{
		if(m_bVisible)
		{
			manager->registerForRender(this);

			if(preRenderLODCalculations())
				preRenderIndicesCalculations();

			ITerrainModel::onRegisterForRender(manager);
		}
	}
}
}
}