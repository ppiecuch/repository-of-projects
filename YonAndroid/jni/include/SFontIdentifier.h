#ifndef _YON_TEXT_SFONTIDENTIFIER_H_
#define _YON_TEXT_SFONTIDENTIFIER_H_

#include "IFontFamily.h"

namespace yon{
namespace text{

	//字体风格
	enum MASK_FONT_STYLE{
		MASK_FONT_STYLE_NORMAL = 0x00,		//默认
		MASK_FONT_STYLE_BOLD = 0x01,		//粗体
		MASK_FONT_STYLE_ITALIC = 0x02,		//斜体
		MASK_FONT_STYLE_UNDERLINE = 0x04,	//下划线
		MASK_FONT_STYLE_STRICKOUT = 0x08,	//删除线
		MASK_FONT_STYLE_STROKE = 0x16,		//描边
		MASK_FONT_STYLE_COUNT = 6
	};

	//字体渲染模式
	enum ENUM_FONT_RENDER_MODE{
		ENUM_FONT_RENDER_MODE_MONO,
		ENUM_FONT_RENDER_MODE_NORMAL,
		ENUM_FONT_RENDER_MODE_COUNT
	};

	//字体识别符
	struct SFontIdentifier{
		IFontFamily* FontFamily;
		u8 FontSize;
		ENUM_FONT_RENDER_MODE RenderMode;
		MASK_FONT_STYLE FontStyle;

		SFontIdentifier(IFontFamily* fontFamily,u8 fontSize,MASK_FONT_STYLE fontStyle,ENUM_FONT_RENDER_MODE renderMode)
			:FontFamily(fontFamily),FontSize(fontSize),FontStyle(fontStyle),RenderMode(renderMode){}

		inline bool operator < (const SFontIdentifier& other) const
		{
			if(FontFamily!=other.FontFamily)
				return FontFamily<other.FontFamily;
			else if(FontSize!=other.FontSize)
				return FontSize<other.FontSize;
			else if(FontStyle!=other.FontStyle)
				return FontStyle<other.FontStyle;
			else
				return RenderMode<other.RenderMode;
		}

		inline bool operator!=(const SFontIdentifier& other) const{
			return FontFamily!=other.FontFamily ||
				FontSize!=other.FontSize ||
				FontStyle!=other.FontStyle ||
				RenderMode!=other.RenderMode;
		}

		inline bool operator==(const SFontIdentifier& other) const
		{ return !(other!=*this); }
	};
}
}
#endif