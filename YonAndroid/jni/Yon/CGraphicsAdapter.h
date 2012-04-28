#ifndef _YON_SCENE_CGRAPHICSADAPTER_H_
#define _YON_SCENE_CGRAPHICSADAPTER_H_

#include "IGraphicsAdapter.h"
#include "rect.h"
#include "dimension2d.h"
#include "position2d.h"
#include "yonArray.h"
#include "IEntity.h"

namespace yon{
	namespace video{
		class IVideoDriver;
	}
namespace scene{

	class ISceneManager;
	class IGeometryFactory;
	class CGraphicsAdapter : public IGraphicsAdapter{
	private:
		struct SLayer{
			s32 layerId;
			IEntity* entity;

			SLayer(s32 layerId,IEntity* entity)
				:layerId(layerId),entity(entity){}

			~SLayer(){
				entity->drop();
			}
		};
		const static s32 Z_INC=1;
		core::array<SLayer*> m_layers;
		ISceneManager* m_pSceneMgr;
		video::IVideoDriver* m_pDriver;
		u32 m_uCurrentLayerIndex;
		s32 m_iZValue;//根据layerId与调用drawImage顺序计算的z值
	public:
		CGraphicsAdapter(video::IVideoDriver* driver,ISceneManager* sceneMgr);
		virtual ~CGraphicsAdapter();

		virtual void beginBatch(s32 layerId);
		virtual void endBatch();

		
		void convertUVCoordinate(const core::recti& src,const core::dimension2du& size,core::rectf& uv);
		//计算z坐标
		f32 calcZ();

		virtual bool drawImage(const c8* imageName, s32 srcX, s32 srcY, u32 srcWidth, u32 srcHeight, s32 destX, s32 destY, bool useAlpha,u32 color);

		virtual bool drawImage(const c8* imageName, s32 x, s32 y, MASK_ACTHOR anchor);
		virtual bool drawRegion(const c8* imageName, s32 x_src, s32 y_src, s32 width, s32 height, ENUM_TRANS transform, s32 x_dest, s32 y_dest, MASK_ACTHOR anchor);
	};
	IGraphicsAdapter* createGraphicsAdapter(video::IVideoDriver* driver,scene::ISceneManager* sceneMgr);
}
}
#endif