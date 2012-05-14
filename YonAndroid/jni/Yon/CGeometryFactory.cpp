#include "CGeometryFactory.h"
#include "SVertex.h"
//#include "CUnit.h"
#include "CEntity.h"
//#include "CUnit2D.h"
#include "yonMath.h"
#include "SUnit.h"
#include "teapot.h"
#include <memory.h>
#ifdef YON_COMPILE_WITH_ANDROID
#include <string.h>
#endif

#include "ILogger.h"

using namespace yon::core;

namespace yon{
namespace scene{

	/*
	----------x1,y1
	|			|
	|			|
	x0,y0------
	*/
	IShap* CGeometryFactory::createXYRectangle2D(s32 x0,s32 y0,s32 x1,s32 y1,f32 u0,f32 v0,f32 u1,f32 v1,const video::SColor& color) const{
		Shap2D* shap=new Shap2D();

		// Create indices
		const static u8 u[6] = {0,  1,  3,  3,  1,  2};

		shap->m_indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			shap->m_indices.push_back(u[i]);

		//Create vertexs
		shap->m_vertices.reallocate(4);

		shap->m_vertices.push_back(S2DVertex((f32)x0,(f32)y0,u0,v0,color));
		shap->m_vertices.push_back(S2DVertex((f32)x1,(f32)y0,u1,v0,color));
		shap->m_vertices.push_back(S2DVertex((f32)x1,(f32)y1,u1,v1,color));
		shap->m_vertices.push_back(S2DVertex((f32)x0,(f32)y1,u0,v1,color));

		return shap;

	}
	IShap* CGeometryFactory::createXYRectangle3D(s32 x0,s32 y0,s32 x1,s32 y1,f32 z,f32 u0,f32 v0,f32 u1,f32 v1,const video::SColor& color) const{
		Shap3D* shap=new Shap3D();

		// Create indices
		const static u8 u[6] = {0,  1,  3,  3,  1,  2};

		shap->m_indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			shap->m_indices.push_back(u[i]);

		//Create vertexs
		shap->m_vertices.reallocate(4);

		shap->m_vertices.push_back(SVertex((f32)x0,(f32)y0,z,u0,v0,color));
		shap->m_vertices.push_back(SVertex((f32)x1,(f32)y0,z,u1,v0,color));
		shap->m_vertices.push_back(SVertex((f32)x1,(f32)y1,z,u1,v1,color));
		shap->m_vertices.push_back(SVertex((f32)x0,(f32)y1,z,u0,v1,color));

		return shap;

	}

	IShap* CGeometryFactory::createXYRectangle3D(s32 x0,s32 y0,s32 x1,s32 y1,f32 z,f32 u0,f32 v0,f32 u1,f32 v1,f32 u2,f32 v2,f32 u3,f32 v3,const video::SColor& color) const{
		Shap3D* shap=new Shap3D();

		// Create indices
		const static u8 u[6] = {0,  1,  3,  3,  1,  2};

		shap->m_indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			shap->m_indices.push_back(u[i]);

		//Create vertexs
		shap->m_vertices.reallocate(4);

		shap->m_vertices.push_back(SVertex((f32)x0,(f32)y0,z,u0,v0,color));
		shap->m_vertices.push_back(SVertex((f32)x1,(f32)y0,z,u1,v1,color));
		shap->m_vertices.push_back(SVertex((f32)x1,(f32)y1,z,u2,v2,color));
		shap->m_vertices.push_back(SVertex((f32)x0,(f32)y1,z,u3,v3,color));

		return shap;
	}

	IShap* CGeometryFactory::createXYRectangle2D2T(s32 x0,s32 y0,s32 x1,s32 y1,f32 u0,f32 v0,f32 u1,f32 v1,f32 s0,f32 t0,f32 s1,f32 t1,const video::SColor& color) const{
		Shap3D2T* shap=new Shap3D2T();

		// Create indices
		const static u8 u[6] = {0,  1,  3,  3,  1,  2};

		shap->m_indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			shap->m_indices.push_back(u[i]);

		//Create vertexs
		shap->m_vertices.reallocate(4);

		shap->m_vertices.push_back(SVertex2TCoords((f32)x0,(f32)y0,0,u0,v0,s0,t0,color));
		shap->m_vertices.push_back(SVertex2TCoords((f32)x1,(f32)y0,0,u1,v0,s1,t0,color));
		shap->m_vertices.push_back(SVertex2TCoords((f32)x1,(f32)y1,0,u1,v1,s1,t1,color));
		shap->m_vertices.push_back(SVertex2TCoords((f32)x0,(f32)y1,0,u0,v1,s0,t1,color));

		return shap;
	}

