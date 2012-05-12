#ifndef _YON_GUI_MYGUI_IMYGUITEXTURE_H_
#define _YON_GUI_MYGUI_IMYGUITEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"

#include "ITexture.h"

namespace yon{
namespace gui{
namespace mygui{

	class IMyGUITexture : public MyGUI::ITexture{
	public:
		virtual unsigned int getTextureId() = 0;
		virtual video::ITexture* getTexture() const = 0;
	};
}
}
}
#endif