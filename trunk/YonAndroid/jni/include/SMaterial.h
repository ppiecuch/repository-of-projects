#ifndef _YON_VIDEO_SMATERIAL_H_
#define _YON_VIDEO_SMATERIAL_H_

#include "SMaterialLayer.h"

namespace yon{
namespace video{

	enum ENUM_BLEND_FACTOR{
		ENUM_BLEND_FACTOR_ZERO	= 0,				//!< src & dest	(0, 0, 0, 0)
		ENUM_BLEND_FACTOR_ONE,						//!< src & dest	(1, 1, 1, 1)
		ENUM_BLEND_FACTOR_DST_COLOR, 				//!< src (destR, destG, destB, destA)
		ENUM_BLEND_FACTOR_ONE_MINUS_DST_COLOR, 		//!< src (1-destR, 1-destG, 1-destB, 1-destA)
		ENUM_BLEND_FACTOR_SRC_COLOR,				//!< dest (srcR, srcG, srcB, srcA)
		ENUM_BLEND_FACTOR_ONE_MINUS_SRC_COLOR, 		//!< dest (1-srcR, 1-srcG, 1-srcB, 1-srcA)
		ENUM_BLEND_FACTOR_SRC_ALPHA,				//!< src & dest	(srcA, srcA, srcA, srcA)
		ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,		//!< src & dest	(1-srcA, 1-srcA, 1-srcA, 1-srcA)
		ENUM_BLEND_FACTOR_DST_ALPHA,				//!< src & dest	(destA, destA, destA, destA)
		ENUM_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,		//!< src & dest	(1-destA, 1-destA, 1-destA, 1-destA)
		ENUM_BLEND_FACTOR_SRC_ALPHA_SATURATE,		//!< src (min(srcA, 1-destA), idem, ...)
		ENUM_BLEND_FACTOR_COUNT
	};

	enum ENUM_MODULATE
	{
		ENUM_MODULATE_1X	= 1,
		ENUM_MODULATE_2X	= 2,
		ENUM_MODULATE_4X	= 4
	};

	enum ENUM_ALPHA_SOURCE
	{
		ENUM_ALPHA_SOURCE_NONE = 0,
		ENUM_ALPHA_SOURCE_VERTEX,
		ENUM_ALPHA_SOURCE_TEXTURE
	};

	enum ENUM_MATERIAL_TYPE{
		ENUM_MATERIAL_TYPE_SOLID = 0,
		ENUM_MATERIAL_TYPE_BLEND,
		//ENUM_MATERIAL_TYPE_LIGHTEN,
		ENUM_MATERIAL_TYPE_TRANSPARENT,
		ENUM_MATERIAL_TYPE_TRANSPARENT_REF,
		//ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR,
		ENUM_MATERIAL_TYPE_MASK,
		ENUM_MATERIAL_TYPE_DETAIL_MAP,
		ENUM_MATERIAL_TYPE_COMPOSITE1,
		ENUM_MATERIAL_TYPE_COMPOSITE2,
		ENUM_MATERIAL_TYPE_COUNT
	};

	enum ENUM_POLYGON_MODE{
		ENUM_POLYGON_MODE_FILL = 0,
		ENUM_POLYGON_MODE_LINE,
		ENUM_POLYGON_MODE_POINT,
		ENUM_POLYGON_MODE_COUNT
	};

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

	enum ENUM_CULLING_MODE
	{
		ENUM_CULLING_MODE_NONE =  0,
		ENUM_CULLING_MODE_FRONT =  0x0404,
		ENUM_CULLING_MODE_BACK =  0x0405,
		ENUM_CULLING_MODE_ALL =  0x0408
	};

	enum ENUM_FRONT_FACE
	{
		ENUM_FRONT_FACE_CW = 0x0900,
		ENUM_FRONT_FACE_CCW = 0x0901,
	};

	enum ENUM_COLOR_MASK
	{
		ENUM_COLOR_MASK_NONE=0,
		ENUM_COLOR_MASK_RED=1,
		ENUM_COLOR_MASK_GREEN=2,
		ENUM_COLOR_MASK_BLUE=4,
		ENUM_COLOR_MASK_ALPHA=8,
		ENUM_COLOR_MASK_RGB=7,
		ENUM_COLOR_MASK_RGBA=15
	};


