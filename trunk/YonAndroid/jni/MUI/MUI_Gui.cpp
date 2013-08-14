#include "MUI_Gui.h"

namespace mui{

	Gui::Gui()
		:m_bIsInitialise(false),
		m_pInputManager(NULL),
		m_pSkinManager(NULL),
		m_pLayerManager(NULL),
		m_pLayoutManager(NULL),
		m_pResourceManager(NULL),
		m_pSubWidgetManager(NULL),
		m_pWidgetManager(NULL){

	}

	void Gui::initialize(const core::stringc& xml){
		YON_DEBUG_BREAK_IF(m_bIsInitialise==true);

		//TODO 部分可以单例化
		m_pInputManager=new InputManager;
		m_pSkinManager=new SkinManager;
		m_pLayerManager=new LayerManager;
		m_pLayoutManager=new LayoutManager;
		m_pResourceManager=new ResourceManager;
		m_pSubWidgetManager=new SubWidgetManager;
		m_pWidgetManager=new WidgetManager;

		//TODO 为了良好体验，部分模块是否延缓初始化？
		m_pResourceManager->initialise();
		m_pInputManager->initialise();
		m_pSkinManager->initialise();
		m_pLayoutManager->initialise();
		m_pLayerManager->initialise();
		m_pSubWidgetManager->initialise();
		m_pWidgetManager->initialise();

		//load the default settings if necessary
		//if (!_core.empty())
		//	mResourceManager->load(_core);

		m_bIsInitialise=true;
	}

	void Gui::shutdown(){
		m_pWidgetManager->shutdown();
		m_pSubWidgetManager->shutdown();
		m_pLayerManager->shutdown();
		m_pLayoutManager->shutdown();
		m_pSkinManager->shutdown();
		m_pInputManager->shutdown();
		m_pResourceManager->shutdown();
		
		delete m_pWidgetManager;
		delete m_pSubWidgetManager;
		delete m_pLayerManager;
		delete m_pLayoutManager;
		delete m_pSkinManager;
		delete m_pInputManager;
		delete m_pResourceManager;

		m_bIsInitialise=false;
	}

	Widget* baseCreateWidget(WidgetStyle style, const core::stringc& type, const core::stringc& skin, const core::coordi& coord, Align align, const core::stringc& layer, const core::stringc& name)
	{
		/*Widget* widget = WidgetManager::getInstance().createWidget(_style, _type, _skin, _coord, nullptr, nullptr, _name);
		mWidgetChild.push_back(widget);

		widget->setAlign(_align);

		// attach the widget with the level of
		if (!_layer.empty())
			LayerManager::getInstance().attachToLayerNode(_layer, widget);
		return widget;*/
		//TODO
	}

	Widget* Gui::createWidgetRealT(const core::stringc& type, const core::stringc& skin, f32 left, f32 top, f32 width, f32 height, Align align, const core::stringc& layer, const core::stringc& name){
		//IntSize size = RenderManager::getInstance().getViewSize();
		core::dimension2di size=RenderManager::getInstance().getViewSize();
		//return createWidgetT(_type, _skin, IntCoord((int)(_left * size.width), (int)(_top * size.height), (int)(_width * size.width), (int)(_height * size.height)), _align, _layer, _name);
		return baseCreateWidget(WidgetStyle::Overlapped, _type, _skin, _coord, _align, _layer, _name);
	}

	void Gui::destroyWidget(Widget* widget){

	}

	Widget* Gui::findWidgetT(const core::stringc& name, bool throwException){

	}

	void Gui::frameEvent(f32 time){

	}
}