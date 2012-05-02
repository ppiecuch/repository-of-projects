#ifndef _YON_CORE_LINE2D_H_
#define _YON_CORE_LINE2D_H_

#include "vector2d.h"

namespace yon{
namespace core{

	template<class T>
	class line2d{
	public:
		//! Default constructor for line going from (0,0) to (1,1).
		line2d() : start(0,0), end(1,1) {}
		//! Constructor for line between the two points.
		line2d(T xa, T ya, T xb, T yb) : start(xa, ya), end(xb, yb) {}
		//! Constructor for line between the two points given as vectors.
		line2d(const vector2d<T>& start, const vector2d<T>& end) : start(start), end(end) {}
		//! Copy constructor.
		line2d(const line2d<T>& other) : start(other.start), end(other.end) {}

		// operators

		line2d<T> operator+(const vector2d<T>& point) const { return line2d<T>(start + point, end + point); }
		line2d<T>& operator+=(const vector2d<T>& point) { start += point; end += point; return *this; }

		line2d<T> operator-(const vector2d<T>& point) const { return line2d<T>(start - point, end - point); }
		line2d<T>& operator-=(const vector2d<T>& point) { start -= point; end -= point; return *this; }

		bool operator==(const line2d<T>& other) const
		{ return (start==other.start && end==other.end) || (end==other.start && start==other.end);}
		bool operator!=(const line2d<T>& other) const
		{ return !(start==other.start && end==other.end) || (end==other.start && start==other.end);}

		// functions
		//! Set this line to new line going through the two points.
		void setLine(const T& xa, const T& ya, const T& xb, const T& yb){start.set(xa, ya); end.set(xb, yb);}
		//! Set this line to new line going through the two points.
		void setLine(const vector2d<T>& nstart, const vector2d<T>& nend){start.set(nstart); end.set(nend);}
		//! Set this line to new line given as parameter.
		void setLine(const line2d<T>& line){start.set(line.start); end.set(line.end);}

		//! Get length of line
		/** \return Length of the line. */
		f64 getLength() const { return start.getDistanceFrom(end); }

		//! Get squared length of the line
		/** \return Squared length of line. */
		T getLengthSQ() const { return start.getDistanceFromSQ(end); }

		//! Get middle of the line
		/** \return center of the line. */
		vector2d<T> getMiddle() const
		{
			return (start + end) * (T)0.5;
		}

		//! Get the vector of the line.
		/** \return The vector of the line. */
		vector2d<T> getVector() const { return vector2d<T>(end.x - start.x, end.y - start.y); }

