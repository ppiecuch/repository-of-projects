#ifndef _YON_SCENE_SDYNAIMCSHAP_H_
#define _YON_SCENE_SDYNAIMCSHAP_H_

#include "SShap.h"

namespace yon{
namespace scene{

	template<class V,class I,size_t VT>
	class SDynamicShap : public SShap<V,I,VT>{
	public:
		core::array<V>& getVertexArray(){
			return m_vertices;
		}
		core::array<I>& getIndexArray(){
			return m_indices;
		}
	};

	typedef SDynamicShap<SVertex,u16,ENUM_VERTEX_TYPE_3V1T1C> SDynamicShap3D;
	typedef SDynamicShap<SVertex2TCoords,u16,ENUM_VERTEX_TYPE_3V2T1C> SDynamicShap3D2T;
	typedef SDynamicShap<S2DVertex,u16,ENUM_VERTEX_TYPE_2V1T1C> SDynamicShap2D;
}
}
#endif