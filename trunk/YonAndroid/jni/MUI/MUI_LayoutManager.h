#ifndef _MUI_LAYOUTMANAGER_H_
#define _MUI_LAYOUTMANAGER_H_

namespace mui{

	typedef delegates::CMultiDelegate3<Widget*, const core::stringc&, const core::stringc&> EventHandle_AddUserStringDelegate;

	class LayoutManager{
	private:
		bool m_bIsInitialise;
		core::stringc m_currentLayoutName;

		//void _load(xml::ElementPtr _node, const std::string& _file, Version _version);
		void load(xml::ElementPtr node, const core::stringc& file);
	public:
		LayoutManager();

		void initialize();
		void shutdown();

		/** Load layout file
		@param _file name of layout
		@param _prefix will be added to all loaded widgets names
		@param _parent widget to load on
		@return Return vector of pointers of loaded root widgets (root == without parents)
		*/
		//VectorWidgetPtr loadLayout(const std::string& _file, const std::string& _prefix = "", Widget* _parent = nullptr);
		bool loadLayout(const core::stringc& file, core::array<Widget*>& result,const core::stringc& prefix = "", Widget* parent = NULL);

		/** Unload layout (actually deletes vector of widgets returned by loadLayout) */
		//void unloadLayout(VectorWidgetPtr& _widgets);

		const core::stringc& getCurrentLayout() const;

		/** Event : Multidelegate. UserString was added from layout.\n
		signature : void method(MyGUI::Widget* _widget, const std::string& _key, const std::string& _value)
		@param _widget Widget that got new UserString.
		@param _key UserString key.
		@param _key UserString value.
		@note Happens only when UserString was loaded from layout, but not when it was added in code.
		*/
		EventHandle_AddUserStringDelegate eventAddUserString;

	};
}
#endif