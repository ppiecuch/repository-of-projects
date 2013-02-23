#ifndef _YON_TEXT_IFONTPIECE_H_
#define _YON_TEXT_IFONTPIECE_H_

#include "yonUString.h"
#include "position2d.h"
#include "SColor.h"
#include "SFontIdentifier.h"

namespace yon{
namespace text{

	class IFontFamily;
	class IFontPiece{
	protected:
		core::ustring m_text;
		core::position2di m_pos;
		video::SColor m_color;
		SFontIdentifier m_identifier;
	public:
		IFontPiece(const core::ustring& text,const SFontIdentifier& identifier,const core::position2di& pos,const video::SColor& color)
			:m_text(text),m_identifier(identifier),m_pos(pos),m_color(color){}

		void u32 getFontCount() const{
			return m_text.size();
		}

		const core::ustring& getText() const{
			return m_text;
		}

		core::ustring getText(){
			return m_text;
		}

		virtual void setText(const core::ustring& text){
			m_text=text;
		}

		virtual IFontFamily* getFontFamily() const{
			return m_identifier.FontFamily;
		}

		virtual void setFontFamily(IFontFamily* fontFamily){
			m_identifier.FontFamily=fontFamily;
		}

		virtual u8 getFontSize() const{
			return m_identifier.FontSize;
		}

		virtual void setFontSize(u8 size){
			m_identifier.FontSize=size;
		}

		virtual MASK_FONT_STYLE getFontStyle(){
			return m_identifier.FontStyle;
		}

		virtual void setFontStyle(MASK_FONT_STYLE fontStyle){
			m_identifier.FontStyle=fontStyle;
		}

		const core::position2di& getPosition() const{
			return m_pos;
		}

		core::position2di getPosition(){
			return m_pos;
		}

		virtual void setPosition(const core::position2di& pos){
			m_pos=pos;
		}

		virtual const video::SColor& getColor() const{
			return m_color;
		}

		video::SColor getColor(){
			return m_color;
		}

		virtual void setColor(const video::SColor& color){
			m_color=color;
		}
	};
}
}
#endif