#ifndef _YON_SCENE_SKINNEDUNIT_H_
#define _YON_SCENE_SKINNEDUNIT_H_

#include "SUnit.h"
#include "matrix4.h"
#include "SShap.h"

namespace yon{
namespace scene{

	struct SSkinnedUnit : public Unit3D{
	public:
		SVertex* getVertex(u32 index){
			if(m_pShap)
			{
				Shap3D* shap=reinterpret_cast<Shap3D*>(this->m_pShap);
				return &shap->m_vertices[index];
			}
			return NULL;
		}

		core::array<SVertex>& getVertexArray(){
			YON_DEBUG_BREAK_IF(this->m_pShap);
			Shap3D* shap=reinterpret_cast<Shap3D*>(this->m_pShap);
			return shap->m_vertices;
		}
		core::array<u16>& getIndexArray(){
			YON_DEBUG_BREAK_IF(this->m_pShap);
			Shap3D* shap=reinterpret_cast<Shap3D*>(this->m_pShap);
			return shap->m_indices;
		}
		core::matrix4f Transformation;
	};
}
}
#endif