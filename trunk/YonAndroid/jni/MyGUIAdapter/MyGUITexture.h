#ifndef _MYGUI_MYGUITEXTURE_H_
#define _MYGUI_MYGUITEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"

#include "IVideoDriver.h"
#include "ITexture.h"

using namespace yon;

namespace MyGUI{

	class MyGUIRTTexture;
	class MyGUIRenderManager;

	class MyGUITexture : public MyGUI::ITexture{
	private:
		std::string m_name;
		bool m_bLock;
		MyGUI::TextureUsage m_usage;
		//size_t m_numElemBytes;

		const MyGUI::MyGUIRenderManager* m_pRenderManager;
		MyGUI::MyGUIRTTexture* m_pRenderTarget;

		video::IVideoDriver* m_pDriver;
		video::ITexture* m_pTexture;
	public:
		MyGUITexture(const std::string& name,video::IVideoDriver* driver,const MyGUI::MyGUIRenderManager* renderManager);
		//MyGUITexture(const std::string& name,video::IVideoDriver* driver);
		~MyGUITexture();

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
			if(m_pTexture){
				switch(m_pTexture->getColorFormat())
				{
				case video::ENUM_COLOR_FORMAT_L8:
					return MyGUI::PixelFormat::L8;
				case video::ENUM_COLOR_FORMAT_L8A8:
					return MyGUI::PixelFormat::L8A8;
				case video::ENUM_COLOR_FORMAT_R8G8B8:
					return MyGUI::PixelFormat::R8G8B8;
				case video::ENUM_COLOR_FORMAT_R8G8B8A8:
					return MyGUI::PixelFormat::R8G8B8A8;
				}
			}
			return MyGUI::PixelFormat::Unknow;
		}
		virtual MyGUI::TextureUsage getUsage(){
			return m_usage;
		}
		virtual size_t getNumElemBytes(){
			return yon::video::getPixelBitsByFormat(m_pTexture->getColorFormat())>>3;
		}

		virtual MyGUI::IRenderTarget* getRenderTarget();

		virtual video::ITexture* getTexture() const{
			return m_pTexture;
		}
	};
}
#endif