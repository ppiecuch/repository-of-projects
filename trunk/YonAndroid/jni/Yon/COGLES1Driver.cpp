#include "COGLES1Driver.h"
#include "SVertex.h"
#include "COGLES1Texture.h"
#include "COGLES1MaterialRenderer.h"
#include "CLogger.h"
#include "CDebugPrinter.h"
#include "yonUtil.h"
#include "COGLES1HardwareBuffer.h"
#include "CImage.h"
#include "COGLES1FBOTexture.h"
#include "EGLInfo.h"
#include "CMaterial.h"

namespace yon{
namespace video{
namespace ogles1{

#ifdef YON_COMPILE_WITH_WIN32
	
	/*core::map<s32,core::stringc> EGL_NAME_FLAGS;
	//EGL_NAME_FLAGS[(s32)EGL_RED_SIZE]="EGL_RED_SIZE";
	EGL_NAME_FLAGS.set(EGL_RED_SIZE,"EGL_RED_SIZE");
	EGL_NAME_FLAGS[EGL_GREEN_SIZE]="EGL_GREEN_SIZE";
	EGL_NAME_FLAGS[EGL_BLUE_SIZE]="EGL_BLUE_SIZE";
	EGL_NAME_FLAGS[EGL_ALPHA_SIZE]="EGL_ALPHA_SIZE";
	EGL_NAME_FLAGS[EGL_BUFFER_SIZE]="EGL_BUFFER_SIZE";
	//EGL_NAME_FLAGS[EGL_SURFACE_TYPE]="EGL_SURFACE_TYPE";
	EGL_NAME_FLAGS[EGL_DEPTH_SIZE]="EGL_DEPTH_SIZE";
	EGL_NAME_FLAGS[EGL_STENCIL_SIZE]="EGL_STENCIL_SIZE";
	EGL_NAME_FLAGS[EGL_SAMPLE_BUFFERS]="EGL_SAMPLE_BUFFERS";
	EGL_NAME_FLAGS[EGL_SAMPLES]="EGL_SAMPLES";
	//EGL_NAME_FLAGS[EGL_RENDERABLE_TYPE]="EGL_RENDERABLE_TYPE";
	EGL_NAME_FLAGS[EGL_NONE]="EGL_NONE";

	core::map<u16,core::stringc> EGL_SURFACE_FLAGS;
	EGL_SURFACE_FLAGS[EGL_PBUFFER_BIT]="EGL_PBUFFER_BIT";
	EGL_SURFACE_FLAGS[EGL_PIXMAP_BIT]="EGL_PIXMAP_BIT";
	EGL_SURFACE_FLAGS[EGL_WINDOW_BIT]="EGL_WINDOW_BIT";
	EGL_SURFACE_FLAGS[EGL_VG_COLORSPACE_LINEAR_BIT]="EGL_VG_COLORSPACE_LINEAR_BIT";
	EGL_SURFACE_FLAGS[EGL_VG_ALPHA_FORMAT_PRE_BIT]="EGL_VG_ALPHA_FORMAT_PRE_BIT";
	EGL_SURFACE_FLAGS[EGL_MULTISAMPLE_RESOLVE_BOX_BIT]="EGL_MULTISAMPLE_RESOLVE_BOX_BIT";
	EGL_SURFACE_FLAGS[EGL_SWAP_BEHAVIOR_PRESERVED_BIT]="EGL_SWAP_BEHAVIOR_PRESERVED_BIT";

	core::map<u16,core::stringc> EGL_RENDERABLE_FLAGS;
	EGL_RENDERABLE_FLAGS[EGL_OPENGL_ES_BIT]="EGL_OPENGL_ES_BIT";
	EGL_RENDERABLE_FLAGS[EGL_OPENVG_BIT]="EGL_OPENVG_BIT";
	EGL_RENDERABLE_FLAGS[EGL_OPENGL_ES2_BIT]="EGL_OPENGL_ES2_BIT";
	EGL_RENDERABLE_FLAGS[EGL_OPENGL_BIT]="EGL_OPENGL_BIT";*/
	

	
	void COGLES1Driver::printEGLConfig(EGLConfig& config)
	{
		EGLint value;
		core::map<u16,core::stringc>::Iterator it=EGLInfo::getInstance().EGL_NAME_FLAGS.getIterator();
		for (;!it.atEnd();++it)
		{
			eglGetConfigAttrib(m_eglDisplay, config, it->getKey(), &value);
			Logger->debug("%s:%d\n",it->getValue().c_str(),value);
		}
		if(eglGetConfigAttrib(m_eglDisplay, config, EGL_SURFACE_TYPE, &value))
		{
			core::stringc result;
			core::map<u16,core::stringc>::Iterator it=EGLInfo::getInstance().EGL_SURFACE_FLAGS.getIterator();
			for (;!it.atEnd();++it)
			{
				if(it->getKey()&value)
				{
					if(result.length()>0)
						result+=",";
					result+=it->getValue();
				}
			}
			Logger->debug("EGL_SURFACE_TYPE:%s\n",result);
		}
		else
			Logger->debug("EGL_SURFACE_TYPE:Unknown\n");
		if(eglGetConfigAttrib(m_eglDisplay, config, EGL_RENDERABLE_TYPE, &value))
		{
			core::stringc result;
			core::map<u16,core::stringc>::Iterator it=EGLInfo::getInstance().EGL_RENDERABLE_FLAGS.getIterator();
			for (;!it.atEnd();++it)
			{
				if(it->getKey()&value)
				{
					if(result.length()>0)
						result+=",";
					result+=it->getValue();
				}
			}
			Logger->debug("EGL_RENDERABLE_TYPE:%s\n",result);
		}
		else
			Logger->debug("EGL_RENDERABLE_TYPE:Unknown\n");
	}
	void COGLES1Driver::printEGLAttribute(EGLint attribs[])
	{
		s32 i=0;
		while(attribs[i]!=EGL_NONE)
		{
			if(attribs[i]==EGL_SURFACE_TYPE)
				Logger->debug("EGL_SURFACE_TYPE:%s\n",((core::stringc)EGLInfo::getInstance().EGL_SURFACE_FLAGS[attribs[i+1]]).c_str());
			else if(attribs[i]==EGL_RENDERABLE_TYPE)
				Logger->debug("EGL_RENDERABLE_TYPE:%s\n",((core::stringc)EGLInfo::getInstance().EGL_RENDERABLE_FLAGS[attribs[i+1]]).c_str());
			else
				Logger->debug("%s:%d\n",((core::stringc)EGLInfo::getInstance().EGL_NAME_FLAGS[attribs[i]]).c_str(),attribs[i+1]);
			i+=2;
		}
	}
	
#endif

	u32 COGLES1Driver::s_uInstanceCount=0;

