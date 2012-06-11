#include "CGraphicsAdapter.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "IGeometryFactory.h"
#include "CEntity.h"
#include "ILogger.h"

namespace yon{
namespace scene{

	void swap(s32& a,s32& b){
		static s32 t;
		t=a;
		a=b;
		b=t;
	}
	void swap(f32& a,f32& b){
		static f32 t;
		t=a;
		a=b;
		b=t;
	}

	const u16 CGraphicsAdapter::INDICES[6]={0,1,3,3,1,2};

	CGraphicsAdapter::CGraphicsAdapter(video::IVideoDriver* driver,ISceneManager* sceneMgr)
		:m_pSceneMgr(sceneMgr),m_pDriver(driver),m_uCurrentLayerIndex(0),m_iZValue(0),
		m_defaultPool(DefaultUnitPool(3)),m_pSolidMaterial(NULL),m_pTransparentMaterial(NULL)
	{
		m_pSolidMaterial=driver->createMaterial();
		m_pTransparentMaterial=driver->createMaterial();
		m_pTransparentMaterial->setMaterialType(video::ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR);
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance CGraphicsAdapter");
	}
	CGraphicsAdapter::~CGraphicsAdapter(){
		m_pTransparentMaterial->drop();
		m_pSolidMaterial->drop();
		m_defaultPool.clear();

		for(u32 i=0;i<m_layers.size();++i)
			delete m_layers[i];
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release CGraphicsAdapter");
	}

	void CGraphicsAdapter::beginBatch(s32 layerId){
		/*core::list<SLayer*>::Iterator it = m_layers.begin();
		for (; it != m_layers.end(); ++it){
			if((*it)->layerId==layerId)
				return (*it)->entity;
		}
		SLayer* layer=new SLayer(layerId,new CEntity());
		return layer->entity;*/
		m_iZValue=layerId;
		//TODO优化
		if(m_layers.size()>0&&m_layers[m_uCurrentLayerIndex]->layerId==layerId)
			return;
		for(u32 i=0;i<m_layers.size();++i){
			if(m_layers[i]->layerId==layerId){
				m_uCurrentLayerIndex=i;
				return;
			}
		}
		m_layers.push_back(new SLayer(layerId,new CEntity()));
		m_uCurrentLayerIndex=m_layers.size()-1;
		return;
	}
	void CGraphicsAdapter::endBatch(){
		//init中调用模式
		//m_pSceneMgr->addModel(m_layers[m_uCurrentLayerIndex]->entity);

		//drawframe调用模式
		IEntity* entity=m_layers[m_uCurrentLayerIndex]->entity;
		m_pDriver->setTransform(video::ENUM_TRANSFORM_WORLD, core::IDENTITY_MATRIX);
		for(u32 i=0;i<entity->getUnitCount();++i){
			m_pDriver->setMaterial(entity->getUnit(i)->getMaterial());
			m_pDriver->drawUnit(entity->getUnit(i));
		}
		delete m_layers[m_uCurrentLayerIndex];
		m_layers.erase(m_uCurrentLayerIndex);
		m_uCurrentLayerIndex=0;
	}

	f32 CGraphicsAdapter::calcZ(){
		m_iZValue+=Z_INC;
		return m_iZValue;
	}
	void CGraphicsAdapter::convertUVCoordinate(const core::recti& src,const core::dimension2du& size,core::rectf& uv){
		const f32 invW = 1.f / static_cast<f32>(size.w);
		const f32 invH = 1.f / static_cast<f32>(size.h);
		uv.topLeft.x=src.topLeft.x*invW;
		uv.topLeft.y=(size.h-src.topLeft.y)*invH;
		uv.bottomRight.x=src.bottomRight.x*invW;
		uv.bottomRight.y=(size.h-src.bottomRight.y)*invH;
	}


