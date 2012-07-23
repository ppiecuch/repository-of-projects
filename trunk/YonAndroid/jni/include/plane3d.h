#ifndef _YON_CORE_PLANE3D_H_
#define _YON_CORE_PLANE3D_H_

#include "yonTypes.h"
#include "vector3d.h"

namespace yon{
namespace core{

	template<class T>
	class plane3d{
	public:
		plane3d():normal(0,1,0){recalculateD(core::UP_VECTOR3DF);}
		plane3d(const core::vector3d<T>& normal,const core::vector3d<T> point):normal(normal){recalculateD(point);}

		plane3d(T px, T py, T pz, T nx, T ny, T nz) : normal(nx, ny, nz) { recalculateD(vector3d<T>(px, py, pz)); }

		plane3d(const vector3d<T>& point1, const vector3d<T>& point2, const vector3d<T>& point3)
		{ setPlane(point1, point2, point3); }

		plane3d(const vector3d<T> & normal, const T d) : normal(normal), d(d) {}

		inline bool operator==(const plane3d<T>& other) const { return (equals(d, other.d) && normal==other.normal);}
		inline bool operator!=(const plane3d<T>& other) const { return !(*this == other);}

		void setPlane(const vector3d<T>& point, const vector3d<T>& nvector)
		{
			normal = nvector;
			recalculateD(point);
		}
		void setPlane(const vector3d<T>& nvect, T d)
		{
			normal = nvect;
			d = d;
		}
		void setPlane(const vector3d<T>& point1, const vector3d<T>& point2, const vector3d<T>& point3)
		{
			// creates the plane from 3 memberpoints
			normal = (point2 - point1).crossProduct(point3 - point1);
			normal.normalize();

			recalculateD(point1);
		}

		//Ax+By+Cz+D=0
		void recalculateD(const vector3d<T>& point)
		{
			d = - point.dotProduct(normal);
		}

		//! Get an intersection with a 3d line.
		/** \param lineVect Vector of the line to intersect with.
		\param linePoint Point of the line to intersect with.
		\param outIntersection Place to store the intersection point, if there is one.
		\return True if there was an intersection, false if there was not.
		*/
		bool getIntersectionWithLine(const vector3d<T>& linePoint,const vector3d<T>& lineVect,vector3d<T>& outIntersection) const
		{
			T t2 = normal.dotProduct(lineVect);

			if (t2 == 0)
				return false;

			T t =- (normal.dotProduct(linePoint) + d) / t2;
			outIntersection = linePoint + (lineVect * t);
			return true;
		}

		//! Tests if there is an intersection with the other plane
		/** \return True if there is a intersection. */
		bool existsIntersection(const plane3d<T>& other) const
		{
			vector3d<T> cross = other.normal.crossProduct(normal);
			return cross.getLength() > core::ROUNDING_ERROR_f32;
		}

		//TODO 待理解
		//! Intersects this plane with another.
		/** \param other Other plane to intersect with.
		\param outLinePoint Base point of intersection line.
		\param outLineVect Vector of intersection.
		\return True if there is a intersection, false if not. */
		bool getIntersectionWithPlane(const plane3d<T>& other,vector3d<T>& outLinePoint,vector3d<T>& outLineVect) const
		{
			//使用叉乘要6次乘法运算,而使用点乘则只需要5次乘法运算
			const T fn00 = normal.getLength();
			const T fn01 = normal.dotProduct(other.normal);
			const T fn11 = other.normal.getLength();
			const f64 det = fn00*fn11 - fn01*fn01;

			if (fabs(det) < ROUNDING_ERROR_f64 )
				return false;

			const f64 invdet = 1.0 / det;
			const f64 fc0 = (fn11*-d + fn01*other.d) * invdet;
			const f64 fc1 = (fn00*-other.d + fn01*d) * invdet;

			outLineVect = normal.crossProduct(other.normal);
			outLinePoint = normal*(T)fc0 + other.normal*(T)fc1;
			return true;
		}

		//! Get the intersection point with two other planes if there is one.
		bool getIntersectionWithPlanes(const plane3d<T>& o1,const plane3d<T>& o2, vector3d<T>& outPoint) const
		{
			vector3d<T> linePoint, lineVect;
			if (getIntersectionWithPlane(o1, linePoint, lineVect))
				return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);

			return false;
		}

		core::vector3d<T> normal;
		T d;
	};
	typedef plane3d<f32> plane3df;
}
}
#endif