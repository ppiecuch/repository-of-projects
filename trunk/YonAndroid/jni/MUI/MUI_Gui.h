#ifndef _MUI_GUI_H_
#define _MUI_GUI_H_

using namespace yon;

namespace mui{

	class Gui{
	private:
		InputManager* m_pInputManager;
		SkinManager* m_pSkinManager;
		LayoutManager* m_pLayoutManager;
		ResourceManager* m_pResourceManager;
		SubWidgetManager* m_pSubWidgetManager;
		WidgetManager* m_pWidgetManager;
		//TODO 允许继承
		//LanguageManager* mLanguageManager;
		//TODO 允许继承
		//FontManager* mFontManager;
	public:
		Gui();

		/** 
		* @brief Initialise GUI and all GUI Managers
		* @param[in] xml name of core config file for MyGUI (contain main config files with skins, layers, fonts, etc.)
		*/
		//TODO 不从资源加载
		void initialise(const core::stringc& xml = "MyGUI_Core.xml");

		/** 
		* @brief Shutdown GUI and all GUI Managers
		*/
		void shutdown();

		/** 
		* @brief Create widget using coordinates relative to parent. see Gui::createWidgetT 
		*/
		//TODO 使用enum WidgetType
		Widget* createWidgetRealT(const core::stringc& type, const core::stringc& skin, f32 left, f32 top, f32 width, f32 height, Align align, const core::stringc& layer, const core::stringc& name = "");

		/** 
		* @brief Destroy any created widget 
		*/
		void destroyWidget(Widget* widget);

		/** 
		* @brief Find widget by name
		*
		* If widget is not found the exception will be thrown, or if the second parameter is false the nullptr pointer will be returned
		*/
		Widget* findWidgetT(const core::stringc& name, bool throwException = true);

		/** 
		* @brief Inject frame entered event (called be renderer, do not call it manually).
		*
		* This function is called every frame by renderer.
		*/
		void frameEvent(f32 time);
	};
}
#endif