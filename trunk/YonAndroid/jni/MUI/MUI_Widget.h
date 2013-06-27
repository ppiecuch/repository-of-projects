#ifndef _MUI_WIDGET_H_
#define _MUI_WIDGET_H_

namespace mui{

	//Widget
	class Widget
		:public ICroppedRectangle, public SkinLayerItem,{

	public:

		/**
		 *@brief Is this widget is root widget (root == without parents) 
		 */
		bool isRootWidget() const;

		/**
		 *@brief Get name of widget
		 */
		const core::stringc& getName() const;

		/**
		 *@brief Hide or show widget 
		 */
		virtual void setVisible(bool on);

		/** 
		 *@return Return true if visible 
		 */
		//bool getVisible() const;
		bool isVisible() const;

		/** 
		 *@brief Enable or disable widget 
		 */
		virtual void setEnabled(bool on);

		/** 
		 *@brief Is widget enabled 
		 */
		bool getEnabled() const;

		/** 
		 *@brief Set align 
		 */
		virtual void setAlign(Align value);

		/** 
		 *@brief Get align 
		 */
		Align getAlign() const;

		/** 
		 *@brief Set widget opacity 
		 */
		void setAlpha(f32 value);

		/** 
		 *@brief Get widget opacity 
		 */
		f32 getAlpha() const;

		/** 
		 *@brief Get parent widget or NULL if no parent 
		 */
		Widget* getParent() const;

		//IntSize getParentSize() const;
		core::dimension2di getParentSize() const;

		/** 
		 *@brief Get child count 
		 */
		//size_t getChildCount();
		u32 getChildrenCount();

		/** 
		 *@brief Get child by index (index from 0 to child_count - 1) 
		 */
		//Widget* getChildAt(size_t _index);
		Widget* getChild(u32 index);

		/** 
		 *@brief Get client area widget or nullptr if widget don't have client 
		 */
		//Widget* getClientWidget();
		Widget* getClientArea();

		/** 
		 *@brief Detach widget from widgets hierarchy
		 *@param layer Attach to specified layer (if any)
		 */
		void detachFromWidget(const core::stringc& layer = "");

		/** 
		 *@brief Attach widget to parent
		 *@param _parent New parent
		 *@param _style New widget style (see WidgetStyle::Enum)
		 *@param _layer Attach to specified layer (if any)
		 */
		void attachToWidget(Widget* parent, WidgetStyle style = WidgetStyle::Child, const core::stringc& layer = "");

		/** 
		 *@brief Change widget skin 
		 */
		void changeWidgetSkin(const core::stringc& skinName);

		/** 
		 *@brief Set widget style.
		 *@param _style New widget style (see WidgetStyle::Enum)
		 *@param _layer Attach to specified layer (if any)
		 *@note When choosing WidgetStyle::Popup style you also need attach widget to layer
		 */
		void setWidgetStyle(WidgetStyle style, const core::stringc& layer = "");

		/** 
		 *@brief Get widget style 
		 */
		WidgetStyle getWidgetStyle() const;


	};
}
#endif