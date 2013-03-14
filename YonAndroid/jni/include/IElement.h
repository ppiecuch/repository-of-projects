#ifndef _YON_GUI_IELEMENT_H_
#define _YON_GUI_IELEMENT_H_

#include "symbol.h"
#include "yonString.h"

namespace yon{
namespace gui{

	//refer to:http://www.w3school.com.cn/tags/tag_input.asp
	/*
	button
	checkbox
	file
	hidden
	image
	password
	radio
	reset
	submit
	text
	*/

	/**
		A generic element in the DOM tree.
	*/

	class IElement{
	public:

		enum ENUM_ANCHOR
		{
			TOP = 1 << 0,
			BOTTOM = 1 << 1,
			LEFT = 1 << 2,
			RIGHT = 1 << 3,

			TOP_LEFT = TOP | LEFT,
			TOP_RIGHT = TOP | RIGHT,
			BOTTOM_LEFT = BOTTOM | LEFT,
			BOTTOM_RIGHT = BOTTOM | RIGHT
		};

		/// Constructs a new DOM element. This should not be called directly; use the Factory
		/// instead.
		/// @param[in] tag The tag the element was declared as in XHTML.
		//TODO 是否使用Element(const ENUM_TAG& tag);更好？
		//SElement(const core::stringc& tag);
		IElement(symbol::ENUM_TAG tag);
		virtual ~IElement();

		void update();
		//void render();

		/// Clones this element, returning a new, unparented element.
		//Element* Clone() const;

		/// Returns the visibility of the element.
		/// @return True if the element is visible, false otherwise.
		bool isVisible() const;

		//TODO 为什么是float型
		/// Returns the z-index of the element.
		/// @return The element's z-index.
		float getZIndex() const;

		/// Checks if a given point in screen coordinates lies within the bordered area of this element.
		/// @param[in] point The point to test.
		/// @return True if the element is within this element, false otherwise.
		virtual bool isPointWithinElement(const core::position2df& point);

		/// Gets the outer-most focus element down the tree from this node.
		/// @return Outer-most focus element.
		IElement* getFocusElement();

		IElement::ENUM_TAG getTag() const;

		/// Gets the name of the element.
		/// @return The name of the element.
		const core::stringc& getTagName() const;

		//TODO add SClass

		/// Returns one of this element's properties. If this element is not defined this property, or a parent cannot
		/// be found that we can inherit the property from, add the property to the element and return it;
		/// @param[in] name The name of the property to fetch the value for.
		/// @return The value of this property for this element.
		SProperty& getProperty(symbol::ENUM_PROPERTY sb){
			//TODO
		}

	};
}
}
#endif