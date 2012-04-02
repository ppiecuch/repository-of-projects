#ifndef _YON_DEBUG_CDEBUGPRINTER_H_
#define _YON_DEBUG_CDEBUGPRINTER_H_

#include "IDebugPrinter.h"
#include "yonString.h"
#include "position2d.h"
#include "rect.h"

namespace yon{
namespace debug{
		
	class CDebugPrinter : public IDebugPrinter{
	private:
		video::IVideoDriver* m_pDriver;
		video::ITexture* m_pTexture;
		scene::IGeometryFactory* m_pGeometryFty;
		core::dimension2du m_fontSize;
		//core::recti*** m_texcoords;
		core::rectf*** m_texcoords;
		core::dimension2du m_fontStep;
	public:
		CDebugPrinter(video::IVideoDriver* driver,video::ITexture* texture,scene::IGeometryFactory* geometryFty);
		~CDebugPrinter();
		virtual void drawString(const core::stringc& str,const core::position2di& pos,const video::SColor& color);
	};
}
}
#endif