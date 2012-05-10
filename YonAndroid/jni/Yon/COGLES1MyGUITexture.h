#ifndef _YON_GUI_MYGUI_OGLES1_COGLES1MYGUITEXTURE_H_
#define _YON_GUI_MYGUI_OGLES1_COGLES1MYGUITEXTURE_H_

#include "IMyGUITexture.h"
#include "COGLES1Driver.h"
#include "COGLES1Texture.h"

namespace yon{
namespace gui{
namespace mygui{
namespace ogles1{

	class COGLES1MyGUITexture : public IMyGUITexture{
	private:
		std::string m_name;
		bool m_bLock;
		MyGUI::PixelFormat m_format;
		MyGUI::TextureUsage m_usage;
		size_t m_numElemBytes;

		video::ogles1::COGLES1Driver* m_pDriver;
		video::ogles1::COGLES1Texture* m_pTexture;
	public:
		COGLES1MyGUITexture(const std::string& name,video::ogles1::COGLES1Driver* driver);
		~COGLES1MyGUITexture();

		virtual const std::string& getName() const{
			return m_name;
		}

		virtual void createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename);

		virtual void destroy();

		virtual int getWidth(){
			if(m_pTexture)
				return m_pTexture->getSize().w;
			return 0;
		}
		virtual int getHeight(){
			if(m_pTexture)
				return m_pTexture->getSize().h;
			return 0;
		}

		virtual void* lock(MyGUI::TextureUsage _access);
		virtual void unlock();
		virtual bool isLocked(){
			return m_bLock;
		}

		virtual MyGUI::PixelFormat getFormat(){
			return m_format;
		}
		virtual MyGUI::TextureUsage getUsage(){
			return m_usage;
		}
		virtual size_t getNumElemBytes(){
			return m_numElemBytes;
		}

		virtual MyGUI::IRenderTarget* getRenderTarget();

		virtual unsigned int getTextureId(){
			if(m_pTexture)
				return m_pTexture->getTextureId();
			return 0;
		}

	};
}
}
}
}
#endif