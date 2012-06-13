#ifndef _YON_SCENE_CGRAPHICSADAPTER_H_
#define _YON_SCENE_CGRAPHICSADAPTER_H_

#include "IGraphicsAdapter.h"
#include "rect.h"
#include "dimension2d.h"
#include "position2d.h"
#include "yonArray.h"
#include "IEntity.h"
#include "objectpool.h"

namespace yon{
	namespace video{
		class IVideoDriver;
		class ITexture;
		class IMaterial;
	}
namespace scene{

	class ISceneManager;
	class IGeometryFactory;
	class CGraphicsAdapter : public IGraphicsAdapter{
	private:
		const static u16 INDICES[6];
		struct DefaultUnit : public core::IRecyclable
		{
			virtual void reset(){
				m_pTexture=0;
				m_vertices.set_used(0);
			}

			core::array<SVertex> m_vertices;
			video::ITexture* m_pTexture;
		};
		struct SolidEntry{

			SolidEntry(DefaultUnit* unit):m_pUnit(unit){}

			bool operator < (const SolidEntry& other) const
			{
				return (m_pUnit->m_pTexture < other.m_pUnit->m_pTexture);
			}

			DefaultUnit* m_pUnit;
		};
		struct TransparentEntry{
			TransparentEntry(DefaultUnit* unit,f32 z):m_pUnit(unit),m_fZ(z){}

			bool operator < (const TransparentEntry& other) const
			{
				return m_fZ<other.m_fZ;
			}

			f32 m_fZ;
			DefaultUnit* m_pUnit;
		};
		struct EffectEntry{
			EffectEntry(video::IMaterial* material,const void* vertices, u32 vertexCount,const void* indice, u32 indexCount,scene::ENUM_VERTEX_TYPE vType,f32 z)
				:m_pMaterial(material),m_pVertices(vertices),m_uVertexCount(vertexCount),m_pIndices(indice),m_uIndexCount(indexCount),m_vertexType(vType),m_fZ(z)
			{}

			bool operator < (const EffectEntry& other) const
			{
				return m_fZ<other.m_fZ;
			}

			f32 m_fZ;
			video::IMaterial* m_pMaterial;
			const void* m_pVertices;
			u32 m_uVertexCount;
			const void* m_pIndices;
			u32 m_uIndexCount;
			scene::ENUM_VERTEX_TYPE m_vertexType;
		};
		
		//typedef core::CObjectPoolNill<DefaultUnit> DefaultUnitPool;
		typedef core::CObjectPool<DefaultUnit> DefaultUnitPool;
		DefaultUnitPool m_defaultPool;

		core::array<SolidEntry> m_solids;
		core::array<TransparentEntry> m_transparents;
		core::array<EffectEntry> m_effects;

		//video::IMaterial* m_pSolidMaterial;
		video::IMaterial* m_pTransparentRefMaterial;
		video::IMaterial* m_pTransparentMaterial;

		/*struct SLayer{
			s32 layerId;
			IEntity* entity;

			SLayer(s32 layerId,IEntity* entity)
				:layerId(layerId),entity(entity){}

			~SLayer(){
				entity->drop();
			}
		};
		core::array<SLayer*> m_layers;
		*/
		const static s32 Z_INC=1;
		ISceneManager* m_pSceneMgr;
		video::IVideoDriver* m_pDriver;
		//u32 m_uCurrentLayerIndex;
		f32 m_iZValue;//根据layerId与调用drawImage顺序计算的z值
	public:
		CGraphicsAdapter(video::IVideoDriver* driver,ISceneManager* sceneMgr);
		virtual ~CGraphicsAdapter();

		//virtual void beginBatch(s32 layerId);
		//virtual void endBatch();

		
		void convertUVCoordinate(const core::recti& src,const core::dimension2du& size,core::rectf& uv);
		//计算z坐标
		f32 calcZ();

		virtual bool drawImage(const c8* imageName, s32 srcX, s32 srcY, u32 srcWidth, u32 srcHeight, s32 destX, s32 destY, bool useAlpha,u32 color);

		//virtual bool drawImage(const c8* imageName, s32 x, s32 y, MASK_ACTHOR anchor);
		//virtual bool drawRegion(const c8* imageName, s32 x_src, s32 y_src, s32 width, s32 height, ENUM_TRANS transform, s32 x_dest, s32 y_dest, MASK_ACTHOR anchor);

		virtual void clearZ(s32 z);
		virtual bool drawRegion(const c8* imageName, const core::rectf& uv, s32 x_dest, s32 y_dest, s32 destW, s32 destH, ENUM_TRANS transform, MASK_ACTHOR anchor,bool useAlpha, u32 color);
		virtual bool drawRegion(const c8* imageName, const core::rectf& uv, const core::position2di poss[4], ENUM_TRANS transform, bool useAlpha, u32 color);
		virtual void drawVertexPrimitiveList(video::IMaterial* material,const void* vertices, u32 vertexCount,const void* indice, u32 indexCount,scene::ENUM_VERTEX_TYPE vType);
		virtual void render();
	};
	IGraphicsAdapter* createGraphicsAdapter(video::IVideoDriver* driver,scene::ISceneManager* sceneMgr);
}
}
#endif