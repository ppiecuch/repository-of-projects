#ifndef _YON_VIDEO_SCOLOR_H_
#define _YON_VIDEO_SCOLOR_H_

#include "yonMath.h"

namespace yon{
namespace video{

	inline u32 toRGB(s32 r,s32 g,s32 b)
	{
		return ((u32)(((u8)(r)|((u16)((u8)(g))<<8))|(((u16)(u8)(b))<<16)));
	}


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

	//! Converts a 32bit (A8B8G8R8) color to a 16bit A1B5G5R5 color
	inline u16 A8B8G8R8toR5G5B5A1(u32 color)
	{
		return (u16)(( color & 0x80000000) >> 31|
			( color & 0x00F80000) >> 18 |
			( color & 0x0000F800) >> 5 |
			( color & 0x000000F8) << 8);
	}

	inline u16 A8B8G8R8toR4G4B4A4(u32 color)
	{
		return (u16)(( color & 0xF0000000) >> 28|
			( color & 0x00F00000) >> 16 |
			( color & 0x0000F000) >> 4 |
			( color & 0x000000F0) << 8);
	}

	//! Converts a 32bit (X8R8G8B8) color to a 16bit A1R5G5B5 color
	inline u16 X8R8G8B8toA1R5G5B5(u32 color)
	{
		return (u16)(0x8000 |
			( color & 0x00F80000) >> 9 |
			( color & 0x0000F800) >> 6 |
			( color & 0x000000F8) >> 3);
	}

	

	//改为RGBA-->错了,OpenGL中顶点颜色使用的就是ABGR
	//使用ABGR
	class SColor
	{
	public:

		//! Constructor of the Color. Does nothing.
		/** The color value is not initialized to save time. */
		//SColor() {}

		SColor()
			:color(0){}

		//[0,255]
		explicit SColor (u32 a, u32 r, u32 g, u32 b)
			:color(((a & 0xff)<<24) | ((b & 0xff)<<16) | ((g & 0xff)<<8) | (r & 0xff)) {}

		explicit SColor(f32 a, f32 r, f32 g, f32 b)
		{
				u32 aa=(u32)core::clamp((s32)(a*255),0,255);
				u32 rr=(u32)core::clamp((s32)(r*255),0,255);
				u32 gg=(u32)core::clamp((s32)(g*255),0,255);
				u32 bb=(u32)core::clamp((s32)(b*255),0,255);
				color=((aa & 0xff)<<24) | ((bb & 0xff)<<16) | ((gg & 0xff)<<8) | (rr & 0xff);
		}

		SColor(u32 clr)
			: color(clr) {}

		inline bool operator!=(const SColor& other) const{
			return color!=other.color;
		}

		inline bool operator==(const SColor& other) const{return !(*this!=other);}

		//TODO调整后再返回
		u32 toA8R8G8B8() const { return color; }

		u32 getAlpha() const { return color>>24; }
		u32 getBlue() const { return (color>>16) & 0xff; }
		u32 getGreen() const { return (color>>8) & 0xff; }
		u32 getRed() const { return color & 0xff; }

		void setAlpha(u32 a) { color = ((a & 0xff)<<24) | (color & 0x00ffffff); }
		void setBlue(u32 r) { color = ((r & 0xff)<<16) | (color & 0xff00ffff); }
		void setGreen(u32 g) { color = ((g & 0xff)<<8) | (color & 0xffff00ff); }
		void setRed(u32 b) { color = (b & 0xff) | (color & 0xffffff00); }

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
			color = (((a & 0xff)<<24) | ((b & 0xff)<<16) | ((g & 0xff)<<8) | (r & 0xff));
		}
		void set(u32 col) { color = col; }

		//! color in A8R8G8B8 Format
		u32 color;

	};