	/*const static c8* MATERIAL_STATE_NAMES[]=
	{
		//"AlphaTest",
		//"Blend",
		"ColorMaterial",
		//"CullFace",
		//"DepthTest",
		"Dither",
		"Fog",
		"Lighting",
		"LineSmooth",
		"Normalize",
		//"RescaleNormal",
		"ScissorTest",
		"StencilTest"
	};*/

	const static u32 MATERIAL_MAX_TEXTURES = YON_MATERIAL_MAX_TEXTURES;

	inline bool blendFactorHasAlpha(const ENUM_BLEND_FACTOR factor)
	{
		switch (factor)
		{
		case ENUM_BLEND_FACTOR_SRC_ALPHA:
		case ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
		case ENUM_BLEND_FACTOR_DST_ALPHA:
		case ENUM_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:
		case ENUM_BLEND_FACTOR_SRC_ALPHA_SATURATE:
			return true;
		default:
			return false;
		}
	}

	inline bool isTransparent(ENUM_MATERIAL_TYPE type)
	{
		switch(type)
		{
		case ENUM_MATERIAL_TYPE_BLEND:
		case ENUM_MATERIAL_TYPE_TRANSPARENT:
			//case ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR:
		case ENUM_MATERIAL_TYPE_MASK:
		case ENUM_MATERIAL_TYPE_COMPOSITE2:
			return true;
		default:
			return false;
		}
	}

	struct SMaterial{

		bool ColorMaterial:1;
		//bool CullFace:1;
		//bool DepthTest:1;
		bool Dither:1;
		bool Fog:1;
		bool Lighting:1;
		bool LineSmooth:1;
		bool Normalize:1;
		bool RescaleNormal:1;
		bool ScissorTest:1;
		bool StencilTest:1;
		bool GouraudShading:1;

		ENUM_MATERIAL_TYPE MaterialType;
		//ENUM_CMP_FUNC ZBuffer;
		ENUM_POLYGON_MODE PolygonMode;
		ENUM_CULLING_MODE CullingMode;
		ENUM_FRONT_FACE FrontFace;
		ENUM_BLEND_FACTOR BlendSrc,BlendDst;
		ENUM_MODULATE Modulate;
		ENUM_ALPHA_SOURCE AlphaSource;
		ENUM_COLOR_MASK ColorMask;
		SMaterialLayer TextureLayers[MATERIAL_MAX_TEXTURES];

		SMaterial()
			:MaterialType(ENUM_MATERIAL_TYPE_SOLID),PolygonMode(ENUM_POLYGON_MODE_FILL),
			CullingMode(ENUM_CULLING_MODE_BACK),FrontFace(ENUM_FRONT_FACE_CCW),
			BlendSrc(ENUM_BLEND_FACTOR_SRC_ALPHA),BlendDst(ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA),
			Modulate(ENUM_MODULATE_1X),AlphaSource(ENUM_ALPHA_SOURCE_NONE),ColorMask(ENUM_COLOR_MASK_RGBA),
			ColorMaterial(false),
			//CullFace(true),
			//DepthTest(false),
			Dither(true),
			Fog(false),
			Lighting(false),
			LineSmooth(false),
			Normalize(false),
			//RescaleNormal(false),
			ScissorTest(false),
			StencilTest(false),
			GouraudShading(true){
		}

		SMaterial(ENUM_MATERIAL_TYPE materialType)
			:MaterialType(materialType),PolygonMode(ENUM_POLYGON_MODE_FILL),
			CullingMode(ENUM_CULLING_MODE_BACK),FrontFace(ENUM_FRONT_FACE_CCW),
			BlendSrc(ENUM_BLEND_FACTOR_SRC_ALPHA),BlendDst(ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA),
			Modulate(ENUM_MODULATE_1X),AlphaSource(ENUM_ALPHA_SOURCE_NONE),ColorMask(ENUM_COLOR_MASK_RGBA),
			ColorMaterial(false),
			//CullFace(true),
			//DepthTest(false),
			Dither(true),
			Fog(false),
			Lighting(false),
			LineSmooth(false),
			Normalize(false),
			//RescaleNormal(false),
			ScissorTest(false),
			StencilTest(false),
			GouraudShading(true){
		}

