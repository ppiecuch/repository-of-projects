#ifndef _YON_CORE_QUATERNION_H_
#define _YON_CORE_QUATERNION_H_

#include "matrix4.h"

namespace yon{
namespace core{

	//四元数是由数学家William Rowan Hamilton于19世纪发明的（哈密顿（Hamiltonian）路径就是他提出的）。

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

		//! Constructor which converts euler angles (radians) to a quaternion
		quaternion(f32 x, f32 y, f32 z);

		//! Constructor which converts euler angles (radians) to a quaternion
		quaternion(const vector3df& vec);

		//! Constructor which converts a matrix to a quaternion
		quaternion(const matrix4f& mat);

		//! Equalilty operator
		bool operator==(const quaternion& other) const;

		//! inequality operator
		bool operator!=(const quaternion& other) const;

		//! returns if this quaternion equals the other one, taking floating point rounding errors into account
		inline bool equals(const quaternion& other,const f32 tolerance = ROUNDING_ERROR_f32 ) const;

		//! Assignment operator
		inline quaternion& operator=(const quaternion& other);

		//! Matrix assignment operator
		inline quaternion& operator=(const matrix4f& other);

		//! Add operator
		quaternion operator+(const quaternion& other) const;

		//! Add operator
		quaternion& operator+=(const quaternion& other) ;

		//! Minus operator
		quaternion operator-(const quaternion& other) const;

		//! Minus operator
		quaternion& operator-=(const quaternion& other);

		//! Multiplication operator with scalar
		quaternion operator*(f32 s) const;

		//! Multiplication operator with scalar
		quaternion& operator*=(f32 s);

		//! Multiplication operator
		quaternion operator*(const quaternion& other) const;

		//! Multiplication operator
		quaternion& operator*=(const quaternion& other);

		//! Calculates the dot product
		f32 dotProduct(const quaternion& q2) const;

		//! Multiplication operator
		vector3df operator*(const vector3df& p) const;

		//! Sets new quaternion based on euler angles (radians)
		quaternion& set(f32 x, f32 y, f32 z);

		//! Sets new quaternion
		inline quaternion& set(f32 x, f32 y, f32 z, f32 w);

		//! Sets new quaternion from other quaternion
		inline quaternion& set(const core::quaternion& quat);

		//! Sets new quaternion based on euler angles (radians)
		inline quaternion& set(const core::vector3df& vec);

		//! Set quaternion to identity
		quaternion& makeIdentity();

		//! Normalizes the quaternion
		inline quaternion& normalize();

		//! Creates a matrix from this quaternion
		matrix4f getMatrix() const;

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

		//! Output this quaternion to an euler angle (radians)
		void toEuler(vector3df& euler) const;

		//! Set this quaternion to the result of the interpolation between two quaternions
		quaternion& slerp( quaternion q1, quaternion q2, f32 interpolate );
	};

	inline quaternion::quaternion(f32 x, f32 y, f32 z)
	{
		set(x,y,z);
	}

	inline quaternion::quaternion(const vector3df& vec)
	{
		set(vec.x,vec.y,vec.z);
	}

	inline quaternion::quaternion(const matrix4f& mat)
	{
		(*this) = mat;
	}

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

	inline quaternion& quaternion::operator=(const matrix4f& m)
	{
		//TODO 优化
		//scale=reciprocal(scale)

		const f32 diag = m.m[0][0] + m.m[1][1] + m.m[2][2] + 1;

		if( diag > 0.0f )
		{
			const f32 scale = sqrtf(diag) * 2.0f; // get scale from diagonal

			// TODO: speed this up
			x = ( m.m[1][2] - m.m[2][1]) / scale;
			y = ( m.m[2][0] - m.m[0][2]) / scale;
			z = ( m.m[0][1] - m.m[1][0]) / scale;
			w = 0.25f * scale;
		}
		else
		{
			if ( m.m[0][0]> m.m[1][1] && m.m[0][0] > m.m[2][2])
			{
				// 1st element of diag is greatest value
				// find scale according to 1st element, and double it
				//x=sqrt(m11-m22-m33+1)/2
				const f32 scale = sqrtf( 1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]) * 2.0f;

				// TODO: speed this up
				x = 0.25f * scale;
				y = (m.m[1][0] + m.m[0][1]) / scale;
				z = (m.m[0][2] + m.m[2][0]) / scale;
				w = (m.m[1][2] - m.m[2][1]) / scale;
			}
			else if ( m.m[1][1] > m.m[2][2])
			{
				// 2nd element of diag is greatest value
				// find scale according to 2nd element, and double it
				//y=sqrt(-m11+m22-m33+1)/2
				const f32 scale = sqrtf( 1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]) * 2.0f;

				// TODO: speed this up
				x = (m.m[1][0] + m.m[0][1] ) / scale;
				y = 0.25f * scale;
				z = (m.m[2][1] + m.m[1][2] ) / scale;
				w = (m.m[2][0] - m.m[0][2] ) / scale;
			}
			else
			{
				// 3rd element of diag is greatest value
				// find scale according to 3rd element, and double it
				//z=sqrt(-m11-m22+m33+1)/2
				const f32 scale = sqrtf( 1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]) * 2.0f;

