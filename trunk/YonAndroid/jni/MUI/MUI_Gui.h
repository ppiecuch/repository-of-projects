#ifndef _MUI_GUI_H_
#define _MUI_GUI_H_

using namespace yon;

namespace mui{

	class Gui{
	private:
		//负责输入事件的管理与分派
		InputManager* m_pInputManager;
		//TODO 具体职责
		SkinManager* m_pSkinManager;
		//TODO 具体职责
		LayerManager* m_pLayerManager;
		//TODO 具体职责
		LayoutManager* m_pLayoutManager;
		//TODO 具体职责
		ResourceManager* m_pResourceManager;
		//TODO 具体职责
		SubWidgetManager* m_pSubWidgetManager;
		//TODO 具体职责
		WidgetManager* m_pWidgetManager;
		//TODO 允许继承
		//LanguageManager* mLanguageManager;
		//TODO 允许继承
		//FontManager* mFontManager;

		bool m_bIsInitialise;

		Widget* baseCreateWidget(WidgetStyle style, const core::stringc& type, const core::stringc& skin, const core::coordi& coord, Align align, const core::stringc& layer, const core::stringc& name);
	public:
		Gui();

		/** 
		* @brief Initialize GUI and all GUI Managers
		* @param[in] xml name of core config file for MUI (contain main config files with skins, layers, fonts, etc.)
		*/
		//TODO 不从资源加载
		void initialize(const core::stringc& xml = "MUI_Core.xml");

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