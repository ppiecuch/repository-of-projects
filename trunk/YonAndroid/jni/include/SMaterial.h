#ifndef _YON_VIDEO_SMATERIAL_H_
#define _YON_VIDEO_SMATERIAL_H_

namespace yon{
namespace video{

	struct SMaterial{
		bool ColorMaterial:1;
		bool CullFace:1;
		//bool DepthTest:1;
		bool Dither:1;
		bool Fog:1;
		bool Lighting:1;
		bool LineSmooth:1;
		bool Normalize:1;
		bool RescaleNormal:1;
		bool ScissorTest:1;
		bool StencilTest:1;

		ENUM_MATERIAL_TYPE MaterialType;
		ENUM_POLYGON_MODE PolygonMode;
		ENUM_CULLING_MODE CullingMode;
		ENUM_FRONT_FACE FrontFace;
		ENUM_BLEND_FACTOR BlendSrc,BlendDst;
		ENUM_MODULATE Modulate;
		ENUM_ALPHA_SOURCE AlphaSource;
		SMaterialLayer TextureLayers[MATERIAL_MAX_TEXTURES];

		SMaterial()
			:MaterialType(ENUM_MATERIAL_TYPE_SOLID),PolygonMode(ENUM_POLYGON_MODE_FILL),
			CullingMode(ENUM_CULLING_MODE_BACK),FrontFace(ENUM_FRONT_FACE_CCW),
			BlendSrc(ENUM_BLEND_FACTOR_SRC_ALPHA),BlendDst(ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA),
			Modulate(ENUM_MODULATE_1X),AlphaSource(ENUM_ALPHA_SOURCE_NONE),
			ColorMaterial(false),
			CullFace(true),
			//DepthTest(false),
			Dither(true),
			Fog(false),
			Lighting(false),
			LineSmooth(false),
			Normalize(false),
			RescaleNormal(false),
			ScissorTest(false),
			StencilTest(false){
		}

		SMaterial(ENUM_MATERIAL_TYPE materialType)
			:MaterialType(materialType),PolygonMode(ENUM_POLYGON_MODE_FILL),
			CullingMode(ENUM_CULLING_MODE_BACK),FrontFace(ENUM_FRONT_FACE_CCW),
			BlendSrc(ENUM_BLEND_FACTOR_SRC_ALPHA),BlendDst(ENUM_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA),
			Modulate(ENUM_MODULATE_1X),AlphaSource(ENUM_ALPHA_SOURCE_NONE),
			ColorMaterial(false),
			CullFace(true),
			//DepthTest(false),
			Dither(true),
			Fog(false),
			Lighting(false),
			LineSmooth(false),
			Normalize(false),
			RescaleNormal(false),
			ScissorTest(false),
			StencilTest(false){
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
				CullFace != b.CullFace ||
				Dither != b.Dither ||
				Fog != b.Fog ||
				Lighting != b.Lighting ||
				LineSmooth != b.LineSmooth ||
				Normalize != b.Normalize ||
				RescaleNormal != b.RescaleNormal ||
				ScissorTest != b.ScissorTest ||
				StencilTest != b.StencilTest;
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
			CullFace = other.CullFace;
			Dither = other.Dither;
			Fog = other.Fog;
			Lighting = other.Lighting;
			LineSmooth = other.LineSmooth;
			Normalize = other.Normalize;
			RescaleNormal = other.RescaleNormal;
			ScissorTest = other.ScissorTest;
			StencilTest = other.StencilTest;
			return *this;
		}

	};

	YON_API extern SMaterial DEFAULT_MATERIAL;
	YON_API extern SMaterial MYGUI_MATERIAL;
}
}
#endif