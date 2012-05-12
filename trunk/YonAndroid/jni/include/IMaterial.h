#ifndef _YON_VIDEO_IMATERIAL_H_
#define _YON_VIDEO_IMATERIAL_H_

#include "IReferencable.h"
#include "ITexture.h"
#include "matrix4.h"

namespace yon{
namespace video{

	enum ENUM_MATERIAL_TYPE{
		ENUM_MATERIAL_TYPE_NONE = 0,
		ENUM_MATERIAL_TYPE_SOLID,
		ENUM_MATERIAL_TYPE_LIGHTEN,
		ENUM_MATERIAL_TYPE_TRANSPARENT,
		ENUM_MATERIAL_TYPE_TRANSPARENT_BLEND_COLOR,
		ENUM_MATERIAL_TYPE_MASK,
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
		ENUM_WRAP_MODE_REPEAT = 0,
		ENUM_WRAP_MODE_CLAMP_TO_EDGE,
		ENUM_WRAP_MODE_COUNT
	};

	enum ENUM_CULLING_MODE
	{
		ENUM_CULLING_MODE_FRONT =  0x0404,
		ENUM_CULLING_MODE_BACK =  0x0405,
		ENUM_CULLING_MODE_ALL =  0x0408
	};

	enum ENUM_FRONT_FACE
	{
		ENUM_FRONT_FACE_CW = 0x0900,
		ENUM_FRONT_FACE_CCW = 0x0901,
	};

	const static c8* MATERIAL_STATE_NAMES[]=
	{
		//"AlphaTest",
		//"Blend",
		"ColorMaterial",
		"CullFace",
		//"DepthTest",
		"Dither",
		"Fog",
		"Lighting",
		"LineSmooth",
		"Normalize",
		"RescaleNormal",
		"ScissorTest",
		"StencilTest"
	};

	enum ENUM_MATERIAL_STATE
	{
		//ENUM_MATERIAL_STATE_ALPHA_TEST = 0,
		//ENUM_MATERIAL_STATE_BLEND,
		ENUM_MATERIAL_STATE_COLOR_MATERIAL = 0,
		ENUM_MATERIAL_STATE_CULL_FACE,
		//ENUM_MATERIAL_STATE_DEPTH_TEST,
		ENUM_MATERIAL_STATE_DITHER,
		ENUM_MATERIAL_STATE_FOG,
		ENUM_MATERIAL_STATE_LIGHTING,
		ENUM_MATERIAL_STATE_LINESMOOTH,
		ENUM_MATERIAL_STATE_NORMALIZE,
		ENUM_MATERIAL_STATE_RESCALE_NORMALIZE,
		ENUM_MATERIAL_STATE_SCISSOR_TEST,
		ENUM_MATERIAL_STATE_STENCIL_TEST,
		ENUM_MATERIAL_STATE_COUNT
	};

	const static u32 MATERIAL_MAX_TEXTURES = YON_MATERIAL_MAX_TEXTURES;

	class IMaterial : public virtual core::IReferencable{
	public:

		struct State{
			State():
				//AlphaTest(false),
				//Blend(false),
				ColorMaterial(false),
				CullFace(false),
				//DepthTest(false),
				Dither(true),
				Fog(false),
				Lighting(false),
				LineSmooth(false),
				Normalize(false),
				RescaleNormal(false),
				ScissorTest(false),
				StencilTest(false){}

			//bool AlphaTest:1;
			//bool Blend:1;
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

			int operator ^(const State& other) const{
				return getInt()^other.getInt();
			}

			bool operator !=(const State& other) const{
				return ((*this)^other)!=0;
			}
			bool operator ==(const State& other) const{
				return !(*this!=other);
			}

			int getInt() const{
				int *a;
				a=reinterpret_cast<int*>((void*)(this));
				return *a;
			}
		};
		State states;

		virtual ENUM_MATERIAL_TYPE getMaterialType() const = 0;
		virtual void setMaterialType(ENUM_MATERIAL_TYPE type) = 0;