		void setTexture(u32 index, ITexture* tex)
		{
			if (index>=MATERIAL_MAX_TEXTURES)
				return;
			TextureLayers[index].texture = tex;
		}
		ITexture* getTexture(u32 index) const
		{
			return index < MATERIAL_MAX_TEXTURES ? TextureLayers[index].texture : 0;
		}
		core::matrix4f& getTextureMatrix(u32 index)
		{
			return TextureLayers[index].getTextureMatrix();
		}
		const core::matrix4f& getTextureMatrix(u32 index) const
		{
			if (index<MATERIAL_MAX_TEXTURES)
				return TextureLayers[index].getTextureMatrix();
			else
				return core::IDENTITY_MATRIX;
		}
		void setTextureMatrix(u32 index, const core::matrix4f& mat)
		{
			if (index>=MATERIAL_MAX_TEXTURES)
				return;
			TextureLayers[index].setTextureMatrix(mat);
		}
		ENUM_WRAP_MODE getWrapModeU(u32 index) const{
			return TextureLayers[index].wrapU;
		}
		void setWrapModeU(u32 index,ENUM_WRAP_MODE mode){
			TextureLayers[index].wrapU=mode;
		}
		ENUM_WRAP_MODE getWrapModeV(u32 index) const{
			return TextureLayers[index].wrapV;
		}
		void setWrapModeV(u32 index,ENUM_WRAP_MODE mode){
			TextureLayers[index].wrapV=mode;
		}

		ENUM_FILTER_MODE getFilterMode(u32 index) const{
			return TextureLayers[index].filter;
		}
		void setFilterMode(u32 index,ENUM_FILTER_MODE mode){
			TextureLayers[index].filter=mode;
		}

		inline bool operator!=(const SMaterial& b) const
		{
			bool different =
				MaterialType != b.MaterialType ||
				PolygonMode != b.PolygonMode ||
				CullingMode != b.CullingMode ||
				FrontFace != b.FrontFace ||
				BlendSrc != b.BlendSrc ||
				BlendDst != b.BlendDst ||
				Modulate != b.Modulate ||
				AlphaSource != b.AlphaSource ||
				ColorMaterial != b.ColorMaterial ||
				Dither != b.Dither ||
				Fog != b.Fog ||
				Lighting != b.Lighting ||
				LineSmooth != b.LineSmooth ||
				Normalize != b.Normalize ||
				ScissorTest != b.ScissorTest ||
				StencilTest != b.StencilTest ||
				ColorMask != b.ColorMask ||
				GouraudShading != b.GouraudShading;
			for (u32 i=0; (i<MATERIAL_MAX_TEXTURES) && !different; ++i)
			{
				different |= (TextureLayers[i] != b.TextureLayers[i]);
			}
			return different;
		}

		inline bool operator==(const SMaterial& b) const
		{ return !(b!=*this); }

		SMaterial& operator=(const SMaterial& other)
		{
			// Check for self-assignment!
			if (this == &other)
				return *this;

			MaterialType = other.MaterialType;
			PolygonMode = other.PolygonMode;
			CullingMode = other.CullingMode;
			FrontFace = other.FrontFace;
			BlendSrc = other.BlendSrc;
			BlendDst = other.BlendDst;
			Modulate = other.Modulate;
			AlphaSource = other.AlphaSource;
			for (u32 i=0; i<MATERIAL_MAX_TEXTURES; ++i)
			{
				TextureLayers[i] = other.TextureLayers[i];
			}
			ColorMaterial = other.ColorMaterial;
			Dither = other.Dither;
			Fog = other.Fog;
			Lighting = other.Lighting;
			LineSmooth = other.LineSmooth;
			Normalize = other.Normalize;
			ScissorTest = other.ScissorTest;
			StencilTest = other.StencilTest;
			ColorMask = other.ColorMask;
			GouraudShading = other.GouraudShading;
			return *this;
		}
	};

	YON_API extern SMaterial DEFAULT_MATERIAL;
	YON_API extern SMaterial MYGUI_MATERIAL;
}
}
#endif