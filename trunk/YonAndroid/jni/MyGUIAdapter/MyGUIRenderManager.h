#ifndef _MYGUI_MYGUIRENDERMANAGER_H_
#define _MYGUI_MYGUIRENDERMANAGER_H_

#include <MyGUI.h>
#include "MyGUI_Prerequest.h"
#include "MyGUI_RenderFormat.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_RenderManager.h"

#include "IVideoDriver.h"
#include "IGeometryFactory.h"
#include "IUnit.h"

using namespace yon;

namespace MyGUI{

	class MyGUIRenderManager : public MyGUI::RenderManager, public MyGUI::IRenderTarget{
	private:
		MyGUI::IntSize m_viewSize;
		MyGUI::RenderTargetInfo m_info;
		bool m_bUpdate;
		typedef std::map<std::string, MyGUI::ITexture*> MapTexture;
		MapTexture m_textures;

		video::IVideoDriver* m_pDriver;
		const ITimer* m_pTimer;
		const scene::IGeometryFactory* m_pGeometryFty;
		core::matrix4f m_projection;
		core::matrix4f m_oldProjection;
		core::matrix4f m_oldWorld;
		core::matrix4f m_oldView;
		scene::IUnit* m_pUnit;

	public:
		MyGUIRenderManager(video::IVideoDriver* driver,const ITimer* timer,const scene::IGeometryFactory* geometryFty);
		~MyGUIRenderManager();

		/** @see OpenGLRenderManager::getViewSize */
		virtual const MyGUI::IntSize& getViewSize() const{
			return m_viewSize;
		}

		/** @see OpenGLRenderManager::getVertexFormat */
		virtual MyGUI::VertexColourType getVertexFormat(){
			return MyGUI::VertexColourType::ColourABGR;
		}

		/** @see OpenGLRenderManager::createVertexBuffer */
		virtual MyGUI::IVertexBuffer* createVertexBuffer();
		/** @see OpenGLRenderManager::destroyVertexBuffer */
		virtual void destroyVertexBuffer(MyGUI::IVertexBuffer* _buffer);

		/** @see OpenGLRenderManager::createTexture */
		virtual MyGUI::ITexture* createTexture(const std::string& _name);
		/** @see OpenGLRenderManager::destroyTexture */
		virtual void destroyTexture(MyGUI::ITexture* _texture);
		/** @see OpenGLRenderManager::getTexture */
		virtual MyGUI::ITexture* getTexture(const std::string& _name);


		/** @see IRenderTarget::begin */
		virtual void begin();
		/** @see IRenderTarget::end */
		virtual void end();
		/** @see IRenderTarget::doRender */
		virtual void doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count);
		/** @see IRenderTarget::getInfo */
		virtual const MyGUI::RenderTargetInfo& getInfo(){
			return m_info;
		}

		void drawOneFrame();
		void setViewSize(int _width, int _height);

		static MyGUIRenderManager& getInstance()
		{
			return *getInstancePtr();
		}
		static MyGUIRenderManager* getInstancePtr()
		{
			return static_cast<MyGUIRenderManager*>(MyGUI::RenderManager::getInstancePtr());
		}
	};
}
#endif