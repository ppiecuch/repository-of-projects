#include "CGeomipmapTerrain.h"

#include "ILogger.h"

namespace yon{
namespace scene{
namespace terrain{

	void CGeomipmapTerrain::loadHeightMap(video::IImage* image,ENUM_PATCH_SIZE patchSize)
	{
		if(image==NULL)
			return;
		YON_DEBUG_BREAK_IF(image->getDimension().w!=image->getDimension().h);

		m_iSizePerSide=image->getDimension().w;

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

		m_unit.setShap(&m_shap);
	}
}
}
}