		//! Tests if this line intersects with another line.
		/** \param l: Other line to test intersection with.
		\param out: If there is an intersection, the location of the
		intersection will be stored in this vector.
		\return True if there is an intersection, false if not. */
		bool intersectWith(const line2d<T>& l, vector2d<T>& out) const
		{
			// Uses the method given at:
			// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/ 
			const f32 commonDenominator = (l.end.y - l.start.y)*(end.x - start.x) -
				(l.end.x - l.start.x)*(end.y - start.y);

			const f32 numeratorA = (l.end.x - l.start.x)*(start.y - l.start.y) -
				(l.end.y - l.start.y)*(start.x -l.start.x);

			const f32 numeratorB = (end.x - start.x)*(start.y - l.start.y) -
				(end.y - start.y)*(start.x -l.start.x); 

			if(equals(commonDenominator, 0.f))
			{ 
				// The lines are either coincident or parallel
				// if both numerators are 0, the lines are coincident
				if(equals(numeratorA, 0.f) && equals(numeratorB, 0.f))
				{
					// Try and find a common endpoint
					if(l.start == start || l.end == start)
						out = start;
					else if(l.end == end || l.start == end)
						out = end;
					// now check if the two segments are disjunct
					else if (l.start.x>start.x && l.end.x>start.x && l.start.x>end.x && l.end.x>end.x)
						return false;
					else if (l.start.y>start.y && l.end.y>start.y && l.start.y>end.y && l.end.y>end.y)
						return false;
					else if (l.start.x<start.x && l.end.x<start.x && l.start.x<end.x && l.end.x<end.x)
						return false;
					else if (l.start.y<start.y && l.end.y<start.y && l.start.y<end.y && l.end.y<end.y)
						return false;
					// else the lines are overlapping to some extent
					else
					{
						// find the points which are not contributing to the
						// common part
						vector2d<T> maxp;
						vector2d<T> minp;
						if ((start.x>l.start.x && start.x>l.end.x && start.x>end.x) || (start.y>l.start.y && start.y>l.end.y && start.y>end.y))
							maxp=start;
						else if ((end.x>l.start.x && end.x>l.end.x && end.x>start.x) || (end.y>l.start.y && end.y>l.end.y && end.y>start.y))
							maxp=end;
						else if ((l.start.x>start.x && l.start.x>l.end.x && l.start.x>end.x) || (l.start.y>start.y && l.start.y>l.end.y && l.start.y>end.y))
							maxp=l.start;
						else
							maxp=l.end;
						if (maxp != start && ((start.x<l.start.x && start.x<l.end.x && start.x<end.x) || (start.y<l.start.y && start.y<l.end.y && start.y<end.y)))
							minp=start;
						else if (maxp != end && ((end.x<l.start.x && end.x<l.end.x && end.x<start.x) || (end.y<l.start.y && end.y<l.end.y && end.y<start.y)))
							minp=end;
						else if (maxp != l.start && ((l.start.x<start.x && l.start.x<l.end.x && l.start.x<end.x) || (l.start.y<start.y && l.start.y<l.end.y && l.start.y<end.y)))
							minp=l.start;
						else
							minp=l.end;

						// one line is contained in the other. Pick the center
						// of the remaining points, which overlap for sure
						out = core::vector2d<T>();
						if (start != maxp && start != minp)
							out += start;
						if (end != maxp && end != minp)
							out += end;
						if (l.start != maxp && l.start != minp)
							out += l.start;
						if (l.end != maxp && l.end != minp)
							out += l.end;
						out *= 0.5f;
					}

					return true; // coincident
				}

				return false; // parallel
			}

			// Get the point of intersection on this line, checking that
			// it is within the line segment.
			const f32 uA = numeratorA / commonDenominator;
			if(uA < 0.f || uA > 1.f)
				return false; // Outside the line segment

			const f32 uB = numeratorB / commonDenominator;
			if(uB < 0.f || uB > 1.f)
				return false; // Outside the line segment

			// Calculate the intersection point.
			out.x = start.x + uA * (end.x - start.x);
			out.y = start.y + uA * (end.y - start.y);
			return true; 
		}

		//! Get unit vector of the line.
		/** \return Unit vector of this line. */
		vector2d<T> getUnitVector() const
		{
			T len = (T)(1.0 / getLength());
			return vector2d<T>((end.x - start.x) * len, (end.y - start.y) * len);
		}

		//! Get angle between this line and given line.
		/** \param l Other line for test.
		\return Angle in degrees. */
		f64 getAngleWith(const line2d<T>& l) const
		{
			vector2d<T> vect = getVector();
			vector2d<T> vect2 = l.getVector();
			return vect.getAngleWith(vect2);
		}

		//! Tells us if the given point lies to the left, right, or on the line.
		/** \return 0 if the point is on the line
		<0 if to the left, or >0 if to the right. */
		T getPointOrientation(const vector2d<T>& point) const
		{
			return ( (end.x - start.x) * (point.y - start.y) -
				(point.x - start.x) * (end.y - start.y) );
		}

		//! Check if the given point is a member of the line
		/** \return True if point is between start and end, else false. */
		bool isPointOnLine(const vector2d<T>& point) const
		{
			T d = getPointOrientation(point);
			return (d == 0 && point.isBetweenPoints(start, end));
		}

		//! Check if the given point is between start and end of the line.
		/** Assumes that the point is already somewhere on the line. */
		bool isPointBetweenStartAndEnd(const vector2d<T>& point) const
		{
			return point.isBetweenPoints(start, end);
		}

		//! Get the closest point on this line to a point
		vector2d<T> getClosestPoint(const vector2d<T>& point) const
		{
			vector2d<T> c = point - start;
			vector2d<T> v = end - start;
			T d = (T)v.getLength();
			v /= d;
			T t = v.dotProduct(c);

			if (t < (T)0.0) return start;
			if (t > d) return end;

			v *= t;
			return start + v;
		}

		//! Start point of the line.
		vector2d<T> start;
		//! End point of the line.
		vector2d<T> end;
	};

	//! Typedef for an f32 line.
	typedef line2d<f32> line2df;
	//! Typedef for an integer line.
	typedef line2d<s32> line2di;
}
}
#endif