	COGLES1Driver::COGLES1Driver(const SOGLES1Parameters& param,io::IFileSystem* fs,ITimer* timer,scene::IGeometryFactory* geometryFty)
		:m_bRenderModeChange(true),m_pLastMaterial(NULL),m_pCurrentMaterial(NULL),
		m_pDebugPrinter(NULL),m_uPrimitiveDrawn(0),m_videoMemory(0),
#ifdef YON_COMPILE_WITH_WIN32
		m_eglDisplay(EGL_NO_DISPLAY),
		m_hDc(NULL),m_hWnd(param.hWnd),
#endif
		m_windowSize(core::dimension2di((s32)param.windowSize.w,(s32)param.windowSize.h)),IVideoDriver(fs,timer),COGLES1ExtensionHandler(){

		m_imageLoaders.push_back(createImageLoaderPNG());

		m_materialRenderers.push_back(createMaterialRendererSolid(this));
		m_materialRenderers.push_back(createMaterialRendererBlend(this));
		m_materialRenderers.push_back(createMaterialRendererLighten(this));
		m_materialRenderers.push_back(createMaterialRendererTransparent(this));
		m_materialRenderers.push_back(createMaterialRendererTransparentRef(this));
		m_materialRenderers.push_back(createMaterialRendererTransparentBlendColor(this));
		m_materialRenderers.push_back(createMaterialRendererMask(this));

#ifdef YON_COMPILE_WITH_WIN32
		EGLInfo::getInstance().init();
		initEGL(m_hWnd);
#endif//YON_COMPILE_WITH_WIN32

		initExtensionHandler();

		u32 i;
		onResize(param.windowSize);

		for(i=0; i<ENUM_TRANSFORM_COUNT; ++i)
			setTransform(static_cast<ENUM_TRANSFORM>(i), core::IDENTITY_MATRIX);

		for(i=0;i<YON_MATERIAL_MAX_TEXTURES;++i)
			m_currentTextures[i]=NULL;

		setRender3DMode();

		glClearColor(0.1f,0.2f,0.3f,1);
		glColor4f(1, 1, 1, 1);

		//FPS控制计算
		if(param.fpsLimit==0){
			m_FPSAssist.limit=false;
			Logger->debug("no FPS limit\n");
		}else{
			s32 g=core::gcd(param.fpsLimit,1000);
			m_FPSAssist.limit=true;
			m_FPSAssist.frameUnit=param.fpsLimit/g;
			m_FPSAssist.timeUnit=1000/g;
			m_FPSAssist.timeCounter=30;
			m_FPSAssist.frameCounter=0;
			m_FPSAssist.refreshedTime=timer->getRealTime();

			Logger->debug("{fpsLimit:%d,frameLimit:%d,timeUnit:%d}\n",param.fpsLimit,m_FPSAssist.frameUnit,m_FPSAssist.timeUnit);
		}

		/*video::IImage* image=DebugFont::getInstance().createImage();
		ITexture* tex=createDeviceDependentTexture(image,io::path("_yon_debug_font_"));
		addTexture(tex);
		tex->drop();
		image->drop();
		DebugFont::getInstance().m_pTexture=tex;
		DebugFont::getInstance().m_pDriver=this;*/
		video::IImage* image=debug::createDebugPrinterTextureImage();
		const bool useMipmap = getTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_MIPMAPS);
		setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_MIPMAPS, false);
		ITexture* tex=createDeviceDependentTexture(image,io::path("_yon_debug_font_"));
		setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_MIPMAPS, useMipmap);
		addTexture(tex);
		tex->drop();
		image->drop();
		m_pDebugPrinter=debug::createDebugPrinter(this,tex,geometryFty);
		((CLogger*)Logger)->setDebugPrinter(this,m_pDebugPrinter);

		//实例计数器加1
		++s_uInstanceCount;

		Logger->info(YON_LOG_SUCCEED_FORMAT,core::stringc("Instance COGLES1Driver:%d",s_uInstanceCount).c_str());
	}

	COGLES1Driver::~COGLES1Driver(){

#ifdef YON_COMPILE_WITH_WIN32
		if(s_uInstanceCount>1&&eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)==EGL_FALSE){
			Logger->debug("Could not Bind Contexts and Drawables for OpenGL-ES1 display.\n");
		}
#endif

		//TODO 灵异事件，如果将YON_MATERIAL_MAX_TEXTURES设为1，此m_pDebugPrinter会变成NULL
		if(m_pDebugPrinter)
			m_pDebugPrinter->drop();

		if(m_pLastMaterial)
			m_pLastMaterial->drop();
		if(m_pCurrentMaterial)
			m_pCurrentMaterial->drop();

		u32 i=0;
		u32 size=0;

		//TODO map.drop
		size=m_pHardwareBuffers.size();
		for(i=0;i<m_pHardwareBuffers.size();++i){
			delete m_pHardwareBuffers[i];
		}
		Logger->debug("Release %d/%d Hardwarebuffer\n",i,size);

		

		size=m_imageLoaders.size();
		for(i=0;i<m_imageLoaders.size();++i)
			m_imageLoaders[i]->drop();
		Logger->debug("Release %d/%d ImageLoader\n",i,size);

		size=m_textures.size();
		for(i=0;i<m_textures.size();++i)
			m_textures[i]->drop();
		Logger->debug("Release %d/%d Texture\n",i,size);

		size=m_materialRenderers.size();
		for(i=0;i<m_materialRenderers.size();++i)
			m_materialRenderers[i]->drop();
		Logger->debug("Release %d/%d MaterialRenderer\n",i,size);

#ifdef YON_COMPILE_WITH_WIN32
		destroyEGL();
		EGLInfo::getInstance().destroy();
#endif//YON_COMPILE_WITH_WIN32

		//实例计数器减1-->不减,因为最后一个被释放时,也需要MakeCurrent
		//--s_uInstanceCount;


		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release COGLES1Driver");
	}

	void COGLES1Driver::clearView(const bool& backBuffer,const bool& zBuffer,const video::SColor& color) const{
		GLbitfield mask = 0;

		if (backBuffer)
		{
			const static f32 inv = 1.0f / 255.0f;
			glClearColor(color.getRed() * inv, color.getGreen() * inv,color.getBlue() * inv, color.getAlpha() * inv);

			mask |= GL_COLOR_BUFFER_BIT;
		}

		if (zBuffer)
		{
			glDepthMask(GL_TRUE);
			//LastMaterial.ZWriteEnable=true;
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		glClear(mask);
	}

	void COGLES1Driver::begin(bool backBuffer,bool zBuffer,const video::SColor& color)
	{
#ifdef YON_COMPILE_WITH_WIN32
		if(s_uInstanceCount>1&&eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)==EGL_FALSE){
			Logger->debug("Could not Bind Contexts and Drawables for OpenGL-ES1 display.\n");
		}
#endif
		clearView(backBuffer,zBuffer,color);

		m_clearSetting.clearBackBuffer=backBuffer;
		m_clearSetting.clearZBuffer=zBuffer;
		m_clearSetting.color=color;

		m_uPrimitiveDrawn=0;
	}
	void COGLES1Driver::end()
	{
#ifdef YON_COMPILE_WITH_WIN32
		eglSwapBuffers(m_eglDisplay, m_eglSurface);
#endif//YON_COMPILE_WITH_WIN32
		//Logger->debug("realtime:%d\n",m_pTimer->getRealTime());
		m_FPSCounter.registerFrame(m_pTimer->getRealTime(),m_uPrimitiveDrawn);

		if(!m_FPSAssist.limit)
			return;
		++m_FPSAssist.frameCounter;
		
		if((s32)m_pTimer->getRealTime()-m_FPSAssist.refreshedTime>m_FPSAssist.timeUnit){
			//Logger->debug("frameCounter:%d,frameUnit:%d\n",m_FPSAssist.frameCounter,m_FPSAssist.frameUnit);
			if(m_FPSAssist.frameCounter>m_FPSAssist.frameUnit){
				++m_FPSAssist.timeCounter;
			}else if(m_FPSAssist.frameCounter<m_FPSAssist.frameUnit||m_FPSAssist.frameUnit==1){
				--m_FPSAssist.timeCounter;
				if(m_FPSAssist.timeCounter<0)
					m_FPSAssist.timeCounter=0;
			}
			m_FPSAssist.frameCounter=0;
			m_FPSAssist.refreshedTime=m_pTimer->getRealTime();
		}
		//Logger->debug("%d\n",m_FPSAssist.timeCounter);
		core::yonSleep(m_FPSAssist.timeCounter);
	}
	void COGLES1Driver::setViewPort(const core::recti& r){
#ifdef YON_COMPILE_WITH_WIN32
		if(s_uInstanceCount>1&&eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)==EGL_FALSE){
			Logger->debug("Could not Bind Contexts and Drawables for OpenGL-ES1 display.\n");
		}
