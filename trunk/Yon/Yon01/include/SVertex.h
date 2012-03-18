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

			void print(){
				printf("{%.2f,%.2f,%.2f}\n",pos.x,pos.y,pos.z);
			}
		};
	}//scene
}//yon
#endif