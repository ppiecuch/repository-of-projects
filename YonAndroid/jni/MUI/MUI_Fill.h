#ifndef _MUI_FILL_H_
#define _MUI_FILL_H_

#include "SDynamicShap.h"
#include "SColor.h"

namespace mui{

	//0----1(4)
	//
	//2(3)  (5)

	struct QuadShap{
		enum Enum
		{
			LT  = 0,
			RT  = 1,
			LB  = 2,
			LB2 = 3,
			RT2 = 4,
			RB  = 5,
			COUNT = 6
		};

		static void set(scene::SDynamicShap3D& shap,f32 _l, f32 _t, f32 _r, f32 _b, f32 _z, f32 _u1, f32 _v1, f32 _u2, f32 _v2, const video::SColor& color)
		{
			core::array<scene::SVertex>& vertices=shap.getVertexArray();
			core::array<u16>& indices=shap.getIndexArray();
			//TODO append是否需要支持
			/*vertices.set_used(6);
			vertices[LT].set(_l, _t, _z, _u1, _v1, color);
			vertices[RT].set(_r, _t, _z, _u2, _v1, color);
			vertices[LB].set(_l, _b, _z, _u1, _v2, color);
			vertices[RB].set(_r, _b, _z, _u2, _v2, color);
			vertices[RT2] = vertices[RT];
			vertices[LB2] = vertices[LB];*/
			//优化
			vertices.set_used(4);
			vertices[LT].set(_l, _t, _z, _u1, _v1, color);
			vertices[RT].set(_r, _t, _z, _u2, _v1, color);
			vertices[LB].set(_l, _b, _z, _u1, _v2, color);
			vertices[RB].set(_r, _b, _z, _u2, _v2, color);
		}

		static void set(scene::SDynamicShap3D& shap,f32 _x1, f32 _y1, f32 _x2, f32 _y2, f32 _x3, f32 _y3, f32 _x4, f32 _y4, f32 _z, f32 _u1, f32 _v1, f32 _u2, f32 _v2, const video::SColor& color)
		{
			core::array<scene::SVertex>& vertices=shap.getVertexArray();
			core::array<u16>& indices=shap.getIndexArray();
			/*
			vertices.set_used(6);
			vertices[LT].set(_x1, _y1, _z, _u1, _v1, color);
			vertices[RT].set(_x2, _y2, _z, _u2, _v1, color);
			vertices[LB].set(_x4, _y4, _z, _u1, _v2, color);
			vertices[RB].set(_x3, _y3, _z, _u2, _v2, color);
			vertices[RT2] = vertices[RT];
			vertices[LB2] = vertices[LB];*/

			//优化
			vertices.set_used(4);
			vertices[LT].set(_x1, _y1, _z, _u1, _v1, color);
			vertices[RT].set(_x2, _y2, _z, _u2, _v1, color);
			vertices[LB].set(_x4, _y4, _z, _u1, _v2, color);
			vertices[RB].set(_x3, _y3, _z, _u2, _v2, color);
		}
	};
}
#endif