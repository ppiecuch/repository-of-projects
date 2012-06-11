#ifndef _YON_VIDEO_IVIDEODRIVER_H_
#define _YON_VIDEO_IVIDEODRIVER_H_

#include "IReferencable.h"
#include "dimension2d.h"
#include "position2d.h"
#include "rect.h"
#include "matrix4.h"
#include "IFileSystem.h"
#include "ITexture.h"
#include "IImageLoader.h"
#include "IMaterial.h"
#include "SColor.h"
#include "ITimer.h"
#include "SVertex.h"

namespace yon{

	namespace scene{
		class IUnit;
		class ISceneManager;
		ISceneManager* createSceneManager();
	}

	namespace video{

		IImageLoader* createImageLoaderPNG();

		enum ENUM_TRANSFORM{
			ENUM_TRANSFORM_VIEW = 0,		//视图空间矩阵
			ENUM_TRANSFORM_WORLD,			//世界空间矩阵
			ENUM_TRANSFORM_PROJECTION,		//投影空间矩阵
			ENUM_TRANSFORM_TEXTURE0,		//纹理0矩阵
			ENUM_TRANSFORM_TEXTURE1,		//纹理1矩阵
			ENUM_TRANSFORM_COUNT
		};

		/*
		#define GL_POINTS                         0x0000
		#define GL_LINES                          0x0001
		#define GL_LINE_LOOP                      0x0002
		#define GL_LINE_STRIP                     0x0003
		#define GL_TRIANGLES                      0x0004
		#define GL_TRIANGLE_STRIP                 0x0005
		#define GL_TRIANGLE_FAN                   0x0006
		*/
		enum ENUM_PRIMITIVE_TYPE{
			ENUM_PRIMITIVE_TYPE_POINTS = 0x0000,
			ENUM_PRIMITIVE_TYPE_LINES = 0x0001,
			ENUM_PRIMITIVE_TYPE_TRIANGLES = 0x0004,
			ENUM_PRIMITIVE_TYPE_TRIANGLE_STRIP = 0x0005,
			ENUM_PRIMITIVE_TYPE_COUNT = 1
		};

		/*
		#define GL_BYTE                           0x1400
		#define GL_UNSIGNED_BYTE                  0x1401
		#define GL_SHORT                          0x1402
		#define GL_UNSIGNED_SHORT                 0x1403
		#define GL_FLOAT                          0x1406
		#define GL_FIXED                          0x140C
		*/
		enum ENUM_INDEX_TYPE{
			ENUM_INDEX_TYPE_8BIT = 0x1401,
			ENUM_INDEX_TYPE_16BIT = 0x1403,
			ENUM_INDEX_TYPE_COUNT = 2
		};

		enum ENUM_VIDEO_FEATURE{
			ENUM_VIDEO_FEATURE_FBO = 0,
			//items below are not in GL_EXTENSIONS list,need be processed separately
			ENUM_VIDEO_FEATURE_GENERATE_MIPMAP,
			ENUM_VIDEO_FEATURE_COUNT
		};

		struct SClearSetting{
			bool clearBackBuffer;
			bool clearZBuffer;
			video::SColor color;

			SClearSetting(bool backBuffer=true,bool zBuffer=true,video::SColor color=video::COLOR_BLACK)
				:clearBackBuffer(backBuffer),clearZBuffer(zBuffer),color(color){}
		};

		//视频驱动器接口
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
			ITimer* m_pTimer;
			virtual video::ITexture* createDeviceDependentTexture(IImage* image, const io::path& name) = 0;
		public:
			IVideoDriver(io::IFileSystem* fs,ITimer* timer)
				:m_renderMode(ENUM_RENDER_MODE_NONE),m_pFileSystem(fs),m_pTimer(timer){
					if(m_pFileSystem)
						m_pFileSystem->grab();
					if(m_pTimer)
						m_pTimer->grab();
			}
			virtual ~IVideoDriver(){
				if(m_pFileSystem)
					m_pFileSystem->drop();
				if(m_pTimer)
					m_pTimer->drop();
			};
			io::IFileSystem* getFileSystem() const{
				return m_pFileSystem;
			}