	IShap* CGeometryFactory::createShap(ENUM_VERTEX_TYPE type,u32 verticeCount,u32 indiceCount) const{
		switch(type)
		{
		case ENUM_VERTEX_TYPE_2V1T1C:
			{
				Shap2D* shap=new Shap2D();
				shap->m_vertices.set_used(verticeCount);
				shap->m_indices.set_used(indiceCount);
				memset(shap->m_vertices.pointer(), 0x0, shap->getVertexSize()*shap->getVertexCount());
				memset(shap->m_indices.pointer(), 0x0, shap->getIndexSize()*shap->getIndexCount());
				return shap;
			}
		case ENUM_VERTEX_TYPE_3V1T1C:
			{
				Shap3D* shap=new Shap3D();
				shap->m_vertices.set_used(verticeCount);
				shap->m_indices.set_used(indiceCount);
				memset(shap->m_vertices.pointer(), 0x0, shap->getVertexSize()*shap->getVertexCount());
				memset(shap->m_indices.pointer(), 0x0, shap->getIndexSize()*shap->getIndexCount());
				return shap;
			}
		case ENUM_VERTEX_TYPE_3V2T1C:
			{
				Shap3D2T* shap=new Shap3D2T();
				shap->m_vertices.set_used(verticeCount);
				shap->m_indices.set_used(indiceCount);
				memset(shap->m_vertices.pointer(), 0x0, shap->getVertexSize()*shap->getVertexCount());
				memset(shap->m_indices.pointer(), 0x0, shap->getIndexSize()*shap->getIndexCount());
				return shap;
			}
		}
		return NULL;
	}

	void CGeometryFactory::fillShapIndices(IShap* shap) const{
		if(shap)
		{
			switch(shap->getVertexType())
			{
			case ENUM_VERTEX_TYPE_2V1T1C:
				{
					Shap2D* s=static_cast<Shap2D*>(shap);
					u32 vCount=s->getVertexCount();
					s->m_indices.set_used(vCount);
					for(u32 i=0;i<vCount;++i)
						s->m_indices[i]=i;
				}
			case ENUM_VERTEX_TYPE_3V1T1C:
				{
					Shap3D* s=static_cast<Shap3D*>(shap);
					u32 vCount=s->getVertexCount();
					s->m_indices.set_used(vCount);
					for(u32 i=0;i<vCount;++i)
						s->m_indices[i]=i;
				}
			case ENUM_VERTEX_TYPE_3V2T1C:
				{
					Shap3D2T* s=static_cast<Shap3D2T*>(shap);
					u32 vCount=s->getVertexCount();
					s->m_indices.set_used(vCount);
					for(u32 i=0;i<vCount;++i)
						s->m_indices[i]=i;
				}
			}
		}
	}

	IUnit* CGeometryFactory::createUnit(IShap* shap)const {
		switch(shap->getVertexType())
		{
		case ENUM_VERTEX_TYPE_2V1T1C:
			{
				Unit2D* unit=new Unit2D();
				unit->setShap(shap);
				return unit;
			}
		case ENUM_VERTEX_TYPE_3V1T1C:
			{
				Unit3D* unit=new Unit3D();
				unit->setShap(shap);
				return unit;
			}
		case ENUM_VERTEX_TYPE_3V2T1C:
			{
				Unit3D2T* unit=new Unit3D2T();
				unit->setShap(shap);
				return unit;
			}
		}
		/*if(shap->getDimenMode()==ENUM_DIMEN_MODE_2D){
			Unit2D* unit=new Unit2D();
			unit->setShap(shap);
			return unit;
		}else if(shap->getDimenMode()==ENUM_DIMEN_MODE_3D){
			Unit3D* unit=new Unit3D();
			unit->setShap(shap);
			return unit;
		}*/
		return NULL;
	}

