#include "MyGUIRenderManager.h"
#include "MyGUIVertexBuffer.h"
#include "MyGUITexture.h"

#include "ILogger.h"

namespace MyGUI{

	MyGUIRenderManager::MyGUIRenderManager(video::IVideoDriver* driver,ITimer* timer,const scene::IGeometryFactory* geometryFty)
		:m_bUpdate(false),
		m_pDriver(driver),
		m_pTimer(timer),
		m_pGeometryFty(geometryFty)
		//,m_pUnit(NULL)
	{

		m_viewSize.width=driver->getCurrentRenderTargetSize().w;
		m_viewSize.height=driver->getCurrentRenderTargetSize().h;

		scene::IShap* shap=geometryFty->createShap(scene::ENUM_VERTEX_TYPE_3V1T1C,0,0);
		//m_pUnit=geometryFty->createUnit(shap,video::MYGUI_MATERIAL);  

		shap->drop();

		m_projection.makeIdentity();
		m_projection.ortho(-1, 1, -1, 1, -1, 1);

		m_material.MaterialType=video::ENUM_MATERIAL_TYPE_TRANSPARENT;
		m_material.FrontFace=video::ENUM_FRONT_FACE_CW;
		//m_material.setFilterMode(0,video::ENUM_FILTER_MODE_NEAREST);
		//TODO由于MyGUI引擎的freetype字体顶点序列是CCW的，而其UI界面顶点序列则是CW的，临时解决办法是关闭CullFace
		m_material.CullingMode=video::ENUM_CULLING_MODE_NONE;

		//出错 TODO
		//m_unit.setHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);

		YON_DEBUG(YON_LOG_SUCCEED_FORMAT,"Instance MyGUIRenderManager");
	}

	MyGUIRenderManager::~MyGUIRenderManager(){
		for (MapTexture::const_iterator item = m_textures.begin(); item != m_textures.end(); ++item)
		{
			delete item->second;
		}
		m_textures.clear();
		//m_pUnit->drop();
		YON_DEBUG(YON_LOG_SUCCEED_FORMAT,"Release MyGUIRenderManager");
	}
	MyGUI::IVertexBuffer* MyGUIRenderManager::createVertexBuffer(){
		return new MyGUIVertexBuffer(m_pGeometryFty);
	}
	void MyGUIRenderManager::destroyVertexBuffer(MyGUI::IVertexBuffer* _buffer){
		delete _buffer;
	}
	MyGUI::ITexture* MyGUIRenderManager::createTexture(const std::string& _name){
		MapTexture::const_iterator item = m_textures.find(_name);

		YON_DEBUG_BREAK_IF(item!=m_textures.end());

		MyGUITexture* texture = new MyGUITexture(_name,m_pDriver,this);
		//MyGUITexture* texture = new MyGUITexture(_name,m_pDriver);
		m_textures[_name] = texture;

		YON_DEBUG(YON_LOG_SUCCEED_FORMAT,core::stringc("MyGUIRenderManager.createTexture:%s",_name.c_str()).c_str());
		return texture;
	}
	void MyGUIRenderManager::destroyTexture(MyGUI::ITexture* _texture){
		YON_DEBUG_BREAK_IF(_texture==nullptr);

		if (_texture == nullptr){
			return;
		}

		MapTexture::iterator item = m_textures.find(_texture->getName());
		YON_DEBUG_BREAK_IF(item==m_textures.end());

		m_textures.erase(item);

		YON_DEBUG(YON_LOG_SUCCEED_FORMAT,core::stringc("MyGUIRenderManager.destroyTexture:%s",_texture->getName().c_str()).c_str());
		delete _texture;
	}
	MyGUI::ITexture* MyGUIRenderManager::getTexture(const std::string& _name){
		if(_name.length()==0)
			return nullptr;
		MapTexture::const_iterator item = m_textures.find(_name);
		if (item == m_textures.end()){
			YON_WARN(YON_LOG_WARN_FORMAT,core::stringc("MyGUIRenderManager.getTexture:%s",_name.c_str()).c_str());
			return nullptr;
		}else{
			YON_DEBUG(YON_LOG_SUCCEED_FORMAT,core::stringc("MyGUIRenderManager.getTexture:%s",_name.c_str()).c_str());
		}
		return item->second;
	}

	void MyGUIRenderManager::begin(){
		m_oldProjection=m_pDriver->getTransform(video::ENUM_TRANSFORM_PROJECTION);
		m_pDriver->setTransform(video::ENUM_TRANSFORM_PROJECTION,m_projection);
		m_oldWorld=m_pDriver->getTransform(video::ENUM_TRANSFORM_WORLD);
		m_oldView=m_pDriver->getTransform(video::ENUM_TRANSFORM_VIEW);
		m_pDriver->setTransform(video::ENUM_TRANSFORM_WORLD,core::IDENTITY_MATRIX);
		m_pDriver->setTransform(video::ENUM_TRANSFORM_VIEW,core::IDENTITY_MATRIX);
		//m_pDriver->setMaterial(video::MYGUI_MATERIAL);
		//YON_DEBUG("begin\n");
	}

	void MyGUIRenderManager::end(){
		m_pDriver->setTransform(video::ENUM_TRANSFORM_PROJECTION,m_oldProjection);
		m_pDriver->setTransform(video::ENUM_TRANSFORM_WORLD,m_oldWorld);
		m_pDriver->setTransform(video::ENUM_TRANSFORM_VIEW,m_oldView);
		//YON_DEBUG("end\n");
	}

	void MyGUIRenderManager::doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count){

		MyGUIVertexBuffer* buffer = static_cast<MyGUIVertexBuffer*>(_buffer);
		if (_texture)
		{
			MyGUITexture* texture = static_cast<MyGUITexture*>(_texture);
			//m_pUnit->getMaterial().setTexture(0,texture->getTexture());
			//m_pDriver->setMaterial(m_pUnit->getMaterial());
			m_material.setTexture(0,texture->getTexture());
			m_pDriver->setMaterial(m_material);
			//YON_DEBUG("setTexture:%s\n",texture->getTexture()->getPath());

		}
		buffer->fillShapIndices(_count);
		scene::IShap* shap=buffer->getShap();
		//m_pUnit->setShap(shap);
		//m_pDriver->drawUnit(m_pUnit,true);
		m_pDriver->drawShap(shap,true);
		//YON_DEBUG("doRender:%08X(v:%d,i:%d)\n",shap,shap->getVertexCount(),shap->getIndexCount());
		//printf("doRender:%08X(v:%d,i:%d),_count:%d\n",shap,shap->getVertexCount(),shap->getIndexCount(),_count);
	}

	void MyGUIRenderManager::drawOneFrame(){
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

	void MyGUIRenderManager::setViewSize(int _width, int _height){
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

}