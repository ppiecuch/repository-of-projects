#include "MUI_Gui.h"
#include "MUI_FactoryManager.h"
#include "MUI_Widget.h"

namespace mui{

	Gui::Gui()
		:m_pFactoryManager(NULL),
		//m_pInputManager(NULL),
		//m_pSkinManager(NULL),
		//m_pLayerManager(NULL),
		//m_pLayoutManager(NULL),
		//m_pResourceManager(NULL),
		//m_pSubWidgetManager(NULL),
		//m_pWidgetManager(NULL),
		m_bIsInitialise(false){

	}

	void Gui::initialize(const core::stringc& xml){
		YON_DEBUG_BREAK_IF(m_bIsInitialise==true);

		//TODO 部分可以单例化
		//m_pFactoryManager=new FactoryManager;
		FactoryManager::create();
		//m_pResourceManager=new ResourceManager;
		//m_pInputManager=new InputManager;
		//m_pSkinManager=new SkinManager;
		//m_pLayoutManager=new LayoutManager;
		//m_pLayerManager=new LayerManager;
		//m_pSubWidgetManager=new SubWidgetManager;
		//m_pWidgetManager=new WidgetManager;

		//TODO 为了良好体验，部分模块可否延缓初始化？
		m_pFactoryManager->initialize();
		//m_pResourceManager->initialize();
		//m_pInputManager->initialize();
		//m_pSkinManager->initialize();
		//m_pLayoutManager->initialize();
		//m_pLayerManager->initialize();
		//m_pSubWidgetManager->initialize();
		//m_pWidgetManager->initialize();

		//load the default settings if necessary
		//if (!_core.empty())
		//	mResourceManager->load(_core);

		m_bIsInitialise=true;
	}

	void Gui::shutdown(){

		destroyAllChildWidget();

		//m_pWidgetManager->shutdown();
		//m_pSubWidgetManager->shutdown();
		//m_pLayerManager->shutdown();
		//m_pLayoutManager->shutdown();
		//m_pSkinManager->shutdown();
		//m_pInputManager->shutdown();
		//m_pResourceManager->shutdown();
		m_pFactoryManager->shutdown();
		
		//delete m_pWidgetManager;
		//delete m_pSubWidgetManager;
		//delete m_pLayerManager;
		//delete m_pLayoutManager;
		//delete m_pSkinManager;
		//delete m_pInputManager;
		//delete m_pResourceManager;
		//delete m_pFactoryManager;
		m_pFactoryManager->destroy();

		m_bIsInitialise=false;
	}

	Widget* Gui::baseCreateWidget(WidgetStyle style, const core::stringc& type, const core::stringc& skin, const core::coordi& coord, Align align, const core::stringc& layer, const core::stringc& name)
	{
		/*Widget* widget = WidgetManager::getInstance().createWidget(_style, _type, _skin, _coord, nullptr, nullptr, _name);
		mWidgetChild.push_back(widget);

		widget->setAlign(_align);

		// attach the widget with the level of
		if (!_layer.empty())
			LayerManager::getInstance().attachToLayerNode(_layer, widget);
		return widget;*/
		//TODO
		return NULL;
	}

	void Gui::destroyAllChildWidget(){
		/*while (!mWidgetChild.empty())
		{
			// сразу себя отписывем, иначе вложенной удаление убивает все
			Widget* widget = mWidgetChild.back();
			mWidgetChild.pop_back();

			// отписываем от всех
			mWidgetManager->unlinkFromUnlinkers(widget);

			// и сами удалим, так как его больше в списке нет
			WidgetManager::getInstance()._deleteWidget(widget);
		}*/

		//TODO 为什么要反序？==>难道是出于数组删除的性能考虑
		for(u32 i=m_widgetChildren.size()-1;i>=0;--i)
		{
			Widget* widget=m_widgetChildren[i];
			m_widgetChildren.erase(i);

			//TODO
		}
	}

	Widget* Gui::createWidgetRealT(const core::stringc& type, const core::stringc& skin, f32 left, f32 top, f32 width, f32 height, Align align, const core::stringc& layer, const core::stringc& name){
		//IntSize size = RenderManager::getInstance().getViewSize();
		//return createWidgetT(_type, _skin, IntCoord((int)(_left * size.width), (int)(_top * size.height), (int)(_width * size.width), (int)(_height * size.height)), _align, _layer, _name);

		//core::dimension2di size=RenderManager::getInstance().getViewSize();
		//TODO 为什么直接使用了Overlapped，使用Shared性能更优吧
		//return baseCreateWidget(WidgetStyle::Overlapped, type, skin, coord, align, layer, name);
		return NULL;
	}

	Widget* Gui::findWidgetT(const core::stringc& name){
		/*for (VectorWidgetPtr::iterator iter = mWidgetChild.begin(); iter != mWidgetChild.end(); ++iter)
		{
			Widget* widget = (*iter)->findWidget(_name);
			if (widget != nullptr) return widget;
		}
		MYGUI_ASSERT(!_throw, "Widget '" << _name << "' not found");
		return nullptr;
		for(u32 i=0;i<m_widgetChildren.size();++i)
		{
			if(m_widgetChildren[i]->findWidget())
		}*/
		//TODO
		return NULL;
	}

	void Gui::onFrame(u32 time){
		frameEvents(time);
	}
}