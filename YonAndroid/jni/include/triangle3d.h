#ifndef _YON_CORE_TRIANGLE3D_H_
#define _YON_CORE_TRIANGLE3D_H_

#include "vector3d.h"
#include "line3d.h"
#include "plane3d.h"
#include "aabbox3d.h"

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

		//! Determines if the triangle is totally inside a bounding box.
		/** \param box Box to check.
		\return True if triangle is within the box, otherwise false. */
		bool isTotalInsideBox(const aabbox3d<T>& box) const
		{
			return (box.isInside(A) &&
				box.isInside(B) &&
				box.isInside(C));
		}

		//! Determines if the triangle is totally outside a bounding box.
		/** \param box Box to check.
		\return True if triangle is outside the box, otherwise false. */
		bool isTotalOutsideBox(const aabbox3d<T>& box) const
		{
			return ((A.x > box.maxEdge.x && B.x > box.maxEdge.x && C.x > box.maxEdge.x) ||
				(A.y > box.maxEdge.y && B.y > box.maxEdge.y && C.y > box.maxEdge.y) ||
				(A.z > box.maxEdge.z && B.z > box.maxEdge.z && C.z > box.maxEdge.z) ||
				(A.x < box.minEdge.x && B.x < box.minEdge.x && C.x < box.minEdge.x) ||
				(A.y < box.minEdge.y && B.y < box.minEdge.y && C.y < box.minEdge.y) ||
				(A.z < box.minEdge.z && B.z < box.minEdge.z && C.z < box.minEdge.z));
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

		//原理：分别求出三条边上离点P最近的点（可能在边上），然后对三个结果进行比较取其最优者
		//注意：显然不支持三角形内部的点
		//! Get the closest point on a triangle to a point on the same plane.
		/** \param p Point which must be on the same plane as the triangle.
		\return The closest point of the triangle */
		core::vector3d<T> getClosestPoint(const core::vector3d<T>& p) const
		{
			const core::vector3d<T> rab = line3d<T>(A, B).getClosestPoint(p);
			const core::vector3d<T> rbc = line3d<T>(B, C).getClosestPoint(p);
			const core::vector3d<T> rca = line3d<T>(C, A).getClosestPoint(p);

			const T d1 = rab.getDistanceFrom(p);
			const T d2 = rbc.getDistanceFrom(p);
			const T d3 = rca.getDistanceFrom(p);

			if (d1 < d2)
				return d1 < d3 ? rab : rca;

			return d2 < d3 ? rbc : rca;
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

		//TODO
		//bool isTotalInsideBox(const aabbox3d<T>& box) const
		//bool isTotalOutsideBox(const aabbox3d<T>& box) const
	};

	typedef triangle3d<f32> triangle3df;
	typedef triangle3d<s32> triangle3di;

}
}
#endif