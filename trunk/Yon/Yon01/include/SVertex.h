#ifndef _YON_SCENE_SVERTEX_H_
#define _YON_SCENE_SVERTEX_H_

#include "yonTypes.h"
#include "vector3d.h"

namespace yon{
	namespace scene{
		
		//TODO
		//enum ENUM_VERTEX_TYPE{
		//};

		struct SVertex{
			SVertex(){}
			SVertex(f32 x,f32 y,f32 z):pos(x,y,z){}

			core::vector3df pos;
		};
	}//scene
}//yon
#endif