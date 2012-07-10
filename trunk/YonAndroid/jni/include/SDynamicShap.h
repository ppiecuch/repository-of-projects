#ifndef _YON_SCENE_SDYNAIMCSHAP_H_
#define _YON_SCENE_SDYNAIMCSHAP_H_

#include "SShap.h"

namespace yon{
namespace scene{

	template<class V,class I,size_t VT>
	class SDynamicShap : public SShap<V,I,VT>{
	public:
		//note:
		//use
		//core::array<V>& getVertexArray(){
		//	return m_vertices;
		//}
		//will compile failed.for:error: 'm_vertices' was not declared in this scope
		//explaination: 
		//m_vertices and m_indices are not a dependent name, i.e. it doesn't depend on the template arguments explicitly so the compiler tries to look it up early. You must make it a dependent name:
		//this->....
		//Now it depends on the type of this and thus on the template arguments. Therefore the compiler will bind it at the time of instantiation.
		//refer to:http://stackoverflow.com/questions/7076169/not-declared-in-this-scope-error-with-templates-and-inheritance
		core::array<V>& getVertexArray(){
			return this->m_vertices;
		}
		core::array<I>& getIndexArray(){
			return this->m_indices;
		}
	};

	typedef SDynamicShap<SVertex,u16,ENUM_VERTEX_TYPE_3V1T1C> SDynamicShap3D;
	typedef SDynamicShap<SVertex2TCoords,u16,ENUM_VERTEX_TYPE_3V2T1C> SDynamicShap3D2T;
	typedef SDynamicShap<S2DVertex,u16,ENUM_VERTEX_TYPE_2V1T1C> SDynamicShap2D;
}
}
#endif