	IEntity* CGeometryFactory::createEntity(IUnit* unit)const {
		CEntity* entity=new CEntity();
		entity->add(unit);
		return entity;
	}


	/*IEntity* CGeometryFactory::createXYPlane2D(const core::dimension2df& size) const{
		Unit2D* unit=new Unit2D();

		// Create indices
		const u8 u[6] = {
			0,  2,  1,
			0,  3,  2
		};

		unit->m_indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			unit->m_indices.push(u[i]);

		//Create vertexs
		unit->m_vertices.reallocate(4);

		f32 phw,phh,mhw,mhh;
		phw=size.w/2;
		phh=size.h/2;
		mhw=phw-size.w;
		mhh=phh-size.h;

		f32 u0,u1,v0,v1;
		u0=v0=0;
		u1=v1=1;

		unit->m_vertices.push(S2DVertex(mhw,mhh,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(S2DVertex(mhw,phh,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(S2DVertex(phw,phh,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(S2DVertex(phw,mhh,u1,v1,video::COLOR_WHITE));

		CEntity* entity=new CEntity(ENUM_DIMEN_MODE_2D);
		entity->addUnit(unit);

		unit->drop();

		return entity;

	}

	IEntity* CGeometryFactory::createXYPlane(const core::dimension2df& size,f32 z) const{
		

		CUnit* unit=new CUnit();
		Shap3D* shap=new Shap3D();

		// Create indices
		const u16 u[6] = {
			0,  2,  1,
			0,  3,  2
		};

		shap->m_indices.reallocate(6);
		for (u32 i=0; i<6; ++i)
			shap->m_indices.push(u[i]);

		//Create vertexs
		shap->m_vertices.reallocate(4);

		f32 phw,phh,mhw,mhh;
		phw=size.w/2;
		phh=size.h/2;
		mhw=phw-size.w;
		mhh=phh-size.h;

		f32 u0,u1,v0,v1;
		u0=v0=0;
		u1=v1=1;

		shap->m_vertices.push(SVertex(mhw,mhh,z,u0,v1,video::COLOR_WHITE));
		shap->m_vertices.push(SVertex(mhw,phh,z,u0,v0,video::COLOR_WHITE));
		shap->m_vertices.push(SVertex(phw,phh,z,u1,v0,video::COLOR_WHITE));
		shap->m_vertices.push(SVertex(phw,mhh,z,u1,v1,video::COLOR_WHITE));

		unit->setShap(shap);
		shap->drop();

		CEntity* entity=new CEntity();
		entity->addUnit(unit);

		unit->drop();

		return entity;

	}

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
			unit->m_indices.push(u[i]);

		//Create vertexs
		unit->m_vertices.reallocate(24);

		f32 phw,phh,phd,mhw,mhh,mhd;
		phw=size.w/2;
		phh=size.h/2;
		phd=size.d/2;
		mhw=phw-size.w;
		mhh=phh-size.h;
		mhd=phd-size.d;

		f32 u0,u1,v0,v1;
		u0=v0=0;
		u1=v1=1;

		Logger->debug("half cube size:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",phw,phh,phd,mhw,mhh,mhd);

		unit->m_vertices.push(SVertex(mhw,mhh,mhd,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,mhh,phd,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,phd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,mhd,u1,v1,video::COLOR_WHITE));

		unit->m_vertices.push(SVertex(mhw,phh,mhd,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,phd,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,phd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,mhd,u1,v1,video::COLOR_WHITE));

		unit->m_vertices.push(SVertex(mhw,mhh,mhd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,mhd,u1,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,mhd,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,mhd,u0,v0,video::COLOR_WHITE));

		unit->m_vertices.push(SVertex(mhw,mhh,phd,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,phd,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,phd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,phd,u1,v1,video::COLOR_WHITE));

		//左
		unit->m_vertices.push(SVertex(mhw,mhh,mhd,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,mhh,phd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,phd,u1,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(mhw,phh,mhd,u0,v1,video::COLOR_WHITE));

		//右
		unit->m_vertices.push(SVertex(phw,mhh,mhd,u1,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,mhh,phd,u0,v0,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,phd,u0,v1,video::COLOR_WHITE));
		unit->m_vertices.push(SVertex(phw,phh,mhd,u1,v1,video::COLOR_WHITE));

		CEntity* entity=new CEntity();
		entity->addUnit(unit);

		unit->drop();

		return entity;
	}*/

