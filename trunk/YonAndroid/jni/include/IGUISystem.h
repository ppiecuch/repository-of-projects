#ifndef _YON_GUI_IGUISYSTEM_H_
#define _YON_GUI_IGUISYSTEM_H_

#include "IReferencable.h"
#include "IRenderable.h"
#include "IResizable.h"
#include "IEventReceiver.h"
#include "path.h"
#include "xmldata.h"
#include "widget.h"

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

		virtual video::IVideoDriver* getVideoDriver() const = 0;

		virtual const IWidget* getRootWidget() const = 0;

		virtual bool bindTheme(ITheme* theme) = 0;
		virtual ITheme* getBindedTheme() = 0;

		virtual ITheme* getTheme(const io::path& name) = 0;
		virtual bool removeTheme(ITheme* theme) = 0;
	};

	IGUISystem* createGUISystem(io::IFileSystem* fileSystem,video::IVideoDriver* videoDriver);
}
}
#endif