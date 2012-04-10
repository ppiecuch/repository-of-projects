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
			u32 layerId;
			IEntity* entity;

			SLayer(u32 layerId,IEntity* entity)
				:layerId(layerId),entity(entity){}

			~SLayer(){
				entity->drop();
			}
		};
		core::array<SLayer*> m_layers;
		ISceneManager* m_pSceneMgr;
		video::IVideoDriver* m_pDriver;
		u32 m_uCurrentLayerIndex;
	public:
		CGraphicsAdapter(video::IVideoDriver* driver,ISceneManager* sceneMgr);
		virtual ~CGraphicsAdapter();

		virtual void beginBatch(u32 layerId);
		virtual void endBatch();

		
		void convertUVCoordinate(const core::recti& src,const core::dimension2du& size,core::rectf& uv);
		//¼ÆËãz×ø±ê
		f32 calcZ();

		virtual bool drawImage(c8* imageName, s32 srcX, s32 srcY, u32 srcWidth, u32 srcHeight, s32 destX, s32 destY, bool useAlpha,u32 color);
	};
	IGraphicsAdapter* createGraphicsAdapter(video::IVideoDriver* driver,scene::ISceneManager* sceneMgr);
}
}
#endif