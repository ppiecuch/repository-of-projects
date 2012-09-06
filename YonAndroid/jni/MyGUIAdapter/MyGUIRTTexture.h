#ifndef _MYGUI_MYGUIRTTEXTURE_H_
#define _MYGUI_MYGUIRTTEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

#include "ITexture.h"

using namespace yon;

namespace MyGUI{

	class MyGUIRenderManager;

class MyGUIRTTexture : public MyGUI::IRenderTarget{
private:
	RenderTargetInfo m_renderTargetInfo;
	MyGUI::MyGUIRenderManager* m_pRenderManager;
	video::ITexture* m_pTexture;
	video::IVideoDriver* m_pDriver;
	//core::matrix4f m_projection,m_oldProjection;
public:
	MyGUIRTTexture(video::ITexture* texture,video::IVideoDriver* driver,MyGUI::MyGUIRenderManager* renderManager);
	//MyGUIRTTexture(video::ITexture* texture);
	virtual ~MyGUIRTTexture();

	virtual void begin();
	virtual void end();

	virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

	video::ITexture* getTexture(){
		return m_pTexture;
	}

	virtual const RenderTargetInfo& getInfo()
	{
		return m_renderTargetInfo;
	}
};

}
#endif