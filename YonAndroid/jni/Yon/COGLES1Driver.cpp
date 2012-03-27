#include "COGLES1Driver.h"
#include "SVertex.h"
#include "COGLES1Texture.h"

#include "ILogger.h"

namespace yon{
namespace video{
namespace ogles1{
	unsigned char mIndices[] = { 0, 1, 2 };
	signed short mVertices[] = {
		-50, -29, 0,
		50, -29, 0,
		0,  58, 0
	};
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
	


	COGLES1Driver::COGLES1Driver(const SOGLES1Parameters& param,io::IFileSystem* fs):
			m_renderModeChange(true),
		IVideoDriver(fs){

		m_imageLoaders.push(createImageLoaderPNG());

#ifdef YON_COMPILE_WITH_WIN32
		initEGL(param.hWnd);
#endif//YON_COMPILE_WITH_WIN32

			u32 i;
			onResize(param.windowSize);

			for (i=0; i<ENUM_TRANSFORM_COUNT; ++i)
				setTransform(static_cast<ENUM_TRANSFORM>(i), core::IDENTITY_MATRIX);

			setRender3DMode();

		glClearColor(0.1f,0.2f,0.3f,1);
		glColor4f(1, 1, 1, 1);

		Logger->info(YON_LOG_SUCCEED_FORMAT,"Instance COGLES1Driver");

	}

	COGLES1Driver::~COGLES1Driver(){
		for(u32 i=0;i<m_imageLoaders.size();++i)
			m_imageLoaders[i]->drop();

		for(u32 i=0;i<m_textures.size();++i)
			m_textures[i]->drop();
#ifdef YON_COMPILE_WITH_WIN32
		destroyEGL();
#endif//YON_COMPILE_WITH_WIN32
		Logger->info(YON_LOG_SUCCEED_FORMAT,"Release COGLES1Driver");
	}

	void COGLES1Driver::begin(bool zBuffer,core::color c)
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}
	void COGLES1Driver::end()
	{
#ifdef YON_COMPILE_WITH_WIN32
		eglSwapBuffers(m_eglDisplay, m_eglSurface);
#endif//YON_COMPILE_WITH_WIN32
	}
	void COGLES1Driver::setViewPort(const yon::core::recti& r){
		glViewport(0, 0, r.w, r.h);
		Logger->debug("setViewPort(0,0,%d,%d)\n",r.w,r.h);
	}
	void COGLES1Driver::onResize(const yon::core::dimension2du& size){
		setViewPort(core::recti(0,0,size.w,size.h));
	}
	void COGLES1Driver::drawUnit(scene::IUnit* unit) const{
		glEnableClientState(GL_VERTEX_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, sizeof(scene::SVertex),&unit->getVertices()[0].pos);
		//OpenGL ES下支持GL_UNSIGNED_BYTE 或GL_UNSIGNED_SHORT.
		glDrawElements(GL_LINES, unit->getIndexCount(), GL_UNSIGNED_SHORT,unit->getIndices());
		glDisableClientState(GL_VERTEX_ARRAY);
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
			if(m_textures[i]->getPath()==filename){
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
		IImage* image = createImageFromFile(file);

		if (image)
		{
			texture = createDeviceDependentTexture(image, file->getPathName());
			Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("load texture:%s",file->getPathName().c_str()).c_str());
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
		if (texture)
			return texture;

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
			glAlphaFunc(GL_GREATER, 0.f);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);

			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf((m_matrix[ENUM_TRANSFORM_WORLD]*m_matrix[ENUM_TRANSFORM_VIEW]).pointer());

			glMatrixMode(GL_PROJECTION);
			glLoadMatrixf(m_matrix[ENUM_TRANSFORM_PROJECTION].pointer());

			m_renderModeChange = true;
		}
		m_renderMode=ENUM_RENDER_MODE_3D;
	}
	void COGLES1Driver::setRender2DMode(){}
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
