#ifndef _YON_CORE_TRIANGLE3D_H_
#define _YON_CORE_TRIANGLE3D_H_

#include "vector3d.h"
#include "line3d.h"
#include "plane3d.h"

namespace yon{
namespace core{

	//! 3d triangle template class for doing collision detection and other things.
	template <class T>
	class triangle3d
	{
		//p1与p2是否在ab所在直线的同侧
		bool isOnSameSide(const vector3d<T>& p1, const vector3d<T>& p2, const vector3d<T>& a, const vector3d<T>& b) const
		{
			vector3d<T> bminusa = b - a;
			vector3d<T> cp1 = bminusa.crossProduct(p1 - a);
			vector3d<T> cp2 = bminusa.crossProduct(p2 - a);
			return (cp1.dotProduct(cp2) >= 0.0f);
		}
	public:
		vector3d<T> A;
		vector3d<T> B;
		vector3d<T> C;

		triangle3d() {}
		triangle3d(const vector3d<T> v1, const vector3d<T> v2, const vector3d<T> v3) : A(v1), B(v2), C(v3) {}

		bool operator==(const triangle3d<T>& other) const
		{
			return other.A==A && other.B==B && other.C==C;
		}

		bool operator!=(const triangle3d<T>& other) const
		{
			return !(*this==other);
		}

		void set(const core::vector3d<T>& a, const core::vector3d<T>& b, const core::vector3d<T>& c)
		{
			A = a;
			B = b;
			C = c;
		}

		//! Get the normal of the triangle.
		/** Please note: The normal is not always normalized. */
		vector3d<T> getNormal() const
		{
			return (B - A).crossProduct(C - A);
		}

		//! Test if the triangle would be front or backfacing from any point.
		/** Thus, this method assumes a camera position from which the
		triangle is definitely visible when looking at the given direction.
		Do not use this method with points as it will give wrong results!
		\param lookDirection Look direction.
		\return True if the plane is frontfacing and false if it is backfacing. */
		bool isFrontFacing(const vector3d<T>& lookDirection) const
		{
			const vector3d<T> n = getNormal().normalize();
			const f32 d = (f32)n.dotProduct(lookDirection);
			return d<=0.0f;
		}

		//DONE:面积判断法S(PAB)+S(PAC)+S(PBC)与S(ABC)比较(求面积需一次叉乘跟一次开方（显然不行了）)
		//! Check if a point is inside the triangle
		/** \param p Point to test. Assumes that this point is already
		on the plane of the triangle.
		\return True if the point is inside the triangle, otherwise false. */
		bool isPointInside(const vector3d<T>& p) const
		{
			return (isOnSameSide(p, A, B, C) &&
				isOnSameSide(p, B, A, C) &&
				isOnSameSide(p, C, A, B));
		}


		//数学依据：AB*AC=|AB||AC|Sin∠BAC=2S三角形BAC
		//How to determine area? you have two options: 
		//1) Heron's theorem, involves sqrt, slower 
		//2) the more perferred way is the cross products (or effectively, the half of absolute value of (sum of the down products minus the sum of up products)) 
		//for example, if A=(x1,y1) B=(x2,y2), C=(x3,y3) 
		//Area= abs(x1*y2+x2*y3+x3*y1-x1*y3-x3*y2-x2*y1)/2 
		//! Get the area of the triangle
		T getArea() const
		{
			return (B - A).crossProduct(C - A).getLength() * 0.5f;
		}

		//! Get the plane of this triangle.
		plane3d<T> getPlane() const
		{
			return plane3d<T>(A, B, C);
		}

	};

	typedef triangle3d<f32> triangle3df;
	typedef triangle3d<s32> triangle3di;

}
}
#endif