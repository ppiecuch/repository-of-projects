#ifndef _MUI_GUI_H_
#define _MUI_GUI_H_

//refer to:FactoryManager ��Ҫ����ؼ�������Delegate��ע��Map 
//http://blog.csdn.net/geometry_/article/details/7354252

#include "MUI_Prepare.h"
#include "MUI_Align.h"
#include "MUI_WidgetStyle.h"

namespace mui{

	typedef core::CMultiDelegate1<u32> EventHandle_FrameEventDelegate;

	class Gui{
	private:
		//���Ǽ̳��ڻ���IObject �����඼����ͨ��FactoryManger������
		FactoryManager* m_pFactoryManager;
		//���������¼��Ĺ��������
		//InputManager* m_pInputManager;
		//TODO ����ְ��
		//SkinManager* m_pSkinManager;
		//TODO ����ְ��
		//LayerManager* m_pLayerManager;
		//TODO ����ְ��
		//LayoutManager* m_pLayoutManager;
		//TODO ����ְ��
		//ResourceManager* m_pResourceManager;
		//TODO ����ְ��
		//SubWidgetManager* m_pSubWidgetManager;
		//TODO ����ְ��
		//WidgetManager* m_pWidgetManager;
		//TODO ����̳�
		//LanguageManager* mLanguageManager;
		//TODO ����̳�
		//FontManager* mFontManager;

		bool m_bIsInitialise;

		//VectorWidgetPtr mWidgetChild;
		core::array<Widget*> m_widgetChildren;

		Widget* baseCreateWidget(WidgetStyle style, const core::stringc& type, const core::stringc& skin, const core::coordi& coord, Align align, const core::stringc& layer, const core::stringc& name);

		//removes all children
		//void _destroyAllChildWidget();
		void destroyAllChildWidget();
	public:
		Gui();

		/** 
		* @brief Initialize GUI and all GUI Managers
		* @param[in] xml name of core config file for MUI (contain main config files with skins, layers, fonts, etc.)
		*/
		//TODO ������Դ����
		void initialize(const core::stringc& xml = "MUI_Core.xml");

		/** 
		* @brief Shutdown GUI and all GUI Managers
		*/
		void shutdown();

		/** 
		* @brief Create widget using coordinates relative to parent. see Gui::createWidgetT 
		*/
		//TODO ʹ��enum WidgetType
		Widget* createWidgetRealT(const core::stringc& type, const core::stringc& skin, f32 left, f32 top, f32 width, f32 height, Align align, const core::stringc& layer, const core::stringc& name = "");

		/** 
		* @brief Destroy any created widget 
		*/
		//void destroyWidget(Widget* widget);

		/** 
		* @brief Find widget by name
		*
		* If widget is not found the exception will be thrown, or if the second parameter is false the nullptr pointer will be returned
		*/
		//Widget* findWidgetT(const core::stringc& name, bool throwException = true);
		Widget* findWidgetT(const core::stringc& name);

		/** 
		* @brief Inject frame entered event (called be renderer, do not call it manually).
		*
		* This function is called every frame by renderer.
		*/
		//void frameEvent(float _time);
		void onFrame(u32 time);

		/** 
		* @brief Event : Multidelegate. GUI per frame call.
		*
		* signature : void method(float time)
		* @param time Time elapsed since last frame
		*/
		//EventHandle_FrameEventDelegate eventFrameStart;
		EventHandle_FrameEventDelegate frameEvents;
	};
}
#endif