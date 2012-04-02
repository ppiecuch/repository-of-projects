#ifndef _YON_SCENE_SSHAP_H_
#define _YON_SCENE_SSHAP_H_

#include "IReferencable.h"
#include "yonArray.h"
#include "SVertex.h"

namespace yon{
namespace scene{

	class CGeometryFactory;

	template<class V,class I,size_t M>
	class SShap : public virtual core::IReferencable{
	protected:
		core::array<V> m_vertices;
		core::array<I> m_indices;
		friend class CGeometryFactory;
	public:
		virtual const V* getVertices() const{
			return m_vertices.pointer();
		}
		virtual V* getVertices(){
			return m_vertices.pointer();
		}
		virtual u32 getVertexCount() const{
			return m_vertices.size();
		}

		virtual const I* getIndices() const{
			return m_indices.pointer();
		}
		virtual I* getIndices(){
			return m_indices.pointer();
		}
		virtual u32 getIndexCount() const{
			return m_indices.size();
		}

		virtual void append(const  V* const vertices, u32 numVertices, const I* const indices, u32 numIndices){
			if (vertices == getVertices())
				return;

			const u32 vertexCount = getVertexCount();
			u32 i;

			m_vertices.reallocate(vertexCount+numVertices);
			for (i=0; i<numVertices; ++i){
				m_vertices.push(vertices[i]);
			}

			m_indices.reallocate(getIndexCount()+numIndices);
			for (i=0; i<numIndices; ++i){
				m_indices.push(indices[i]+vertexCount);
			}
		}

		virtual void append(const SShap* const other){
			if (this==other)
				return;

			append(other->getVertices(),other->getVertexCount(),other->getIndices(),other->getIndexCount());
		}

		virtual ENUM_DIMEN_MODE getDimenMode() const{
			return static_cast<const ENUM_DIMEN_MODE>(M);
		}
	};

	typedef SShap<SVertex,u16,ENUM_DIMEN_MODE_3D> Shap3D;
	typedef SShap<S2DVertex,u16,ENUM_DIMEN_MODE_2D> Shap2D;
} 
}
#endif