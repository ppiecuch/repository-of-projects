#ifndef _YON_VIDEO_SMATERIALLAYER_H_
#define _YON_VIDEO_SMATERIALLAYER_H_

#include "IMaterial.h"

namespace yon{
namespace video{

	struct SMaterialLayer{
		ITexture* texture;
		core::matrix4f textureMatrix;
		ENUM_FILTER_MODE filter;
		ENUM_WRAP_MODE wrapU,wrapV;

		SMaterialLayer()
			:texture(NULL),
			wrapU(ENUM_WRAP_MODE_REPEAT),
			wrapV(ENUM_WRAP_MODE_REPEAT),
			filter(ENUM_FILTER_MODE_BILINEAR),
			textureMatrix(true)
		{}
		SMaterialLayer(const SMaterialLayer& other)
		{
			*this = other;
		}
		SMaterialLayer& operator=(const SMaterialLayer& other)
		{
			// Check for self-assignment!
			if (this == &other)
				return *this;

			texture = other.texture;
			wrapU = other.wrapU;
			wrapV = other.wrapV;
			filter = other.filter;
			return *this;
		}
		inline bool operator!=(const SMaterialLayer& other) const
		{
			bool different =
				texture != other.texture ||
				wrapU != other.wrapU ||
				wrapV != other.wrapV ||
				filter != other.filter;
			if (different)
				return true;
			return false;
		}

		inline bool operator==(const SMaterialLayer& other) const
		{ return !(other!=*this); }
	};
}
}
#endif