	bool CGraphicsAdapter::drawImage(const c8* imageName, s32 srcX, s32 srcY, u32 srcWidth, u32 srcHeight, s32 destX, s32 destY, bool useAlpha,u32 color){
		video::ITexture* texture=m_pDriver->getTexture(imageName);
		if(texture==NULL){
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("image:%s can not found!",imageName).c_str());
			return false;
		}
		video::ENUM_MATERIAL_TYPE materialType=useAlpha?video::ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR:video::ENUM_MATERIAL_TYPE_SOLID;
		//计算位置坐标(左上角)
		core::position2di spos(destX,destY);
		core::position2df rpos;
		m_pDriver->convertPosCoordinate(spos,rpos);
		Logger->debug("convertPosCoordinate:%d,%d->%.2f,%.2f\n",spos.x,spos.y,rpos.x,rpos.y);
		//计算UV坐标
		core::recti src(srcX,srcY,srcX+srcWidth,srcY+srcHeight);
		const core::dimension2du& size=texture->getSize();
		core::rectf uv;
		convertUVCoordinate(src,size,uv);
		IShap* shap=m_pSceneMgr->getGeometryFactory()->createXYRectangle3D(rpos.x,rpos.y-srcHeight,rpos.x+srcWidth,rpos.y,calcZ(),uv.topLeft.x,uv.bottomRight.y,uv.bottomRight.x,uv.topLeft.y,video::SColor(color));
		m_layers[m_uCurrentLayerIndex]->entity->add(texture,materialType,shap);
		shap->drop();
		return true;
	}

	bool CGraphicsAdapter::drawImage(const c8* imageName, s32 x, s32 y, MASK_ACTHOR anchor){
		video::ITexture* texture=m_pDriver->getTexture(imageName);
		if(texture==NULL)
			return false;
		video::ENUM_MATERIAL_TYPE materialType=video::ENUM_MATERIAL_TYPE_SOLID;
		s32 width=texture->getSize().w;
		s32 height=texture->getSize().h;
		//根据锚点计算新位置
		if(anchor&MASK_ACTHOR_HCENTER)
			x-=width>>1;
		else if(anchor&MASK_ACTHOR_RIGHT)
			x-=width;
		if(anchor&MASK_ACTHOR_VCENTER)
			y-=height>>1;
		else if(anchor&MASK_ACTHOR_BOTTOM)
			y-=height;
		//计算位置坐标(左上角)
		core::position2di spos(x,y);
		core::position2df rpos;
		m_pDriver->convertPosCoordinate(spos,rpos);
		Logger->debug("convertPosCoordinate:%d,%d->%.2f,%.2f\n",spos.x,spos.y,rpos.x,rpos.y);
		//创建形态
		IShap* shap=m_pSceneMgr->getGeometryFactory()->createXYRectangle3D(rpos.x,rpos.y-height,rpos.x+width,rpos.y,calcZ());
		m_layers[m_uCurrentLayerIndex]->entity->add(texture,materialType,shap);
		shap->drop();
		return true;
	}
	bool CGraphicsAdapter::drawRegion(const c8* imageName, s32 x_src, s32 y_src, s32 width, s32 height, ENUM_TRANS transform, s32 x_dest, s32 y_dest, MASK_ACTHOR anchor){
		video::ITexture* texture=m_pDriver->getTexture(imageName);
		if(texture==NULL)
			return false;

		video::ENUM_MATERIAL_TYPE materialType=video::ENUM_MATERIAL_TYPE_SOLID;

		//计算UV坐标
		core::recti src(x_src,y_src,x_src+width,y_src+height);
		const core::dimension2du& size=texture->getSize();
		core::rectf uv;
		convertUVCoordinate(src,size,uv);
		f32 u0,u1,u2,u3,v0,v1,v2,v3;
		//如果是转置变换
		if(transform>=ENUM_TRANS_MIRROR_ROT270){
			swap(width,height);

			switch(transform){
			case ENUM_TRANS_ROT90:
				u1=u0=uv.bottomRight.x;
				u3=u2=uv.topLeft.x;
				v3=v0=uv.bottomRight.y;
				v2=v1=uv.topLeft.y;
				break;
			case ENUM_TRANS_MIRROR_ROT90:
				u1=u0=uv.topLeft.x;
				u3=u2=uv.bottomRight.x;
				v3=v0=uv.bottomRight.y;
				v2=v1=uv.topLeft.y;
				break;
			case ENUM_TRANS_ROT270:
				u1=u0=uv.topLeft.x;
				u3=u2=uv.bottomRight.x;
				v3=v0=uv.topLeft.y;
				v2=v1=uv.bottomRight.y;
				break;
			case ENUM_TRANS_MIRROR_ROT270:
				u1=u0=uv.bottomRight.x;
				u3=u2=uv.topLeft.x;
				v3=v0=uv.topLeft.y;
				v2=v1=uv.bottomRight.y;
			}
		}else if(transform>ENUM_TRANS_NONE){
			switch(transform){
				case ENUM_TRANS_ROT180:
					u3=u0=uv.bottomRight.x;
					u2=u1=uv.topLeft.x;
					v1=v0=uv.topLeft.y;
					v3=v2=uv.bottomRight.y;
					break;
				case ENUM_TRANS_MIRROR:
					u3=u0=uv.bottomRight.x;
					u2=u1=uv.topLeft.x;
					v1=v0=uv.bottomRight.y;
					v3=v2=uv.topLeft.y;
					break;
				case ENUM_TRANS_MIRROR_ROT180:
					u3=u0=uv.topLeft.x;
					u2=u1=uv.bottomRight.x;
					v1=v0=uv.topLeft.y;
					v3=v2=uv.bottomRight.y;
			}
		}else{
			u3=u0=uv.topLeft.x;
			u2=u1=uv.bottomRight.x;
			v1=v0=uv.bottomRight.y;
			v3=v2=uv.topLeft.y;
		}
		//根据锚点计算新位置
		s32 x=x_dest;
		s32 y=y_dest;
		if(anchor&MASK_ACTHOR_HCENTER)
			x-=width>>1;
		else if(anchor&MASK_ACTHOR_RIGHT)
			x-=width;
		if(anchor&MASK_ACTHOR_VCENTER)
			y-=height>>1;
		else if(anchor&MASK_ACTHOR_BOTTOM)
			y-=height;
		//计算位置坐标(左上角)
		core::position2di spos(x,y);
		core::position2df rpos;
		m_pDriver->convertPosCoordinate(spos,rpos);
		Logger->debug("convertPosCoordinate:%d,%d->%.2f,%.2f\n",spos.x,spos.y,rpos.x,rpos.y);
		//创建形态
		IShap* shap=m_pSceneMgr->getGeometryFactory()->createXYRectangle3D(rpos.x,rpos.y-height,rpos.x+width,rpos.y,calcZ(),u0,v0,u1,v1,u2,v2,u3,v3);
		m_layers[m_uCurrentLayerIndex]->entity->add(texture,materialType,shap);
		shap->drop();
		return true;
	}