#endif
		glViewport(0, 0, r.getWidth(), r.getHeight());
		Logger->debug("setViewPort(0,0,%d,%d)\n",r.getWidth(), r.getHeight());
	}
	const core::dimension2di& COGLES1Driver::getCurrentRenderTargetSize() const{
		return m_windowSize;
	}
	void COGLES1Driver::onResize(const core::dimension2du& size){
		m_windowSize.w=(s32)size.w;
		m_windowSize.h=(s32)size.h;
		setViewPort(core::recti(0,0,size.w,size.h));

		IVideoDriver::onResize(size);
	}
	void COGLES1Driver::drawUnit(scene::IUnit* unit){
		/*if(unit->getDimenMode()==ENUM_DIMEN_MODE_3D)
			setRender3DMode();
		else
			setRender2DMode();*/
		if(unit->getVertexType()==scene::ENUM_VERTEX_TYPE_2V1T1C)
			setRender2DMode();
		else if(m_pCurrentMaterial==video::MYGUI_MATERIAL)
			setRender2DMode();
		else
			setRender3DMode();
		checkMaterial();

		/*glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_SHORT, 0, mVertices);
		glColorPointer(4,GL_UNSIGNED_BYTE,0,mColors);
		glTexCoordPointer(2, GL_FLOAT, 0,mTexcoords);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, mIndices);
		glRotatex(2<<16, 0, 0, 0x10000);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(scene::SVertex),&unit->getVertices()[0].pos);
		glColorPointer(4,GL_UNSIGNED_BYTE, sizeof(scene::SVertex),&unit->getVertices()[0].color);
		glTexCoordPointer(2, GL_FLOAT, sizeof(scene::SVertex),&unit->getVertices()[0].texcoords);
		glDrawElements(GL_TRIANGLES, unit->getIndexCount(), GL_UNSIGNED_SHORT,unit->getIndices());
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);*/

		static ENUM_PRIMITIVE_TYPE mode;
		switch(m_pCurrentMaterial->getPolygonMode()){
		case ENUM_POLYGON_MODE_FILL:
			mode=ENUM_PRIMITIVE_TYPE_TRIANGLES;
			break;
		case ENUM_POLYGON_MODE_LINE:
			mode=ENUM_PRIMITIVE_TYPE_LINES;
			break;
		case ENUM_POLYGON_MODE_POINT:
			mode=ENUM_PRIMITIVE_TYPE_POINTS;
			break;
		default:
			mode=ENUM_PRIMITIVE_TYPE_TRIANGLES;
			break;
		}

		if(needHardwareBuffer(unit)){
			//TODO map.find
			IHardwareBuffer* buffer=NULL;
			for(u32 i=0;i<m_pHardwareBuffers.size();++i){
				if(m_pHardwareBuffers[i]->unit==unit){
					buffer=m_pHardwareBuffers[i]->buffer;
					break;
				}
			}
			if(buffer==NULL){
				buffer=createHardwareBuffer(unit);
				m_pHardwareBuffers.push_back(new SHardwareBufferPair());
				m_pHardwareBuffers[m_pHardwareBuffers.size()-1]->unit=unit;
				m_pHardwareBuffers[m_pHardwareBuffers.size()-1]->buffer=buffer;
			}
			buffer->draw(mode);

			u32 indexCount=unit->getShap()->getIndexCount();
			m_uPrimitiveDrawn+=indexCount/3;
		}else{
			const void* vertice=unit->getShap()->getVertices();
			const void* indice=unit->getShap()->getIndices();
			u32 vertexCount=unit->getShap()->getVertexCount();
			u32 indexCount=unit->getShap()->getIndexCount();

			//drawVertexPrimitiveList(vertice,vertexCount,indice,indexCount,mode,unit->getVertexType());
			drawVertex(vertice,vertexCount,indice,indexCount,mode,unit->getVertexType());
		}

		//Logger->debug("drawUnit:%08x\n",unit);
	}

	void COGLES1Driver::drawVertexPrimitiveList(const void* vertices, u32 vertexCount,const void* indice, u32 indexCount,ENUM_PRIMITIVE_TYPE pType,scene::ENUM_VERTEX_TYPE vType){
		if(vType==scene::ENUM_VERTEX_TYPE_2V1T1C)
			setRender2DMode();
		else if(m_pCurrentMaterial==video::MYGUI_MATERIAL)
			setRender2DMode();
		else
			setRender3DMode();
		checkMaterial();
		drawVertex(vertices,vertexCount,indice,indexCount,pType,vType);
	}

	void COGLES1Driver::drawVertex(const void* vertices, u32 vertexCount,const void* indice, u32 indexCount,ENUM_PRIMITIVE_TYPE pType,scene::ENUM_VERTEX_TYPE vType){

		switch(vType){
		case scene::ENUM_VERTEX_TYPE_2V1T1C:
			{
				static u32 stride=sizeof(scene::S2DVertex);
				scene::S2DVertex* v=(scene::S2DVertex*)vertices;
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glVertexPointer(2, GL_FLOAT, stride,&v[0].pos);
				glColorPointer(4,GL_UNSIGNED_BYTE, stride,&v[0].color);
				glTexCoordPointer(2, GL_FLOAT, stride,&v[0].texcoords);
				glDrawElements(pType, indexCount, GL_UNSIGNED_SHORT,indice);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
			break;
		case scene::ENUM_VERTEX_TYPE_3V1T1C:
			{
				static u32 stride=sizeof(scene::SVertex);
				scene::SVertex* v=(scene::SVertex*)vertices;
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glVertexPointer(3, GL_FLOAT, stride,&v[0].pos);
				glColorPointer(4,GL_UNSIGNED_BYTE, stride,&v[0].color);
				glTexCoordPointer(2, GL_FLOAT, stride,&v[0].texcoords);
				glDrawElements(pType, indexCount, GL_UNSIGNED_SHORT,indice);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
			break;
		case scene::ENUM_VERTEX_TYPE_3V2T1C:
			{
				static u32 stride=sizeof(scene::SVertex2TCoords);
				scene::SVertex2TCoords* v=(scene::SVertex2TCoords*)vertices;
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				
				glVertexPointer(3, GL_FLOAT, stride,&v[0].pos);
				glColorPointer(4,GL_UNSIGNED_BYTE, stride,&v[0].color);

				glClientActiveTexture(GL_TEXTURE0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, stride,&v[0].texcoords0);

				glClientActiveTexture(GL_TEXTURE1);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, stride,&v[0].texcoords1);

				glDrawElements(pType, indexCount, GL_UNSIGNED_SHORT,indice);

				glClientActiveTexture(GL_TEXTURE1);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glClientActiveTexture(GL_TEXTURE0);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
			}
			break;
		}

		m_uPrimitiveDrawn+=indexCount/3;

		
		/*glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(scene::SVertex),&unit->getVertices()[0].pos);
		glColorPointer(4,GL_UNSIGNED_BYTE, sizeof(scene::SVertex),&unit->getVertices()[0].color);
		glTexCoordPointer(2, GL_FLOAT, sizeof(scene::SVertex),&unit->getVertices()[0].texcoords);
		glDrawElements(GL_TRIANGLES, unit->getIndexCount(), GL_UNSIGNED_SHORT,unit->getIndices());
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);*/
	}


	void COGLES1Driver::draw2DImage(const video::ITexture* texture, const core::position2di& destPos,const core::recti& sourceRect, const core::recti* clipRect,video::SColor color, bool useAlphaChannelOfTexture)
	{
		if (!texture)
			return;

		if (!sourceRect.isValid())
			return;

		core::position2di targetPos(destPos);
		core::position2di sourcePos(sourceRect.topLeft);
		core::dimension2di sourceSize(sourceRect.getSize());
		if (clipRect)
		{
			if (targetPos.x < clipRect->topLeft.x)
			{
				sourceSize.w += targetPos.x - clipRect->topLeft.x;
				if (sourceSize.w <= 0)
					return;

				sourcePos.x -= targetPos.x - clipRect->topLeft.x;
				targetPos.x = clipRect->topLeft.x;
			}

			if (targetPos.x + sourceSize.w > clipRect->bottomRight.x)
			{
				sourceSize.w -= (targetPos.x + sourceSize.w) - clipRect->bottomRight.x;
				if (sourceSize.w <= 0)
					return;
			}

			if (targetPos.y < clipRect->topLeft.y)
			{
				sourceSize.h += targetPos.y - clipRect->topLeft.y;
				if (sourceSize.h <= 0)
					return;

				sourcePos.y -= targetPos.y - clipRect->topLeft.y;
				targetPos.y = clipRect->topLeft.y;
			}

			if (targetPos.y + sourceSize.h > clipRect->bottomRight.y)
			{
				sourceSize.h -= (targetPos.y  + sourceSize.h) - clipRect->bottomRight.y;
				if (sourceSize.h <= 0)
					return;
			}
		}

		if (targetPos.x<0)
		{
			sourceSize.w += targetPos.x;
			if (sourceSize.w <= 0)
				return;

			sourcePos.x -= targetPos.x;
			targetPos.x = 0;
		}

		if (targetPos.y<0)
		{
			sourceSize.h += targetPos.y;
			if (sourceSize.h <= 0)
				return;

			sourcePos.y -= targetPos.y;
			targetPos.y = 0;
		}

		const core::dimension2di& renderTargetSize = getCurrentRenderTargetSize();

		if (targetPos.x + sourceSize.w > (s32)renderTargetSize.w)
		{
			sourceSize.w -= (targetPos.x + sourceSize.w) - renderTargetSize.w;
			if (sourceSize.w <= 0)
				return;
		}

		if (targetPos.y + sourceSize.h > (s32)renderTargetSize.h)
		{
			sourceSize.h -= (targetPos.y + sourceSize.h) - renderTargetSize.h;
			if (sourceSize.h <= 0)
				return;
		}

		// ok, we've clipped everything.
		// now draw it.
		const core::dimension2du& ss = texture->getSize();
		const f32 invW = 1.f / static_cast<f32>(ss.w);
		const f32 invH = 1.f / static_cast<f32>(ss.h);
		const core::rectf tcoords(
			sourcePos.x * invW,sourcePos.y * invH,
			(sourcePos.x + sourceSize.w) * invW,(sourcePos.y + sourceSize.h) * invH);
		const core::recti poss(targetPos, sourceSize);

		setRender2DMode();
		checkMaterial();

		if (!setTexture(0, texture))
			return;

		static u16 indices[] = {0,1,2,3};
		scene::SVertex vertices[4];
		vertices[0] = scene::SVertex((f32)poss.topLeft.x, (f32)poss.topLeft.y, 0, tcoords.topLeft.x, tcoords.topLeft.y, color);
		vertices[1] = scene::SVertex((f32)poss.bottomRight.x, (f32)poss.topLeft.y, 0, tcoords.bottomRight.x, tcoords.topLeft.y, color);
		vertices[2] = scene::SVertex((f32)poss.topLeft.x, (f32)poss.bottomRight.y, 0, tcoords.topLeft.x, tcoords.bottomRight.y, color);
		vertices[3] = scene::SVertex((f32)poss.bottomRight.x, (f32)poss.bottomRight.y, 0, tcoords.bottomRight.x, tcoords.bottomRight.y, color);


		/*glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(scene::SVertex),&vertices[0].pos);
		glColorPointer(4,GL_UNSIGNED_BYTE, sizeof(scene::SVertex),&vertices[0].color);
		glTexCoordPointer(2, GL_FLOAT, sizeof(scene::SVertex),&vertices[0].texcoords);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT,indices);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);*/

		//drawVertexPrimitiveList(vertices,4,indices,4,ENUM_PRIMITIVE_TYPE_TRIANGLE_STRIP,scene::ENUM_VERTEX_TYPE_3V1T1C);
		drawVertex(vertices,4,indices,4,ENUM_PRIMITIVE_TYPE_TRIANGLE_STRIP,scene::ENUM_VERTEX_TYPE_3V1T1C);

	}

	void COGLES1Driver::draw3DLine(const core::vector3df& start,const core::vector3df& end, video::SColor color){
		setRender3DMode();
		checkMaterial();

		//TODO优化
		static u16 indices[] = {0,1};
		scene::SVertex vertices[2];
		vertices[0] = scene::SVertex(start.x,start.y,start.z,0,0,color);
		vertices[1] = scene::SVertex(end.x,end.y,end.z,0,0,color);
		//drawVertexPrimitiveList(vertices, 2, indices, 2, ENUM_PRIMITIVE_TYPE_LINES, scene::ENUM_VERTEX_TYPE_3V1T1C);
		drawVertex(vertices, 2, indices, 2, ENUM_PRIMITIVE_TYPE_LINES, scene::ENUM_VERTEX_TYPE_3V1T1C);
	}

	bool COGLES1Driver::setTexture(u32 stage, const video::ITexture* texture){
		if(m_currentTextures[stage]==texture)
		{
			//Logger->debug("same texture:%d\n",static_cast<const COGLES1Texture*>(texture)->getTextureId());
			return true;
		}

		glActiveTexture(GL_TEXTURE0 + stage);
		if (!texture){
			glDisable(GL_TEXTURE_2D);
			m_currentTextures[stage]=NULL;
		}
		else{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,static_cast<const COGLES1Texture*>(texture)->getTextureId());
			m_currentTextures[stage]=const_cast<video::ITexture*>(texture);

			//Logger->debug("glBindTexture:%d\n",static_cast<const COGLES1Texture*>(texture)->getTextureId());
			//checkGLError(__FILE__,__LINE__);
		}
		return true;
	}

	void COGLES1Driver::setTransform(ENUM_TRANSFORM transform, const core::matrix4f& mat){
		m_matrix[transform] = mat;
		switch(transform)
		{
		case ENUM_TRANSFORM_VIEW:
			{
				glMatrixMode(GL_MODELVIEW);
				//TODO
				//OpenGL对顶点先进行模型变换，再进行视图变换，即v'=V*M*v，运算顺序从右到左，因为矩阵运算满足结合律，故有：
				//v'=(V*M)*v（不过还有一个终点,matrix4中对矩阵进行scale时却是*this*=scale）
				//glLoadMatrixf((m_matrix[ENUM_TRANSFORM_WORLD]*m_matrix[ENUM_TRANSFORM_VIEW]).pointer());
				glLoadMatrixf((m_matrix[ENUM_TRANSFORM_VIEW]*m_matrix[ENUM_TRANSFORM_WORLD]).pointer());
				//Logger->debug("setViewMatrix:\n");
				//m_matrix[ENUM_TRANSFORM_VIEW].print();
			}
			break;
		case ENUM_TRANSFORM_WORLD:
			{
				//OpenGL ES1中只存在ModelView矩阵,其实就是由World矩阵与View矩阵相乘而成
				glMatrixMode(GL_MODELVIEW);
				//glLoadMatrixf((m_matrix[ENUM_TRANSFORM_WORLD]*m_matrix[ENUM_TRANSFORM_VIEW]).pointer());
				glLoadMatrixf((m_matrix[ENUM_TRANSFORM_VIEW]*m_matrix[ENUM_TRANSFORM_WORLD]).pointer());
				//Logger->debug("setWorldMatrix:\n");
				//m_matrix[ENUM_TRANSFORM_WORLD].print();
			}
			break;
		case ENUM_TRANSFORM_PROJECTION:
			{
				glMatrixMode(GL_PROJECTION);
				glLoadMatrixf(m_matrix[ENUM_TRANSFORM_PROJECTION].pointer());
				//Logger->debug("setProjectionMatrix:\n");
				//m_matrix[ENUM_TRANSFORM_PROJECTION].print();
			}
			break;
		case ENUM_TRANSFORM_TEXTURE0:
		case ENUM_TRANSFORM_TEXTURE1:
			if(m_pCurrentMaterial!=NULL){
				const u32 i = transform - ENUM_TRANSFORM_TEXTURE0;
				glActiveTexture(GL_TEXTURE0 + i);
				glMatrixMode(GL_TEXTURE);
				if(mat.isIdentity())
					glLoadIdentity();
				else
					glLoadMatrixf(mat.pointer());
			}
			break;
		default:
			Logger->warn(YON_LOG_WARN_FORMAT,"Set unknown transform");
			break;
		}
	}

	const core::matrix4f& COGLES1Driver::getTransform(ENUM_TRANSFORM transform) const{
		return m_matrix[transform];
	}

	ITexture* COGLES1Driver::addRenderTargetTexture(const core::dimension2du& size,const io::path& name, video::ENUM_COLOR_FORMAT format){
		//disable mip-mapping
		const bool useMipmap = getTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_MIPMAPS);
		setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_MIPMAPS, false);

		video::ITexture* rtt = 0;
		

		if(queryFeature(ENUM_VIDEO_FEATURE_FBO))
		{
			Logger->debug(YON_LOG_SUCCEED_FORMAT,"use fast RTT for supporting FBO!");

			rtt = new COGLES1FBOTexture(size, name, this, format);
			if (rtt)
			{
				addTexture(rtt);
				rtt->drop();
			}
		}
		else
		{
			Logger->warn(YON_LOG_WARN_FORMAT,"use slow RTT for not supporting FBO!");

			//android上不支持R8G8B8A8/R5G5B5A1，glTexSubImage2D出现GL_INVALID_OPERATION
#ifdef YON_COMPILE_WITH_ANDROID
			if(video::hasAlpha(format))
			{
				video::ENUM_COLOR_FORMAT newFormat=video::discardAlpha(format);
				Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("not support %s, change to %s!",COLOR_FORMAT_NAME[format],COLOR_FORMAT_NAME[newFormat]).c_str());
				format=newFormat;
			}
