#ifndef _YON_GUI_IGUISYSTEM_H_
#define _YON_GUI_IGUISYSTEM_H_

#include "IReferencable.h"
#include "IRenderable.h"
#include "IResizable.h"
#include "IEventReceiver.h"
#include "path.h"
#include "xmldata.h"
#include "widget.h"
#include "rect.h"

namespace yon{
	namespace io{
		class IFileSystem;
	}
	namespace video{
		class IVideoDriver;
	}
namespace gui{

	class ITheme;
	class IWidget;
	class IButton;

	class IGUISystem : public core::IResizable,public core::IRenderable,public core::IReferencable,public event::IEventReceiver{
	public:
		// Define a function pointer to functions that create Primitive instances.
		typedef IWidget* (*ConstructorWrapper)(core::xmldata*);

		virtual ~IGUISystem(){}

		/**
		* @brief Static function that creates an instance of a widget object with the type specified,
		* 
		* @return an instance of a widget with specified type if has registered, otherwise return NULL.
		*/
		virtual IWidget* createWidget(widget::ENUM_TYPE type, core::xmldata* d) = 0;

		/**
		* @brief Static function that stores a pointer to the given function, linked to the type specified.
		* 
		* The function should create instances of Widget objects with the same type specified.
		*/
		virtual void registerWidgetType(widget::ENUM_TYPE type, ConstructorWrapper func) = 0;

		virtual IButton* addButton(IWidget* parent,const core::stringc& id,const core::recti& rectangle) = 0;

		virtual video::IVideoDriver* getVideoDriver() const = 0;

		virtual const IWidget* getRootWidget() const = 0;

		/** 
		 * @brief Sets the focus to an element.
		 *
		 * Causes a BLUR message followed by a FOCUS message. 
		 * If someone absorbed either of the messages,
		 * then the focus will not be changed.
		 * @param widget Pointer to the element which shall get the focus.
		 * @return true on success, false on failure
		 */
		virtual bool setFocus(IWidget* widget) = 0;

		/** 
		 * @brief Returns the widget which holds the focus.
		 *
		 * @return Pointer to the widget with focus.
		 */
		virtual IWidget* getFocus() const = 0;

		/**
		 * @brief Removes the focus from an element.
		 *
		 * Causes a BLUR message. If the event is absorbed
		 * then the focus will not be changed.
		 * @param widget Pointer to the element which shall lose the focus.
		 * @return true on success, false on failure
		 */
		virtual bool removeFocus(IWidget* widget) = 0;

		/** 
		 * @brief Returns whether the widget has focus
		 *
		 * @param widget Pointer to the widget which is tested.
		 * @return True if the element has focus, else false.
		 */
		virtual bool hasFocus(IWidget* widget) const = 0;

		virtual IWidget* getHovered() const = 0;

		virtual bool bindTheme(ITheme* theme) = 0;
		virtual ITheme* getBindedTheme() = 0;

		virtual ITheme* getTheme(const io::path& name) = 0;
		virtual bool removeTheme(const io::path& name) = 0;

		virtual core::stringc getStateName(widget::ENUM_STATE state) const = 0;
	};

	IGUISystem* createGUISystem(io::IFileSystem* fileSystem,video::IVideoDriver* videoDriver);
}
}
#endif