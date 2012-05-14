#include "COGLES1MyGUIRenderManager.h"
#include "COGLES1MyGUIVertexBuffer.h"
#include "COGLES1MyGUITexture.h"

#include "ILogger.h"

namespace yon{
namespace gui{
namespace mygui{
namespace ogles1{

	COGLES1MyGUIRenderManager::COGLES1MyGUIRenderManager(video::ogles1::COGLES1Driver* driver,ITimer* timer,scene::IGeometryFactory* geometryFty)
		:m_bUpdate(false),
		m_bIsInitialise(false),
		m_pDriver(driver),
		m_pTimer(timer),
		m_pGeometryFty(geometryFty),
		m_unit(video::MYGUI_MATERIAL)
	{

			m_projection.makeIdentity();
			m_projection.ortho(-1, 1, -1, 1, -1, 1);

			//出错 TODO
			//m_unit.setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);
	}

	void COGLES1MyGUIRenderManager::initialise(){
		/*
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mVertexFormat = VertexColourType::ColourABGR;

		mUpdate = false;
		mImageLoader = _loader;

		//		glewInit();

		//		mPboIsSupported = glewIsExtensionSupported("GL_EXT_pixel_buffer_object") != 0;

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
		*/
		m_vertexFormat = MyGUI::VertexColourType::ColourABGR;
		m_bUpdate=false;
		m_bIsInitialise=true;

		Logger->debug(YON_LOG_SUCCEED_FORMAT,"initialize COGLES1MyGUIRenderManager");
	}
	void COGLES1MyGUIRenderManager::shutdown(){
		/*
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
		*/
		destroyAllResources();
		m_bIsInitialise=false;
		Logger->debug(YON_LOG_SUCCEED_FORMAT,"shutdown COGLES1MyGUIRenderManager");
	}

	const MyGUI::IntSize& COGLES1MyGUIRenderManager::getViewSize() const{
		return m_viewSize;
	}

	MyGUI::VertexColourType COGLES1MyGUIRenderManager::getVertexFormat(){
		return m_vertexFormat;
	}

	MyGUI::IVertexBuffer* COGLES1MyGUIRenderManager::createVertexBuffer(){
		/*
		return new OpenGLVertexBuffer();
		*/
		return new COGLES1MyGUIVertexBuffer(m_pGeometryFty);
	}
	
	void COGLES1MyGUIRenderManager::destroyVertexBuffer(MyGUI::IVertexBuffer* _buffer){
		/*
		delete _buffer;
		*/
		delete _buffer;
	}

	MyGUI::ITexture* COGLES1MyGUIRenderManager::createTexture(const std::string& _name){
		/*
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		OpenGLTexture* texture = new OpenGLTexture(_name, mImageLoader);
		mTextures[_name] = texture;
		return texture;
		*/
		MapTexture::const_iterator item = m_textures.find(_name);

		YON_DEBUG_BREAK_IF(item!=m_textures.end());
		
		COGLES1MyGUITexture* texture = new COGLES1MyGUITexture(_name,m_pDriver);
		m_textures[_name] = texture;

		Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("COGLES1MyGUIRenderManager.createTexture:%s",_name.c_str()).c_str());
		return texture;
	}
	
	void COGLES1MyGUIRenderManager::destroyTexture(MyGUI::ITexture* _texture){
		/*
		if (_texture == nullptr)
			return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
		*/
		YON_DEBUG_BREAK_IF(_texture==nullptr);

		if (_texture == nullptr){
			return;
		}

		MapTexture::iterator item = m_textures.find(_texture->getName());
		YON_DEBUG_BREAK_IF(item==m_textures.end());

		m_textures.erase(item);
		delete _texture;
	}
	
	MyGUI::ITexture* COGLES1MyGUIRenderManager::getTexture(const std::string& _name){
		/*
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
			return nullptr;
		return item->second;
		*/
		MapTexture::const_iterator item = m_textures.find(_name);
		if (item == m_textures.end()){
			Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("COGLES1MyGUIRenderManager.getTexture:%s",_name.c_str()).c_str());
			return nullptr;
		}else{
			Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("COGLES1MyGUIRenderManager.getTexture:%s",_name.c_str()).c_str());
		}
		return item->second;
	}


