#ifndef _YON_VIDEO_SMATERIALLAYER_H_
#define _YON_VIDEO_SMATERIALLAYER_H_

//#include "IMaterial.h"
#include "matrix4.h"
#include "yonAllocator.h"

namespace yon{
namespace video{

	enum ENUM_WRAP_MODE
	{
		ENUM_WRAP_MODE_REPEAT = 0x2901,
		ENUM_WRAP_MODE_CLAMP_TO_EDGE = 0x812F
	};

	//或者放到Engine层面更合适（变更是否需要重新生成纹理？）
	enum ENUM_FILTER_MODE
	{
		ENUM_FILTER_MODE_NEAREST = 0,	//最近点采样
		ENUM_FILTER_MODE_BILINEAR,		//双线性过滤
		ENUM_FILTER_MODE_TRILINEAR,		//三线性过滤
		ENUM_FILTER_MODE_ANISOTROPIC	//各向异性过滤
	};

	class ITexture;
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