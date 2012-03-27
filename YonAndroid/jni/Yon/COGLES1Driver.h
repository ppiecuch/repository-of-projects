#ifndef _YON_VIDEO_OGLES1_COGLES1DRIVER_H_
#define _YON_VIDEO_OGLES1_COGLES1DRIVER_H_

#include "IVideoDriver.h"
#include "SOGLES1Parameters.h"
#include "yonArray.h"
#include "ITexture.h"

#ifdef YON_COMPILE_WITH_WIN32
//加载OpenGL ES1需要的库及头文件
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv1_CM.lib")
#include <EGL/egl.h>
#include <GLES/gl.h>
#elif defined(YON_COMPILE_WITH_ANDROID)
#include <GLES/gl.h>
#endif//YON_COMPILE_WITH_WIN32

namespace yon{
	namespace video{
		namespace ogles1{
			
			class COGLES1Driver:public IVideoDriver{
			public:

				COGLES1Driver(const SOGLES1Parameters& param,io::IFileSystem* fs);
				virtual ~COGLES1Driver();

				virtual void begin(bool zBuffer,core::color c);
				virtual void end();
				virtual void setViewPort(const core::recti& r);
				virtual void onResize(const core::dimension2du& size);
				virtual IImage* createImageFromFile(io::IReadFile* file);
				virtual ITexture* getTexture(const io::path& filename);
				virtual video::ITexture* findTexture(const io::path& filename);
				virtual void setTransform(ENUM_TRANSFORM transform, const core::matrix4f& mat);
				virtual const core::matrix4f& getTransform(ENUM_TRANSFORM transform) const;

				virtual void drawUnit(scene::IUnit* unit) const;

				void setRender3DMode();
				void setRender2DMode();
				//virtual u32 getFPS() const;

			private:
				void addTexture(video::ITexture* texture);
				video::ITexture* loadTextureFromFile(io::IReadFile* file);
				video::ITexture* createDeviceDependentTexture(IImage* image, const io::path& name);

				core::matrix4f m_matrix[ENUM_TRANSFORM_COUNT];
				bool m_renderModeChange;

				core::array<video::ITexture*> m_textures;
				core::array<video::IImageLoader*> m_imageLoaders;
#ifdef YON_COMPILE_WITH_WIN32
				bool initEGL(const HWND& hwnd);
				void destroyEGL();

				EGLDisplay m_eglDisplay;
				EGLSurface m_eglSurface;
				EGLContext m_eglContext;
#endif//YON_COMPILE_WITH_WIN32
			};
		}
	}
}
#endif