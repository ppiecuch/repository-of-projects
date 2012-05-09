

#include "CTexture.h"

namespace yon{
namespace gui{
namespace mygui{

	CTexture::CTexture(const std::string& name)
		:m_name(name),m_iWidth(0),m_iHeight(0),m_bLock(false),
		m_format(MyGUI::PixelFormat::Unknow),m_usage(MyGUI::TextureUsage::Default),m_numElemBytes(0){
	}


	void  CTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format){}
	void  CTexture::loadFromFile(const std::string& _filename){
		destroy();
		//getTexture
	}
	void  CTexture::saveToFile(const std::string& _filename){
		//TODO
	}


	void* CTexture::lock(MyGUI::TextureUsage _access){
		//´´½¨Image
		//·µ»ØImage.lock
	}
	void CTexture::unlock(){
		//glTexImage....
		//delete image
		//setTexture(0,0);
	}

	void CTexture::destroy(){

		//erase texture

		m_iWidth = 0;
		m_iHeight = 0;
		m_bLock = false;
		m_numElemBytes = 0;
		m_format = PixelFormat::Unknow;
		m_usage = TextureUsage::Default;
	}

	MyGUI::IRenderTarget* CTexture::getRenderTarget(){
		//TODO RTT
		return NULL;
	}

	
}
}
}