#endif
			core::dimension2du destSize(core::min_(size.w,(u32)m_windowSize.w), core::min_(size.h,(u32)m_windowSize.h));
			//but why?
			//destSize = destSize.getOptimalSize((size==size.getOptimalSize()), false, false);
			destSize = destSize.getOptimalSize(true, false, false);
			rtt = addTexture(destSize, name, format);
			if (rtt)
				static_cast<COGLES1Texture*>(rtt)->setIsRenderTarget(true);

			Logger->debug("addRenderTargetTexture:%s\n",name.c_str());
		}

		//restore mip-mapping
		setTextureCreationConfig(MASK_TEXTURE_CREATION_CONFIG_MIPMAPS, useMipmap);
		return rtt;

	}
	//bool COGLES1Driver::setRenderTarget(video::ITexture* texture,bool clearBackBuffer, bool clearZBuffer,video::SColor color){
	//	return false;
	//}

	IImage* COGLES1Driver::createImageFromFile(const io::path& filename,bool translateIntoGray){
		//io::IReadFile* file = m_pFileSystem->createAndOpenFile(filename);
		io::IReadStream* file = m_pFileSystem->createAndOpenReadFileStream(filename);
		IImage* image=createImageFromFile(file,translateIntoGray);
		file->drop();
		return image;
	}

	IImage* COGLES1Driver::createImageFromFile(io::IReadStream* file,bool translateIntoGray){
		if (!file)
			return NULL;

		IImage* image = NULL;

		u32 i;

		for (i=0; i<m_imageLoaders.size(); ++i)
		{
			if (m_imageLoaders[i]->checkFileExtension(file->getPath()))
			{
				// reset file position which might have changed due to previous loadImage calls
				file->seek(0);
				image = m_imageLoaders[i]->loadImage(file,translateIntoGray);
				if (image)
					return image;
			}
		}

		for (i=0; i<m_imageLoaders.size(); ++i)
		{
			file->seek(0);
			if (m_imageLoaders[i]->checkFileHeader(file))
			{
				file->seek(0);
				image = m_imageLoaders[i]->loadImage(file,translateIntoGray);
				if (image)
					return image;
			}
		}

		return NULL;
	}

	void COGLES1Driver::removeTexture(ITexture* texture){
		if (!texture)
			return;

		for (u32 i=0; i<m_textures.size(); ++i)
		{
			if (m_textures[i] == texture)
			{
				texture->drop();
				m_textures.erase(i);
			}
		}
	}

	video::ITexture* COGLES1Driver::findTexture(const io::path& filename){
		//TODO 优化
		for(u32 i=0;i<m_textures.size();++i){
			//Logger->debug("check %s==%s\n",m_textures[i]->getPath().c_str(),absolutePath.c_str());
			if(m_textures[i]->getPath()==filename){
				return m_textures[i];
			}
		}

		//const io::path absolutePath = m_pFileSystem->getAbsolutePath(filename);
		const io::path absolutePath = m_pFileSystem->getResourcePath(filename);
		for(u32 i=0;i<m_textures.size();++i){
			//Logger->debug("check %s==%s\n",m_textures[i]->getPath().c_str(),absolutePath.c_str());
			if(m_textures[i]->getPath()==absolutePath){
				return m_textures[i];
			}
		}
		return NULL;
	}

	ITexture* COGLES1Driver::addTexture(const core::dimension2du& size,const io::path& name, ENUM_COLOR_FORMAT format){
		if(IImage::isRenderTargetOnlyFormat(format))
		{
			Logger->warn("Could not create ITexture, format only supported for render target textures.\n");
			return NULL;
		}

		if ( 0 == name.length () )
		{
			Logger->warn("Could not create ITexture with name is empty.\n");
			return NULL;
		}

		IImage* image= new CImage(format, size);
		//IImage* image = new CImage(format, size, NULL, false, false);
		/*if(empty)
			 image = new CImage(format, size, NULL, false, false);
		else
			 image = new CImage(format, size);
		 */
		ITexture* t = createDeviceDependentTexture(image, name);
		image->drop();
		addTexture(t);

		if (t)
			t->drop();

		return t;
	}

	void COGLES1Driver::addTexture(video::ITexture* texture)
	{
		if (texture)
		{
			texture->grab();
			m_textures.push_back(texture);
		}
	}

	video::ITexture* COGLES1Driver::loadTextureFromFile(io::IReadStream* file){
		ITexture* texture = NULL;
		Logger->debug("start load texture:%s\n",getFileName(file->getPath()).c_str());
		IImage* image = createImageFromFile(file,false);

		/*u32 index=0;
		u8* data=(u8*)image->lock();
		Logger->setFormat(debug::MASK_FORMAT_LOG);
		for (u32 i=0; i<image->getDimension().h; ++i)
		{
			for(u32 j=0;j<image->getByteCountPerRow();++j)
			{
				Logger->debug("%d,",data[index]);
				index++;
			}
			Logger->debug("\n");
		}
		image->unlock();*/

		if (image)
		{
			texture = createDeviceDependentTexture(image, file->getPath());
			Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("end load texture:%s",getFileName(file->getPath()).c_str()).c_str());
			image->drop();
		}

		return texture;
	}

	video::ITexture* COGLES1Driver::createDeviceDependentTexture(IImage* image, const io::path& name)
	{
		return new COGLES1Texture(image, name, this);
	}

	ITexture* COGLES1Driver::getTexture(const io::path& filename){
		ITexture* texture = findTexture(filename);
		if (texture){
			//Logger->debug("getTexture(%s) finded!\n",filename.c_str());
			return texture;
		}

		//io::IReadFile* file = m_pFileSystem->createAndOpenFile(filename);
		io::IReadStream* file = m_pFileSystem->createAndOpenReadFileStream(filename);

		if(!file)
		{
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("getTexture(%s) failed,for file do not exist!",filename.c_str()).c_str());
			return NULL;
		}

		texture = loadTextureFromFile(file);
		file->drop();

		if (texture)
		{
			addTexture(texture);
			texture->drop(); // drop it because we created it, one grab too much
		}
		else
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("addTexture for %s failed!",filename.c_str()).c_str());
		return texture;
	}

	IMaterial* COGLES1Driver::createMaterial(){
		return new CMaterial();
	}

	void  COGLES1Driver::setMaterial(IMaterial* material){
		if(material==NULL||m_pCurrentMaterial==material)
			return;
		//Logger->debug("setMaterial:%08X\n",material);
		material->grab();
		if(m_pCurrentMaterial)
			m_pCurrentMaterial->drop();
		m_pCurrentMaterial=material;
		//TODO优化，有些材质并不需要加载所有矩阵
		for (s32 i = MATERIAL_MAX_TEXTURES-1; i>= 0; --i)
		{
			setTransform ((ENUM_TRANSFORM)( ENUM_TRANSFORM_TEXTURE0+i),m_pCurrentMaterial->getTextureMatrix(i));
		}
	}

	void COGLES1Driver::checkMaterial(){
		ENUM_MATERIAL_TYPE cmt=m_pCurrentMaterial->getMaterialType();
		if(m_pLastMaterial!=m_pCurrentMaterial){
			m_pCurrentMaterial->grab();
			if(m_pLastMaterial){
				ENUM_MATERIAL_TYPE lmt=m_pLastMaterial->getMaterialType();
				if(lmt!=cmt)
					m_materialRenderers[lmt]->onUnsetMaterial();
				m_materialRenderers[cmt]->onSetMaterial(m_pCurrentMaterial,m_pLastMaterial);
				m_pLastMaterial->drop();
			}else{
				m_materialRenderers[cmt]->onSetMaterial(m_pCurrentMaterial,m_pLastMaterial);
			}
			m_pLastMaterial=m_pCurrentMaterial;
		}else if(m_pCurrentMaterial->checkChanged()){
			m_materialRenderers[cmt]->onSetMaterial(m_pCurrentMaterial,m_pLastMaterial);
		}
		/*if(m_pLastMaterial!=m_pCurrentMaterial){
			m_pCurrentMaterial->grab();
			if(m_pLastMaterial){
				ENUM_MATERIAL_TYPE lmt=m_pLastMaterial->getMaterialType();
				ENUM_MATERIAL_TYPE cmt=m_pCurrentMaterial->getMaterialType();
				if(lmt!=cmt)
				{
					if(m_pLastMaterial->states!=m_pCurrentMaterial->states)
					{
						int xorValue=m_pLastMaterial->states^(m_pCurrentMaterial->states);
						int mask=1;
						int cmask=m_pCurrentMaterial->states.getInt();
						for(int i=0;i<ENUM_MATERIAL_STATE_COUNT;++i)
						{
							if(xorValue&mask)
							{
								if(cmask&mask){
									//Logger->debug("%s change to true\n",MATERIAL_STATE_NAMES[i]);
									glEnable(MATERIAL_GLSTATES[i]);
								}else{
									//Logger->debug("%s change to false\n",MATERIAL_STATE_NAMES[i]);
									glDisable(MATERIAL_GLSTATES[i]);
								}
							}
							mask<<=1;
						}
					}
					if(m_pLastMaterial->getCullingMode()!=m_pCurrentMaterial->getCullingMode())
						glCullFace(m_pCurrentMaterial->getCullingMode());
					if(m_pLastMaterial->getFrontFace()!=m_pCurrentMaterial->getFrontFace())
						glFrontFace(m_pCurrentMaterial->getFrontFace());
					m_materialRenderers[lmt]->onUnsetMaterial();
				}
				m_materialRenderers[cmt]->onSetMaterial(m_pCurrentMaterial,m_pLastMaterial);

				m_pLastMaterial->drop();
			}else{
				ENUM_MATERIAL_TYPE cmt=m_pCurrentMaterial->getMaterialType();
				int mask=1;
				int cmask=m_pCurrentMaterial->states.getInt();
				for(int i=0;i<ENUM_MATERIAL_STATE_COUNT;++i)
				{
					if(cmask&mask){
						Logger->debug("%s change to true\n",MATERIAL_STATE_NAMES[i]);
						glEnable(MATERIAL_GLSTATES[i]);
					}else{
						Logger->debug("%s change to false\n",MATERIAL_STATE_NAMES[i]);
						glDisable(MATERIAL_GLSTATES[i]);
					}
					mask<<=1;
				}
				glCullFace(m_pCurrentMaterial->getCullingMode());
				glFrontFace(m_pCurrentMaterial->getFrontFace());
				m_materialRenderers[cmt]->onSetMaterial(m_pCurrentMaterial,NULL);
			}
			m_pLastMaterial=m_pCurrentMaterial;
		}*/
	}

	void COGLES1Driver::setRender3DMode(){
		if (m_renderMode != ENUM_RENDER_MODE_3D)
		{
			glEnable(GL_DEPTH_TEST);
			glClearDepthf(1.0f);
			glDepthFunc(GL_LEQUAL);

			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
			glHint(GL_GENERATE_MIPMAP_HINT, GL_FASTEST);
			glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
			glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);

			//glEnable(GL_CULL_FACE);
			//glDisable(GL_CULL_FACE);
			//glCullFace(GL_BACK);
			//glFrontFace(GL_CCW);

			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);

			//setMaterial(video::DEFAULT_3D_MATERIAL);
			//checkMaterial();

			//glMatrixMode(GL_MODELVIEW);
			//glLoadMatrixf((m_matrix[ENUM_TRANSFORM_VIEW]*m_matrix[ENUM_TRANSFORM_WORLD]).pointer());

			//glMatrixMode(GL_PROJECTION);
			//glLoadMatrixf(m_matrix[ENUM_TRANSFORM_PROJECTION].pointer());

			m_bRenderModeChange = true;

			m_renderMode=ENUM_RENDER_MODE_3D;
		}
	}
	void COGLES1Driver::setRender2DMode(){
		if (m_renderMode != ENUM_RENDER_MODE_2D)
		{
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//setMaterial(video::DEFAULT_2D_MATERIAL);
			//checkMaterial();

			m_bRenderModeChange = true;

			m_renderMode=ENUM_RENDER_MODE_2D;
		}
	}

	bool COGLES1Driver::checkGLError(const c8* file,s32 line)
	{
		return checkError(file,line);
	}

	bool COGLES1Driver::checkError(const c8* file,s32 line)
	{
		GLenum g = glGetError();
		switch(g)
		{
		case GL_NO_ERROR:
			Logger->info(YON_LOG_SUCCEED_FORMAT,core::stringc("GL_NO_ERROR at line:%i(%s) ",line,file).c_str());break;
			return false;
		case GL_INVALID_ENUM:
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("GL_INVALID_ENUM at line:%i(%s) ",line,file).c_str());break;
		case GL_INVALID_VALUE:
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("GL_INVALID_VALUE at line:%i(%s) ",line,file).c_str());break;
		case GL_INVALID_OPERATION:
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("GL_INVALID_OPERATION at line:%i(%s) ",line,file).c_str());break;
		case GL_STACK_OVERFLOW:
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("GL_STACK_OVERFLOW at line:%i(%s) ",line,file).c_str());break;
		case GL_STACK_UNDERFLOW:
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("GL_STACK_UNDERFLOW at line:%i(%s) ",line,file).c_str());break;
		case GL_OUT_OF_MEMORY:
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("GL_OUT_OF_MEMORY at line:%i(%s) ",line,file).c_str());break;
		};
		return true;
	}

