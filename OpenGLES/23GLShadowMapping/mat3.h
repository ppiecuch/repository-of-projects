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

#ifndef MATH3DPP_MAT3_H
#define MATH3DPP_MAT3_H

#define Real float

/// 3x3 matrix class
class mat3 {
	Real data[3][3];
public:
	mat3();

	mat3(Real a11, Real a12, Real a13,
		Real a21, Real a22, Real a23,
		Real a31, Real a32, Real a33);

	operator Real* ();
	operator const Real* () const;

	Real* operator[] (int i);
	const Real* operator[] (int i) const;

	mat3& operator+= (const mat3& m);
	mat3& operator-= (const mat3& m);
	mat3& operator*= (const mat3& m);

	mat3& operator*= (Real s);
	mat3& operator/= (Real s);
};

// constructors

inline mat3::mat3() {
	// initialize to zero
	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k )
			data[j][k] = 0.0;
}

inline mat3::mat3(
				  Real a11, Real a12, Real a13,
				  Real a21, Real a22, Real a23,
				  Real a31, Real a32, Real a33)
{
	data[0][0] = a11; data[0][1] = a12; data[0][2] = a13;
	data[1][0] = a21; data[1][1] = a22; data[1][2] = a23;
	data[2][0] = a31; data[2][1] = a32; data[2][2] = a33;
}

// operators for access to the individual matrix elements (unchecked)

/// Conversion operator so that you can access the individual components. 
inline mat3::operator Real* () {
	return &data[0][0];
}

/// Conversion operator so that you can access the individual components.
inline mat3::operator const Real* () const {
	return &data[0][0];
}

/// Subscripting operator.
/// You can write code like m[0][0] = value
inline Real* mat3::operator[] (int i) {
	return data[i];
}

/// Subscripting operator.
/// You can write code like value = m[0][0]
inline const Real* mat3::operator[] (int i) const {
	return data[i];
}


// arithmetic operators
inline mat3& mat3::operator+= (const mat3& m) {
	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k )
			data[j][k] += m.data[j][k];

	return *this;
}

inline mat3& mat3::operator-= (const mat3& m) {
	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k )
			data[j][k] -= m.data[j][k];

	return *this;
}


// forward declaration of matrix multiplication function
/// @relates mat3
/// Matrix multiplication
mat3 operator* (const mat3& a, const mat3& b);

// matrix multiplication
inline mat3& mat3::operator*= (const mat3& m) {
	*this = *this * m;
	return *this;
}


inline mat3& mat3::operator*= (Real s) {
	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k )
			data[j][k] *= s;

	return *this;
}


inline mat3& mat3::operator/= (Real s) {
	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k )
			data[j][k] /= s;

	return *this;	
}

// non-member operators

/// @relates mat3
/// Matrix addition
inline mat3 operator+ (const mat3& a, const mat3& b) {
	mat3 r = a;
	return r += b;
}

/// @relates mat3
/// Matrix subtraction
inline mat3 operator- (const mat3& a, const mat3& b) {
	mat3 r = a;
	return r -= b;
}

/// @relates mat3
/// Matrix multiplication
inline mat3 operator* (const mat3& a, const mat3& b) {
	mat3 r;

	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k ) {
			r[j][k] = 0;
			for ( int i = 0; i < 3; ++i )	
				r[j][k] += a[j][i] * b[i][k];
		}

		return r;
}

/// @relates mat3
/// Unary minus
inline mat3 operator- (const mat3& a) {
	mat3 r;
	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k )
			r[j][k] = -a[j][k];

	return r;
}

/// @relates mat3
/// Matrix multiplication with a scalar
inline mat3 operator* (Real s, const mat3& a) {
	mat3 r;
	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k )
			r[j][k] = s * a[j][k];

	return r;
}

/// @relates mat3
/// Multiplies the matrix by the inverse of the scalar s (1/s * M)
inline mat3 operator/ (const mat3& a, Real s) {
	mat3 r;
	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k )
			r[j][k] = a[j][k] / s;

	return r;
}

/// @relates mat3
/// Matrix comparison
inline bool operator== (const mat3& a, const mat3& b) {

	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k )
		{
#ifndef USE_FEQUAL_COMPARE	
			if ( a[j][k] != b[j][k] ) return false;
#else
			if ( !fequal(a[j][k], b[j][k] ) ) return false;
#endif
		}

		return true;
}

/// @relates mat3
/// Matrix comparison
inline bool operator!= (const mat3& a, const mat3& b) {
	return !operator==(a, b);
}

/******************************************************************************/

/// @relates mat3
/// Returns the transpose of the matrix passed as argument
inline mat3 transpose(const mat3& a) {
	mat3 r(
		a[0][0], a[1][0], a[2][0], 
		a[0][1], a[1][1], a[2][1], 
		a[0][2], a[1][2], a[2][2]
	);	

	return r;
}

/// @relates mat3
/// Transposes the passed matrix and returns a reference to it
inline mat3& makeTranspose(mat3& a) {
	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < j; ++k ) {
			Real tmp = a[j][k];
			a[j][k] = a[k][j];
			a[k][j] = tmp;
		}

		return a;
}

/// @relates mat3
/// Returns the 3x3 identity matrix
inline mat3 identity3() {
	return mat3(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
		);
}

/// @relates mat3
/// Make the passed matrix the identity matrix and returns a reference
inline mat3& makeIdentity(mat3& a) {
	for ( int j = 0; j < 3; ++j )
		for ( int k = 0; k < 3; ++k )
			a[j][k] = j == k ? 1 : 0;

	return a;
}

/// @relates mat3
/// Returns the matrix determinant
inline Real det(const mat3& a) {
	return (
		a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]) +
		a[1][0] * (a[0][2] * a[2][1] - a[0][1] * a[2][2]) +
		a[2][0] * (a[0][1] * a[1][2] - a[0][2] * a[1][1])
		);
}


/// @relates mat3
/// Returns the inverse of the matrix. By default the parameter detA is set to
/// zero and therefore the function will calculate the needed matrix determinant
/// on its own.
/// If you already know the matrix determinant you can pass its value in the
/// parameter detA so that the function doesn't have to calculate it.
/// If the matrix is singular it is impossible to calculate the inverse matrix
/// and the identity matrix is returned.
inline mat3 inverse(const mat3& a, Real detA = 0) {

	// only calculate the determinant if it has not been passed as 
	// argument
	if ( detA == 0 ) {
		detA = det(a);
	}

	if ( detA != 0 ) {
		mat3 r(
			(a[1][1] * a[2][2] - a[1][2] * a[2][1]), (a[0][2] * a[2][1] - a[0][1] * a[2][2]), (a[0][1] * a[1][2] - a[0][2] * a[1][1]),
			(a[1][2] * a[2][0] - a[1][0] * a[2][2]), (a[0][0] * a[2][2] - a[0][2] * a[2][0]), (a[0][2] * a[1][0] - a[0][0] * a[1][2]),
			(a[1][0] * a[2][1] - a[1][1] * a[2][0]), (a[0][1] * a[2][0] - a[0][0] * a[2][1]), (a[0][0] * a[1][1] - a[0][1] * a[1][0])
			);

		r /= detA;

		return r;
	}

	return identity3();
}

#endif

