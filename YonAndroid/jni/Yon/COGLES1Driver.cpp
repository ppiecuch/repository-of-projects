#include "COGLES1Driver.h"
#include "SVertex.h"
#include "COGLES1Texture.h"
#include "COGLES1MaterialRenderer.h"
#include "ILogger.h"
#include "CDebugPrinter.h"

namespace yon{
namespace video{
namespace ogles1{
	unsigned char mIndices[] = { 0, 1, 2 };
	signed short mVertices[] = {
		-50, -29, 0,
		50, -29, 0,
		0,  58, 0
	};
	GLubyte mColors[12]={255,0,0,64,  0,255,0,255,  0,0,255,0};
	GLfloat mTexcoords[6]={0,0,1,0,0.5f,1};
	void InitGL()
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_SHORT, 0, mVertices);

		glMatrixMode(GL_PROJECTION);
		//glOrthox(-160<<16, 160<<16, -120<<16, 120<<16, -128<<16, 128<<16);

		//gluPerspective(60.0f,(float)/(float)height,0.1f,200);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClearColor(0.1f,0.2f,0.3f,1);
		glColor4x(0x10000, 0, 0, 0);
	}
	


	COGLES1Driver::COGLES1Driver(const SOGLES1Parameters& param,io::IFileSystem* fs,ITimer* timer,scene::IGeometryFactory* geometryFty)
		:m_bRenderModeChange(true),m_pLastMaterial(NULL),m_pCurrentMaterial(NULL),
		m_pDebugPrinter(NULL),
		m_windowSize(param.windowSize),IVideoDriver(fs,timer){

		m_imageLoaders.push(createImageLoaderPNG());

		m_materialRenderers.push(createMaterialRendererSolid(this));
		m_materialRenderers.push(createMaterialRendererLighten(this));
		m_materialRenderers.push(createMaterialRendererTransparent(this));
		m_materialRenderers.push(createMaterialRendererTransparentBlendColor(this));
		m_materialRenderers.push(createMaterialRendererMask(this));

#ifdef YON_COMPILE_WITH_WIN32
		initEGL(param.hWnd);
#endif//YON_COMPILE_WITH_WIN32

		u32 i;
		onResize(param.windowSize);

		for(i=0; i<ENUM_TRANSFORM_COUNT; ++i)
			setTransform(static_cast<ENUM_TRANSFORM>(i), core::IDENTITY_MATRIX);

		for(i=0;i<YON_MATERIAL_MAX_TEXTURES;++i)
			m_currentTextures[i]=NULL;

		setRender3DMode();

		glClearColor(0.1f,0.2f,0.3f,1);
		glColor4f(1, 1, 1, 1);

		Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance COGLES1Driver");

		/*video::IImage* image=DebugFont::getInstance().createImage();
		ITexture* tex=createDeviceDependentTexture(image,io::path("_yon_debug_font_"));
		addTexture(tex);
		tex->drop();
		image->drop();
		DebugFont::getInstance().m_pTexture=tex;
		DebugFont::getInstance().m_pDriver=this;*/
		video::IImage* image=debug::createDebugPrinterTextureImage();
		ITexture* tex=createDeviceDependentTexture(image,io::path("_yon_debug_font_"));
		addTexture(tex);
		tex->drop();
		image->drop();
		m_pDebugPrinter=debug::createDebugPrinter(this,tex,geometryFty);
		Logger->setDebugPrinter(m_pDebugPrinter);
	}

	COGLES1Driver::~COGLES1Driver(){

		if(m_pDebugPrinter)
			m_pDebugPrinter->drop();

		if(m_pLastMaterial)
			m_pLastMaterial->drop();
		if(m_pCurrentMaterial)
			m_pCurrentMaterial->drop();

		u32 i=0;
		u32 size=0;

		size=m_imageLoaders.size();
		for(i=0;i<m_imageLoaders.size();++i)
			m_imageLoaders[i]->drop();
		Logger->info("Release %d/%d ImageLoader\n",i,size);

		size=m_textures.size();
		for(i=0;i<m_textures.size();++i)
			m_textures[i]->drop();
		Logger->info("Release %d/%d Texture\n",i,size);

		size=m_materialRenderers.size();
		for(i=0;i<m_materialRenderers.size();++i)
			m_materialRenderers[i]->drop();
		Logger->info("Release %d/%d MaterialRenderer\n",i,size);

#ifdef YON_COMPILE_WITH_WIN32
		destroyEGL();
#endif//YON_COMPILE_WITH_WIN32
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release COGLES1Driver");
	}

