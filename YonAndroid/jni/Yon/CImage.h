#ifndef _YON_VIDEO_CIMAGE_H_
#define _YON_VIDEO_CIMAGE_H_

#include "IImage.h"
namespace yon{
namespace video{

	class CImage : public IImage{
	private:

		u8* m_pData;
		core::dimension2d<u32> m_size;
		u32 m_bytesPerPixel;
		u32 m_bytesPerRow;
		ENUM_COLOR_FORMAT m_format;

		bool m_bDeleteData;
	public:
		//构造空图片
		CImage(ENUM_COLOR_FORMAT format, const core::dimension2d<u32>& size);

		//copyData:是否复制数据
		CImage(ENUM_COLOR_FORMAT format, const core::dimension2d<u32>& size,
			void* data, bool copyData= true,bool deleteData=true);

		virtual void* lock()
		{
			return m_pData;
		}

		virtual void unlock() {}

		virtual void upsidedown();

		virtual ~CImage();

		virtual const core::dimension2d<u32>& getDimension() const{
			return m_size;
		}

		virtual ENUM_COLOR_FORMAT getColorFormat() const{
			return m_format;
		}

		virtual u32 getByteCountPerRow() const{
			return m_bytesPerRow;
		}
		virtual u32 getByteCountPerPixel() const{
			return m_bytesPerPixel;
		}
		virtual u32 getImageDataSizeInPixels() const{
			return m_size.w*m_size.h;
		}

		virtual SColor getPixel(u32 x, u32 y) const;

		virtual u32 getValue(u32 x,u32 y) const{
			if (x >= m_size.w || y >= m_size.h)
				return -1;
			return m_pData[y*m_size.w + x];
		}
	};

}//video
}//yon
#endif