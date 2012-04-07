#ifndef _YON_SCENE_SVERTEX_H_
#define _YON_SCENE_SVERTEX_H_

#include "yonTypes.h"
#include "vector3d.h"
#include "vector2d.h"
#include "SColor.h"

namespace yon{
	namespace scene{
		
		enum ENUM_VERTEX_TYPE{
			ENUM_VERTEX_TYPE_3V1T1C = 0,
			ENUM_VERTEX_TYPE_2V1T1C,
			ENUM_VERTEX_TYPE_3V2T1C
		};

		struct SVertex{
			SVertex(){}
			SVertex(f32 x,f32 y,f32 z,f32 tu, f32 tv,video::SColor c):pos(x,y,z),texcoords(tu,tv),color(c){}
			//SVertex(f32 x,f32 y,f32 z,f32 tu, f32 tv):pos(x,y,z),texcoords(tu,tv){}

			core::vector3df pos;
			core::vector2df texcoords;
			video::SColor color;
			
			ENUM_VERTEX_TYPE getType() const{
				return ENUM_VERTEX_TYPE_3V1T1C;
			}

		};

		struct S2DVertex{
			S2DVertex(){}
			S2DVertex(f32 x,f32 y,f32 tu, f32 tv,video::SColor c):pos(x,y),texcoords(tu,tv),color(c){}

			core::vector2df pos;
			core::vector2df texcoords;
			video::SColor color;

			ENUM_VERTEX_TYPE getType() const{
				return ENUM_VERTEX_TYPE_2V1T1C;
			}
		};

		struct SVertex2TCoords{
			SVertex2TCoords(){}
			SVertex2TCoords(f32 x,f32 y,f32 z,f32 tu0, f32 tv0,f32 tu1, f32 tv1,video::SColor c):pos(x,y,z),texcoords0(tu0,tv0),texcoords1(tu1,tv1),color(c){}

			core::vector3df pos;
			core::vector2df texcoords0;
			core::vector2df texcoords1;
			video::SColor color;

			ENUM_VERTEX_TYPE getType() const{
				return ENUM_VERTEX_TYPE_3V2T1C;
			}
		};
	}//scene
}//yon
#endif