	void COGLES1Driver::begin(bool zBuffer,video::SColor c)
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}
	void COGLES1Driver::end()
	{
#ifdef YON_COMPILE_WITH_WIN32
		eglSwapBuffers(m_eglDisplay, m_eglSurface);
#endif//YON_COMPILE_WITH_WIN32
		//Logger->debug("realtime:%d\n",m_pTimer->getRealTime());
		m_FPSCounter.registerFrame(m_pTimer->getRealTime(),0);
	}
	void COGLES1Driver::setViewPort(const core::recti& r){
		glViewport(0, 0, r.getWidth(), r.getHeight());
		Logger->debug("setViewPort(0,0,%d,%d)\n",r.getWidth(), r.getHeight());
	}
	const core::dimension2du& COGLES1Driver::getCurrentRenderTargetSize() const{
		return m_windowSize;
	}
	void COGLES1Driver::onResize(const core::dimension2du& size){
		m_windowSize.w=size.w;
		m_windowSize.h=size.h;
		setViewPort(core::recti(0,0,size.w,size.h));
	}
	void COGLES1Driver::drawUnit(scene::IUnit* unit){
		if(unit->getDimenMode()==ENUM_DIMEN_MODE_3D)
			setRender3DMode();
		else
			setRender2DMode();
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

		static GLenum mode;
		switch(m_pCurrentMaterial->getPolygonMode()){
		case ENUM_POLYGON_MODE_FILL:
			mode=GL_TRIANGLES;
			break;
		case ENUM_POLYGON_MODE_LINE:
			mode=GL_LINES;
			break;
		case ENUM_POLYGON_MODE_POINT:
			mode=GL_POINTS;
			break;
		default:
			mode=GL_TRIANGLES;
			break;
		}

		if(unit->getDimenMode()==ENUM_DIMEN_MODE_3D)
		{
			/*glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(unit->getDimenMode(), GL_FLOAT, sizeof(scene::SVertex),&((scene::SVertex*)unit->getVertices())[0].pos);
			glColorPointer(4,GL_UNSIGNED_BYTE, sizeof(scene::SVertex),&((scene::SVertex*)unit->getVertices())[0].color);
			glTexCoordPointer(2, GL_FLOAT, sizeof(scene::SVertex),&((scene::SVertex*)unit->getVertices())[0].texcoords);
			glDrawElements(GL_TRIANGLES, unit->getIndexCount(), GL_UNSIGNED_SHORT,unit->getIndices());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);*/

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(unit->getDimenMode(), GL_FLOAT, sizeof(scene::SVertex),&((scene::SVertex*)unit->getShap()->getVertices())[0].pos);
			glColorPointer(4,GL_UNSIGNED_BYTE, sizeof(scene::SVertex),&((scene::SVertex*)unit->getShap()->getVertices())[0].color);
			glTexCoordPointer(2, GL_FLOAT, sizeof(scene::SVertex),&((scene::SVertex*)unit->getShap()->getVertices())[0].texcoords);
			glDrawElements(mode, unit->getShap()->getIndexCount(), GL_UNSIGNED_SHORT,unit->getShap()->getIndices());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}else{
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(unit->getDimenMode(), GL_FLOAT, sizeof(scene::S2DVertex),&((scene::S2DVertex*)unit->getShap()->getVertices())[0].pos);
			glColorPointer(4,GL_UNSIGNED_BYTE, sizeof(scene::S2DVertex),&((scene::S2DVertex*)unit->getShap()->getVertices())[0].color);
			glTexCoordPointer(2, GL_FLOAT, sizeof(scene::S2DVertex),&((scene::S2DVertex*)unit->getShap()->getVertices())[0].texcoords);
			glDrawElements(mode, unit->getShap()->getIndexCount(), GL_UNSIGNED_SHORT,unit->getShap()->getIndices());
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		//Logger->debug("drawUnit:%08x\n",unit);
	}

	void COGLES1Driver::drawVertexPrimitiveList(const void* vertices, u32 vertexCount,const void* indexList, u32 primCount,ENUM_PRIMITIVE_TYPE pType,ENUM_INDEX_TYPE iType){
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

		const core::dimension2du& renderTargetSize = getCurrentRenderTargetSize();

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


		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, sizeof(scene::SVertex),&vertices[0].pos);
		glColorPointer(4,GL_UNSIGNED_BYTE, sizeof(scene::SVertex),&vertices[0].color);
		glTexCoordPointer(2, GL_FLOAT, sizeof(scene::SVertex),&vertices[0].texcoords);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT,indices);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

	}

	bool COGLES1Driver::setTexture(u32 stage, const video::ITexture* texture){
		if(m_currentTextures[stage]==texture)
			return true;
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
				Logger->debug("setViewMatrix:\n");
				m_matrix[ENUM_TRANSFORM_VIEW].print();
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
				Logger->debug("setProjectionMatrix:\n");
				m_matrix[ENUM_TRANSFORM_PROJECTION].print();
			}
			break;
		default:
			break;
		}
	}

	const core::matrix4f& COGLES1Driver::getTransform(ENUM_TRANSFORM transform) const{
		return m_matrix[transform];
	}

	IImage* COGLES1Driver::createImageFromFile(const io::path& filename){
		io::IReadFile* file = m_pFileSystem->createAndOpenFile(filename);
		IImage* image=createImageFromFile(file);
		file->drop();
		return image;
	}

	IImage* COGLES1Driver::createImageFromFile(io::IReadFile* file){
		if (!file)
			return NULL;

		IImage* image = NULL;

		u32 i;

		for (i=0; i<m_imageLoaders.size(); ++i)
		{
			if (m_imageLoaders[i]->checkFileExtension(file->getFileName()))
			{
				// reset file position which might have changed due to previous loadImage calls
				file->seek(0);
				image = m_imageLoaders[i]->loadImage(file);
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
				image = m_imageLoaders[i]->loadImage(file);
				if (image)
					return image;
			}
		}

		return NULL;
	}

	video::ITexture* COGLES1Driver::findTexture(const io::path& filename){
		//TODO 优化
		const io::path absolutePath = m_pFileSystem->getAbsolutePath(filename);
		for(u32 i=0;i<m_textures.size();++i){
			//Logger->debug("check %s==%s\n",m_textures[i]->getPath().c_str(),absolutePath.c_str());
			if(m_textures[i]->getPath()==absolutePath){
				return m_textures[i];
			}
		}
		return NULL;
	}

	void COGLES1Driver::addTexture(video::ITexture* texture)
	{
		if (texture)
		{
			texture->grab();
			m_textures.push(texture);
		}
	}

	video::ITexture* COGLES1Driver::loadTextureFromFile(io::IReadFile* file){
		ITexture* texture = NULL;
		Logger->debug("start load texture:%s\n",file->getPathName().c_str());
		IImage* image = createImageFromFile(file);

		if (image)
		{
			texture = createDeviceDependentTexture(image, file->getPathName());
			Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("end load texture:%s",file->getPathName().c_str()).c_str());
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
			Logger->debug("getTexture(%s) finded!\n",filename.c_str());
			return texture;
		}

		io::IReadFile* file = m_pFileSystem->createAndOpenFile(filename);

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

	void  COGLES1Driver::setMaterial(IMaterial* material){
		if(material==NULL||m_pCurrentMaterial==material)
			return;
		material->grab();
		if(m_pCurrentMaterial)
			m_pCurrentMaterial->drop();
		m_pCurrentMaterial=material;
	}

	void COGLES1Driver::checkMaterial(){
		if(m_pLastMaterial!=m_pCurrentMaterial){
			m_pCurrentMaterial->grab();
			if(m_pLastMaterial){
				ENUM_MATERIAL_TYPE lmt=m_pLastMaterial->getMaterialType();
				ENUM_MATERIAL_TYPE cmt=m_pCurrentMaterial->getMaterialType();
				if(lmt!=cmt)
					m_materialRenderers[lmt]->onUnsetMaterial();
				m_materialRenderers[cmt]->onSetMaterial(m_pCurrentMaterial);

				m_pLastMaterial->drop();
			}else{
				ENUM_MATERIAL_TYPE cmt=m_pCurrentMaterial->getMaterialType();
				m_materialRenderers[cmt]->onSetMaterial(m_pCurrentMaterial);
			}
			m_pLastMaterial=m_pCurrentMaterial;
		}
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

			glEnable(GL_CULL_FACE);
			//glDisable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);

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

			m_bRenderModeChange = true;

			m_renderMode=ENUM_RENDER_MODE_2D;
		}
	}

	bool COGLES1Driver::checkGLError(const c8* file,s32 line)
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
		HDC hdc = GetDC(hwnd);

		//First Step：Get EGLDisplay Object
		//The type and format of display_id are implementation-specific,
		//and it describes a specific display provided by the system EGL is running on.
		//For example, an EGL implementation under Microsoft Windows could define 
		//display_id to be a Windows Device Context.
		//If no display matching display_id is available,EGL_NO_DISPLAY is returned;
		//no error condition is raised in this case.
		m_eglDisplay = eglGetDisplay(hdc);

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
			Logger->info(YON_LOG_FAILED_FORMAT,"Initialize EglDisplay Object");
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
		EGLint attribs[] =
		{
			EGL_RED_SIZE, 5,
			EGL_GREEN_SIZE, 5,
			EGL_BLUE_SIZE, 5,
			EGL_ALPHA_SIZE, 1,
			EGL_BUFFER_SIZE, 16,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_DEPTH_SIZE, 1,
			EGL_STENCIL_SIZE, false,
			EGL_SAMPLE_BUFFERS, 0,
			EGL_SAMPLES, 0,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
			EGL_NONE, 0
		};

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
			Logger->info(YON_LOG_FAILED_FORMAT,"Choose EGLConfig");
			return false;
		}

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
			Logger->info(YON_LOG_FAILED_FORMAT,"Create EGLSurface");
			return false;
		}

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
			Logger->info(YON_LOG_FAILED_FORMAT,"Unbind GL contexts and surface");
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
			Logger->info(YON_LOG_FAILED_FORMAT,"Release all resources in EGL and display");
		}
	}
#endif//YON_COMPILE_WITH_WIN32

}//namespace ogles1
}//namespace video
}//namespace yon
