#ifndef _YON_DEBUG_CDEBUGPRINTER_H_
#define _YON_DEBUG_CDEBUGPRINTER_H_

#include "IDebugPrinter.h"
#include "yonString.h"
#include "position2d.h"
#include "rect.h"
#include "objectpool.h"
#include "SColor.h"
#include "SVertex.h"
#include "IUnit.h"
#include "matrix4.h"
#include "IResizable.h"

namespace yon{
namespace debug{

	class Shap2DRecyclable : public core::IRecyclable{
	public:
		scene::S2DVertex vertices[4];
		static u16 indices[6];

		virtual void reset(){}

		void set(f32 x0,f32 y0,f32 x1,f32 y1,f32 u0,f32 v0,f32 u1,f32 v1,const video::SColor& color){
			/*
			shap->m_vertices.push_back(S2DVertex((f32)x0,(f32)y0,u0,v0,color));
			shap->m_vertices.push_back(S2DVertex((f32)x1,(f32)y0,u1,v0,color));
			shap->m_vertices.push_back(S2DVertex((f32)x1,(f32)y1,u1,v1,color));
			shap->m_vertices.push_back(S2DVertex((f32)x0,(f32)y1,u0,v1,color));
			*/
			vertices[0].pos.x=vertices[3].pos.x=x0;
			vertices[0].pos.y=vertices[1].pos.y=y0;
			vertices[1].pos.x=vertices[2].pos.x=x1;
			vertices[2].pos.y=vertices[3].pos.y=y1;

			vertices[0].texcoords.x=vertices[3].texcoords.x=u0;
			vertices[0].texcoords.y=vertices[1].texcoords.y=v0;
			vertices[1].texcoords.x=vertices[2].texcoords.x=u1;
			vertices[2].texcoords.y=vertices[3].texcoords.y=v1;

			vertices[0].color=vertices[1].color=vertices[2].color=vertices[3].color=color;

		}
	};

	typedef core::CObjectPool<Shap2DRecyclable> Shap2DPool;
		
	class CDebugPrinter : public IDebugPrinter,public core::IResizable{
	private:
		video::IVideoDriver* m_pDriver;
		video::ITexture* m_pTexture;
		scene::IGeometryFactory* m_pGeometryFty;
		core::dimension2du m_fontSize;
		//core::recti*** m_texcoords;
		core::rectf*** m_texcoords;
		core::dimension2du m_fontStep;
		Shap2DPool m_pool;
		scene::IShap* m_pShap;
		scene::IUnit* m_pUnit;

		core::matrix4f m_projection;
		core::matrix4f m_view;
		core::matrix4f m_oldProjection;
		core::matrix4f m_oldWorld;
		core::matrix4f m_oldView;

		u32 m_uRowCount;
		u32 m_uCharCountPerRow;
	public:
		CDebugPrinter(video::IVideoDriver* driver,video::ITexture* texture,scene::IGeometryFactory* geometryFty);
		~CDebugPrinter();
		virtual void drawString(const core::stringc& str,const core::position2di& pos,const video::SColor& color);
		virtual video::ITexture* getTexture() const{
			return m_pTexture;
		};
		virtual void onResize(const core::dimension2du& size);
		virtual void setTexture(video::ITexture* texture){
			m_pTexture=texture;

			m_uRowCount=m_pTexture->getSize().h/m_fontSize.h;
		}
	};
}
}
#endif