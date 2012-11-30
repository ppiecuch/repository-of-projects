#include "MyGUIRenderManager.h"
#include "MyGUIRTTexture.h"

#include "ILogger.h"


namespace MyGUI{

	MyGUIRTTexture::MyGUIRTTexture(video::ITexture* texture,video::IVideoDriver* driver,MyGUI::MyGUIRenderManager* renderManager):
	//MyGUIRTTexture::MyGUIRTTexture(video::ITexture* texture):
		m_pTexture(texture),m_pRenderManager(renderManager),m_pDriver(driver)
	{
		f32 height=(f32)texture->getSize().h;
		f32 width=(f32)texture->getSize().w;
		m_renderTargetInfo.maximumDepth = 1.0f;
		m_renderTargetInfo.hOffset = 0;
		m_renderTargetInfo.vOffset = 0;
		m_renderTargetInfo.aspectCoef = height/width;
		m_renderTargetInfo.pixScaleX = 1.0f / width;
		m_renderTargetInfo.pixScaleY = 1.0f / height;

		//f32 hh=height/2;
		//f32 hw=width/2;

		//YON_DEBUG("half:%.2f,%.2f\r\n",hw,hh);

		//m_projection.makeIdentity();
		//m_projection.ortho(-hw,hw,-hh,hh,1,3000);
	}
	MyGUIRTTexture::~MyGUIRTTexture(){
	}

	void MyGUIRTTexture::begin(){
		//YON_DEBUG("beginRTT\r\n");
		m_pDriver->setRenderTarget(m_pTexture);
		//m_oldProjection=m_pDriver->getTransform(video::ENUM_TRANSFORM_PROJECTION);
		//m_pDriver->setTransform(video::ENUM_TRANSFORM_PROJECTION,m_projection);
	}
	void MyGUIRTTexture::end(){
		//YON_DEBUG("endRTT\r\n");
		//m_pDriver->setTransform(video::ENUM_TRANSFORM_PROJECTION,m_oldProjection);
		m_pDriver->setRenderTarget(NULL,true,true,video::COLOR_DEFAULT);
	}

	void MyGUIRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count){
		m_pRenderManager->doRender(_buffer,_texture,_count);
		//MyGUIRenderManager::getInstance().doRender(_buffer,_texture,_count);
	}

}