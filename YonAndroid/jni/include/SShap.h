#ifndef _YON_SCENE_SSHAP_H_
#define _YON_SCENE_SSHAP_H_

#include "IShap.h"
#include "yonArray.h"
#include "SVertex.h"


namespace yon{
namespace scene{

	class CGeometryFactory;
	struct SSkinnedUnit;

	template<class V,class I,size_t VT>
	class SShap : public IShap{
	protected:
		static u32 s_lCounter;

		core::array<V> m_vertices;
		core::array<I> m_indices;
		
		u32 m_uVerticesChangedId;
		u32 m_uIndicesChangedId;

		friend class CGeometryFactory;
		friend struct SSkinnedUnit;
	public:
		SShap()
			:m_uVerticesChangedId((s_lCounter<<16)+1),m_uIndicesChangedId((s_lCounter<<16)+1){
				++s_lCounter;
		}
		/*virtual const V* getVertices() const{
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
		}*/

		virtual u32 getVerticesChangedId() const{
			return m_uVerticesChangedId;
		}
		virtual u32 getIndicesChangedId() const{
			return m_uIndicesChangedId;
		}

		virtual const void* getVertices() const{
			return m_vertices.const_pointer();
		}
		virtual void* getVertices(){
			return m_vertices.pointer();
		}
		virtual u32 getVertexCount() const{
			return m_vertices.size();
		}

		virtual const void* getIndices() const{
			return m_indices.const_pointer();
		}
		virtual void* getIndices(){
			return m_indices.pointer();
		}
		virtual u32 getIndexCount() const{
			return m_indices.size();
		}

		virtual void append(const  void* const vertices, u32 numVertices, const void* const indices, u32 numIndices){
			if (vertices == getVertices())
				return;

			const u32 vertexCount = getVertexCount();
			u32 i;

			for (i=0; i<numVertices; ++i){
				m_vertices.push_back(((V*)vertices)[i]);
			}
			for (i=0; i<numIndices; ++i){
				m_indices.push_back(((I*)indices)[i]+vertexCount);
			}

			//BUG fix:每一次都要reallocate，这不要命嘛
			/*m_vertices.reallocate(vertexCount+numVertices);
			for (i=0; i<numVertices; ++i){
				m_vertices.push_back(((V*)vertices)[i]);
			}

			m_indices.reallocate(getIndexCount()+numIndices);
			for (i=0; i<numIndices; ++i){
				m_indices.push_back(((I*)indices)[i]+vertexCount);
			}*/

			++m_uVerticesChangedId;
			++m_uIndicesChangedId;
		}

		virtual void append(const IShap* const other){
			YON_DEBUG_BREAK_IF(getVertexType()!=other->getVertexType());
			if (this==other)
				return;

			append(other->getVertices(),other->getVertexCount(),other->getIndices(),other->getIndexCount());
		}

		virtual void setVerticesDirty(){
			++m_uVerticesChangedId;
		}
		virtual void setIndicesDirty(){
			++m_uIndicesChangedId;
		}

		/*virtual ENUM_DIMEN_MODE getDimenMode() const{
			return static_cast<const ENUM_DIMEN_MODE>(M);
		}*/

		virtual ENUM_VERTEX_TYPE getVertexType() const{
			return static_cast<const ENUM_VERTEX_TYPE>(VT);
		}
		virtual u32 getVertexSize() const{
			return sizeof(V);
		}
		virtual u32 getIndexSize() const{
			return sizeof(I);
		}
	};

	typedef SShap<SVertex,u16,ENUM_VERTEX_TYPE_3V1T1C> Shap3D;
	typedef SShap<SVertex2TCoords,u16,ENUM_VERTEX_TYPE_3V2T1C> Shap3D2T;
	typedef SShap<S2DVertex,u16,ENUM_VERTEX_TYPE_2V1T1C> Shap2D;

	template<class V,class I,size_t VT>
	u32 SShap<V,I,VT>::s_lCounter=1;
} 
}
#endif