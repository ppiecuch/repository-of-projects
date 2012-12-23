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

		template <>
		bool isOnSameSide(const vector3d<s32>& p1, const vector3d<s32>& p2, const vector3d<s32>& a, const vector3d<s32>& b) const;
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

		//TODO：待理解
		//! Check if a point is inside the triangle.
		/** This method is an implementation of the example used in a
		paper by Kasper Fauerby original written by Keidy from
		Mr-Gamemaker.
		\param p Point to test. Assumes that this point is already
		on the plane of the triangle.
		\return True if point is inside the triangle, otherwise false. */
		bool isPointInsideFast(const vector3d<T>& p) const
		{
#if 1
			//据说精度有问题
			const vector3d<T> f = B - A;
			const vector3d<T> g = C - A;

			const f32 a = (f32)f.dotProduct(f);
			const f32 b = (f32)f.dotProduct(g);
			const f32 c = (f32)g.dotProduct(g);

			const vector3d<T> vp = p - A;
			const f32 d = (f32)vp.dotProduct(f);
			const f32 e = (f32)vp.dotProduct(g);

			f32 x = (f32)(d*c)-(e*b);
			f32 y = (f32)(e*a)-(d*b);
			const f32 ac_bb = (f32)(a*c)-(b*b);
			f32 z = x+y-ac_bb;

			// return sign(z) && !(sign(x)||sign(y))
			return (( (IR(z)) & ~((IR(x))|(IR(y))) ) & 0x80000000)!=0;
#else
			//测试不通过
			//refer to:http://irrlicht.sourceforge.net/forum/viewtopic.php?f=7&t=44372&p=254331
			// The triangle is described by 3 points. (P1,P2,P3). The point to check if is inside is "Point"
			const vector3d<T> a = C - A;
			const vector3d<T> b = B - A;
			const vector3d<T> c = p - A;

			// square lenght of triangle sides
			const T squareA = a.getLengthSQ();
			const T squareB = b.getLengthSQ();

			// dot products.
			const T projAB = a.dotProduct( b);
			const T projAC = a.dotProduct( c);
			const T projBC = b.dotProduct( c);

			// get coordinates as X and Y
			const f32 lenght =  (squareA * squareB - projAB* projAB);  
			const T X = (squareB * projAC - projAB * projBC );
			const T Y = (squareA * projBC - projAB * projAC );
			const f32 check = (X+Y)/(lenght);

			// final checks
			//!Marshalling bug fix here?
			return ( check<1 ) && (X>0) && (Y>0);
#endif
		}

		//求三角形与线段的交点
		//! Get an intersection with a 3d line.
		/** \param line Line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, false if not. */
		bool getIntersectionWithLimitedLine(const line3d<T>& line,
			vector3d<T>& outIntersection) const
		{
			return getIntersectionWithLine(line.start,
				line.getVector(), outIntersection) &&
				outIntersection.isBetweenPoints(line.start, line.end);
		}

		//求三角形与直线的交点
		//! Get an intersection with a 3d line.
		/** Please note that also points are returned as intersection which
		are on the line, but not between the start and end point of the line.
		If you want the returned point be between start and end
		use getIntersectionWithLimitedLine().
		\param linePoint Point of the line to intersect with.
		\param lineVect Vector of the line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, false if there was not. */
		bool getIntersectionWithLine(const vector3d<T>& linePoint,
			const vector3d<T>& lineVect, vector3d<T>& outIntersection) const
		{
			if (getIntersectionOfPlaneWithLine(linePoint, lineVect, outIntersection))
				return isPointInside(outIntersection);

			return false;
		}

		//数据依据：a*b=|a||b|cosθ
		//当向量a与向量b平行时,a*b=0
		//显然直接的方程是：(P=linePoint,V=lineVect)
		//x = Px + Vx*s 
		//y = Py + Vy*s (1)
		//z = Pz + Vz*s 
		//假设交点为I=outIntersection，平面法线为N，则平面的方程是：
		//Nx(x-Dx)+Ny(y-Dy)+Nz(z-Dz)=0 (2)
		//存在
		//Ix = Px + Vx*t
		//Iy = Py + Vy*t
		//Iz = Pz + Vz*t
		//Nx(Ix-Dx)+Ny(Iy-Dy)+Nz(Iz-Dz)=0
		//联立：
		//Nx(Px+Vx*t-Dx)+Ny(Py+Vy*t-Dy)+Nz(Pz+Vz*t-Dz)=0;==>
		//t=(NxDx+NyDy+NzDz-NxPx+NyPy+NzPz)/(NxVx+NyVy+NzVz))
		//=(N*D-N*P)/(N*V)
		//! Calculates the intersection between a 3d line and the plane the triangle is on.
		/** \param lineVect Vector of the line to intersect with.
		\param linePoint Point of the line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, else false. */
		bool getIntersectionOfPlaneWithLine(const vector3d<T>& linePoint,
			const vector3d<T>& lineVect, vector3d<T>& outIntersection) const
		{
			const vector3d<T> normal = getNormal().normalize();
			T t2= normal.dotProduct(lineVect);

			if (core::iszero(t2))
				return false;

			//如果直线不与平面平行，将存在交点。
			//平面过点A，有Nx(Ax-Dx)+Ny(Ay-Dy)+Nz(Az-Dz)=0
			//A*N=NxAx+NyAy+NzAz，则d=NxDx+NyDy+NzDz
			T d = A.dotProduct(normal);
			//t=(N*D-N*P)/(N*V)=-(N*P-d)/(N*V)
			T t = -(normal.dotProduct(linePoint) - d) / t2;
			outIntersection = linePoint + (lineVect * t);
			return true;
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



		//数学依据：AB×AC=|AB||AC|Sin∠BAC=2S三角形BAC
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

	//fix bug:http://irrlicht.sourceforge.net/forum/viewtopic.php?f=7&t=44372&p=254331
	template <>
	bool triangle3d<s32>::isOnSameSide(const vector3d<s32>& p1, const vector3d<s32>& p2, const vector3d<s32>& a, const vector3d<s32>& b) const
	{
		vector3d<s64> bminusa(b.x-a.x,b.y-a.y,b.z-a.z);
		vector3d<s64> ap1(p1.x-a.x,p1.y-a.y,p1.z-a.z);
		vector3d<s64> ap2(p2.x-a.x,p2.y-a.y,p2.z-a.z);
		vector3d<s64> cp1 = bminusa.crossProduct(ap1);
		vector3d<s64> cp2 = bminusa.crossProduct(ap2);
		return (cp1.dotProduct(cp2) >= 0.0f);
	}

	typedef triangle3d<f32> triangle3df;
	typedef triangle3d<s32> triangle3di;

}
}
#endif