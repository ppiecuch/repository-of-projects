#ifndef _YON_VIDEO_OGLES1_COGLES1DRIVER_H_
#define _YON_VIDEO_OGLES1_COGLES1DRIVER_H_

#include "IVideoDriver.h"
#include "SOGLES1Parameters.h"
#include "yonArray.h"
#include "ITexture.h"
#include "IMaterial.h"
#include "IMaterialRenderer.h"
#include "IDebugPrinter.h"
#include "IGeometryFactory.h"
#include "CFPSCounter.h"

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

				COGLES1Driver(const SOGLES1Parameters& param,io::IFileSystem* fs,ITimer* timer,scene::IGeometryFactory* geometryFty);
				virtual ~COGLES1Driver();

				virtual void begin(bool zBuffer,video::SColor c);
				virtual void end();
				virtual void setViewPort(const core::recti& r);
				virtual const core::dimension2du& getCurrentRenderTargetSize() const;
				virtual void onResize(const core::dimension2du& size);

				virtual IImage* createImageFromFile(const io::path& filename);
				virtual IImage* createImageFromFile(io::IReadFile* file);

				virtual bool setTexture(u32 stage, const video::ITexture* texture);
				virtual ITexture* getTexture(const io::path& filename);
				virtual video::ITexture* findTexture(const io::path& filename);

				virtual void setTransform(ENUM_TRANSFORM transform, const core::matrix4f& mat);
				virtual const core::matrix4f& getTransform(ENUM_TRANSFORM transform) const;

				virtual void setMaterial(IMaterial* material);

				virtual void drawUnit(scene::IUnit* unit);
				virtual void draw2DImage(const video::ITexture* texture, const core::position2di& destPos,const core::recti& sourceRect, const core::recti* clipRect,video::SColor color, bool useAlphaChannelOfTexture);
				virtual void drawVertexPrimitiveList(const void* vertices, u32 vertexCount,const void* indice, u32 indexCount,ENUM_PRIMITIVE_TYPE pType,scene::ENUM_VERTEX_TYPE vType);

				virtual bool checkGLError(const c8* file,s32 line);
 
				void setRender3DMode();
				void setRender2DMode();
				virtual u32 getFPS() const{
					return m_FPSCounter.getFPS();
				}

			private:
				void checkMaterial();
				void addTexture(video::ITexture* texture);
				video::ITexture* loadTextureFromFile(io::IReadFile* file);
				virtual video::ITexture* createDeviceDependentTexture(IImage* image, const io::path& name);

				core::matrix4f m_matrix[ENUM_TRANSFORM_COUNT];
				bool m_bRenderModeChange;

				video::IMaterial *m_pLastMaterial,*m_pCurrentMaterial;
				//当前使用的纹理,用于优化效率
				video::ITexture* m_currentTextures[YON_MATERIAL_MAX_TEXTURES];

				debug::IDebugPrinter* m_pDebugPrinter;

				core::array<video::ITexture*> m_textures;
				core::array<video::IImageLoader*> m_imageLoaders;
				core::array<video::IMaterialRenderer*> m_materialRenderers;

				CFPSCounter m_FPSCounter;

				core::dimension2du m_windowSize;
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