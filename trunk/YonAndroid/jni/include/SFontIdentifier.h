#ifndef _YON_TEXT_SFONTIDENTIFIER_H_
#define _YON_TEXT_SFONTIDENTIFIER_H_

#include "IFontFamily.h"

namespace yon{
namespace text{

	//������
	enum MASK_FONT_STYLE{
		MASK_FONT_STYLE_NORMAL = 0x00,		//Ĭ��
		MASK_FONT_STYLE_BOLD = 0x01,		//����
		MASK_FONT_STYLE_ITALIC = 0x02,		//б��
		MASK_FONT_STYLE_UNDERLINE = 0x04,	//�»���
		MASK_FONT_STYLE_STRICKOUT = 0x08,	//ɾ����
		MASK_FONT_STYLE_STROKE = 0x16,		//���
		MASK_FONT_STYLE_COUNT = 6
	};

	//����ʶ���
	struct SFontIdentifier{
		IFontFamily* FontFamily;
		u8 FontSize;
		MASK_FONT_STYLE FontStyle;

		SFontIdentifier(IFontFamily* fontFamily,u8 fontSize,MASK_FONT_STYLE fontStyle)
			:FontFamily(fontFamily),FontSize(fontSize),FontStyle(fontStyle){}

		inline bool operator < (const SFontIdentifier& other) const
		{
			if(FontFamily!=other.FontFamily)
				return FontFamily<other.FontFamily;
			else if(FontSize!=other.FontSize)
				return FontSize<other.FontSize;
			else
				return FontStyle<other.FontStyle;
		}

		inline bool operator!=(const SFontIdentifier& other) const{
			return FontFamily!=other.FontFamily ||
				FontSize!=other.FontSize ||
				FontStyle!=other.FontStyle;
		}

		inline bool operator==(const SFontIdentifier& other) const
		{ return !(other!=*this); }
	};
}
}
#endif