#ifdef YON_COMPILE_WITH_WIN32
	bool COGLES1Driver::initEGL(const HWND& hwnd){

		EGLConfig config;
		EGLint num_configs;
		m_hDc = ::GetDC(hwnd);

		//First Step：Get EGLDisplay Object
		//The type and format of display_id are implementation-specific,
		//and it describes a specific display provided by the system EGL is running on.
		//For example, an EGL implementation under Microsoft Windows could define 
		//display_id to be a Windows Device Context.
		//If no display matching display_id is available,EGL_NO_DISPLAY is returned;
		//no error condition is raised in this case.
		m_eglDisplay = eglGetDisplay(m_hDc);
		/*if(m_eglDisplay == EGL_NO_DISPLAY)
		{
			m_eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);
			Logger->info("eglGetDisplay(EGL_DEFAULT_DISPLAY)\n");
		}*/

		//Second Step:Initialize EglDisplay Object
		//EGL_TRUE is returned on success, and major and minor are updated with
		// the major and minor version numbers of the EGL implementation.major 
		//and minor are not updated if they are specified as NULL.
		//EGL_FALSE is returned on failure and major and minor are not updated.
		//An EGL_BAD_DISPLAY error is generated if the dpy argument does not refer
		//to a valid EGLDisplay.An EGL_NOT_INITIALIZED error is generated if EGL cannnot be 
		//initialized for an otherwise valid dpy.
		//Initializing an already-initialized display is allowed,but the only effect of such 
		//a call is to return EGL_TRUE and update the EGL version numbers.
		EGLint majorVersion, minorVersion;
		if (!eglInitialize( m_eglDisplay, &majorVersion, &minorVersion ) )
		{
			//MessageBox(NULL,TEXT("Could not initialize OpenGL-ES1 display."),TEXT("Error"),MB_OK);
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("Initialize EglDisplay Object:%s",((core::stringc)EGLInfo::getInstance().EGL_ERROR_FLAGS[eglGetError()]).c_str()).c_str());
			return false;
		}
		else
		{
			Logger->info("EglDisplay initialized. Egl version %d.%d\n", majorVersion, minorVersion);
		}

		//EGL_RED_SIZE: bits of red in the color buffer
		//EGL_GREEN_SIZE: ...
		//EGL_BLUE_SIZE: ...
		//EGL_ALPHA_SIZE: ...
		//EGL_BUFFER_SIZE: total color component bits in the color buffer
		//EGL_SURFACE_TYPE: a mask indicating capabilities of surfaces that
		//				can be created with the corresponding EGLConfig(Default:EGL_WINDOW_BIT)
		//EGL_DEPTH_SIZE: bits of Z in the depth buffer
		//EGL_STENCIL_SIZE: bits of Stencil in the stencil buffer
		//EGL_SAMPLE_BUFFERS: number of multisample buffers
		//EGL_RENDERABLE_TYPE: a mask indicating which client APIs can render into a surface
		//				created with respect to and EGLConfig.
		//Creation of a client API context based on an EGLConfig will fail unless the EGLConfig's
		//EGL_RENDERABLE_TYPE attribute include the bit corresponding to that API and version.
		//				(Default:EGL_OPENGL_ES_BIT)