	void COGLES1MyGUIRenderManager::begin(){
		/*
		//save current attributes
#if 0
		glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glPolygonMode(GL_FRONT, GL_FILL);
#endif
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrthof(-1, 1, -1, 1, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_FOG);
#if 0
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
#endif
		

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
		*/

		//经测试两种方法效率相当

		m_oldProjection=m_pDriver->getTransform(video::ENUM_TRANSFORM_PROJECTION);
		m_pDriver->setTransform(video::ENUM_TRANSFORM_PROJECTION,m_projection);
		m_oldWorld=m_pDriver->getTransform(video::ENUM_TRANSFORM_WORLD);
		m_oldView=m_pDriver->getTransform(video::ENUM_TRANSFORM_VIEW);
		m_pDriver->setTransform(video::ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
		m_pDriver->setTransform(video::ENUM_TRANSFORM_VIEW,core::IDENTITY_MATRIX);
		
		/*glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrthof(-1, 1, -1, 1, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();*/
		
		/*
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_FOG);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
		*/

		m_pDriver->setMaterial(video::MYGUI_MATERIAL);
		//m_pDriver->checkMaterial();

		
	}

	void COGLES1MyGUIRenderManager::end(){
		/*
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

#if 0
		//restore former attributes
		glPopAttrib();
		glPopClientAttrib();
#endif
		*/

		/*glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);*/

		m_pDriver->setTransform(video::ENUM_TRANSFORM_PROJECTION,m_oldProjection);
		m_pDriver->setTransform(video::ENUM_TRANSFORM_WORLD,m_oldWorld);
		m_pDriver->setTransform(video::ENUM_TRANSFORM_VIEW,m_oldView);
	}

	void COGLES1MyGUIRenderManager::doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count){
		/*
		OpenGLVertexBuffer* buffer = static_cast<OpenGLVertexBuffer*>(_buffer);
#if 0
		unsigned int buffer_id = buffer->getBufferID();
		MYGUI_PLATFORM_ASSERT(buffer_id, "Vertex buffer is not created");
#endif
		unsigned int texture_id = 0;
		if (_texture)
		{
			OpenGLTexture* texture = static_cast<OpenGLTexture*>(_texture);
			texture_id = texture->getTextureID();
			//MYGUI_PLATFORM_ASSERT(texture_id, "Texture is not created");
		}
		glBindTexture(GL_TEXTURE_2D, texture_id);

#if 0
		glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
#endif
		// enable vertex arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		// before draw, specify vertex and index arrays with their offsets
		//		size_t offset = 0;
		unsigned char* offset = (unsigned char*)buffer->getClientArrayPtr();
		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offset);
		offset += (sizeof(float) * 3);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offset);
		offset += (4);
		glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offset);

		glDrawArrays(GL_TRIANGLES, 0, _count);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
#if 0
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
		glBindTexture(GL_TEXTURE_2D, 0);
		*/

		/*
		COGLES1MyGUIVertexBuffer* buffer = static_cast<COGLES1MyGUIVertexBuffer*>(_buffer);
		unsigned int textureId = 0;
		if (_texture)
		{
			COGLES1MyGUITexture* texture = static_cast<COGLES1MyGUITexture*>(_texture);
			textureId = texture->getTextureId();
			//MYGUI_PLATFORM_ASSERT(texture_id, "Texture is not created");
		}
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);
		// enable vertex arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		// before draw, specify vertex and index arrays with their offsets
		//		size_t offset = 0;
		unsigned char* offset = (unsigned char*)buffer->getClientArrayPtr();
		glVertexPointer(3, GL_FLOAT, sizeof(MyGUI::Vertex), (void*)offset);
		offset += (sizeof(float) * 3);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(MyGUI::Vertex), (void*)offset);
		offset += (4);
		glTexCoordPointer(2, GL_FLOAT, sizeof(MyGUI::Vertex), (void*)offset);

		glDrawArrays(GL_TRIANGLES, 0, _count);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);*/
		
		

		
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		
		
		COGLES1MyGUIVertexBuffer* buffer = static_cast<COGLES1MyGUIVertexBuffer*>(_buffer);
		if (_texture)
		{
			COGLES1MyGUITexture* texture = static_cast<COGLES1MyGUITexture*>(_texture);
			m_pDriver->setTexture(0,texture->getTexture());
			m_unit.getMaterial()->setTexture(0,texture->getTexture());
		}
		buffer->fillShapIndices();
		scene::IShap* shap=buffer->getShap();
		m_unit.setShap(shap);
		m_pDriver->drawUnit(&m_unit);
		//Logger->debug("doRender:%08X(v:%d,i:%d)\n",shap,shap->getVertexCount(),shap->getIndexCount());
		//m_pDriver->setTexture(0,NULL);
		
	}

	const MyGUI::RenderTargetInfo& COGLES1MyGUIRenderManager::getInfo(){
		return m_info;
	}

	void COGLES1MyGUIRenderManager::drawOneFrame(){
		/*
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		begin();
		onRenderToTarget(this, mUpdate);
		end();

		mUpdate = false;
		*/

		MyGUI::Gui* gui = MyGUI::Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		static unsigned long last_time = m_pTimer->getRealTime();
		unsigned long now_time = m_pTimer->getRealTime();
		unsigned long time = now_time - last_time;

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		begin();
		onRenderToTarget(this, m_bUpdate);
		end();

		m_bUpdate = false;
	}

	void COGLES1MyGUIRenderManager::setViewSize(int _width, int _height){
		/*
		if (_height == 0)
			_height = 1;
		if (_width == 0)
			_width = 1;

		mViewSize.set(_width, _height);

		mInfo.maximumDepth = 1;
		mInfo.hOffset = 0;
		mInfo.vOffset = 0;
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);

		onResizeView(mViewSize);
		mUpdate = true;
		*/
		if (_height == 0)
			_height = 1;
		if (_width == 0)
			_width = 1;

		m_viewSize.set(_width, _height);

		m_info.maximumDepth = 1;
		m_info.hOffset = 0;
		m_info.vOffset = 0;
		m_info.aspectCoef = float(m_viewSize.height) / float(m_viewSize.width);
		m_info.pixScaleX = 1.0f / float(m_viewSize.width);
		m_info.pixScaleY = 1.0f / float(m_viewSize.height);

		onResizeView(m_viewSize);
		m_bUpdate = true;
	}

	void COGLES1MyGUIRenderManager::destroyAllResources(){
		/*
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
		*/

		for (MapTexture::const_iterator item = m_textures.begin(); item != m_textures.end(); ++item)
		{
			delete item->second;
		}
		m_textures.clear();
	}
}
}
}
}