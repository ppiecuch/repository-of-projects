#ifndef _YON_GUI_MYGUI_OGLES1_COGLES1MYGUIENVIRENMENT_H_
#define _YON_GUI_MYGUI_OGLES1_COGLES1MYGUIENVIRENMENT_H_

#include "IMyGUIEnvirenment.h"
#include "COGLES1MyGUIRenderManager.h"

namespace yon{
namespace gui{
namespace mygui{
namespace ogles1{

	class COGLES1MyGUIEnvirenment : public IMyGUIEnvirenment{
	private:
		bool m_bIsInitialise;
		std::string m_rootMedia;
		COGLES1MyGUIRenderManager* m_pRenderManager;

		void setupResources();
		void addResourceLocation(const std::string& _name, bool _recursive = false);
	protected:
		virtual void shutdown();
	public:
		COGLES1MyGUIEnvirenment(io::IFileSystem* fs,video::IVideoDriver* driver,ITimer* timer);
		virtual ~COGLES1MyGUIEnvirenment();

		virtual IMyGUIRenderManager* getRenderManagerPtr(){
			return m_pRenderManager;
		}

		virtual void init();

		virtual void render();

		//virtual bool postEventFromUser(const event::SEvent& evt);
		virtual void onResize(const core::dimension2du& size);
	};
}
}
}
}
#endif