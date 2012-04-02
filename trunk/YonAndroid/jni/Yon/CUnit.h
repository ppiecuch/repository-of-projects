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
				:m_pMaterial(new video::CMaterial()),m_pShap(NULL){}
			virtual ~CUnit(){
				m_pMaterial->drop();
				//TODOÈ¥µôÅÐ¶Ï
				if(m_pShap)
					m_pShap->drop();
			}

			virtual video::IMaterial* getMaterial(){
				return m_pMaterial;
			}
			virtual const video::IMaterial* getMaterial() const{
				return m_pMaterial;
			}

			virtual const Shap2D* getShap()const {
				return m_pShap;
			}
			virtual void setShap(Shap2D* shap){
				shap->grab();
				m_pShap=shap;
			}


			virtual const void* getVertices() const{
				return m_vertices.pointer();
			}
			virtual void* getVertices(){
				return m_vertices.pointer();
			}
			virtual u32 getVertexCount() const{
				return m_vertices.size();
			}

			virtual const void* getIndices() const{
				return m_indices.pointer();
			}
			virtual void* getIndices(){
				return m_indices.pointer();
			}
			virtual u32 getIndexCount() const{
				return m_indices.size();
			}

			virtual void append(const void* const vertices, u32 numVertices, const void* const indices, u32 numIndices){
				if (vertices == getVertices())
					return;

				const u32 vertexCount = getVertexCount();
				u32 i;

				m_vertices.reallocate(vertexCount+numVertices);
				for (i=0; i<numVertices; ++i){
					m_vertices.push(((SVertex*)vertices)[i]);
				}

				m_indices.reallocate(getIndexCount()+numIndices);
				for (i=0; i<numIndices; ++i){
					m_indices.push(((u16*)indices)[i]+vertexCount);
				}
			}

			virtual void append(const IUnit* const other){
				if (this==other)
					return;

				append(other->getVertices(),other->getVertexCount(),other->getIndices(),other->getIndexCount());
			}
		private:

			Shap2D* m_pShap;

			video::CMaterial* m_pMaterial;


			core::array<SVertex> m_vertices;
			core::array<u16> m_indices;

			friend class CGeometryFactory;
		};
	}//scene
}//yon
#endif