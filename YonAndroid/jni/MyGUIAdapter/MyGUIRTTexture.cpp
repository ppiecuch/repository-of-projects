#include "MyGUIRTTexture.h"

namespace MyGUI{

	MyGUIRTTexture::MyGUIRTTexture(video::ITexture* texture,const MyGUI::MyGUIRenderManager* renderManager):
		m_pTexture(texture),m_pRenderManager(renderManager){
		f32 height=(f32)texture->getSize().h;
		f32 width=(f32)texture->getSize().w;
		m_renderTargetInfo.maximumDepth = 1.0f;
		m_renderTargetInfo.hOffset = 0;
		m_renderTargetInfo.vOffset = 0;
		m_renderTargetInfo.aspectCoef = height/width;
		m_renderTargetInfo.pixScaleX = 1.0f / width;
		m_renderTargetInfo.pixScaleY = 1.0f / height;
	}
	MyGUIRTTexture::~MyGUIRTTexture(){
	}

	void MyGUIRTTexture::begin(){
		m_pTexture->beginRTT();
	}
	void MyGUIRTTexture::end(){
		m_pTexture->endRTT(false);
	}

	void MyGUIRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count){
		m_pRenderManager->doRender(_buffer,_texture,_count);
	}

}