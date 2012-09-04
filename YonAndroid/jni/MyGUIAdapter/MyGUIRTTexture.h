#ifndef _MYGUI_MYGUIRTTEXTURE_H_
#define _MYGUI_MYGUIRTTEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI{

class MyGUIRTTexture : public MyGUI::IRenderTarget{
private:
	RenderTargetInfo m_renderTargetInfo;
	int m_iWidth;
	int m_iHeight;
public:
	MyGUIRTTexture(unsigned int _texture);
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