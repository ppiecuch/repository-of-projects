#ifndef _YON_VIDEO_CCOLORCONVERTER_H_
#define _YON_VIDEO_CCOLORCONVERTER_H_

#include "SColor.h"

namespace yon{
namespace video{

	class CColorConverter{
	public:
		//! functions for converting one image format to another efficiently
		//! and hopefully correctly.
		//!
		//! \param sP pointer to source pixel data
		//! \param sN number of source pixels to copy
		//! \param dP pointer to destination data buffer. must be big enough
		//! to hold sN pixels in the output format.
		static void convert_A8B8G8R8toR5G5B5A1(const void* sP, s32 sN, void* dP);
		static void convert_A8B8G8R8toR4G4B4A4(const void* sP, s32 sN, void* dP);
		static void convert_R8G8B8toR5G6B5(const void* sP, s32 sN, void* dP);

	};
}
}
#endif