#include "MyGUITexture.h"

#include "ILogger.h"

namespace MyGUI{

	MyGUITexture::MyGUITexture(const std::string& name,video::IVideoDriver* driver)
		:m_name(name),m_bLock(false),m_pDriver(driver),m_pTexture(NULL),
		m_usage(MyGUI::TextureUsage::Default),m_numElemBytes(0){
	}
	MyGUITexture::~MyGUITexture(){
		destroy();
	}

	void  MyGUITexture::createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format){
		video::ENUM_COLOR_FORMAT format;
		if (_format == MyGUI::PixelFormat::L8){
			format=video::ENUM_COLOR_FORMAT_L8;
		}else if (_format == MyGUI::PixelFormat::L8A8){
			format=video::ENUM_COLOR_FORMAT_L8A8;
		}else if (_format == MyGUI::PixelFormat::R8G8B8){
			format=video::ENUM_COLOR_FORMAT_R8G8B8;
		}else if (_format == MyGUI::PixelFormat::R8G8B8A8){
			format=video::ENUM_COLOR_FORMAT_R8G8B8A8;
		}else{
			Logger->warn(YON_LOG_WARN_FORMAT,"do not support PixelFormat currently!");
			return;
		}
		const bool useMipmap=m_pDriver->getTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_MIPMAPS);
		const bool use16bit1Alpha=m_pDriver->getTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_1ALPHA);
		const bool use16bit4Alpha=m_pDriver->getTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA);
		const bool reserveImage=m_pDriver->getTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_MIPMAPS,false);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_1ALPHA,false);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA,false);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE,true);
		m_pTexture = static_cast<video::ITexture*>(m_pDriver->addTexture(core::dimension2du(_width,_height),io::path(m_name.c_str()),format));
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_MIPMAPS,useMipmap);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_1ALPHA,use16bit1Alpha);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA,use16bit4Alpha);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE,reserveImage);
	}

	void  MyGUITexture::loadFromFile(const std::string& _filename){
		destroy();
		const bool useMipmap=m_pDriver->getTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_MIPMAPS);
		const bool use16bit1Alpha=m_pDriver->getTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_1ALPHA);
		const bool use16bit4Alpha=m_pDriver->getTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA);
		const bool reserveImage=m_pDriver->getTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_MIPMAPS,false);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_1ALPHA,false);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA,false);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE,true);
		m_pTexture=m_pDriver->getTexture(io::path(_filename.c_str()));
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_MIPMAPS,useMipmap);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_1ALPHA,use16bit1Alpha);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_16BIT_4ALPHA,use16bit4Alpha);
		m_pDriver->setTextureCreationConfig(video::MASK_TEXTURE_CREATION_CONFIG_RESERVE_IMAGE,reserveImage);
		m_pTexture->upsidedown();
		Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("loadFromFile:%s",_filename.c_str()).c_str());
	}

	void  MyGUITexture::saveToFile(const std::string& _filename){
		//TODO
		Logger->warn(YON_LOG_WARN_FORMAT,"saveToFile not support");
	}

	void* MyGUITexture::lock(MyGUI::TextureUsage _access){
		return m_pTexture->lock();
	}
	void MyGUITexture::unlock(){
		m_pTexture->unlock();
	}
	void MyGUITexture::destroy(){
		m_pDriver->removeTexture(m_pTexture);

		m_bLock = false;
		m_numElemBytes = 0;
		m_usage = MyGUI::TextureUsage::Default;
	}
	MyGUI::IRenderTarget* MyGUITexture::getRenderTarget(){
		//TODO RTT
		Logger->warn(YON_LOG_WARN_FORMAT,"getRenderTarget not support");
		return NULL;
	}

}