	IShap* CGeometryFactory::createCube(f32 width,f32 height,f32 depth,const video::SColor& color) const{
		Shap3D* shap=new Shap3D();

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
		shap->m_indices.reallocate(36);
		for (u32 i=0; i<36; ++i)
			shap->m_indices.push_back(u[i]);

		//Create vertexs
		shap->m_vertices.reallocate(24);

		f32 phw,phh,phd,mhw,mhh,mhd;
		phw=width/2;
		phh=height/2;
		phd=depth/2;
		mhw=phw-width;
		mhh=phh-height;
		mhd=phd-depth;

		f32 u0,u1,v0,v1;
		u0=v0=0;
		u1=v1=1;

		Logger->debug("half cube size:%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",phw,phh,phd,mhw,mhh,mhd);

		shap->m_vertices.push_back(SVertex(mhw,mhh,mhd,u0,v1,color));
		shap->m_vertices.push_back(SVertex(mhw,mhh,phd,u0,v0,color));
		shap->m_vertices.push_back(SVertex(phw,mhh,phd,u1,v0,color));
		shap->m_vertices.push_back(SVertex(phw,mhh,mhd,u1,v1,color));

		shap->m_vertices.push_back(SVertex(mhw,phh,mhd,u0,v1,color));
		shap->m_vertices.push_back(SVertex(mhw,phh,phd,u0,v0,color));
		shap->m_vertices.push_back(SVertex(phw,phh,phd,u1,v0,color));
		shap->m_vertices.push_back(SVertex(phw,phh,mhd,u1,v1,color));

		shap->m_vertices.push_back(SVertex(mhw,mhh,mhd,u1,v0,color));
		shap->m_vertices.push_back(SVertex(mhw,phh,mhd,u1,v1,color));
		shap->m_vertices.push_back(SVertex(phw,phh,mhd,u0,v1,color));
		shap->m_vertices.push_back(SVertex(phw,mhh,mhd,u0,v0,color));

		shap->m_vertices.push_back(SVertex(mhw,mhh,phd,u0,v1,color));
		shap->m_vertices.push_back(SVertex(mhw,phh,phd,u0,v0,color));
		shap->m_vertices.push_back(SVertex(phw,phh,phd,u1,v0,color));
		shap->m_vertices.push_back(SVertex(phw,mhh,phd,u1,v1,color));

		//左
		shap->m_vertices.push_back(SVertex(mhw,mhh,mhd,u0,v0,color));
		shap->m_vertices.push_back(SVertex(mhw,mhh,phd,u1,v0,color));
		shap->m_vertices.push_back(SVertex(mhw,phh,phd,u1,v1,color));
		shap->m_vertices.push_back(SVertex(mhw,phh,mhd,u0,v1,color));

		//右
		shap->m_vertices.push_back(SVertex(phw,mhh,mhd,u1,v0,color));
		shap->m_vertices.push_back(SVertex(phw,mhh,phd,u0,v0,color));
		shap->m_vertices.push_back(SVertex(phw,phh,phd,u0,v1,color));
		shap->m_vertices.push_back(SVertex(phw,phh,mhd,u1,v1,color));

		return shap;
	}
	IShap* CGeometryFactory::createSphere(f32 radius,u32 hSteps,u32 vSteps) const{
		//CUnit* unit=new CUnit();
		Shap3D* shap=new Shap3D();

		f32 dtheta=(float)PI2/hSteps;	//水平方向步增
		f32 dphi=(float)PI/vSteps;		//垂直方向步增

		u32 numVertices=(vSteps+1)*(hSteps+1);
		u32 numIndices=vSteps*hSteps*6;
	
		shap->m_vertices.reallocate(numVertices);
		shap->m_indices.reallocate(numIndices);

		f32 phi=0;
		f32 theta=0;
		f32 x,y,z;
		f32 u,v;
		u32 v0,v1,v2,v3;
		for(u32 i=0;i<=vSteps;++i,phi+=dphi)
		{
			theta=0;
			for(u32 j=0;j<=hSteps;++j,theta+=dtheta)
			{
				z = (f32) (radius * sinf(phi) * cosf(theta));
				x = (f32) (radius * sinf(phi) * sinf(theta));
				y = (f32) (radius * cosf(phi));

				u = (f32)j/hSteps;
				v = (f32)(vSteps-i)/vSteps;
				shap->m_vertices.push_back(SVertex(x,y,z,u,v,video::COLOR_WHITE));

				//Logger->debug("v[%d][%d]={%.2f,%.2f,%.2f}\n",i,j,x,y,z);
			}
		}
		u32 count=hSteps+1;
		for(u32 i=0;i<vSteps;++i)
		{
			for(u32 j=0;j<hSteps;++j)
			{
				v0=i*count+j;
				v1=(i+1)*count+j;
				v2=(i+1)*count+j+1;
				v3=i*count+j+1;

				shap->m_indices.push_back(v0);
				shap->m_indices.push_back(v1);
				shap->m_indices.push_back(v2);
				shap->m_indices.push_back(v0);
				shap->m_indices.push_back(v2);
				shap->m_indices.push_back(v3);

				//Logger->debug("i[%d][%d]=%d,%d,%d--%d,%d,%d\n",i,j,v0,v1,v2,v0,v2,v3);

			}
		}
#if 0
		for(u32 i=0;i<vSteps;++i,phi+=dphi)
		{
			for(u32 j=0;j<hSteps;++j,theta+=dtheta)
			{
				z = (f32) (radius * sinf(phi*DEGTORAD) * cosf(DEGTORAD*theta));
				x = (f32) (radius * sinf(phi*DEGTORAD) * sinf(DEGTORAD*theta));
				y = (f32) (radius * cosf(phi*DEGTORAD));
				temp.set(x,y,z);
				temp.normalize();
				u = (f32)j/hSteps;
				v = (f32)asinf(temp.y)/PI+0.5f;
				shap->m_vertices.push(SVertex(x,y,z,u,v,video::COLOR_WHITE));
				/*NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
				if(texCoords!=NULL)
				{
					(*texCoords)[index*2+0]=(float)j/hSteps;
					(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
				}
				if(normals!=NULL)
				{
					(*normals)[index*3+0]=temp[0];
					(*normals)[index*3+1]=temp[1];
					(*normals)[index*3+2]=temp[2];
				}*/
				++index;
				z = (float) (radius * sinf((phi+dphi)*DEGTORAD) * cosf(DEGTORAD*theta));
				x = (float) (radius * sinf((phi+dphi)*DEGTORAD) * sinf(DEGTORAD*theta));
				y = (float) (radius * cosf((phi+dphi)*DEGTORAD));
				temp.set(x,y,z);
				temp.normalize();
				u = (f32)j/hSteps;
				v = (f32)asinf(temp.y)/PI+0.5f;
				shap->m_vertices.push(SVertex(x,y,z,u,v,video::COLOR_WHITE));
				/*NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
				if(texCoords!=NULL)
				{
					(*texCoords)[index*2+0]=(float)j/hSteps;
					(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
				}
				if(normals!=NULL)
				{
					(*normals)[index*3+0]=temp[0];
					(*normals)[index*3+1]=temp[1];
					(*normals)[index*3+2]=temp[2];
				}*/
				++index;
				z  = (float) (radius * sinf((phi+dphi)*DEGTORAD) * cosf(DEGTORAD*(theta+dtheta)));
				x  = (float) (radius * sinf((phi+dphi)*DEGTORAD) * sinf(DEGTORAD*(theta+dtheta)));
				y  = (float) (radius * cosf((phi+dphi)*DEGTORAD));
				temp.set(x,y,z);
				temp.normalize();
				u = (f32)(j+1)/hSteps;
				v = (f32)asinf(temp.y)/PI+0.5f;
				shap->m_vertices.push(SVertex(x,y,z,u,v,video::COLOR_WHITE));
				/*NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
				if(texCoords!=NULL)
				{
					(*texCoords)[index*2+0]=(float)(j+1)/hSteps;
					(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
				}
				if(normals!=NULL)
				{
					(*normals)[index*3+0]=temp[0];
					(*normals)[index*3+1]=temp[1];
					(*normals)[index*3+2]=temp[2];
				}*/
				++index;
				z  = (float) (radius * sinf(phi*DEGTORAD) * cosf(DEGTORAD*(theta+dtheta)));
				x  = (float) (radius * sinf(phi*DEGTORAD) * sinf(DEGTORAD*(theta+dtheta)));
				y  = (float) (radius * cosf(phi*DEGTORAD));
				temp.set(x,y,z);
				temp.normalize();
				u = (f32)(j+1)/hSteps;
				v = (f32)asinf(temp.y)/PI+0.5f;
				shap->m_vertices.push(SVertex(x,y,z,u,v,video::COLOR_WHITE));
				/*NORMALIZE_VERTEX_TO((*vertices)[index*3+0],(*vertices)[index*3+1],(*vertices)[index*3+2],temp);
				if(texCoords!=NULL)
				{
					(*texCoords)[index*2+0]=(float)(j+1)/hSteps;
					(*texCoords)[index*2+1]=(float)asinf(temp[1])/M_PI+0.5f;
				}
				if(normals!=NULL)
				{
					(*normals)[index*3+0]=temp[0];
					(*normals)[index*3+1]=temp[1];
					(*normals)[index*3+2]=temp[2];
				}*/
				++index;

				shap->m_indices.push(index-4);
				shap->m_indices.push(index-3);
				shap->m_indices.push(index-2);
				shap->m_indices.push(index-4);
				shap->m_indices.push(index-2);
				shap->m_indices.push(index-1);

				/*if(indices!=NULL)
				{
					(*indices)[index2++]=index-4;
					(*indices)[index2++]=index-3;
					(*indices)[index2++]=index-2;
					(*indices)[index2++]=index-4;
					(*indices)[index2++]=index-2;
					(*indices)[index2++]=index-1;
				}*/
			}
		}
#endif
		Logger->debug("shpere.v:%d,i:%d\n",shap->getVertexCount(),shap->getIndexCount());

		//CEntity* entity=new CEntity();
		//entity->addUnit(unit);

		//unit->drop();

		return shap;
	}