	void CGraphicsAdapter::clearZ(s32 z){
		m_iZValue=z;
	}

	bool CGraphicsAdapter::drawRegion(const c8* imageName, const core::rectf& uv, s32 x_dest, s32 y_dest, s32 destW, s32 destH, ENUM_TRANS transform, MASK_ACTHOR anchor,bool useAlpha, u32 color){
		video::ITexture* texture=m_pDriver->getTexture(imageName);
		if(texture==NULL)
			return false;

		f32 u0,u1,u2,u3,v0,v1,v2,v3;
		//如果是转置变换
		if(transform>=ENUM_TRANS_MIRROR_ROT270){
			swap(destW, destH);

			switch(transform){
			case ENUM_TRANS_ROT90:
				u1=u0=uv.bottomRight.x;
				u3=u2=uv.topLeft.x;
				v3=v0=uv.bottomRight.y;
				v2=v1=uv.topLeft.y;
				break;
			case ENUM_TRANS_MIRROR_ROT90:
				u1=u0=uv.topLeft.x;
				u3=u2=uv.bottomRight.x;
				v3=v0=uv.bottomRight.y;
				v2=v1=uv.topLeft.y;
				break;
			case ENUM_TRANS_ROT270:
				u1=u0=uv.topLeft.x;
				u3=u2=uv.bottomRight.x;
				v3=v0=uv.topLeft.y;
				v2=v1=uv.bottomRight.y;
				break;
			case ENUM_TRANS_MIRROR_ROT270:
				u1=u0=uv.bottomRight.x;
				u3=u2=uv.topLeft.x;
				v3=v0=uv.topLeft.y;
				v2=v1=uv.bottomRight.y;
			}
		}else if(transform>ENUM_TRANS_NONE){
			switch(transform){
				case ENUM_TRANS_ROT180:
					u3=u0=uv.bottomRight.x;
					u2=u1=uv.topLeft.x;
					v1=v0=uv.topLeft.y;
					v3=v2=uv.bottomRight.y;
					break;
				case ENUM_TRANS_MIRROR:
					u3=u0=uv.bottomRight.x;
					u2=u1=uv.topLeft.x;
					v1=v0=uv.bottomRight.y;
					v3=v2=uv.topLeft.y;
					break;
				case ENUM_TRANS_MIRROR_ROT180:
					u3=u0=uv.topLeft.x;
					u2=u1=uv.bottomRight.x;
					v1=v0=uv.topLeft.y;
					v3=v2=uv.bottomRight.y;
			}
		}else{
			u3=u0=uv.topLeft.x;
			u2=u1=uv.bottomRight.x;
			v1=v0=uv.bottomRight.y;
			v3=v2=uv.topLeft.y;
		}
		//根据锚点计算新位置
		f32 x = x_dest;
		f32 y = y_dest;
		if(anchor&MASK_ACTHOR_HCENTER)
			x -= destW * 0.5f;
		else if(anchor&MASK_ACTHOR_RIGHT)
			x -= destW;
		if(anchor&MASK_ACTHOR_VCENTER)
			y -= destH * 0.5f;
		else if(anchor&MASK_ACTHOR_BOTTOM)
			y -= destH;
		//计算位置坐标(左上角)
		core::position2di spos(x,y);
		core::position2di rpos;
		m_pDriver->convertPosCoordinate(spos,rpos);
		//Logger->debug("convertPosCoordinate:%d,%d->%.2f,%.2f\n",spos.x,spos.y,rpos.x,rpos.y);
		//创建形态
		//IShap* shap=m_pSceneMgr->getGeometryFactory()->createXYRectangle3D(rpos.x,rpos.y-destH,rpos.x+destW,rpos.y,calcZ(),u0,v0,u1,v1,u2,v2,u3,v3);
		//m_layers[m_uCurrentLayerIndex]->entity->add(texture,materialType,shap);
		//shap->drop();
		f32 x0,y0,x1,y1,z;
		x0=rpos.x;
		y0=rpos.y-destH;
		x1=rpos.x+destW;
		y1=rpos.y;
		z=calcZ();
		video::SColor c(color);
		DefaultUnit* unit=m_defaultPool.get();
		unit->m_vertices.push_back(SVertex(x0,y0,z,u0,v0,c));
		unit->m_vertices.push_back(SVertex(x1,y0,z,u1,v1,c));
		unit->m_vertices.push_back(SVertex(x1,y1,z,u2,v2,c));
		unit->m_vertices.push_back(SVertex(x0,y1,z,u3,v3,c));
		unit->m_pTexture=texture;
		if(useAlpha)
		{
			TransparentEntry entry(unit,z);
			m_transparents.push_back(entry);
		}
		else
		{
			SolidEntry entry(unit);
			m_solids.push_back(entry);
		}
		return true;
	}
	bool CGraphicsAdapter::drawRegion(const c8* imageName, const core::rectf& uv, const core::position2di poss[4], ENUM_TRANS transform, bool useAlpha, u32 color){
		video::ITexture* texture=m_pDriver->getTexture(imageName);
		if(texture==NULL)
			return false;

		f32 u0,u1,u2,u3,v0,v1,v2,v3;
		//如果是转置变换
		if(transform>=ENUM_TRANS_MIRROR_ROT270){
			//swap(destW, destH);

			switch(transform){
			case ENUM_TRANS_ROT90:
				u1=u0=uv.bottomRight.x;
				u3=u2=uv.topLeft.x;
				v3=v0=uv.bottomRight.y;
				v2=v1=uv.topLeft.y;
				break;
			case ENUM_TRANS_MIRROR_ROT90:
				u1=u0=uv.topLeft.x;
				u3=u2=uv.bottomRight.x;
				v3=v0=uv.bottomRight.y;
				v2=v1=uv.topLeft.y;
				break;
			case ENUM_TRANS_ROT270:
				u1=u0=uv.topLeft.x;
				u3=u2=uv.bottomRight.x;
				v3=v0=uv.topLeft.y;
				v2=v1=uv.bottomRight.y;
				break;
			case ENUM_TRANS_MIRROR_ROT270:
				u1=u0=uv.bottomRight.x;
				u3=u2=uv.topLeft.x;
				v3=v0=uv.topLeft.y;
				v2=v1=uv.bottomRight.y;
			}
		}else if(transform>ENUM_TRANS_NONE){
			switch(transform){
				case ENUM_TRANS_ROT180:
					u3=u0=uv.bottomRight.x;
					u2=u1=uv.topLeft.x;
					v1=v0=uv.topLeft.y;
					v3=v2=uv.bottomRight.y;
					break;
				case ENUM_TRANS_MIRROR:
					u3=u0=uv.bottomRight.x;
					u2=u1=uv.topLeft.x;
					v1=v0=uv.bottomRight.y;
					v3=v2=uv.topLeft.y;
					break;
				case ENUM_TRANS_MIRROR_ROT180:
					u3=u0=uv.topLeft.x;
					u2=u1=uv.bottomRight.x;
					v1=v0=uv.topLeft.y;
					v3=v2=uv.bottomRight.y;
			}
		}else{
			u3=u0=uv.topLeft.x;
			u2=u1=uv.bottomRight.x;
			v1=v0=uv.bottomRight.y;
			v3=v2=uv.topLeft.y;
		}

		//计算位置坐标
		core::position2di rpos[4];
		m_pDriver->convertPosCoordinate(poss[0],rpos[0]);
		m_pDriver->convertPosCoordinate(poss[1],rpos[1]);
		m_pDriver->convertPosCoordinate(poss[2],rpos[2]);
		m_pDriver->convertPosCoordinate(poss[3],rpos[3]);
		//Logger->debug("convertPosCoordinate:%d,%d->%.2f,%.2f\n",spos.x,spos.y,rpos.x,rpos.y);
		//创建形态
		//IShap* shap=m_pSceneMgr->getGeometryFactory()->createXYRectangle3D(rpos,calcZ(),u0,v0,u1,v1,u2,v2,u3,v3,video::SColor(color));
		//m_layers[m_uCurrentLayerIndex]->entity->add(texture,materialType,shap);
		//shap->drop();
		f32 z=calcZ();
		video::SColor c(color);
		DefaultUnit* unit=m_defaultPool.get();
		unit->m_vertices.push_back(SVertex(rpos[0].x,rpos[0].y,z,u0,v0,c));
		unit->m_vertices.push_back(SVertex(rpos[1].x,rpos[1].y,z,u1,v1,c));
		unit->m_vertices.push_back(SVertex(rpos[2].x,rpos[2].y,z,u2,v2,c));
		unit->m_vertices.push_back(SVertex(rpos[3].x,rpos[3].y,z,u3,v3,c));
		unit->m_pTexture=texture;
		if(useAlpha)
		{
			TransparentEntry entry(unit,z);
			m_transparents.push_back(entry);
		}
		else
		{
			SolidEntry entry(unit);
			m_solids.push_back(entry);
		}
		return true;
	}
	void CGraphicsAdapter::drawVertexPrimitiveList(video::IMaterial* material,const void* vertices, u32 vertexCount,const void* indices, u32 indexCount,scene::ENUM_VERTEX_TYPE vType){
		//TODO
		f32 z=calcZ();
		EffectEntry entry(material,vertices,vertexCount,indices,indexCount,vType,z);
		m_effects.push_back(entry);
	}

