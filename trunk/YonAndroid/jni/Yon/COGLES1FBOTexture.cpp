#include "COGLES1FBOTexture.h"

#include "ILogger.h"

namespace yon{
namespace video{
namespace ogles1{

	COGLES1FBOTexture::COGLES1FBOTexture(const core::dimension2du& size,
		const io::path& name,COGLES1Driver* driver, const ENUM_COLOR_FORMAT format)
		:COGLES1Texture(size,name,driver),m_fboId(0),m_rboId(0),m_format(format){

		m_bIsRenderTarget=true;

		logon();

		Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance COGLES1FBOTexture");
	}
	COGLES1FBOTexture::~COGLES1FBOTexture(){
		if(m_fboId)
			glDeleteFramebuffersOES(1, &m_fboId);
		if(m_rboId)
			glDeleteRenderbuffersOES(1, &m_rboId);

		Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release COGLES1FBOTexture");
	}

	void COGLES1FBOTexture::logon(){
		if(!m_fboId)
			glGenFramebuffersOES(1, &m_fboId);
		if(!m_rboId)
			glGenRenderbuffersOES(1, &m_rboId);
		if(!m_textureId)
			glGenTextures(1, &m_textureId);
		uploadTexture();
	}

	void COGLES1FBOTexture::logoff(){
		//if(m_textureId)
		//{
			//glDeleteTextures(1, &m_textureId);
			//m_textureId=0;
		//}
		/*if(m_fboId)
		{
			glDeleteFramebuffersOES(1, &m_fboId);
			m_fboId=0;
		}
		if(m_rboId)
		{
			glDeleteRenderbuffersOES(1, &m_rboId);
			m_rboId=0;
		}
		if(m_textureId)
		{
			glGenTextures(1, &m_textureId);
			m_textureId=0;
		}*/

		Logger->debug("COGLES1FBOTexture::logoff\r\n");
	}

	void COGLES1FBOTexture::uploadTexture(){
		GLint textureFormat;
		switch(m_format)
		{
		case ENUM_COLOR_FORMAT_R8G8B8:
		case ENUM_COLOR_FORMAT_R5G6B5:
			textureFormat=GL_RGB;
			break;
		default:
			textureFormat=GL_RGBA;
		}
		glBindFramebufferOES(GL_FRAMEBUFFER_OES,m_fboId);


		glBindRenderbufferOES(GL_RENDERBUFFER_OES,m_rboId);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, m_textureSize.w, m_textureSize.h);


		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, m_textureSize.w, m_textureSize.h, 0, textureFormat, GL_UNSIGNED_BYTE, NULL);

		glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, m_textureId, 0);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_rboId);

		glBindFramebufferOES(GL_FRAMEBUFFER_OES,0);

		checkFBOError();
	}

	void COGLES1FBOTexture::upsidedown(){
		Logger->warn(YON_LOG_WARN_FORMAT,"can not upside down a fbo texture, do nothing!");
	}

	void COGLES1FBOTexture::beginRTT(){
		//glViewport(0, 0, m_textureSize.w,m_textureSize.h);
		//core::recti r(0, 0, m_textureSize.w,m_textureSize.h);
		m_pDriver->setViewPort(m_viewport);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES,m_fboId);
		//m_pDriver->clearView(clearBackBuffer,clearZBuffer,color);
	}
	void COGLES1FBOTexture::endRTT(){
		glBindFramebufferOES(GL_FRAMEBUFFER_OES,0);

		/*if(willRenderFrameBuffer){
			const video::SClearSetting& setting=m_pDriver->getClearSetting();
			//glViewport(0,0,m_pDriver->getCurrentRenderTargetSize().w,m_pDriver->getCurrentRenderTargetSize().h);
			//core::recti r(0,0,m_pDriver->getCurrentRenderTargetSize().w,m_pDriver->getCurrentRenderTargetSize().h);
			m_pDriver->setViewPort(m_viewport);
			m_pDriver->clearView(setting.clearBackBuffer,setting.clearZBuffer,setting.color);
		}*/
	}

	bool COGLES1FBOTexture::checkFBOError(){
		GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);

		switch (status)
		{
		case GL_FRAMEBUFFER_COMPLETE_OES:
			Logger->info(YON_LOG_SUCCEED_FORMAT,"check FBO no error");
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES:
			Logger->error(YON_LOG_FAILED_FORMAT,"FBO has one or several incomplete image attachments");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES:
			Logger->error(YON_LOG_FAILED_FORMAT,"FBO missing an image attachment");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES:
			Logger->error(YON_LOG_FAILED_FORMAT,"FBO has one or several image attachments with different dimensions");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES:
			Logger->error(YON_LOG_FAILED_FORMAT,"FBO has one or several image attachments with different internal formats");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_OES:
			Logger->error(YON_LOG_FAILED_FORMAT,"FBO format unsupported");
			break;

		default:
			break;
		}
		Logger->error(YON_LOG_FAILED_FORMAT,"FBO has unknown error");
		return false;

	}
}
}
}