#ifndef _YON_GUI_MYGUI_IMYGUIRENDERMANAGER_H_
#define _YON_GUI_MYGUI_IMYGUIRENDERMANAGER_H_

#include <MyGUI.h>
#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

namespace yon{
namespace gui{
namespace mygui{

	class IMyGUIRenderManager : public MyGUI::RenderManager, public MyGUI::IRenderTarget{
	public:
		static IMyGUIRenderManager& getInstance()
		{
			return *getInstancePtr();
		}
		static IMyGUIRenderManager* getInstancePtr()
		{
			return static_cast<IMyGUIRenderManager*>(MyGUI::RenderManager::getInstancePtr());
		}
	};
}
}
}
#endif