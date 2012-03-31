#ifndef _YON_VIDEO_SCOLOR_H_
#define _YON_VIDEO_SCOLOR_H_

#include "yonMath.h"

namespace yon{
namespace video{


	//! Convert A1R5G5B5 color to A8R8G8B8 color
	/** build a nicer 32bit Color by extending dest lower bits with source high bits. */
	inline u32 A1R5G5B5toA8R8G8B8(u16 color)
	{
		return ( (( -( (s32) color & 0x00008000 ) >> (s32) 31 ) & 0xFF000000 ) |
			(( color & 0x00007C00 ) << 9) | (( color & 0x00007000 ) << 4) |
			(( color & 0x000003E0 ) << 6) | (( color & 0x00000380 ) << 1) |
			(( color & 0x0000001F ) << 3) | (( color & 0x0000001C ) >> 2)
			);
	}

	//! Convert R5G5B5A1 color to R8G8B8A8 color
	inline u32 R5G5B5A1toR8G8B8A8(u16 color)
	{
		//TODO
		return 0;
	}


	//! Returns R5G6B5 color to A8R8G8B8 color
	inline u32 R5G6B5toA8R8G8B8(u16 color)
	{
		return 0xFF000000 |
			((color & 0xF800) << 8)|
			((color & 0x07E0) << 5)|
			((color & 0x001F) << 3);
	}

	//! Returns R5G6B5 color to R8G8B8A8 color
	inline u32 R5G6B5toR8G8B8A8(u16 color)
	{
		//TODO
		return 0;
	}

	

	//TODO改为RGBA-->错了,OpenGL中顶点颜色使用的就是ABGR
	class SColor
	{
	public:

		//! Constructor of the Color. Does nothing.
		/** The color value is not initialized to save time. */
		//SColor() {}

		SColor()
			:color(0){}

		//[0,255]
		SColor (u32 a, u32 r, u32 g, u32 b)
			:color(((a & 0xff)<<24) | ((r & 0xff)<<16) | ((g & 0xff)<<8) | (b & 0xff)) {}

		SColor(u32 clr)
			: color(clr) {}

		u32 getAlpha() const { return color>>24; }
		u32 getRed() const { return (color>>16) & 0xff; }
		u32 getGreen() const { return (color>>8) & 0xff; }
		u32 getBlue() const { return color & 0xff; }

		void setAlpha(u32 a) { color = ((a & 0xff)<<24) | (color & 0x00ffffff); }
		void setRed(u32 r) { color = ((r & 0xff)<<16) | (color & 0xff00ffff); }
		void setGreen(u32 g) { color = ((g & 0xff)<<8) | (color & 0xffff00ff); }
		void setBlue(u32 b) { color = (b & 0xff) | (color & 0xffffff00); }

		//[0,255]
		f32 getLightness() const
		{
			return 0.5f*(core::max_(core::max_(getRed(),getGreen()),getBlue())+core::min_(core::min_(getRed(),getGreen()),getBlue()));
		}

		// [0,255].
		f32 getLuminance() const
		{
			return 0.3f*getRed() + 0.59f*getGreen() + 0.11f*getBlue();
		}

		u32 getAverage() const
		{
			return ( getRed() + getGreen() + getBlue() ) / 3;
		}

		void set(u32 a, u32 r, u32 g, u32 b)
		{
			color = (((a & 0xff)<<24) | ((r & 0xff)<<16) | ((g & 0xff)<<8) | (b & 0xff));
		}
		void set(u32 col) { color = col; }

		//! color in A8R8G8B8 Format
		u32 color;

	};

	YON_API extern const SColor COLOR_BLACK;
	YON_API extern const SColor COLOR_WHITE;
}
}
#endif