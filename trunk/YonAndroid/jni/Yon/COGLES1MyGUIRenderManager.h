#ifndef _YON_GUI_MYGUI_OGLES1_COGLES1MYGUIRENDERMANAGER_H_
#define _YON_GUI_MYGUI_OGLES1_COGLES1MYGUIRENDERMANAGER_H_

#include "IMyGUIRenderManager.h"
#include "COGLES1Driver.h"
#include "IGeometryFactory.h"
#include "SUnit.h"

namespace yon{
namespace gui{
namespace mygui{
namespace ogles1{

	class COGLES1MyGUIRenderManager : public IMyGUIRenderManager{
	private:
		MyGUI::IntSize m_viewSize;
		MyGUI::VertexColourType m_vertexFormat;
		MyGUI::RenderTargetInfo m_info;

		typedef std::map<std::string, MyGUI::ITexture*> MapTexture;
		MapTexture m_textures;
		//bool mPboIsSupported;

		bool m_bUpdate;
		bool m_bIsInitialise;

		video::ogles1::COGLES1Driver* m_pDriver;
		ITimer* m_pTimer;
		scene::IGeometryFactory* m_pGeometryFty;
		core::matrix4f m_projection;
		core::matrix4f m_oldProjection;
		core::matrix4f m_oldWorld;
		core::matrix4f m_oldView;

		scene::Unit3D m_unit;

		void destroyAllResources();
	public:
		COGLES1MyGUIRenderManager(video::ogles1::COGLES1Driver* driver,ITimer* timer,scene::IGeometryFactory* geometryFty);

		/** @see OpenGLRenderManager::getViewSize */
		virtual const MyGUI::IntSize& getViewSize() const;

		/** @see OpenGLRenderManager::getVertexFormat */
		virtual MyGUI::VertexColourType getVertexFormat();

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
		virtual const MyGUI::RenderTargetInfo& getInfo();

		void initialise();
		void shutdown();

		void drawOneFrame();
		void setViewSize(int _width, int _height);
	};
}
}
}
}
#endif