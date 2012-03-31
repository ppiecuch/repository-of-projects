#ifndef _YON_VIDEO_IVIDEODRIVER_H_
#define _YON_VIDEO_IVIDEODRIVER_H_

#include "IReferencable.h"
#include "dimension2d.h"
#include "rect.h"
#include "matrix4.h"
#include "IUnit.h"
#include "IFileSystem.h"
#include "ITexture.h"
#include "IImageLoader.h"
#include "IMaterial.h"
#include "SColor.h"

namespace yon{

	namespace scene{
		class ISceneManager;
		ISceneManager* createSceneManager();
	}
	namespace io
	{
		IFileSystem* createFileSystem();
	}

	namespace video{

		IImageLoader* createImageLoaderPNG();

		enum ENUM_TRANSFORM{
			ENUM_TRANSFORM_VIEW = 0,		//��ͼ�ռ����
			ENUM_TRANSFORM_WORLD,			//����ռ����
			ENUM_TRANSFORM_PROJECTION,		//ͶӰ�ռ����
			ENUM_TRANSFORM_COUNT
		};

		//��Ƶ�������ӿ�
		class IVideoDriver:public virtual core::IReferencable{
		protected:
			enum ENUM_RENDER_MODE{
				ENUM_RENDER_MODE_NONE = 0,
				ENUM_RENDER_MODE_2D,
				ENUM_RENDER_MODE_3D,
				ENUM_RENDER_MODE_COUNT
			};
			ENUM_RENDER_MODE m_renderMode;
			io::IFileSystem* m_pFileSystem;
		public:
			IVideoDriver(io::IFileSystem* fs)
				:m_renderMode(ENUM_RENDER_MODE_NONE),m_pFileSystem(fs){
					if(m_pFileSystem)
						m_pFileSystem->grab();
			}
			virtual ~IVideoDriver(){
				if(m_pFileSystem)
					m_pFileSystem->drop();
			};
			virtual void begin(bool zBuffer=true,video::SColor c=video::SColor(0x000000FF)) = 0;
			virtual void end() = 0;
			virtual void setViewPort(const core::recti& r) = 0;
			virtual const core::dimension2du& getCurrentRenderTargetSize() const = 0;
			virtual void onResize(const core::dimension2du& size) = 0;

			virtual IImage* createImageFromFile(const io::path& filename) = 0;
			virtual IImage* createImageFromFile(io::IReadFile* file) =0;

			virtual video::ITexture* createDeviceDependentTexture(IImage* image, const io::path& name) = 0;
			virtual ITexture* getTexture(const io::path& filename) = 0;
			virtual video::ITexture* findTexture(const io::path& filename) = 0;

			virtual void setTransform(ENUM_TRANSFORM transform, const core::matrix4f& mat) =0;
			virtual const core::matrix4f& getTransform(ENUM_TRANSFORM transform) const =0;

			virtual void setMaterial(IMaterial* material) = 0;

			virtual void drawUnit(scene::IUnit* unit) = 0;
			virtual void draw2DImage(const video::ITexture* texture, const core::position2di& destPos,
				const core::recti& sourceRect, const core::recti* clipRect =NULL,
				video::SColor color=video::COLOR_WHITE, bool useAlphaChannelOfTexture=false) =0;
			//virtual u32 getFPS() const = 0;
		};
	}
}
#endif