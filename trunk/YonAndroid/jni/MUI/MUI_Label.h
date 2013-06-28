#ifndef _MUI_LABEL_H_
#define _MUI_LABEL_H_

namespace mui{

	//TextBox
	class Label : public Widget{
	protected:
		virtual void setPropertyOverride(const core::stringc& key, const core::stringc& value);

	public:
		Label();

		/** 
		 *@brief Get text region coordinate 
		 */
		virtual core::recti getTextRegion();

		/** 
		 *@brief Get text region size 
		 */
		virtual core::dimension2di getTextSize();

		/** 
		 *@brief Set widget caption 
		 */
		virtual void setCaption(const core::ustring& value);

		/** 
		 *@brief Get widget caption 
		 */
		virtual const core::ustring& getCaption();

		/** 
		 *@brief Set widget text font height 
		 */
		virtual void setFontHeight(s32 value);

		/** 
		 *@brief Get widget text font height 
		 */
		s32 getFontHeight();

		/** 
		 *@brief Set widget text align 
		 */
		virtual void setTextAlign(Align _value);

		/** 
		 *@brief Get widget text align 
		 */
		Align getTextAlign();

		/** 
		 *@brief Set widget text colour 
		 */
		virtual void setTextColor(const video::SColor& value);

		/** 
		 *@brief Get widget text colour 
		 */
		const video::SColor& getTextColor();

	};
}
#endif