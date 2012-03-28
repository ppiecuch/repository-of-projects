#ifndef _YON_SCENE_SVERTEX_H_
#define _YON_SCENE_SVERTEX_H_

#include "yonTypes.h"
#include "vector3d.h"
#include "vector2d.h"

namespace yon{
	namespace scene{
		
		//TODO
		//enum ENUM_VERTEX_TYPE{
		//};

		struct SVertex{
			SVertex(){}
			SVertex(f32 x,f32 y,f32 z,f32 tu, f32 tv):pos(x,y,z),texcoords(tu,tv){}

			core::vector3df pos;
			core::vector2df texcoords;
		};
	}//scene
}//yon
#endif