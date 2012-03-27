#include "CImage.h"
#include <memory.h>

#include "ILogger.h"

namespace yon{
namespace video{

	CImage::CImage(ENUM_COLOR_FORMAT format, const core::dimension2d<u32>& size)
		:m_format(format),m_size(size),m_bDeleteData(true){
			m_bytesPerPixel=getPixelBitsByFormat(m_format)>>3;
			m_bytesPerRow=m_bytesPerPixel*m_size.w;
			m_pData = new u8[m_size.h * m_bytesPerRow];

			Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance CImage");
	}

	CImage::CImage(ENUM_COLOR_FORMAT format, const core::dimension2d<u32>& size,
		void* data, bool copyData,bool deleteData)
		:m_format(format),m_size(size),m_pData(NULL),m_bDeleteData(deleteData){
			m_bytesPerPixel=getPixelBitsByFormat(m_format)>>3;
			m_bytesPerRow=m_bytesPerPixel*m_size.w;
			if(copyData){
				u32 count=m_size.h * m_bytesPerRow;
				m_pData = new u8[count];
				memcpy(m_pData, data, count);
			}else{
				m_pData=(u8*)data;
			}

			Logger->debug(YON_LOG_SUCCEED_FORMAT,"Instance CImage");
	}

	CImage::~CImage(){
		if(m_bDeleteData)
			delete[] m_pData;
		Logger->debug(YON_LOG_SUCCEED_FORMAT,"Release CImage");
	}

	SColor CImage::getPixel(u32 x, u32 y) const
	{
		if (x >= m_size.w || y >= m_size.h)
			return SColor(0);

		switch(m_format)
		{
		case ENUM_COLOR_FORMAT_R5G5B5A1:
			return R5G5B5A1toR8G8B8A8(((u16*)m_pData)[y*m_size.w + x]);
		case ENUM_COLOR_FORMAT_R5G6B5:
			return R5G6B5toR8G8B8A8(((u16*)m_pData)[y*m_size.w + x]);
		case ENUM_COLOR_FORMAT_R8G8B8A8:
			return ((u32*)m_pData)[y*m_size.w + x];
		case ENUM_COLOR_FORMAT_R8G8B8:
			{
				u8* p = m_pData+(y*3)*m_size.w + (x*3);
				return SColor(255,p[0],p[1],p[2]);
			}
		}

		return SColor(0);
	}
}//video
}//yon