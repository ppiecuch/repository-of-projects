#ifndef _YON_VIDEO_OGLES1_COGLES1DRIVER_H_
#define _YON_VIDEO_OGLES1_COGLES1DRIVER_H_

#include "IVideoDriver.h"
#include "SOGLES1Parameters.h"

#ifdef YON_COMPILE_WITH_WIN32
//����OpenGL ES1��Ҫ�Ŀ⼰ͷ�ļ�
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
				//virtual u32 getFPS() const;

			private:

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