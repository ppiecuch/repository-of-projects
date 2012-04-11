#include "CGraphicsAdapter.h"
#include "ISceneManager.h"
#include "IVideoDriver.h"
#include "IGeometryFactory.h"
#include "CEntity.h"
#include "ILogger.h"

namespace yon{
namespace scene{
	CGraphicsAdapter::CGraphicsAdapter(video::IVideoDriver* driver,ISceneManager* sceneMgr)
		:m_pSceneMgr(sceneMgr),m_pDriver(driver),m_uCurrentLayerIndex(0),m_iZValue(0){
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance CGraphicsAdapter");
	}
	CGraphicsAdapter::~CGraphicsAdapter(){
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
		m_layers.push(new SLayer(layerId,new CEntity()));
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


	bool CGraphicsAdapter::drawImage(c8* imageName, s32 srcX, s32 srcY, u32 srcWidth, u32 srcHeight, s32 destX, s32 destY, bool useAlpha,u32 color){
		video::ITexture* texture=m_pDriver->getTexture(imageName);
		if(texture==NULL)
			return false;
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
	}

	IGraphicsAdapter* createGraphicsAdapter(video::IVideoDriver* driver,ISceneManager* sceneMgr){
		return new CGraphicsAdapter(driver,sceneMgr);
	}
}
}