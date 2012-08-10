#ifndef _YON_VIDEO_IMAGE_H_
#define _YON_VIDEO_IMAGE_H_

#include "IReferencable.h"
#include "dimension2d.h"
#include "SColor.h"

namespace yon{
namespace video{
	enum ENUM_COLOR_FORMAT
	{
		ENUM_COLOR_FORMAT_R5G5B5A1 = 0,
		ENUM_COLOR_FORMAT_R5G6B5,
		ENUM_COLOR_FORMAT_R4G4B4A4,
		ENUM_COLOR_FORMAT_R8G8B8,
		ENUM_COLOR_FORMAT_R8G8B8A8,
		ENUM_COLOR_FORMAT_A8,
		ENUM_COLOR_FORMAT_L8,
		ENUM_COLOR_FORMAT_L8A8,
		ENUM_COLOR_FORMAT_COUNT
	};
	const static c8* COLOR_FORMAT_NAME[]=
	{
		"R5G5B5A1",
		"R5G6B5",
		"R4G4B4A4",
		"R8G8B8",
		"R8G8B8A8",
		"A8",
		"L8",
		"L8A8"
	};
	static u32 getPixelBitsByFormat(const ENUM_COLOR_FORMAT format)
	{
		switch(format)
		{
		case ENUM_COLOR_FORMAT_R5G5B5A1:
			return 16;
		case ENUM_COLOR_FORMAT_R5G6B5:
			return 16;
		case ENUM_COLOR_FORMAT_R4G4B4A4:
			return 16;
		case ENUM_COLOR_FORMAT_R8G8B8:
			return 24;
		case ENUM_COLOR_FORMAT_R8G8B8A8:
			return 32;
		case ENUM_COLOR_FORMAT_A8:
			return 8;
		case ENUM_COLOR_FORMAT_L8:
			return 8;
		case ENUM_COLOR_FORMAT_L8A8:
			return 16;
		default:
			return 0;
		}
	}

	static bool hasAlpha(const ENUM_COLOR_FORMAT format)
	{
		switch(format)
		{
		case ENUM_COLOR_FORMAT_R5G5B5A1:
		case ENUM_COLOR_FORMAT_R4G4B4A4:
		case ENUM_COLOR_FORMAT_R8G8B8A8:
		case ENUM_COLOR_FORMAT_A8:
		case ENUM_COLOR_FORMAT_L8A8:
			return true;
		}
		return false;
	}
	
	static ENUM_COLOR_FORMAT discardAlpha(const ENUM_COLOR_FORMAT format)
	{
		switch(format)
		{
		case ENUM_COLOR_FORMAT_R5G5B5A1:
			return ENUM_COLOR_FORMAT_R5G6B5;
		case ENUM_COLOR_FORMAT_R4G4B4A4:
			return ENUM_COLOR_FORMAT_R5G6B5;
		case ENUM_COLOR_FORMAT_R8G8B8A8:
			return ENUM_COLOR_FORMAT_R8G8B8;
		case ENUM_COLOR_FORMAT_L8A8:
			return ENUM_COLOR_FORMAT_L8;
		case ENUM_COLOR_FORMAT_A8:	//TODO
			return ENUM_COLOR_FORMAT_R8G8B8;
		}
		return format;
	}
	
	class IImage : public virtual core::IReferencable{
	public:
		virtual const core::dimension2du& getDimension() const = 0;
		virtual ENUM_COLOR_FORMAT getColorFormat() const = 0;
		virtual SColor getPixel(u32 x, u32 y) const = 0;
		virtual u8 getValue(u32 x,u32 y) const = 0;
		
		virtual u32 getByteCountPerRow() const =0;
		virtual u32 getByteCountPerPixel() const =0;
		virtual u32 getImageDataSizeInPixels() const = 0;

		virtual void* lock() = 0;

		virtual void unlock() = 0;

		virtual void upsidedown() = 0;

		//! test if the color format is only viable for RenderTarget textures
		/** Since we don't have support for e.g. floating point IImage formats
		one should test if the color format can be used for arbitrary usage, or
		if it is restricted to RTTs. */
		static bool isRenderTargetOnlyFormat(const ENUM_COLOR_FORMAT format)
		{
			switch(format)
			{
			case ENUM_COLOR_FORMAT_R5G5B5A1:
			case ENUM_COLOR_FORMAT_R5G6B5:
			//TODO 不知是否支持
			//case ENUM_COLOR_FORMAT_R4G4B4A4:
			case ENUM_COLOR_FORMAT_R8G8B8:
			case ENUM_COLOR_FORMAT_R8G8B8A8:
			case ENUM_COLOR_FORMAT_A8:
			case ENUM_COLOR_FORMAT_L8:
			case ENUM_COLOR_FORMAT_L8A8:
				return false;
			default:
				return true;
			}
		}
	};
}//video
}//yon
#endif