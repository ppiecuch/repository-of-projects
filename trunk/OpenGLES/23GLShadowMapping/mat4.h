/*
math3d++ - A C++ 3d math library
Copyright (c) 2004, Trenkwalder Markus
All rights reserved. 

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

- Neither the name of the math3d++'s copyright owner nor the names
of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Contact info:
email: trenki2@gmx.net web: trenki.50free.org
*/

#ifndef MATH3DPP_MAT4_H
#define MATH3DPP_MAT4_H

#include "mat3.h"

/// 4x3 matrix class
class mat4 {
	Real data[4][4];
public:
	mat4();

	mat4(Real a11, Real a12, Real a13, Real a14, 
		Real a21, Real a22, Real a23, Real a24, 
		Real a31, Real a32, Real a33, Real a34,
		Real a41, Real a42, Real a43, Real a44);

	explicit mat4(const mat3& m);

	operator Real* ();
	operator const Real* () const;

	Real* operator[] (int i);
	const Real* operator[] (int i) const;

	mat4& operator+= (const mat4& m);
	mat4& operator-= (const mat4& m);
	mat4& operator*= (const mat4& m);

	mat4& operator*= (Real s);
	mat4& operator/= (Real s);

	inline Real* GetColumn(int i);
};

// constructors


inline mat4::mat4() {
	// initialize to zero
	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k )
			data[j][k] = 0;
}

inline Real* mat4::GetColumn(int i){
	return &data[i][0];
}


inline mat4::mat4(
				  Real a11, Real a12, Real a13, Real a14, 
				  Real a21, Real a22, Real a23, Real a24, 
				  Real a31, Real a32, Real a33, Real a34,
				  Real a41, Real a42, Real a43, Real a44)
{
	data[0][0] = a11; data[0][1] = a12; data[0][2] = a13; data[0][3] = a14;
	data[1][0] = a21; data[1][1] = a22; data[1][2] = a23; data[1][3] = a24;
	data[2][0] = a31; data[2][1] = a32; data[2][2] = a33; data[2][3] = a34;
	data[3][0] = a41; data[3][1] = a42; data[3][2] = a43; data[3][3] = a44;
}

/// Construction from a 3x3 matrix.
/// The 4x4 identity matrix is taken and the upper left 3x3 submatrix is
/// initialized with the values from the argument matrix.
inline mat4::mat4(const mat3& m) {
	*this = mat4(
		m[0][0], m[0][1], m[0][2], 0,
		m[1][0], m[1][1], m[1][2], 0,
		m[2][0], m[2][1], m[2][2], 0,
		0      ,        0,        0, 1
		);
}

// operators for access to the individual matrix elements (unchecked)

/// Conversion operator so that you can access the individual components. 
inline mat4::operator Real* () {
	return &data[0][0];
}

/// Conversion operator so that you can access the individual components.
inline mat4::operator const Real* () const {
	return &data[0][0];
}
/// Subscripting operator.
/// You can write code like m[0][0] = value
inline Real* mat4::operator[] (int i) {
	return data[i];
}

/// Subscripting operator.
/// You can write code like value = m[0][0]
inline const Real* mat4::operator[] (int i) const {
	return data[i];
}


// arithmetic operators
inline mat4& mat4::operator+= (const mat4& m) {
	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k )
			data[j][k] += m.data[j][k];

	return *this;
}

inline mat4& mat4::operator-= (const mat4& m) {
	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k )
			data[j][k] -= m.data[j][k];

	return *this;
}



// forward declaration
/// @relates mat4
/// Matrix multiplication
inline mat4 operator* (const mat4& a, const mat4& b);

// matrix multiplication
inline mat4& mat4::operator*= (const mat4& m) {
	*this = *this * m;
	return *this;
}

inline mat4& mat4::operator*= (Real s) {
	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k )
			data[j][k] *= s;

	return *this;
}

inline mat4& mat4::operator/= (Real s) {
	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k )
			data[j][k] /= s;

	return *this;	
}

// non-member operators

/// @relates mat4
/// Matrix addition
inline mat4 operator+ (const mat4& a, const mat4& b) {
	mat4 r = a;
	return r += b;
}

/// @relates mat4
/// Matrix subtraction
inline mat4 operator- (const mat4& a, const mat4& b) {
	mat4 r = a;
	return r -= b;
}

/// @relates mat4
/// Matrix multiplication
inline mat4 operator* (const mat4& a, const mat4& b) {
	mat4 r;	

	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k ) {
			r[j][k] = 0;
			for ( int i = 0; i < 4; ++i )	
				r[j][k] += a[j][i] * b[i][k];
		}

		return r;
}

/// @relates mat4
/// Unary minus
inline mat4 operator- (const mat4& a) {
	mat4 r;
	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k )
			r[j][k] = -a[j][k];

	return r;
}

/// @relates mat4
/// Multiplication by a scalar
inline mat4 operator* (Real s, const mat4& a) {
	mat4 r;
	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k )
			r[j][k] = s * a[j][k];

	return r;
}

