#ifndef _YON_VIDEO_SMATERIALLAYER_H_
#define _YON_VIDEO_SMATERIALLAYER_H_

//#include "IMaterial.h"
#include "yonAllocator.h"

namespace yon{
namespace video{

	struct SMaterial;

	struct SMaterialLayer{
	private:
		friend struct SMaterial;
		core::yonAllocator<core::matrix4f> MatrixAllocator;
		core::matrix4f* pTextureMatrix;
	public:
		ITexture* texture;
		ENUM_FILTER_MODE filter;
		ENUM_WRAP_MODE wrapU,wrapV;

		SMaterialLayer()
			:texture(NULL),
			wrapU(ENUM_WRAP_MODE_REPEAT),
			wrapV(ENUM_WRAP_MODE_REPEAT),
			filter(ENUM_FILTER_MODE_BILINEAR),
			pTextureMatrix(NULL)
		{}
		SMaterialLayer(const SMaterialLayer& other)
		{
			pTextureMatrix=NULL;
			*this = other;
		}
		~SMaterialLayer()
		{
			MatrixAllocator.destruct(pTextureMatrix);
			MatrixAllocator.deallocate(pTextureMatrix); 
		}
		core::matrix4f& getTextureMatrix()
		{
			if (!pTextureMatrix)
			{
				pTextureMatrix = MatrixAllocator.allocate(1);
				MatrixAllocator.construct(pTextureMatrix,core::IDENTITY_MATRIX);
			}
			return *pTextureMatrix;
		}
		const core::matrix4f& getTextureMatrix() const
		{
			if (pTextureMatrix)
				return *pTextureMatrix;
			else
				return core::IDENTITY_MATRIX;
		}
		void setTextureMatrix(const core::matrix4f& mat)
		{
			if (!pTextureMatrix)
			{
				pTextureMatrix = MatrixAllocator.allocate(1);
				MatrixAllocator.construct(pTextureMatrix,mat);
			}
			else
				*pTextureMatrix = mat;
		}
		SMaterialLayer& operator=(const SMaterialLayer& other)
		{
			// Check for self-assignment!
			if (this == &other)
				return *this;

			if (pTextureMatrix)
			{
				if (other.pTextureMatrix)
					*pTextureMatrix = *other.pTextureMatrix;
				else
				{
					MatrixAllocator.destruct(pTextureMatrix);
					MatrixAllocator.deallocate(pTextureMatrix); 
					pTextureMatrix = NULL;
				}
			}
			else
			{
				if (other.pTextureMatrix)
				{
					pTextureMatrix = MatrixAllocator.allocate(1);
					MatrixAllocator.construct(pTextureMatrix,*other.pTextureMatrix);
				}
				else
					pTextureMatrix = NULL;
			}

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
			else
				different |= (pTextureMatrix != other.pTextureMatrix) &&pTextureMatrix && other.pTextureMatrix &&(*pTextureMatrix != *(other.pTextureMatrix));
			return different;
		}

		inline bool operator==(const SMaterialLayer& other) const
		{ return !(other!=*this); }
	};
}
}
#endif