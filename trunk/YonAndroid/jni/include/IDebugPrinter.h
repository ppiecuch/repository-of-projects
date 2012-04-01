#ifndef _YON_DEBUG_IDEBUGPRINTER_H_
#define _YON_DEBUG_IDEBUGPRINTER_H_

#include "IReferencable.h"
#include "position2d.h"
#include "dimension2d.h"
#include "SColor.h"
#include "yonString.h"
#include "rect.h"

namespace yon{
	namespace video{
		class IVideoDriver;
		class ITexture;
		class IImage;
	}
namespace debug{

	video::IImage* createDebugPrinterTextureImage();
	core::dimension2du getDebugPrinterFontSize();

	class IDebugPrinter : public virtual core::IReferencable{
	public:
		virtual void drawString(const core::stringc& str,const core::position2di& pos=core::ORIGIN_POSITION2DI,const video::SColor& color=video::COLOR_WHITE) = 0;
	};

	IDebugPrinter* createDebugPrinter(video::IVideoDriver* driver,video::ITexture* texture);
}
}
#endif