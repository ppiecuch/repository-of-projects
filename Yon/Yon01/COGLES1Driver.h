#ifndef _YON_VIDEO_OGLES1_COGLES1DRIVER_H_
#define _YON_VIDEO_OGLES1_COGLES1DRIVER_H_

#include "IVideoDriver.h"
#include "SOGLES1Parameters.h"

#ifdef YON_COMPILE_WITH_WIN32
//加载OpenGL ES1需要的库及头文件
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv1_CM.lib")
#include <EGL/egl.h>
#include <GLES/gl.h>
#endif//YON_COMPILE_WITH_WIN32

namespace yon{
	namespace video{
		namespace ogles1{
			
			class COGLES1Driver:public IVideoDriver{
			public:

				COGLES1Driver(const SOGLES1Parameters& param);
				virtual ~COGLES1Driver();

				virtual void begin(bool zBuffer,core::color c);
				virtual void end();
				virtual void setViewPort(const core::recti& rec);
				virtual void setTransform(ENUM_TRANSFORM transform, const core::matrix4f& mat);
				virtual const core::matrix4f& getTransform(ENUM_TRANSFORM transform) const;

				virtual void drawUnit(scene::IUnit* unit) const;
				//virtual u32 getFPS() const;

			private:
				core::matrix4f m_matrix[ENUM_TRANSFORM_COUNT];
#ifdef YON_COMPILE_WITH_WIN32
				bool initEGL(const HWND& hwnd);
				void destroyEGL();

				EGLDisplay m_eglDisplay;
				EGLSurface m_eglSurface;
				EGLContext m_eglContext;

				void DrawFrame();
#endif//YON_COMPILE_WITH_WIN32
			};
		}
	}
}
#endif