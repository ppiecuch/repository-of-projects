#include "CGeomipmapTerrain.h"
#include "CImage.h"
#include "SUnit.h"

#include "ILogger.h"

namespace yon{
namespace scene{
namespace terrain{

	CGeomipmapTerrain::CGeomipmapTerrain(IModel* parent,const core::vector3df& pos,
		const core::vector3df& rot,const core::vector3df& scale)
		:ITerrainModel(parent,pos,rot,scale),m_pPatchs(NULL),m_pUnit(NULL){
			m_pUnit=new Unit3D2T();
			//m_pUnit->getMaterial()->setFrontFace(video::ENUM_FRONT_FACE_CW);
			m_pUnit->getMaterial()->setPolygonMode(video::ENUM_POLYGON_MODE_LINE);
			m_pUnit->setShap(&m_shap);
	}

	CGeomipmapTerrain::~CGeomipmapTerrain()
	{
		m_pUnit->drop();
		if(m_pPatchs)
			delete[] m_pPatchs;
	}

	void CGeomipmapTerrain::loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize)
	{
		if(image==NULL)
			return;
		YON_DEBUG_BREAK_IF(image->getDimension().w!=image->getDimension().h);

		m_iSizePerSide=image->getDimension().w;
		m_iPatchSize=patchSize-1;
		m_iPatchCountPerSide=m_iSizePerSide/m_iPatchSize;

		switch(patchSize)
		{
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
			m_iMaxLOD=3;
		}

		const u32 numVertices=m_iSizePerSide*m_iSizePerSide;
		m_shap.getVertexArray().set_used(numVertices);

		//TODO texcoords
		const f32 texcoordDelta = 1.0f/(f32)(m_iSizePerSide-1);
		s32 index=0;
		float fx=0.f;
		for(s32 x = 0; x<m_iSizePerSide; ++x)
		{
			float fz=0.f;
			for(s32 z = 0; z<m_iSizePerSide; ++z)
			{
				SVertex2TCoords& v=m_shap.getVertexArray()[index++];
				v.pos.x=fx;
				v.pos.y=image->getValue(x,z);
				v.pos.z=fz;

				++fz;
			}
			++fx;
		}

		//calculate all the necessary data for the patches and the terrain
		calculateDistanceThresholds();
		createPatches();
		calculatePatchData();
	}

	void CGeomipmapTerrain::calculateDistanceThresholds()
	{
		m_distanceThresholds.set_used(m_iMaxLOD);
		const f64 size = m_iPatchSize*m_iPatchSize*m_scaleSize.x*m_scaleSize.z;
		for(s32 i=0;i<m_iMaxLOD;++i)
		{
			m_distanceThresholds.push_back(size*((i+1+i/2)*(i+1+i/2)));//but why i+1+i/2?
		}
	}

	void CGeomipmapTerrain::createPatches()
	{
		if(m_pPatchs)
			delete[] m_pPatchs;
		m_pPatchs=new SPatch[m_iPatchCountPerSide*m_iPatchCountPerSide];
	}

	void CGeomipmapTerrain::calculatePatchData()
	{
		s32 index=0;
		for(s32 x=0;x<m_iPatchCountPerSide;++x)
		{
			for(s32 z=0;z<m_iPatchCountPerSide;++z)
			{
				index=x*m_iPatchCountPerSide+z;
				m_pPatchs[index].m_iLOD=0;

				//assign neighbours
				/*if(x>0)
					m_pPatchs[index].left=&m_pPatchs[index-m_iPatchCountPerSide];
				else
					m_pPatchs[index].left=NULL;

				if(x<m_iPatchCountPerSide-1)
					m_pPatchs[index].right=&m_pPatchs[index+m_iPatchCountPerSide];
				else
					m_pPatchs[index].right=NULL;

				if(z>0)
					m_pPatchs[index].bottom=&m_pPatchs[index-1];
				else
					m_pPatchs[index].bottom=NULL;

				if(z<m_iPatchCountPerSide-1)
					m_pPatchs[index].top=&m_pPatchs[index+1];
				else
					m_pPatchs[index].top=NULL;*/
				if(x>0)
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
					m_pPatchs[index].right=NULL;
			}
		}
	}

	void CGeomipmapTerrain::calculateIndices()
	{
		core::array<u16>& indices=m_shap.getIndexArray();
		indices.set_used(0);

		s32 index=0;
		s32 step=0;
		for(s32 i=0;i<m_iPatchCountPerSide;++i)
		{
			for(s32 j=0;j<m_iPatchCountPerSide;++j)
			{
				if(m_pPatchs[index].m_iLOD>=0)
				{
					s32 x=0;
					s32 z=0;
					// calculate the step we take this patch, based on the patches current LOD
					step = 1<<m_pPatchs[index].m_iLOD;
					// Loop through patch and generate indices
					while (z<m_iPatchSize)
					{
						const s32 index11 = getIndex(j, i, index, x, z);
						const s32 index21 = getIndex(j, i, index, x + step, z);
						const s32 index12 = getIndex(j, i, index, x, z + step);
						const s32 index22 = getIndex(j, i, index, x + step, z + step);

						indices.push_back(index12);
						indices.push_back(index11);
						indices.push_back(index22);
						indices.push_back(index22);
						indices.push_back(index11);
						indices.push_back(index21);
						//IndicesToRender+=6;

						// increment index position horizontally
						x += step;

						// we've hit an edge
						if (x>=m_iPatchSize)
						{
							x = 0;
							z += step;
						}
					}
				}
			}
			++index;
		}
	}

	u32 CGeomipmapTerrain::getIndex(const s32 PatchX, const s32 PatchZ,const s32 PatchIndex, u32 vX, u32 vZ) const
	{
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
	}

	void CGeomipmapTerrain::render(video::IVideoDriver* driver)
	{
		if(m_bVisible==false)
			return;

		driver->setTransform(video::ENUM_TRANSFORM_WORLD, getAbsoluteTransformation());
		driver->setMaterial(getMaterial(0));
		driver->drawUnit(m_pUnit);
	}

	void CGeomipmapTerrain::onRegisterForRender(ISceneManager* manager)
	{
		calculateIndices();
		ITerrainModel::onRegisterForRender(manager);
	}
}
}
}