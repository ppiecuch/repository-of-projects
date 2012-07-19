#ifndef _YON_CORE_AABBOX3D_H_
#define _YON_CORE_AABBOX3D_H_

#include "yonMath.h"
#include "plane3d.h"
#include "line3d.h"

namespace yon{
namespace core{

	//! Axis aligned bounding box in 3d dimensional space.
	template <class T>
	class aabbox3d{
		//! The near edge
		vector3d<T> minEdge;

		//! The far edge
		vector3d<T> maxEdge;
	public:

		//! Default Constructor.
		aabbox3d(): minEdge(-1,-1,-1), maxEdge(1,1,1) {}
		//! Constructor with min edge and max edge.
		aabbox3d(const vector3d<T>& min, const vector3d<T>& max): minEdge(min), maxEdge(max) {}
		//! Constructor with only one point.
		aabbox3d(const vector3d<T>& init): minEdge(init), maxEdge(init) {}
		//! Constructor with min edge and max edge as single values, not vectors.
		aabbox3d(T minx, T miny, T minz, T maxx, T maxy, T maxz): minEdge(minx, miny, minz), maxEdge(maxx, maxy, maxz) {}

		inline bool operator==(const aabbox3d<T>& other) const { return (minEdge == other.minEdge && other.maxEdge == maxEdge);}
		inline bool operator!=(const aabbox3d<T>& other) const { return !(minEdge == other.minEdge && other.maxEdge == maxEdge);}

		//! Resets the bounding box to a one-point box.
		/** \param x X coord of the point.
		\param y Y coord of the point.
		\param z Z coord of the point. */
		void reset(T x, T y, T z)
		{
			maxEdge.set(x,y,z);
			minEdge = maxEdge;
		}
		//! Resets the bounding box.
		/** \param initValue New box to set this one to. */
		void reset(const aabbox3d<T>& initValue)
		{
			*this = initValue;
		}
		//! Resets the bounding box to a one-point box.
		/** \param initValue New point. */
		void reset(const vector3d<T>& initValue)
		{
			maxEdge = initValue;
			minEdge = initValue;
		}


		//! Adds a point to the bounding box
		/** The box grows bigger, if point was outside of the box.
		\param p: Point to add into the box. */
		void addInternalPoint(const vector3d<T>& p)
		{
			addInternalPoint(p.X, p.Y, p.Z);
		}
		//! Adds another bounding box
		/** The box grows bigger, if the new box was outside of the box.
		\param b: Other bounding box to add into this box. */
		void addInternalBox(const aabbox3d<T>& b)
		{
			addInternalPoint(b.maxEdge);
			addInternalPoint(b.minEdge);
		}
		//! Adds a point to the bounding box
		/** The box grows bigger, if point is outside of the box.
		\param x X coordinate of the point to add to this box.
		\param y Y coordinate of the point to add to this box.
		\param z Z coordinate of the point to add to this box. */
		void addInternalPoint(T x, T y, T z)
		{
			if (x>maxEdge.X) maxEdge.X = x;
			if (y>maxEdge.Y) maxEdge.Y = y;
			if (z>maxEdge.Z) maxEdge.Z = z;

			if (x<minEdge.X) minEdge.X = x;
			if (y<minEdge.Y) minEdge.Y = y;
			if (z<minEdge.Z) minEdge.Z = z;
		}

		//! Get center of the bounding box
		/** \return Center of the bounding box. */
		vector3d<T> getCenter() const
		{
			return (minEdge + maxEdge) / 2;
		}
		//! Get extent of the box (maximal distance of two points in the box)
		/** \return Extent of the bounding box. */
		vector3d<T> getExtent() const
		{
			return maxEdge - minEdge;
		}
		//! Check if the box is empty.
		/** This means that there is no space between the min and max edge.
		\return True if box is empty, else false. */
		bool isEmpty() const
		{
			return minEdge.equals ( maxEdge );
		}
		//! Get the volume enclosed by the box in cubed units
		T getVolume() const
		{
			const vector3d<T> e = getExtent();
			return e.X * e.Y * e.Z;
		}
		//! Get the surface area of the box in squared units
		T getArea() const
		{
			const vector3d<T> e = getExtent();
			return 2*(e.X*e.Y + e.X*e.Z + e.Y*e.Z);
		}

