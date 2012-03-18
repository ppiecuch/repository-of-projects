#include "CGeometryFactory.h"
#include "CUnit.h"
#include "SVertex.h"
#include "CEntity.h"

namespace yon{
	namespace scene{

		IEntity* CGeometryFactory::createCube(const core::dimension3df& size) const{

			CUnit* unit=new CUnit();

			// Create indices
			const u16 u[36] = {
				0,  2,  1,
				0,  3,  2, 
				4,  5,  6,
				4,  6,  7,
				8,  9,  10,
				8,  10, 11, 
				12, 15, 14,
				12, 14, 13, 
				16, 17, 18,
				16, 18, 19, 
				20, 23, 22,
				20, 22, 21
			};
			unit->m_indices.reallocate(36);
			for (u32 i=0; i<36; ++i)
				unit->m_indices[i] = u[i];

			//Create vertexs
			unit->m_vertices.reallocate(24);

			f32 phw,phh,phd,mhw,mhh,mhd;
			phw=size.w/2;
			phh=size.h/2;
			phd=size.d/2;
			mhw=size.w-phw;
			mhh=size.h-phh;
			mhd=size.d-phd;

			/*
			-0.5f, -0.5f, -0.5f,mhw,mhh,mhd
			-0.5f, -0.5f,  0.5f,mhw,mhh,phd
			0.5f, -0.5f,  0.5f, phw,mhh,phd
			0.5f, -0.5f, -0.5f, phw,mhh,mhd

			-0.5f,  0.5f, -0.5f,mhw,phh,mhd
			-0.5f,  0.5f,  0.5f,mhw,phh,phd
			0.5f,  0.5f,  0.5f, phw,phh,phd
			0.5f,  0.5f, -0.5f, phw,phh,mhd

			-0.5f, -0.5f, -0.5f,mhw,mhh,mhd
			-0.5f,  0.5f, -0.5f,mhw,phh,mhd
			0.5f,  0.5f, -0.5f, phw,phh,mhd
			0.5f, -0.5f, -0.5f, phw,mhh,mhd

			-0.5f, -0.5f, 0.5f,mhw,mhh,phd
			-0.5f,  0.5f, 0.5f,mhw,phh,phd
			0.5f,  0.5f, 0.5f, phw,phh,phd
			0.5f, -0.5f, 0.5f, phw,mhh,phd

			-0.5f, -0.5f, -0.5f,mhw,mhh,mhd
			-0.5f, -0.5f,  0.5f,mhw,mhh,phd
			-0.5f,  0.5f,  0.5f,mhw,phh,phd
			-0.5f,  0.5f, -0.5f,mhw,phh,mhd

			0.5f, -0.5f, -0.5f,phw,mhh,mhd
			0.5f, -0.5f,  0.5f,phw,mhh,phd
			0.5f,  0.5f,  0.5f,phw,phh,phd
			0.5f,  0.5f, -0.5f,phw,phh,mhd
			*/

			unit->m_vertices.push(SVertex(mhw,mhh,mhd));
			unit->m_vertices.push(SVertex(mhw,mhh,phd));
			unit->m_vertices.push(SVertex(phw,mhh,phd));
			unit->m_vertices.push(SVertex(phw,mhh,mhd));

			unit->m_vertices.push(SVertex(mhw,phh,mhd));
			unit->m_vertices.push(SVertex(mhw,phh,phd));
			unit->m_vertices.push(SVertex(phw,phh,phd));
			unit->m_vertices.push(SVertex(phw,phh,mhd));

			unit->m_vertices.push(SVertex(mhw,mhh,mhd));
			unit->m_vertices.push(SVertex(mhw,phh,mhd));
			unit->m_vertices.push(SVertex(phw,phh,mhd));
			unit->m_vertices.push(SVertex(phw,mhh,mhd));

			unit->m_vertices.push(SVertex(mhw,mhh,phd));
			unit->m_vertices.push(SVertex(mhw,phh,phd));
			unit->m_vertices.push(SVertex(phw,phh,phd));
			unit->m_vertices.push(SVertex(phw,mhh,phd));

			unit->m_vertices.push(SVertex(mhw,mhh,mhd));
			unit->m_vertices.push(SVertex(mhw,mhh,phd));
			unit->m_vertices.push(SVertex(mhw,phh,phd));
			unit->m_vertices.push(SVertex(mhw,phh,mhd));

			unit->m_vertices.push(SVertex(phw,mhh,mhd));
			unit->m_vertices.push(SVertex(phw,mhh,phd));
			unit->m_vertices.push(SVertex(phw,phh,phd));
			unit->m_vertices.push(SVertex(phw,phh,mhd));
			
			CEntity* entity=new CEntity();
			entity->addUnit(unit);

			unit->drop();

			return entity;
		}
	}
}