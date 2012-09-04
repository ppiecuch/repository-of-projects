#ifndef _MYGUI_MYGUIRTTEXTURE_H_
#define _MYGUI_MYGUIRTTEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

#include "ITexture.h"

namespace MyGUI{

class MyGUIRTTexture : public MyGUI::IRenderTarget{
private:
	RenderTargetInfo m_renderTargetInfo;
	int m_iWidth;
	int m_iHeight;

	const MyGUI::MyGUIRenderManager* m_pRenderManager;
	video::ITexture* m_pTexture;
public:
	MyGUIRTTexture(video::ITexture* texture,const MyGUI::MyGUIRenderManager* renderManager);
	virtual ~MyGUIRTTexture();

	virtual void begin();
	virtual void end();

	virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

	virtual const RenderTargetInfo& getInfo()
	{
		return m_renderTargetInfo;
	}
};

}
#endif