	//x=(R+r*cosθ)cosΦ
	//y=(R+r*cosθ)sinΦ
	//z=r*sinθ
	IShap* CGeometryFactory::createTorus(f32 cirRadius,f32 orbitRadius,u32 cirSteps,u32 orbitSteps,const video::SColor& color) const{
		
		
		/*Shap3D* shap=new Shap3D();

		f32 dtheta=(float)PI2/cirSteps;		//形圆角步增
		f32 dphi=(float)PI2/orbitSteps;		//轨圆角步增

		u32 numVertices=cirSteps*orbitSteps<<2;
		u32 numIndices=cirSteps*orbitSteps*6;

		shap->m_vertices.reallocate(numVertices);
		shap->m_indices.reallocate(numIndices);

		u32 index=0;
		f32 phi=0;
		f32 theta=0;
		f32 x,y,z;
		f32 u,v;
		for(u32 i=0;i<orbitSteps;++i,phi+=dphi)
		{
			for(u32 j=0;j<cirSteps;++j,theta+=dtheta)
			{
				
				x = (f32) ((cirRadius * cosf(theta)+orbitRadius)*cosf(phi+dphi));
				y = (f32) ((cirRadius * cosf(theta)+orbitRadius)*sinf(phi+dphi));
				z = (f32) (cirRadius * sinf(theta));
				u = (f32)(i+1)/orbitSteps;
				v = (f32)j/cirSteps;
				shap->m_vertices.push(SVertex(x,y,z,u,v,color));
				++index;

				x = (f32) ((cirRadius * cosf(theta+dtheta)+orbitRadius)*cosf(phi+dphi));
				y = (f32) ((cirRadius * cosf(theta+dtheta)+orbitRadius)*sinf(phi+dphi));
				z = (f32) (cirRadius * sinf(theta+dtheta));
				u = (f32)(i+1)/orbitSteps;
				v = (f32)(j+1)/cirSteps;
				shap->m_vertices.push(SVertex(x,y,z,u,v,color));
				++index;

				x = (f32) ((cirRadius * cosf(theta+dtheta)+orbitRadius)*cosf(phi));
				y = (f32) ((cirRadius * cosf(theta+dtheta)+orbitRadius)*sinf(phi));
				z = (f32) (cirRadius * sinf(theta+dtheta));
				u = (f32)i/orbitSteps;
				v = (f32)(j+1)/cirSteps;
				shap->m_vertices.push(SVertex(x,y,z,u,v,color));
				++index;

				x = (f32) ((cirRadius * cosf(theta)+orbitRadius)*cosf(phi));
				y = (f32) ((cirRadius * cosf(theta)+orbitRadius)*sinf(phi));
				z = (f32) (cirRadius * sinf(theta));
				u = (f32)i/orbitSteps;
				v = (f32)j/cirSteps;
				shap->m_vertices.push(SVertex(x,y,z,u,v,color));
				++index;

				shap->m_indices.push(index-4);//0
				shap->m_indices.push(index-3);//1
				shap->m_indices.push(index-2);//2
				shap->m_indices.push(index-4);//0
				shap->m_indices.push(index-2);//2
				shap->m_indices.push(index-1);//3
			}
		}*/

		Shap3D* shap=new Shap3D();

		f32 dtheta=(float)PI2/cirSteps;		//形圆角步增
		f32 dphi=(float)PI2/orbitSteps;		//轨圆角步增

		u32 numVertices=(orbitSteps+1)*(cirSteps+1);
		u32 numIndices=orbitSteps*cirSteps*6;

		shap->m_vertices.reallocate(numVertices);
		shap->m_indices.reallocate(numIndices);

		f32 phi=0;
		f32 theta=0;
		f32 x,y,z;
		f32 u,v;
		u32 v0,v1,v2,v3;
		for(u32 i=0;i<=orbitSteps;++i,phi+=dphi)
		{
			theta=0;
			for(u32 j=0;j<=cirSteps;++j,theta+=dtheta)
			{
				x = (f32) ((cirRadius * cosf(theta)+orbitRadius)*cosf(phi));
				y = (f32) ((cirRadius * cosf(theta)+orbitRadius)*sinf(phi));
				z = (f32) (cirRadius * sinf(theta));
				u = (f32)i/orbitSteps;
				v = (f32)j/cirSteps;
				shap->m_vertices.push_back(SVertex(x,y,z,u,v,color));

				//Logger->debug("v[%d][%d]={%.2f,%.2f,%.2f}\n",i,j,x,y,z);
			}
		}
		u32 count=cirSteps+1;
		for(u32 i=0;i<orbitSteps;++i)
		{
			for(u32 j=0;j<cirSteps;++j)
			{
				v0=i*count+j;
				v1=(i+1)*count+j;
				v2=(i+1)*count+j+1;
				v3=i*count+j+1;

				shap->m_indices.push_back(v0);
				shap->m_indices.push_back(v1);
				shap->m_indices.push_back(v2);
				shap->m_indices.push_back(v0);
				shap->m_indices.push_back(v2);
				shap->m_indices.push_back(v3);

				//Logger->debug("i[%d][%d]=%d,%d,%d--%d,%d,%d\n",i,j,v0,v1,v2,v0,v2,v3);

			}
		}

		return shap;
	}

	IShap* CGeometryFactory::createTeapot(f32 scale,const video::SColor& color) const{
		Shap3D* shap=new Shap3D();

		u32 count=teapotFacesCount*3;
		shap->m_indices.reallocate(count);
		for(u32 i=0; i<count; ++i)
			shap->m_indices.push_back(teapotIndices[i]-1);

		shap->m_vertices.reallocate(teapotVerticesCount);

		//暂不支持纹理坐标
		count=teapotVerticesCount*3;
		for(u32 i=0; i<count; i+=3)
			shap->m_vertices.push_back(SVertex(teapotVertices[i]*scale,teapotVertices[i+1]*scale,teapotVertices[i+2]*scale,0,0,color));

		return shap;
	}
}//scene
}//yon 