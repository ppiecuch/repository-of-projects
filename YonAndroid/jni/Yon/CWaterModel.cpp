#include "CWaterModel.h"
#include "SUnit.h"
#include "ISceneManager.h"

#include "ILogger.h"

namespace yon{
namespace scene{
namespace water{
	CWaterModel::CWaterModel(IModel* parent,s32 xsize,s32 zsize,const core::vector3df& pos,
			const core::vector3df& rot,const core::vector3df& scale)
			:IWaterModel(parent,pos,rot,scale),m_iWidth(zsize),m_iHeight(xsize)
	{
		m_pUnit=new Unit3D();
		//m_pUnit->getMaterial()->setPolygonMode(video::ENUM_POLYGON_MODE_LINE);
		m_pUnit->setShap(&m_shap);

		const u32 numVertices=(xsize+1)*(zsize+1);
		m_shap.getVertexArray().set_used(numVertices);

		s32 index=0;
		f32 fx=0.f;
		f32 fv=0.f;
		video::SColor color(0.45f,1,1,1);
		for(s32 x = 0; x<=xsize; ++x)
		{
			f32 fz=0.f;
			f32 fu=0.f;
			for(s32 z = 0; z<=zsize; ++z)
			{
				SVertex& v=m_shap.getVertexArray()[index++];
				v.pos.x=fx;
				v.pos.y=0;
				v.pos.z=fz;

				v.texcoords.x=fu;
				v.texcoords.y=fv;

				v.color=color;

				++fz;

				fu+=1.0f;
			}
			++fx;
			fv+=1.0f;
		}

		core::array<u16>& indices=m_shap.getIndexArray();
		bool wireframe=m_pUnit->getMaterial()->getPolygonMode()==video::ENUM_POLYGON_MODE_LINE;

		for(s32 x=0;x<xsize;++x)
		{
			for(s32 z=0;z<zsize;++z)
			{
				s32 index00=getIndex(x,z,0,0);
				s32 index01=getIndex(x,z,0,1);
				s32 index10=getIndex(x,z,1,0);
				s32 index11=getIndex(x,z,1,1);
				//Logger->debug("%d,%d->%d,%d,%d,%d\r\n",x,z,index00,index01,index10,index11);

				if(wireframe)
				{
					indices.push_back(index00);
					indices.push_back(index01);
					indices.push_back(index01);
					indices.push_back(index11);
					indices.push_back(index00);
					indices.push_back(index11);
					indices.push_back(index00);
					indices.push_back(index10);
					indices.push_back(index10);
					indices.push_back(index11);
				}
				else
				{
					indices.push_back(index00);
					indices.push_back(index01);
					indices.push_back(index11);
					indices.push_back(index00);
					indices.push_back(index11);
					indices.push_back(index10);
				}
			}
		}
	}
	CWaterModel::~CWaterModel(){
		m_pUnit->drop();
	}

	void CWaterModel::render(video::IVideoDriver* driver){
		driver->setTransform(video::ENUM_TRANSFORM_WORLD,getAbsoluteTransformation());
		driver->setMaterial(getMaterial(0));
		driver->drawUnit(m_pUnit);
	}

	void CWaterModel::onRegisterForRender(){
		if(m_bVisible)
		{
			m_pSceneManager->registerForRender(this);
			IWaterModel::onRegisterForRender();
		}
	}

	s32 CWaterModel::getIndex(s32 px,s32 pz,s32 vx,s32 vz){
		return (px+vx)*(m_iWidth+1)+pz+vz;
	}
}
}
}