		//! Stores all 8 corners of the box into an array
		/** \param edges: Pointer to array of 8 corners. */
		void getCorners(vector3d<T> *corners) const
		{
			const core::vector3d<T> middle = getCenter();
			const core::vector3d<T> diag = middle - maxEdge;

			/*
			Edges are stored in this way:
			Hey, am I an ascii artist, or what? :) niko.
			   /3--------/7
			  / |       / |
			 /  |      /  |
			1---------5   |
			|  /2- - -|- -6
			| /       |  /
			|/        | /
			0---------4/
			*/

			corners[0].set(middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
			corners[1].set(middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
			corners[2].set(middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
			corners[3].set(middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
			corners[4].set(middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
			corners[5].set(middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
			corners[6].set(middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
			corners[7].set(middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
		}

		//! Repairs the box.
		/** Necessary if for example minEdge and maxEdge are swapped. */
		void repair()
		{
			T t;

			if (minEdge.x > maxEdge.x)
			{ t=minEdge.x; minEdge.x = maxEdge.x; maxEdge.x=t; }
			if (minEdge.y > maxEdge.y)
			{ t=minEdge.y; minEdge.y = maxEdge.y; maxEdge.y=t; }
			if (minEdge.z > maxEdge.z)
			{ t=minEdge.z; minEdge.z = maxEdge.z; maxEdge.z=t; }
		}

		//! Calculates a new interpolated bounding box.
		/** d=0 returns other, d=1 returns this, all other values blend between
		the two boxes.
		\param other Other box to interpolate between
		\param d Value between 0.0f and 1.0f.
		\return Interpolated box. */
		aabbox3d<T> getInterpolated(const aabbox3d<T>& other, f32 d) const
		{
			f32 inv = 1.0f - d;
			return aabbox3d<T>((other.minEdge*inv) + (minEdge*d),
				(other.maxEdge*inv) + (maxEdge*d));
		}
		//! Determines if a point is within this box.
		bool isInside(const vector3d<T>& p,bool includeBorder=true) const
		{
			if(includeBorder)
				return (p.x >= minEdge.x && p.x <= maxEdge.x &&
				p.y >= minEdge.y && p.y <= maxEdge.y &&
				p.z >= minEdge.z && p.z <= maxEdge.z);
			else
				return (p.x > minEdge.x && p.x < maxEdge.x &&
				p.y > minEdge.y && p.y < maxEdge.y &&
				p.z > minEdge.z && p.z < maxEdge.z);
		}
		//! Determines if a box is within this box.
		//注意与irrlicht的isFullInside相反
		bool isInside(const aabbox3d<T>& other,bool includeBorder=true) const
		{
			if(includeBorder)
				return (other.minEdge.x >= minEdge.x && other.minEdge.y >= minEdge.y && other.minEdge.z >= minEdge.z &&
				other.maxEdge.x <= maxEdge.x && other.maxEdge.y <= maxEdge.y && other.maxEdge.z <= maxEdge.z);
			else
				return (other.minEdge.x > minEdge.x && other.minEdge.y > minEdge.y && other.minEdge.z > minEdge.z &&
				other.maxEdge.x < maxEdge.x && other.maxEdge.y < maxEdge.y && other.maxEdge.z < maxEdge.z);
		}
		//! Determines if the axis-aligned box intersects with another axis-aligned box.
		/** \param other: Other box to check a intersection with.
		\return True if there is an intersection with the other box,
		otherwise false. */
		bool intersectsWithBox(const aabbox3d<T>& other) const
		{
			return (minEdge.x <= other.maxEdge.x && minEdge.y <= other.maxEdge.y && minEdge.z <= other.maxEdge.z &&
				maxEdge.x >= other.minEdge.x && maxEdge.y >= other.minEdge.y && maxEdge.z >= other.minEdge.z);
		}

		//TODO
		//intersectsWithLine
		//classifyPlaneRelation
	};
	//! Typedef for a f32 3d bounding box.
	typedef aabbox3d<f32> aabbox3df;
	//! Typedef for an integer 3d bounding box.
	typedef aabbox3d<s32> aabbox3di;


}
}
#endif