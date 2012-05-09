#ifndef _YON_GUI_MYGUI_CTEXTURE_H_
#define _YON_GUI_MYGUI_CTEXTURE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderFormat.h"

namespace yon{
namespace gui{
namespace mygui{

	class CTexture : MyGUI::ITexture{
	private:
		std::string m_name;
		int m_iWidth;
		int m_iHeight;
		bool m_bLock;
		MyGUI::PixelFormat m_format;
		MyGUI::TextureUsage m_usage;
		size_t m_numElemBytes;
	public:
		CTexture(const std::string& name);

		virtual const std::string& getName() const{
			return m_name;
		}

		virtual void createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format);
		virtual void loadFromFile(const std::string& _filename);
		virtual void saveToFile(const std::string& _filename);

		virtual void destroy();

		virtual int getWidth(){
			return m_iWidth;
		}
		virtual int getHeight(){
			return m_iHeight;
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

	};
}
}
}
#endif