#ifndef _YON_TEXT_ITEXTSYSTEM_H_
#define _YON_TEXT_ITEXTSYSTEM_H_

#include "IReferencable.h"
#include "IRenderable"
#include "yonUString.h"
#include "IFontPiece.h"

namespace yon{
namespace text{

	//字体缓冲管理策略
	enum ENUM_FONT_CACHE_STRATEGY{
		ENUM_FONT_CACHE_STRATEGY_STATIC = 0,
		ENUM_FONT_CACHE_STRATEGY_COUNT
	};

	class ITextSystem : public virtual core::IReferencable,public virtual core::IResizable,public virtual core::IRenderable{
	public:

		virtual IFontFamily* getFontFamily(const io::path& filename,ENUM_FONT_CACHE_STRATEGY strategy=ENUM_FONT_CACHE_STRATEGY_STATIC) = 0;

		virtual IFontPiece* addText(const core::ustring& text, IFontFamily* fontFamily, const core::position2di& pos, u8 fontSize=12, const video::SColor& color=video::COLOR_BLACK, MASK_FONT_STYLE fontStyle=MASK_FONT_STYLE_NORMAL) = 0;

	};
}
}
#endif