	void CGraphicsAdapter::render(){
		u32 i;

		m_pDriver->setTransform(video::ENUM_TRANSFORM_WORLD, core::IDENTITY_MATRIX);

		m_pDriver->setMaterial(m_pSolidMaterial);
		m_solids.sort();
		for (i=0; i<m_solids.size(); ++i)
		{
			m_pSolidMaterial->setTexture(0,m_solids[i].m_pUnit->m_pTexture);
			// /Logger->debug("draw:%d,%d\n",m_solids[i].m_pUnit->m_pTexture,m_solids[i].m_pUnit->m_vertices.size());
			m_pDriver->drawVertexPrimitiveList(m_solids[i].m_pUnit->m_vertices.const_pointer(),m_solids[i].m_pUnit->m_vertices.size(),CGraphicsAdapter::INDICES,6);
			m_defaultPool.recycle(m_solids[i].m_pUnit);
		}
		m_solids.set_used(0);

		m_pDriver->setMaterial(m_pTransparentMaterial);
		m_transparents.sort();
		for (i=0; i<m_transparents.size(); ++i)
		{
			m_pTransparentMaterial->setTexture(0,m_transparents[i].m_pUnit->m_pTexture);
			// /Logger->debug("draw:%d,%d\n",m_transparents[i].m_pUnit->m_pTexture,m_transparents[i].m_pUnit->m_vertices.size());
			m_pDriver->drawVertexPrimitiveList(m_transparents[i].m_pUnit->m_vertices.const_pointer(),m_transparents[i].m_pUnit->m_vertices.size(),CGraphicsAdapter::INDICES,6);
			m_defaultPool.recycle(m_transparents[i].m_pUnit);
		}
		m_transparents.set_used(0);


		m_effects.sort();
		for (i=0; i<m_effects.size(); ++i)
		{
			m_pDriver->setMaterial(m_effects[i].m_pMaterial);
			m_pDriver->drawVertexPrimitiveList(m_effects[i].m_pVertices,m_effects[i].m_uVertexCount,m_effects[i].m_pIndices,m_effects[i].m_uIndexCount,video::ENUM_PRIMITIVE_TYPE_TRIANGLES,m_effects[i].m_vertexType);
		}
		m_effects.set_used(0);

	}

	IGraphicsAdapter* createGraphicsAdapter(video::IVideoDriver* driver,ISceneManager* sceneMgr){
		return new CGraphicsAdapter(driver,sceneMgr);
	}
}
}