/// @relates mat4
/// Multiplies the matrix by the inverse of s (1/s * M)
inline mat4 operator/ (const mat4& a, Real s) {
	mat4 r;
	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k )
			r[j][k] = a[j][k] / s;

	return r;
}

/// @relates mat4
/// Matrix comparison
inline bool operator== (const mat4& a, const mat4& b) {

	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k )
		{
#ifndef USE_FEQUAL_COMPARE	
			if ( a[j][k] != b[j][k] ) return false;
#else
			if ( !fequal(a[j][k], b[j][k] ) ) return false;
#endif
		}

		return true;
}

/// @relates mat4
/// Matrix comparison
inline bool operator!= (const mat4& a, const mat4& b) {
	return !operator==(a, b);
}

/******************************************************************************/

/// @relates mat4
/// Returns the transpose of the matrix passed as argument
inline mat4 transpose(const mat4& a) {
	mat4 r(
		a[0][0], a[1][0], a[2][0], a[3][0], 
		a[0][1], a[1][1], a[2][1], a[3][1],
		a[0][2], a[1][2], a[2][2], a[3][2],
		a[0][3], a[1][3], a[2][3], a[3][3]
	);	

	return r;
}

/// @relates mat4
/// Transposes the passed matrix and returns a reference to it
inline mat4& makeTranspose(mat4& a) {
	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < j; ++k ) {
			Real tmp = a[j][k];
			a[j][k] = a[k][j];
			a[k][j] = tmp;
		}

		return a;
}


/// @relates mat4
/// Returns the 4x4 identity matrix
inline mat4 identity4() {
	return mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0, 
		0, 0, 0, 1
		);
}

/// @relates mat4
/// Make the passed matrix the identity matrix and returns a reference
inline mat4& makeIdentity(mat4& a) {
	for ( int j = 0; j < 4; ++j )
		for ( int k = 0; k < 4; ++k )
			a[j][k] = j == k ? 1 : 0;

	return a;
}


/// @relates mat4
/// Returns the matrix determinant
inline Real det(const mat4& a) {
	return (
		a[0][0] * (
		a[1][1] * (a[2][2] * a[3][3] - a[2][3] * a[3][2]) +
		a[2][1] * (a[1][3] * a[3][2] - a[1][2] * a[3][3]) +
		a[3][1] * (a[1][2] * a[2][3] - a[1][3] * a[2][2])
		) -
		a[1][0] * (
		a[0][1] * (a[2][2] * a[3][3] - a[2][3] * a[3][2]) +
		a[2][1] * (a[0][3] * a[3][2] - a[0][2] * a[3][3]) +
		a[3][1] * (a[0][2] * a[2][3] - a[0][3] * a[2][2])
		) +
		a[2][0] * (
		a[0][1] * (a[1][2] * a[3][3] - a[1][3] * a[3][2]) +
		a[1][1] * (a[0][3] * a[3][2] - a[0][2] * a[3][3]) +
		a[3][1] * (a[0][2] * a[1][3] - a[0][3] * a[1][2])
		) -
		a[3][0] * (
		a[0][1] * (a[1][2] * a[2][3] - a[1][3] * a[2][2]) +
		a[1][1] * (a[0][3] * a[2][2] - a[0][2] * a[2][3]) +
		a[2][1] * (a[0][2] * a[1][3] - a[0][3] * a[1][2])
		)
		);
}

/// @relates mat4
/// Returns the inverse of the matrix. By default the parameter detA is set to
/// zero and therefore the function will calculate the needed matrix determinant
/// on its own.
/// If you already know the matrix determinant you can pass its value in the
/// parameter detA so that the function doesn't have to calculate it.
/// If the matrix is singular it is impossible to calculate the inverse matrix
/// and the identity matrix is returned.
inline mat4 inverse(const mat4& a, Real detA = 0) {

	// only calculate the determinant if it has not been passed as 
	// argument
	if ( detA == 0 ) {
		detA = det(a);
	}

	// helper class of 3x3 matrix to calculate matrix determinant
	class Minor {
		const mat4& m;
		int j, k;
	public:
		Minor(const mat4& _m, int _j, int _k):m(_m),j(_j),k(_k){};
		Real det() const {
			return (
				(*this)(0,0) * ((*this)(1,1) * (*this)(2,2) - (*this)(1,2) * (*this)(2,1)) +
				(*this)(1,0) * ((*this)(0,2) * (*this)(2,1) - (*this)(0,1) * (*this)(2,2)) +
				(*this)(2,0) * ((*this)(0,1) * (*this)(1,2) - (*this)(0,2) * (*this)(1,1))
				);
		}

		Real operator() (int _j, int _k) const {
			if ( _j >= j ) ++_j;
			if ( _k >= k ) ++_k;

			return m[_j][_k];
		}
	};

	if ( detA != 0 ) {
		mat4 r;

		for ( int j = 0; j < 4; ++j )
			for ( int k = 0; k < 4; ++k ) {
				Minor m(a, j, k);
				r[k][j] = (((j + k) % 2) == 0 ? 1 : -1) * m.det();
			}

			r /= detA;

			return r;
	}

	return identity4();
}

/******************************************************************************/

#endif