				// TODO: speed this up
				x = (m.m[2][0] + m.m[0][2]) / scale;
				y = (m.m[2][1] + m.m[1][2]) / scale;
				z = 0.25f * scale;
				w = (m.m[0][1] - m.m[1][0]) / scale;
			}
		}

		return normalize();
	}

	inline quaternion quaternion::operator+(const quaternion& b) const
	{
		return quaternion(x+b.x, y+b.y, z+b.z, w+b.w);
	}

	inline quaternion& quaternion::operator+=(const quaternion& b)
	{
		return (*this = (*this) + b);
	}

	inline quaternion quaternion::operator-(const quaternion& b) const
	{
		return quaternion(x-b.x, y-b.y, z-b.z, w-b.w);
	}

	inline quaternion& quaternion::operator-=(const quaternion& b)
	{
		return (*this = (*this) - b);
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

	inline f32 quaternion::dotProduct(const quaternion& q2) const
	{
		return (x * q2.x) + (y * q2.y) + (z * q2.z) + (w * q2.w);
	}

	inline vector3df quaternion::operator*(const vector3df& p) const
	{
		// nVidia SDK implementation

		//P'=qPq^-1
		//  =(w^2-v^2)+2wv×p+2(v*p)v
		//  =(1-2v^2)+2wv×p+2(v*p)v

		vector3df uv, uuv;
		vector3df v(x, y, z);
		uv = v.crossProduct(p);
		uuv = v.crossProduct(uv);
		uv *= (2.0f * w);
		uuv *= 2.0f;

		return p + uv + uuv;
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

	inline quaternion& quaternion::set(f32 x, f32 y, f32 z)
	{
		f64 angle;

		//zyx
		/*
		angle = x * 0.5;
		const f64 sr = sin(angle);
		const f64 cr = cos(angle);

		angle = y * 0.5;
		const f64 sp = sin(angle);
		const f64 cp = cos(angle);

		angle = z * 0.5;
		const f64 sy = sin(angle);
		const f64 cy = cos(angle);

		const f64 cpcy = cp * cy;
		const f64 spcy = sp * cy;
		const f64 cpsy = cp * sy;
		const f64 spsy = sp * sy;

		X = (f32)(sr * cpcy - cr * spsy);
		Y = (f32)(cr * spcy + sr * cpsy);
		Z = (f32)(cr * cpsy - sr * spcy);
		W = (f32)(cr * cpcy + sr * spsy);
		*/
		angle = x * 0.5;
		const f64 sp = sin(angle);
		const f64 cp = cos(angle);

		angle = y * 0.5;
		const f64 sy = sin(angle);
		const f64 cy = cos(angle);

		angle = z * 0.5;
		const f64 sr = sin(angle);
		const f64 cr = cos(angle);

		const f64 cpcr = cp * cr;
		const f64 spcr = sp * cr;
		const f64 cpsr = cp * sr;
		const f64 spsr = sp * sr;

		this->x = (f32)(cy * spcr + sy * cpsr);
		this->y = (f32)(sy * cpcr - cy * spsr);
		this->z = (f32)(cy * cpsr - sy * spcr);
		this->w = (f32)(cy * cpcr + sy * spsr);

		return normalize();
	}

	inline quaternion& quaternion::set(const core::vector3df& vec)
	{
		return set(vec.x, vec.y, vec.z);
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

	//TODO 测试
	inline matrix4f quaternion::getMatrix() const
	{
		const f32 xx2=x*x*2.0f;
		const f32 xy2=x*y*2.0f;
		const f32 xz2=x*z*2.0f;
		const f32 xw2=x*w*2.0f;

		const f32 yy2=y*y*2.0f;
		const f32 yz2=y*z*2.0f;
		const f32 yw2=y*w*2.0f;

		const f32 zz2=z*z*2.0f;
		const f32 zw2=z*w*2.0f;

		matrix4f dest;
		//dest.m[0][0] = 1.0f - 2.0f*y*y - 2.0f*z*z;
		//dest.m[0][1] = 2.0f*x*y + 2.0f*z*w;
		//dest.m[0][2] = 2.0f*x*z - 2.0f*y*w;
		dest.m[0][0] = 1.0f - yy2 - zz2;
		dest.m[0][1] = xy2 + zw2;
		dest.m[0][2] = xz2 - yw2;
		dest.m[0][3] = 0.0f;

		//dest.m[1][0] = 2.0f*x*y - 2.0f*z*w;
		//dest.m[1][1] = 1.0f - 2.0f*x*x - 2.0f*z*z;
		//dest.m[1][2] = 2.0f*z*y + 2.0f*x*w;
		dest.m[1][0] = xy2 - zw2;
		dest.m[1][1] = 1.0f - xx2 - zz2;
		dest.m[1][2] = yz2 + xw2;
		dest.m[1][3] = 0.0f;

		//dest.m[2][0] = 2.0f*x*z + 2.0f*y*w;
		//dest.m[2][1] = 2.0f*z*y - 2.0f*x*w;
		//dest.m[2][2] = 1.0f - 2.0f*x*x - 2.0f*y*y;
		dest.m[2][0] = xz2 + yw2;
		dest.m[2][1] = yz2 - xw2;
		dest.m[2][2] = 1.0f - xx2 - yy2;
		dest.m[2][3] = 0.0f;

		dest.m[3][0] = 0.f;
		dest.m[3][1] = 0.f;
		dest.m[3][2] = 0.f;
		dest.m[3][3] = 1.f;

		return dest;
	}

	inline quaternion& quaternion::fromAngleAxis(f32 angle, const vector3df& axis)
	{
		YON_DEBUG_BREAK_IF(!axis.isNormalized());
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

	inline void quaternion::toEuler(vector3df& euler) const
	{
		const f64 sqw = w*w;
		const f64 sqx = x*x;
		const f64 sqy = y*y;
		const f64 sqz = z*z;

		// heading = rotation about z-axis
		euler.z = (f32) (atan2(2.0 * (x*y +w*z),(sqw - sqx + sqy - sqz)));

		// attitude = rotation about y-axis
		euler.y = (f32) (atan2(2.0 * (x*z +w*y),(sqw - sqx - sqy + sqz)));

		// bank = rotation about x-axis
		euler.x = asinf( clamp(-2.0f * (y*z - w*x), -1.0f, 1.0f) );
		
	}

	inline quaternion& quaternion::slerp(quaternion q1, quaternion q2, f32 time)
	{
		f32 angle = q1.dotProduct(q2);

		if (angle < 0.0f)
		{
			q1 *= -1.0f;
			angle *= -1.0f;
		}

		f32 scale;
		f32 invscale;

		//it is impossible that angle <= -0.95f
		//if ((angle + 1.0f) > 0.05f)
		//{
			if ((1.0f - angle) >= 0.05f) // spherical interpolation
			{
				const f32 theta = acosf(angle);
				const f32 invsintheta = reciprocal(sinf(theta));
				scale = sinf(theta * (1.0f-time)) * invsintheta;
				invscale = sinf(theta * time) * invsintheta;
			}
			else // linear interploation
			{
				scale = 1.0f - time;
				invscale = time;
			}
		/*}
		else
		{
			q2.set(-q1.Y, q1.X, -q1.W, q1.Z);
			scale = sinf(PI * (0.5f - time));
			invscale = sinf(PI * time);
		}*/

		return (*this = (q1*scale) + (q2*invscale));
	}
}
}
#endif