			virtual const SClearSetting& getClearSetting() const = 0;
			virtual void begin(bool backBuffer=true,bool zBuffer=true,const video::SColor& color=video::SColor(0xFF132E47)) = 0;
			virtual void end() = 0;
			virtual void setViewPort(const core::recti& r) = 0;
			virtual const core::dimension2du& getCurrentRenderTargetSize() const = 0;
			virtual void onResize(const core::dimension2du& size) = 0;


			//是否支持feature特性
			virtual bool queryFeature(ENUM_VIDEO_FEATURE feature) const =0;

			//从2D的XY坐标系（以左上角为原点,，x向右，y向下）转为3DXYY坐标系（以屏幕中心为原点，x向右，y向上）
			virtual void convertPosCoordinate(const core::position2di& src,core::position2df& dest) = 0;
			virtual void convertPosCoordinate(const core::position2di& src,core::position2di& dest) = 0;
			virtual void convertPosCoordinate(const core::position2df& src,core::position2df& dest) = 0;
			virtual void convertPosCoordinate(s32 srcX,s32 srcY,s32& destX,s32& destY) = 0;

			//添加一张用于渲染缓冲的纹理,size必须是2的N次幂,并且长度都不可超过后缓冲区的大小
			virtual ITexture* addRenderTargetTexture(const core::dimension2du& size,
				const io::path& name = "rtt", const video::ENUM_COLOR_FORMAT format = video::ENUM_COLOR_FORMAT_R5G5B5A1) =0;
			//参数texture:必须是由addRenderTargetTexture生成的texture
			//参数color:render target的背景色
			//virtual bool setRenderTarget(video::ITexture* texture,
			//	bool clearBackBuffer=true, bool clearZBuffer=true,video::SColor color=video::COLOR_BLACK) =0;


			virtual IImage* createImageFromFile(const io::path& filename) = 0;
			virtual IImage* createImageFromFile(io::IReadStream* file) =0;

			virtual ITexture* addTexture(const core::dimension2du& size,
				const io::path& name, ENUM_COLOR_FORMAT format = ENUM_COLOR_FORMAT_R8G8B8A8) = 0;
			virtual bool setTexture(u32 stage, const video::ITexture* texture) = 0;
			virtual ITexture* getTexture(const io::path& filename) = 0;
			virtual video::ITexture* findTexture(const io::path& filename) = 0;
			virtual void removeTexture(ITexture* texture) =0;

			virtual void setTransform(ENUM_TRANSFORM transform, const core::matrix4f& mat) =0;
			virtual const core::matrix4f& getTransform(ENUM_TRANSFORM transform) const =0;

			virtual void setMaterial(IMaterial* material) = 0;
			virtual IMaterial* createMaterial() = 0;

			virtual void drawUnit(scene::IUnit* unit) = 0;
			virtual void draw2DImage(const video::ITexture* texture, const core::position2di& destPos,
				const core::recti& sourceRect, const core::recti* clipRect =NULL,
				video::SColor color=video::COLOR_WHITE, bool useAlphaChannelOfTexture=false) =0;
			virtual void drawVertexPrimitiveList(const void* vertices, u32 vertexCount,
				const void* indice, u32 indexCount,
				ENUM_PRIMITIVE_TYPE pType=ENUM_PRIMITIVE_TYPE_TRIANGLES,
				scene::ENUM_VERTEX_TYPE vType=scene::ENUM_VERTEX_TYPE_3V1T1C) =0;
			virtual void draw3DLine(const core::vector3df& start,const core::vector3df& end, video::SColor color=video::COLOR_WHITE) =0;
			virtual u32 getFPS() const = 0;
		};
	}
}
#endif