	YON_API extern const SColor COLOR_BLACK;
	YON_API extern const SColor COLOR_WHITE;
	YON_API extern const SColor COLOR_RED;
	YON_API extern const SColor COLOR_GREEN;
	YON_API extern const SColor COLOR_BLUE;
	YON_API extern const SColor COLOR_ZERO;
	YON_API extern const SColor COLOR_DEFAULT;

	class SColorf
	{
	public:
		//! Default constructor for SColorf.
		/** Sets red, green and blue to 0.0f and alpha to 1.0f. */
		SColorf() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}

		//! Constructs a color from up to four color values: red, green, blue, and alpha.
		/** \param r: Red color component. Should be a value between
		0.0f meaning no red and 1.0f, meaning full red.
		\param g: Green color component. Should be a value between 0.0f
		meaning no green and 1.0f, meaning full green.
		\param b: Blue color component. Should be a value between 0.0f
		meaning no blue and 1.0f, meaning full blue.
		\param a: Alpha color component of the color. The alpha
		component defines how transparent a color should be. Has to be
		a value between 0.0f and 1.0f, 1.0f means not transparent
		(opaque), 0.0f means fully transparent. */
		SColorf(f32 r, f32 g, f32 b, f32 a = 1.0f) : r(r), g(g), b(b), a(a) {}

		//! Constructs a color from 32 bit Color.
		/** \param c: 32 bit color from which this SColorf class is
		constructed from. */
		SColorf(SColor c)
		{
			const f32 inv = 1.0f / 255.0f;
			r = c.getRed() * inv;
			g = c.getGreen() * inv;
			b = c.getBlue() * inv;
			a = c.getAlpha() * inv;
		}

		//! Converts this color to a SColor without floats.
		SColor toSColor() const
		{
			return SColor((u32)core::round32(a*255.0f), (u32)core::round32(r*255.0f), (u32)core::round32(g*255.0f), (u32)core::round32(b*255.0f));
		}

		//! Sets three color components to new values at once.
		/** \param rr: Red color component. Should be a value between 0.0f meaning
		no red (=black) and 1.0f, meaning full red.
		\param gg: Green color component. Should be a value between 0.0f meaning
		no green (=black) and 1.0f, meaning full green.
		\param bb: Blue color component. Should be a value between 0.0f meaning
		no blue (=black) and 1.0f, meaning full blue. */
		void set(f32 rr, f32 gg, f32 bb) {r = rr; g =gg; b = bb; }

		//! Sets all four color components to new values at once.
		/** \param aa: Alpha component. Should be a value between 0.0f meaning
		fully transparent and 1.0f, meaning opaque.
		\param rr: Red color component. Should be a value between 0.0f meaning
		no red and 1.0f, meaning full red.
		\param gg: Green color component. Should be a value between 0.0f meaning
		no green and 1.0f, meaning full green.
		\param bb: Blue color component. Should be a value between 0.0f meaning
		no blue and 1.0f, meaning full blue. */
		void set(f32 aa, f32 rr, f32 gg, f32 bb) {a = aa; r = rr; g =gg; b = bb; }

		//! Sets a color component by index. R=0, G=1, B=2, A=3
		void setColorComponentValue(s32 index, f32 value)
		{
			switch(index)
			{
			case 0: r = value; break;
			case 1: g = value; break;
			case 2: b = value; break;
			case 3: a = value; break;
			}
		}

		//! Returns the alpha component of the color in the range 0.0 (transparent) to 1.0 (opaque)
		f32 getAlpha() const { return a; }

		//! Returns the red component of the color in the range 0.0 to 1.0
		f32 getRed() const { return r; }

		//! Returns the green component of the color in the range 0.0 to 1.0
		f32 getGreen() const { return g; }

		//! Returns the blue component of the color in the range 0.0 to 1.0
		f32 getBlue() const { return b; }

		//! red color component
		f32 r;

		//! green color component
		f32 g;

		//! blue component
		f32 b;

		//! alpha color component
		f32 a;
	};
}
}
#endif