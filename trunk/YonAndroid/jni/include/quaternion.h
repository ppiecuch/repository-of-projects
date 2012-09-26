#ifndef _YON_CORE_QUATERNION_H_
#define _YON_CORE_QUATERNION_H_

#include "matrix4.h"

namespace yon{
namespace core{

	//��Ԫ��������ѧ��William Rowan Hamilton��19���ͷ����ģ����ܶ٣�Hamiltonian��·������������ģ���

	//! Quaternion class for representing rotations.
	/** It provides cheap combinations and avoids gimbal locks.
	Also useful for interpolations. */
	class quaternion{
	public:
		f32 x; 
		f32 y;
		f32 z;
		f32 w; 

		quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		quaternion(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}

		//! Equalilty operator
		bool operator==(const quaternion& other) const;

		//! inequality operator
		bool operator!=(const quaternion& other) const;

		//! returns if this quaternion equals the other one, taking floating point rounding errors into account
		inline bool equals(const quaternion& other,const f32 tolerance = ROUNDING_ERROR_f32 ) const;

		//! Assignment operator
		inline quaternion& operator=(const quaternion& other);

		//! Add operator
		quaternion operator+(const quaternion& other) const;

		//! Add operator
		quaternion& operator+=(const quaternion& other) const;

		//! Minus operator
		quaternion operator-(const quaternion& other) const;

		//! Minus operator
		quaternion& operator-=(const quaternion& other) const;

		//! Multiplication operator with scalar
		quaternion operator*(f32 s) const;

		//! Multiplication operator with scalar
		quaternion& operator*=(f32 s);

		//! Multiplication operator
		quaternion operator*(const quaternion& other) const;

		//! Multiplication operator
		quaternion& operator*=(const quaternion& other);

		//! Sets new quaternion
		inline quaternion& set(f32 x, f32 y, f32 z, f32 w);

		//! Sets new quaternion from other quaternion
		inline quaternion& set(const core::quaternion& quat);

		//! Set quaternion to identity
		quaternion& makeIdentity();

		//! Normalizes the quaternion
		inline quaternion& normalize();

		//Axis must be unit length.==> make sure the quaternion to be a identity quaternion

		//! Create quaternion from rotation angle and rotation axis.
		/** Axis must be unit length.
		The quaternion representing the rotation is
		q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k).
		\param angle Rotation Angle in radians.
		\param axis Rotation axis. */
		quaternion& fromAngleAxis (f32 angle, const vector3df& axis);

		//! Fills an angle (radians) around an axis (unit vector)
		void toAngleAxis (f32 &angle, core::vector3df& axis) const;
	};

	inline bool quaternion::operator==(const quaternion& other) const
	{
		return ((x == other.x) &&
			(y == other.y) &&
			(z == other.z) &&
			(w == other.w));
	}

	inline bool quaternion::operator!=(const quaternion& other) const
	{
		return !(*this == other);
	}

	inline bool quaternion::equals(const quaternion& other, const f32 tolerance) const
	{
		return core::equals(x, other.x, tolerance) &&
			core::equals(y, other.y, tolerance) &&
			core::equals(z, other.z, tolerance) &&
			core::equals(w, other.w, tolerance);
	}

	inline quaternion& quaternion::operator=(const quaternion& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	inline quaternion quaternion::operator+(const quaternion& b) const
	{
		return quaternion(x+b.x, y+b.y, z+b.z, w+b.w);
	}

	inline quaternion& quaternion::operator+=(const quaternion& b) const
	{
		return (*this = (*this) + other);
	}

	inline quaternion quaternion::operator-(const quaternion& b) const
	{
		return quaternion(x-b.x, y-b.y, z-b.z, w-b.w);
	}

	inline quaternion& quaternion::operator-=(const quaternion& b) const
	{
		return (*this = (*this) - other);
	}


	inline quaternion quaternion::operator*(f32 s) const
	{
		return quaternion(s*x, s*y, s*z, s*w);
	}

	inline quaternion& quaternion::operator*=(f32 s)
	{
		x*=s;
		y*=s;
		z*=s;
		w*=s;
		return *this;
	}

	inline quaternion quaternion::operator*(const quaternion& other) const
	{
		quaternion tmp;

		tmp.w = (other.w * w) - (other.x * x) - (other.y * y) - (other.z * z);
		tmp.x = (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y);
		tmp.y = (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z);
		tmp.z = (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x);

		return tmp;
	}

	inline quaternion& quaternion::operator*=(const quaternion& other)
	{
		return (*this = other * (*this));
	}

	inline quaternion& quaternion::set(f32 x, f32 y, f32 z, f32 w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		return *this;
	}

	inline quaternion& quaternion::set(const core::quaternion& quat)
	{
		return (*this=quat);
	}

	inline quaternion& quaternion::makeIdentity()
	{
		w = 1.f;
		x = 0.f;
		y = 0.f;
		z = 0.f;
		return *this;
	}

	inline quaternion& quaternion::normalize()
	{
		const f32 n = x*x + y*y + z*z + w*w;

		if (n == 1)
			return *this;

		//n = 1.0f / sqrtf(n);
		return (*this *= reciprocal_squareroot ( n ));
	}

	inline quaternion& quaternion::fromAngleAxis(f32 angle, const vector3df& axis)
	{
		const f32 fHalfAngle = 0.5f*angle;
		const f32 fSin = sinf(fHalfAngle);
		w = cosf(fHalfAngle);
		x = fSin*axis.x;
		y = fSin*axis.y;
		z = fSin*axis.z;
		return *this;
	}

	inline void quaternion::toAngleAxis(f32 &angle, core::vector3df &axis) const
	{
		//const f32 scale = sqrtf(x*x + y*y + z*z);
		const f32 scale = x*x+y*y+z*z;

		//why w>1.0 or <-1.0 is illegal?==>refer to fromAngleAxis function
		if (core::iszero(scale) || w > 1.0f || w < -1.0f)
		{
			angle = 0.0f;
			axis.x = 0.0f;
			axis.y = 1.0f;
			axis.z = 0.0f;
		}
		else
		{
			//because in fromAngleAxis function, Axis must be unit length, so:
			//axis.x = sqrt((x/fSin)^2/(scale/fSin^2))=x/sqrt(scale)
			//const f32 invscale = reciprocal(scale);
			const f32 invscale = core::reciprocal_squareroot(scale);
			angle = 2.0f * acosf(w);
			axis.x = x * invscale;
			axis.y = y * invscale;
			axis.z = z * invscale;
		}
	}
}
}
#endif