		virtual ENUM_POLYGON_MODE getPolygonMode() const = 0;
		virtual void setPolygonMode(ENUM_POLYGON_MODE mode) = 0;

		virtual void setTexture(u32 i, ITexture* tex) = 0;
		virtual ITexture* getTexture(u32 i) const = 0;

		void setState(ENUM_MATERIAL_STATE state,bool on){
			switch(state)
			{
			case ENUM_MATERIAL_STATE_COLOR_MATERIAL:
				states.ColorMaterial=on;
				break;
			case ENUM_MATERIAL_STATE_CULL_FACE:
				states.CullFace=on;
				break;
			//case ENUM_MATERIAL_STATE_DEPTH_TEST:
			//	states.DepthTest=on;
			//	break;
			case ENUM_MATERIAL_STATE_DITHER:
				states.Dither=on;
				break;
			case ENUM_MATERIAL_STATE_FOG:
				states.Fog=on;
				break;
			case ENUM_MATERIAL_STATE_LIGHTING:
				states.Lighting=on;
				break;
			case ENUM_MATERIAL_STATE_LINESMOOTH:
				states.LineSmooth=on;
				break;
			case ENUM_MATERIAL_STATE_NORMALIZE:
				states.Normalize=on;
				break;
			case ENUM_MATERIAL_STATE_RESCALE_NORMALIZE:
				states.RescaleNormal=on;
				break;
			case ENUM_MATERIAL_STATE_SCISSOR_TEST:
				states.ScissorTest=on;
				break;
			case ENUM_MATERIAL_STATE_STENCIL_TEST:
				states.StencilTest=on;
				break;
			}
		}
		bool getState(ENUM_MATERIAL_STATE state) const{
			switch(state)
			{
			case ENUM_MATERIAL_STATE_COLOR_MATERIAL:
				return states.ColorMaterial;
			case ENUM_MATERIAL_STATE_CULL_FACE:
				return states.CullFace;
			//case ENUM_MATERIAL_STATE_DEPTH_TEST:
			//	return states.DepthTest;
			case ENUM_MATERIAL_STATE_DITHER:
				return states.Dither;
			case ENUM_MATERIAL_STATE_FOG:
				return states.Fog;
			case ENUM_MATERIAL_STATE_LIGHTING:
				return states.Lighting;
			case ENUM_MATERIAL_STATE_LINESMOOTH:
				return states.LineSmooth;
			case ENUM_MATERIAL_STATE_NORMALIZE:
				return states.Normalize;
			case ENUM_MATERIAL_STATE_RESCALE_NORMALIZE:
				return states.RescaleNormal;
			case ENUM_MATERIAL_STATE_SCISSOR_TEST:
				return states.ScissorTest;
			case ENUM_MATERIAL_STATE_STENCIL_TEST:
				return states.StencilTest;
			}
			return true;
		}

		virtual void setCullingMode(ENUM_CULLING_MODE mode) = 0;
		virtual ENUM_CULLING_MODE getCullingMode() const = 0;

		virtual void setFrontFace(ENUM_FRONT_FACE f) = 0;
		virtual ENUM_FRONT_FACE getFrontFace() const = 0;

		virtual ENUM_WRAP_MODE getWrapModeU(u32 index) const = 0;
		virtual void setWrapModeU(u32 index,ENUM_WRAP_MODE mode) = 0;
		virtual ENUM_WRAP_MODE getWrapModeV(u32 index) const = 0;
		virtual void setWrapModeV(u32 index,ENUM_WRAP_MODE mode) = 0;

		virtual void setTextureMatrix(u32 index, const core::matrix4f& mat) = 0;
		virtual core::matrix4f& getTextureMatrix(u32 index) = 0;
	};

	YON_API extern IMaterial* DEFAULT_MATERIAL;
	//YON_API extern IMaterial* DEFAULT_3D_MATERIAL;
	//YON_API extern IMaterial* DEFAULT_2D_MATERIAL;
	YON_API extern IMaterial* MYGUI_MATERIAL;
}
}
#endif