#if 1
		EGLint attribs[] =
		{
			EGL_RED_SIZE, 5,
			EGL_GREEN_SIZE, 5,
			EGL_BLUE_SIZE, 5,
			EGL_ALPHA_SIZE, 1,
			EGL_BUFFER_SIZE, 16,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_DEPTH_SIZE, 16,
			EGL_STENCIL_SIZE, false,
			EGL_SAMPLE_BUFFERS, 0,
			EGL_SAMPLES, 0,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
			EGL_NONE, 0
		};
#else
		EGLint attribs[] =
		{
			EGL_BUFFER_SIZE, 32,
			EGL_ALPHA_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_DEPTH_SIZE, 16,
			EGL_STENCIL_SIZE, 0,
			EGL_SAMPLES, 0,
			EGL_SAMPLE_BUFFERS, 0,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
#ifdef EGL_VERSION_1_3
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
#endif
			EGL_NONE, 0
		};
#endif
		
		//reference:http://pastebin.com/3dirn3yT
		/*EGLConfig* configs = NULL;
		if (eglGetConfigs(m_eglDisplay, 0, 0, &num_configs) != EGL_TRUE) {
			Logger->error(YON_LOG_FAILED_FORMAT,"Unable to acquire EGL configurations");
			return false;
		} else {
			if (num_configs == 0) {
				Logger->error(YON_LOG_FAILED_FORMAT,"No EGL configurations available.");
				return false;
			}
			configs = static_cast<EGLConfig*>(malloc(num_configs * sizeof(*configs)));
			eglGetConfigs(m_eglDisplay, configs, num_configs, &num_configs);
			if(configs==NULL){
				Logger->error(YON_LOG_FAILED_FORMAT,"No EGL configurations returned");
				return false;
			}
			for(s32 i=0; i<num_configs; ++i) {
				int attributeVal = 0;

				eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_SURFACE_TYPE, &attributeVal);
				if (!(attributeVal & EGL_WINDOW_BIT)) {
					continue;
				}

				eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_RENDERABLE_TYPE, &attributeVal);
				if (!(attributeVal & EGL_OPENGL_ES_BIT)) {
					continue;
				}

				eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_DEPTH_SIZE, &attributeVal);
				if (attributeVal == 0) {
					continue;
				}

				eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_RED_SIZE, &attributeVal);
				if (attributeVal != 8) {
					continue;
				}
				eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_GREEN_SIZE, &attributeVal);
				if (attributeVal != 8) {
					continue;
				}

				eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_BLUE_SIZE, &attributeVal);
				if (attributeVal != 8) {
					continue;
				}

				eglGetConfigAttrib(m_eglDisplay, configs[i], EGL_BUFFER_SIZE, &attributeVal);
				if (attributeVal != 32) {
					continue;
				}

				config = configs[i];
			}
		}

		if(config==NULL){
			Logger->error(YON_LOG_FAILED_FORMAT,"Could not get config for OpenGL-ES display");
			return false;
		}*/

		EGLConfig* configs = NULL;
		if (eglGetConfigs(m_eglDisplay, 0, 0, &num_configs) != EGL_TRUE) {
			Logger->error(YON_LOG_FAILED_FORMAT,"Unable to acquire EGL configurations");
			return false;
		} else {
			if (num_configs == 0) {
				Logger->error(YON_LOG_FAILED_FORMAT,"No EGL configurations available.");
				return false;
			}
			configs = new EGLConfig[num_configs];
			eglGetConfigs(m_eglDisplay, configs, num_configs, &num_configs);

			for(s32 i=0;i<num_configs;++i)
			{
				Logger->debug("Support Config:\n");
				printEGLConfig(configs[i]);
			}

			delete[] configs;
			configs=NULL;
		}

		//Third Step: Choose EGLConfig
		//API: EGLBoolean eglChooseConfig(EGLDisplay dpy,const EGLint *attrib_list,
		//			EGLConfig* configs,EGLint config_size, EGLint *num_config);
		//If no EGLConfig matching the attribute list exists, then the call
		//succeeds,but num_configs is set to 0.
		//If more than one matching EGLConfig is found,then a list of EGLConfigs is returned.
		//In most cases we just want the first config that meets all criteria, so we limit the
		//number of configs returned to 1.
		if (!eglChooseConfig(m_eglDisplay, attribs, &config, 1, &num_configs))
		{
			//MessageBox(NULL,TEXT("Could not get config for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
			
			Logger->error(YON_LOG_FAILED_FORMAT,core::stringc("Choose EGLConfig:%s",((core::stringc)EGLInfo::getInstance().EGL_ERROR_FLAGS[eglGetError()]).c_str()).c_str());
			return false;
		}
		
		
		/*Logger->debug("Try Config:\n");
		printEGLAttribute(attribs);

		int steps=5;
		while (!eglChooseConfig(m_eglDisplay, attribs, &config, 1, &num_configs) || !num_configs)
		{
			switch (steps)
			{
			case 5: // samples
				if (attribs[19]>2)
				{
					--attribs[19];
				}
				else
				{
					attribs[17]=0;
					attribs[19]=0;
					--steps;
				}
				break;
			case 4: // stencil
				if (attribs[15]==16)
				{
					attribs[15]=8;
				}
				else if(attribs[15]==8)
				{
					attribs[15]=0;
				}
				else
					--steps;
				break;
			case 3: // alpha
				if (attribs[7])
				{
					attribs[7]=0;

				}
				else
					--steps;
				break;
			case 2: // depth size
				if (attribs[13]>16)
				{
					attribs[13]-=8;
				}
				else
					--steps;
				break;
			case 1: // buffer size
				if (attribs[9]>16)
				{
					attribs[9]-=8;
				}
				else
					--steps;
				break;
			default:
				Logger->error(YON_LOG_FAILED_FORMAT,"Choose EGLConfig");
				return false;
			}

			Logger->debug("Try Config:\n");
			printEGLAttribute(attribs);
		}*/
		

		//Fourth Step: Create EGLSurface
		//EGLSurface eglCreateWindowSurface(EGLDisplay dpy,EGLConfig config,
		//			EGLNativeWindowType win,const EGLint *attrib_list)
		//eglCreateWindowSurface creates an onscreen EGLSurface and returns a handle to it.
		//Any EGL context created with a compatible EGLConfig can be used to render into this
		//surface.
		//when the attrib_list is NULL,all attributes use default value as below:
		//EGL_RENDER_BUFFER: EGL_BACK_BUFFER
		//On failure eglCrateWindowSurface returns EGL_NO_SURFACE
		//Use eglMakeCurrent to attach an EGL rendering context to the surface
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, hwnd, NULL);
		if (EGL_NO_SURFACE==m_eglSurface)
			//why?
			//Through platform specific functions we need to create a window that we can
			//use for OpenGL ES output.If there is no window system win should remain 0.
			m_eglSurface = eglCreateWindowSurface(m_eglDisplay, config, NULL, NULL);
		if (EGL_NO_SURFACE==m_eglSurface)
		{
			//MessageBox(NULL,TEXT("Could not create surface for OpenGL-ES1 display."),TEXT("Error"),MB_OK);
			Logger->error(YON_LOG_FAILED_FORMAT,"Create EGLSurface");
			return false;
		}

		//eglBindAPI(EGL_OPENGL_ES_API);

		//The list is terminated with EGL_NONE.
		//If an attribute is not specified in eglAttributes,then the default value is used.
		//If EGL_DONT_CARE is specified as an attribute value,then the attribute will not
		//be checked.
		//EGL_DONT_CARE may be specified for all attributes except EGL_LEVEL.
		EGLint eglAttributes[] = {
			EGL_CONTEXT_CLIENT_VERSION, 1,
			EGL_NONE
		};

		//Fifth Step: Create EGLContext
		//EGL has to create a context for OpenGL ES.Our OpenGL ES resources like textures will only be valid inside this context(or shared contexs).
		//API: EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
		//If eglCreateContext succeeds,it initializes the context to the initial state defined for the current
		//rendering API, and returns a handle to it.The context can be used to render to any compatible EGLSurface.
		//On failure eglCreateContext returns EGL_NO_CONTEXT.If the current rendering api is EGL_NONE,then an EGL_BAD_MATCH error is
		//generated(this situation can only arise in an implementation which does not support OpenGL ES,and prior to the first call to
		//eglBindAPI).
		//attrib_list specifies a list of attributes for the context.The list has the same structure as described for eglChooseConfig.
		//The only attribute that can be specified in attrib_list is EGL_CONTEXT_CLIENT_VERSION, and this attribute may only be
		//specified when creating a OpenGL ES context
		//EGL_CONTEXT_CLIENT_VERSION determines which version of an OpenGL ES context to create.An attribute value of 1 specifies creation
		//of an OpenGL ES 1.x context.An attribute value of 2 specifies creation of an OpenGL ES 2.x context.(Default:1)
		m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, eglAttributes);
		//m_eglContext = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, NULL);

		/*if(m_eglContext==EGL_NO_CONTEXT){
			Logger->error(YON_LOG_FAILED_FORMAT,"Create EGLContext");
			return false;
		}*/

		//TODO check error

		//Sixth Step: Bind Contexts and Drawables
		//API: EGLBoolean eglMakeCurrent(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
		//eglMakeCurrent binds ctx to the current rendering thread and to the draw and read surfaces
		//For an OpenGL and OpenGL ES context,draw is used for all operations except for any pixel data read
		//back or copied,which is taken from the frame buffer values of read.
		//eglMakeCurrent returns EGL_FALSE on failure.
		if(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)==EGL_FALSE){
			Logger->info(YON_LOG_FAILED_FORMAT,"Bind Contexts and Drawables");
			return false;
		}
		return true;
	}

	void COGLES1Driver::destroyEGL(){
		//Finial Step: Terminate OpenGL ES and destroy the window(if present).
		//eglTerminate takes care of destroying any context or surface created with this display,so we don't need to call
		//eglDestroySurface or eglDestroyContext here.
		//To release the current context without assigning a new one, set ctx to EGL_NO_CONTEXT and set draw and read to
		//EGL_NO_SURFACE.
		if(eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)==EGL_TRUE){
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Unbind GL contexts and surface");
		}else{
			Logger->error(YON_LOG_FAILED_FORMAT,"Unbind GL contexts and surface");
		}
		eglDestroyContext(m_eglDisplay, m_eglContext);
		eglDestroySurface(m_eglDisplay, m_eglSurface);

		//To release resources associated with use of EGL and client APIs on a display, call
		//EGLBoolean eglTerminate(EGLDisplay dpy);
		//Termination marks all EGL-specific resources,such as contexts and surfaces,associated with the specified display for
		//deletion.Handles to all such resources are invalid as soon as eglTerminate returns,but the dpy handle itself remains
		//valid.
		//eglTerminate returns EGL_TRUE on success.
		if(eglTerminate(m_eglDisplay)==EGL_TRUE){
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Release all resources in EGL and display");
		}else{
			Logger->error(YON_LOG_FAILED_FORMAT,"Release all resources in EGL and display");
		}
		if (m_hDc&&!::ReleaseDC((HWND)m_hWnd, m_hDc)){
			Logger->warn(YON_LOG_WARN_FORMAT,"Release DC");
		}else{
			Logger->info(YON_LOG_SUCCEED_FORMAT,"Release DC");
		}
	}
#endif//YON_COMPILE_WITH_WIN32

}//namespace ogles1
}//namespace video
}//namespace yon
