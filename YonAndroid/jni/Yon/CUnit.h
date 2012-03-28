#ifndef _YON_SCENE_CUNIT_H_
#define _YON_SCENE_CUNIT_H_

#include "IUnit.h"
#include "yonArray.h"
#include "CMaterial.h"

namespace yon{
	namespace scene{
		class CGeometryFactory;

		class CUnit : public IUnit{
		public:
			CUnit()
				:m_pMaterial(new video::CMaterial()){}
			virtual ~CUnit(){
				m_pMaterial->drop();
			}

			virtual video::IMaterial* getMaterial(){
				return m_pMaterial;
			}
			virtual const video::IMaterial* getMaterial() const{
				return m_pMaterial;
			}


			virtual const SVertex* getVertices() const{
				return m_vertices.pointer();
			}
			virtual SVertex* getVertices(){
				return m_vertices.pointer();
			}
			virtual u32 getVertexCount() const{
				return m_vertices.size();
			}

			virtual const u16* getIndices() const{
				return m_indices.pointer();
			}
			virtual u16* getIndices(){
				return m_indices.pointer();
			}
			virtual u32 getIndexCount() const{
				return m_indices.size();
			}

			virtual void append(const SVertex* const vertices, u32 numVertices, const u16* const indices, u32 numIndices){
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

			virtual void append(const IUnit* const other){
				if (this==other)
					return;

				append(other->getVertices(),other->getVertexCount(),other->getIndices(),other->getIndexCount());
			}
		private:
			core::array<SVertex> m_vertices;
			core::array<u16> m_indices;

			video::CMaterial* m_pMaterial;

			friend class CGeometryFactory;
		};
	}//scene
}//yon
#endif