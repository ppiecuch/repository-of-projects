#include "matrix4x4.h"

/*
This is Kevin Harris's matrix class that he has written. I used it since it was already 
available and because I am being lazy. Maybe I should go dig up mine that I wrote some time ago.
*/

Matrix4x4f::Matrix4x4f(float m0, float m1, float m2, float m3, 
					   float m4, float m5, float m6, float m7, 
					   float m8, float m9, float m10, float m11, 
					   float m12, float m13, float m14, float m15)
{
	m_matrix[0] = m0; m_matrix[1] = m1; m_matrix[2] = m2; m_matrix[3] = m4; 
	m_matrix[4] = m4; m_matrix[5] = m5; m_matrix[6] = m6; m_matrix[7] = m8; 
	m_matrix[8] = m8; m_matrix[9] = m9; m_matrix[10] = m10; m_matrix[11] = m12; 
	m_matrix[12] = m12; m_matrix[13] = m13; m_matrix[14] = m14; m_matrix[15] = m15;
}

void Matrix4x4f::identity()
{
	m_matrix[0] = 1; m_matrix[1] = 0; m_matrix[2] = 0; m_matrix[3] = 0; 
	m_matrix[4] = 0; m_matrix[5] = 1; m_matrix[6] = 0; m_matrix[7] = 0; 
	m_matrix[8] = 0; m_matrix[9] = 0; m_matrix[10] = 1; m_matrix[11] = 0; 
	m_matrix[12] = 0; m_matrix[13] = 0; m_matrix[14] = 0; m_matrix[15] = 1;
}

Matrix4x4f Matrix4x4f::transpose()
{
	Matrix4x4f result;
	
	result.m_matrix[0] = m_matrix[0];
	result.m_matrix[1] = m_matrix[4];
	result.m_matrix[2] = m_matrix[8];
	result.m_matrix[3] = m_matrix[12];
	result.m_matrix[4] = m_matrix[1];
	result.m_matrix[5] = m_matrix[5];
	result.m_matrix[6] = m_matrix[9];
	result.m_matrix[7] = m_matrix[13];
	result.m_matrix[8] = m_matrix[2];
	result.m_matrix[9] = m_matrix[6];
	result.m_matrix[10] = m_matrix[10];
	result.m_matrix[11] = m_matrix[14];
	result.m_matrix[12] = m_matrix[3];
	result.m_matrix[13] = m_matrix[7];
	result.m_matrix[14] = m_matrix[11];
	result.m_matrix[15] = m_matrix[15];

	return result;
}

void Matrix4x4f::setMatrix(float *matrix)
{
	m_matrix[0] = matrix[0];
	m_matrix[1] = matrix[1];
	m_matrix[2] = matrix[2];
	m_matrix[3] = matrix[3];
	m_matrix[4] = matrix[4];
	m_matrix[5] = matrix[5];
	m_matrix[6] = matrix[6];
	m_matrix[7] = matrix[7];
	m_matrix[8] = matrix[8];
	m_matrix[9] = matrix[9];
	m_matrix[10] = matrix[10];
	m_matrix[11] = matrix[11];
	m_matrix[12] = matrix[12];
	m_matrix[13] = matrix[13];
	m_matrix[14] = matrix[14];
	m_matrix[15] = matrix[15];
}

//void Matrix4x4f::translate(const vector3f &translate)
//{
//	m_matrix[12] = translate.x;
//	m_matrix[13] = translate.y;
//	m_matrix[14] = translate.z;
//}
//
//void Matrix4x4f::translateX(const float &x_distance)
//{
//	identity();
//	m_matrix[12] = x_distance;
//}
//
//void Matrix4x4f::translateY(const float &y_distance)
//{
//	identity();
//	m_matrix[13] = y_distance;
//}
//
//void Matrix4x4f::translateZ(const float &z_distance)
//{
//	identity();
//	m_matrix[14] = z_distance;
//}
//
//void Matrix4x4f::rotate(const float &angle, vector3f &axis)
//{
//	float s = sinf(DEGTORAD(angle));
//	float c = cosf(DEGTORAD(angle));
//
//	float length = axis.length();
//	float ux = axis.x/length;
//	float uy = axis.y/length;
//	float uz = axis.z/length;
//
//	m_matrix[0] = c + (1-c)*ux;
//	m_matrix[1] = (1-c)*ux*uy + s*uz;
//	m_matrix[2] = (1-c)*ux*uz - s*uy;
//	m_matrix[3] = 0;
//	m_matrix[4] = (1-c)*uy*ux - s*uz;
//	m_matrix[5] = c + (1-c)*uy*uy;
//	m_matrix[6] = (1-c)*uy*uz + s*ux;
//	m_matrix[7] = 0;
//	m_matrix[8] = (1-c)*uz*ux + s*uy;
//	m_matrix[9] = (1-c)*uz*uz - s*ux;
//	m_matrix[10] = c + (1-c)*uz*uz;
//	m_matrix[11] = 0;
//	m_matrix[12] = 0;
//	m_matrix[13] = 0;
//	m_matrix[14] = 0;
//	m_matrix[15] = 1;
//}
//
//void Matrix4x4f::rotateX(const float &angle)
//{
//	float s = sinf(DEGTORAD(angle));
//	float c = cosf(DEGTORAD(angle));
//
//	identity();
//
//	m_matrix[5] = c;
//	m_matrix[6] = s;
//	m_matrix[9] = -s;
//	m_matrix[10] = c;
//}
//
//void Matrix4x4f::rotateY(const float &angle)
//{
//	float s = sinf(DEGTORAD(angle));
//	float c = cosf(DEGTORAD(angle));
//
//	identity();
//
//	m_matrix[0] = c;
//	m_matrix[2] = -s;
//	m_matrix[8] = s;
//	m_matrix[10] = c;
//}
//
//void Matrix4x4f::rotateZ(const float &angle)
//{
//	float s = sinf(DEGTORAD(angle));
//	float c = cosf(DEGTORAD(angle));
//
//	identity();
//
//	m_matrix[0] = c;
//	m_matrix[1] = s;
//	m_matrix[4] = -s;
//	m_matrix[5] = c;
//}
//
//void Matrix4x4f::scale(const vector3f &scale)
//{
//	identity();
//
//	m_matrix[0] = scale.x;
//	m_matrix[5] = scale.y;
//	m_matrix[10] = scale.z;
//}
//
//void Matrix4x4f::transformPoint(vector3f &v1)
//{
//	v1.x = v1.x * m_matrix[0] + v1.y * m_matrix[4] +
//			v1.z * m_matrix[8] + m_matrix[12];
//	v1.y = v1.x * m_matrix[1] + v1.y * m_matrix[5] +
//			v1.z * m_matrix[9] + m_matrix[13];
//	v1.z = v1.x * m_matrix[2] + v1.y * m_matrix[6] +
//			v1.z * m_matrix[10] + m_matrix[14];
//}
//
//void Matrix4x4f::transformVector(vector3f &v1)
//{
//	v1.x = v1.x * m_matrix[0] + v1.y * m_matrix[4] +
//			v1.z * m_matrix[8];
//	v1.y = v1.x * m_matrix[1] + v1.y * m_matrix[5] +
//			v1.z * m_matrix[9];
//	v1.z = v1.x * m_matrix[2] + v1.y * m_matrix[6] +
//			v1.z * m_matrix[10];
//}

Matrix4x4f Matrix4x4f::operator + (const Matrix4x4f &matrix)
{
	Matrix4x4f result;

	result.m_matrix[0] = m_matrix[0] + matrix.m_matrix[0];
	result.m_matrix[1] = m_matrix[1] + matrix.m_matrix[1];
	result.m_matrix[2] = m_matrix[2] + matrix.m_matrix[2];
	result.m_matrix[3] = m_matrix[3] + matrix.m_matrix[3];
	result.m_matrix[4] = m_matrix[4] + matrix.m_matrix[4];
	result.m_matrix[5] = m_matrix[5] + matrix.m_matrix[5];
	result.m_matrix[6] = m_matrix[6] + matrix.m_matrix[6];
	result.m_matrix[7] = m_matrix[7] + matrix.m_matrix[7];
	result.m_matrix[8] = m_matrix[8] + matrix.m_matrix[8];
	result.m_matrix[9] = m_matrix[9] + matrix.m_matrix[9];
	result.m_matrix[10] = m_matrix[10] + matrix.m_matrix[10];
	result.m_matrix[11] = m_matrix[11] + matrix.m_matrix[11];
	result.m_matrix[12] = m_matrix[12] + matrix.m_matrix[12];
	result.m_matrix[13] = m_matrix[13] + matrix.m_matrix[13];
	result.m_matrix[14] = m_matrix[14] + matrix.m_matrix[14];
	result.m_matrix[15] = m_matrix[15] + matrix.m_matrix[15];

	return result;
}

Matrix4x4f Matrix4x4f::operator - (const Matrix4x4f &matrix)
{
	Matrix4x4f result;

	result.m_matrix[0] = m_matrix[0] - matrix.m_matrix[0];
	result.m_matrix[1] = m_matrix[1] - matrix.m_matrix[1];
	result.m_matrix[2] = m_matrix[2] - matrix.m_matrix[2];
	result.m_matrix[3] = m_matrix[3] - matrix.m_matrix[3];
	result.m_matrix[4] = m_matrix[4] - matrix.m_matrix[4];
	result.m_matrix[5] = m_matrix[5] - matrix.m_matrix[5];
	result.m_matrix[6] = m_matrix[6] - matrix.m_matrix[6];
	result.m_matrix[7] = m_matrix[7] - matrix.m_matrix[7];
	result.m_matrix[8] = m_matrix[8] - matrix.m_matrix[8];
	result.m_matrix[9] = m_matrix[9] - matrix.m_matrix[9];
	result.m_matrix[10] = m_matrix[10] - matrix.m_matrix[10];
	result.m_matrix[11] = m_matrix[11] - matrix.m_matrix[11];
	result.m_matrix[12] = m_matrix[12] - matrix.m_matrix[12];
	result.m_matrix[13] = m_matrix[13] - matrix.m_matrix[13];
	result.m_matrix[14] = m_matrix[14] - matrix.m_matrix[14];
	result.m_matrix[15] = m_matrix[15] - matrix.m_matrix[15];

	return result;
}

Matrix4x4f Matrix4x4f::operator * (const float scalar)
{
	Matrix4x4f result;

	result.m_matrix[0] = m_matrix[0] * scalar;
	result.m_matrix[1] = m_matrix[1] * scalar;
	result.m_matrix[2] = m_matrix[2] * scalar;
	result.m_matrix[3] = m_matrix[3] * scalar;
	result.m_matrix[4] = m_matrix[4] * scalar;
	result.m_matrix[5] = m_matrix[5] * scalar;
	result.m_matrix[6] = m_matrix[6] * scalar;
	result.m_matrix[7] = m_matrix[7] * scalar;
	result.m_matrix[8] = m_matrix[8] * scalar;
	result.m_matrix[9] = m_matrix[9] * scalar;
	result.m_matrix[10] = m_matrix[10] * scalar;
	result.m_matrix[11] = m_matrix[11] * scalar;
	result.m_matrix[12] = m_matrix[12] * scalar;
	result.m_matrix[13] = m_matrix[13] * scalar;
	result.m_matrix[14] = m_matrix[14] * scalar;
	result.m_matrix[15] = m_matrix[15] * scalar;

	return result;
}

Matrix4x4f Matrix4x4f::operator * (const Matrix4x4f &matrix)
{
	Matrix4x4f result;

	result.m_matrix[0]  = (m_matrix[0]*matrix.m_matrix[0])+(m_matrix[4]*matrix.m_matrix[1])+(m_matrix[8]*matrix.m_matrix[2])+(m_matrix[12]*matrix.m_matrix[3]);
    result.m_matrix[1]  = (m_matrix[1]*matrix.m_matrix[0])+(m_matrix[5]*matrix.m_matrix[1])+(m_matrix[9]*matrix.m_matrix[2])+(m_matrix[13]*matrix.m_matrix[3]);
    result.m_matrix[2]  = (m_matrix[2]*matrix.m_matrix[0])+(m_matrix[6]*matrix.m_matrix[1])+(m_matrix[10]*matrix.m_matrix[2])+(m_matrix[14]*matrix.m_matrix[3]);
    result.m_matrix[3]  = (m_matrix[3]*matrix.m_matrix[0])+(m_matrix[7]*matrix.m_matrix[1])+(m_matrix[11]*matrix.m_matrix[2])+(m_matrix[15]*matrix.m_matrix[3]);

    result.m_matrix[4]  = (m_matrix[0]*matrix.m_matrix[4])+(m_matrix[4]*matrix.m_matrix[5])+(m_matrix[8]*matrix.m_matrix[6])+(m_matrix[12]*matrix.m_matrix[7]);
    result.m_matrix[5]  = (m_matrix[1]*matrix.m_matrix[4])+(m_matrix[5]*matrix.m_matrix[5])+(m_matrix[9]*matrix.m_matrix[6])+(m_matrix[13]*matrix.m_matrix[7]);
    result.m_matrix[6]  = (m_matrix[2]*matrix.m_matrix[4])+(m_matrix[6]*matrix.m_matrix[5])+(m_matrix[10]*matrix.m_matrix[6])+(m_matrix[14]*matrix.m_matrix[7]);
    result.m_matrix[7]  = (m_matrix[3]*matrix.m_matrix[4])+(m_matrix[7]*matrix.m_matrix[5])+(m_matrix[11]*matrix.m_matrix[6])+(m_matrix[15]*matrix.m_matrix[7]);

    result.m_matrix[8]  = (m_matrix[0]*matrix.m_matrix[8])+(m_matrix[4]*matrix.m_matrix[9])+(m_matrix[8]*matrix.m_matrix[10])+(m_matrix[12]*matrix.m_matrix[11]);
    result.m_matrix[9]  = (m_matrix[1]*matrix.m_matrix[8])+(m_matrix[5]*matrix.m_matrix[9])+(m_matrix[9]*matrix.m_matrix[10])+(m_matrix[13]*matrix.m_matrix[11]);
    result.m_matrix[10] = (m_matrix[2]*matrix.m_matrix[8])+(m_matrix[6]*matrix.m_matrix[9])+(m_matrix[10]*matrix.m_matrix[10])+(m_matrix[14]*matrix.m_matrix[11]);
    result.m_matrix[11] = (m_matrix[3]*matrix.m_matrix[8])+(m_matrix[7]*matrix.m_matrix[9])+(m_matrix[11]*matrix.m_matrix[10])+(m_matrix[15]*matrix.m_matrix[11]);

    result.m_matrix[12] = (m_matrix[0]*matrix.m_matrix[12])+(m_matrix[4]*matrix.m_matrix[13])+(m_matrix[8]*matrix.m_matrix[14])+(m_matrix[12]*matrix.m_matrix[15]);
    result.m_matrix[13] = (m_matrix[1]*matrix.m_matrix[12])+(m_matrix[5]*matrix.m_matrix[13])+(m_matrix[9]*matrix.m_matrix[14])+(m_matrix[13]*matrix.m_matrix[15]);
    result.m_matrix[14] = (m_matrix[2]*matrix.m_matrix[12])+(m_matrix[6]*matrix.m_matrix[13])+(m_matrix[10]*matrix.m_matrix[14])+(m_matrix[14]*matrix.m_matrix[15]);
    result.m_matrix[15] = (m_matrix[3]*matrix.m_matrix[12])+(m_matrix[7]*matrix.m_matrix[13])+(m_matrix[11]*matrix.m_matrix[14])+(m_matrix[15]*matrix.m_matrix[15]);

    return result;
}

Matrix4x4f Matrix4x4f::operator = (const Matrix4x4f &matrix)
{
	m_matrix[0] = matrix.m_matrix[0];
	m_matrix[1] = matrix.m_matrix[1];
	m_matrix[2] = matrix.m_matrix[2];
	m_matrix[3] = matrix.m_matrix[3];
	m_matrix[4] = matrix.m_matrix[4];
	m_matrix[5] = matrix.m_matrix[5];
	m_matrix[6] = matrix.m_matrix[6];
	m_matrix[7] = matrix.m_matrix[7];
	m_matrix[8] = matrix.m_matrix[8];
	m_matrix[9] = matrix.m_matrix[9];
	m_matrix[10] = matrix.m_matrix[10];
	m_matrix[11] = matrix.m_matrix[11];
	m_matrix[12] = matrix.m_matrix[12];
	m_matrix[13] = matrix.m_matrix[13];
	m_matrix[14] = matrix.m_matrix[14];
	m_matrix[15] = matrix.m_matrix[15];

	return *this;
}

Matrix4x4f Matrix4x4f::invertMatrix( const Matrix4x4f *matIn )
{
    Matrix4x4f result;

    // OpenGL matrices are column major and can be quite confusing to access 
    // when stored in the typical, one-dimensional array often used by the API.
    // Here are some shorthand conversion macros, which convert a row/column 
    // combination into an array index.
    
    #define MAT(matIn,r,c) matIn->m_matrix[c*4+r]

    #define m11 MAT(matIn,0,0)
    #define m12 MAT(matIn,0,1)
    #define m13 MAT(matIn,0,2)
    #define m14 MAT(matIn,0,3)
    #define m21 MAT(matIn,1,0)
    #define m22 MAT(matIn,1,1)
    #define m23 MAT(matIn,1,2)
    #define m24 MAT(matIn,1,3)
    #define m31 MAT(matIn,2,0)
    #define m32 MAT(matIn,2,1)
    #define m33 MAT(matIn,2,2)
    #define m34 MAT(matIn,2,3)
    #define m41 MAT(matIn,3,0)
    #define m42 MAT(matIn,3,1)
    #define m43 MAT(matIn,3,2)
    #define m44 MAT(matIn,3,3)

    // Inverse = adjoint / det. (See linear algebra texts.)

    // pre-compute 2x2 dets for last two rows when computing
    // cofactors of first two rows.
    float d12 = (m31 * m42 - m41 * m32);
    float d13 = (m31 * m43 - m41 * m33);
    float d23 = (m32 * m43 - m42 * m33);
    float d24 = (m32 * m44 - m42 * m34);
    float d34 = (m33 * m44 - m43 * m34);
    float d41 = (m34 * m41 - m44 * m31);

    float tmp[16];
    
    tmp[0] =  (m22 * d34 - m23 * d24 + m24 * d23);
    tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
    tmp[2] =  (m21 * d24 + m22 * d41 + m24 * d12);
    tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

    // Compute determinant as early as possible using these cofactors.
    float det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

    // Run singularity test.
    if( det == 0.0 )
    {
        cout << "Warning: Call to invertMatrix produced a Singular matrix." << endl;

        float identity[16] = 
        {
           1.0, 0.0, 0.0, 0.0,
           0.0, 1.0, 0.0, 0.0,
           0.0, 0.0, 1.0, 0.0,
           0.0, 0.0, 0.0, 1.0
        };

       memcpy( result.m_matrix, identity, 16*sizeof(float) );
    }
    else
    {
       float invDet = 1.0f / det;
       
       // Compute rest of inverse.
       tmp[0] *= invDet;
       tmp[1] *= invDet;
       tmp[2] *= invDet;
       tmp[3] *= invDet;

       tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
       tmp[5] =  (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
       tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
       tmp[7] =  (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

       // Pre-compute 2x2 dets for first two rows when computing cofactors 
       // of last two rows.
       d12 = m11 * m22 - m21 * m12;
       d13 = m11 * m23 - m21 * m13;
       d23 = m12 * m23 - m22 * m13;
       d24 = m12 * m24 - m22 * m14;
       d34 = m13 * m24 - m23 * m14;
       d41 = m14 * m21 - m24 * m11;

       tmp[8]  =  (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
       tmp[9]  = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
       tmp[10] =  (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
       tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
       tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
       tmp[13] =  (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
       tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
       tmp[15] =  (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

       memcpy( result.m_matrix, tmp, 16*sizeof(float) );
    }

    #undef m11
    #undef m12
    #undef m13
    #undef m14
    #undef m21
    #undef m22
    #undef m23
    #undef m24
    #undef m31
    #undef m32
    #undef m33
    #undef m34
    #undef m41
    #undef m42
    #undef m43
    #undef m44
    #undef MAT
    
    return result;
}

Matrix4x4f Matrix4x4f::invertMatrix( const Matrix4x4f &matIn )
{
    Matrix4x4f result;

    // OpenGL matrices are column major and can be quite confusing to access 
    // when stored in the typical, one-dimensional array often used by the API.
    // Here are some shorthand conversion macros, which convert a row/column 
    // combination into an array index.
    
    #define MAT(matIn,r,c) matIn.m_matrix[c*4+r]

    #define m11 MAT(matIn,0,0)
    #define m12 MAT(matIn,0,1)
    #define m13 MAT(matIn,0,2)
    #define m14 MAT(matIn,0,3)
    #define m21 MAT(matIn,1,0)
    #define m22 MAT(matIn,1,1)
    #define m23 MAT(matIn,1,2)
    #define m24 MAT(matIn,1,3)
    #define m31 MAT(matIn,2,0)
    #define m32 MAT(matIn,2,1)
    #define m33 MAT(matIn,2,2)
    #define m34 MAT(matIn,2,3)
    #define m41 MAT(matIn,3,0)
    #define m42 MAT(matIn,3,1)
    #define m43 MAT(matIn,3,2)
    #define m44 MAT(matIn,3,3)

    // Inverse = adjoint / det. (See linear algebra texts.)

    // pre-compute 2x2 dets for last two rows when computing
    // cofactors of first two rows.
    float d12 = (m31 * m42 - m41 * m32);
    float d13 = (m31 * m43 - m41 * m33);
    float d23 = (m32 * m43 - m42 * m33);
    float d24 = (m32 * m44 - m42 * m34);
    float d34 = (m33 * m44 - m43 * m34);
    float d41 = (m34 * m41 - m44 * m31);

    float tmp[16];
    
    tmp[0] =  (m22 * d34 - m23 * d24 + m24 * d23);
    tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
    tmp[2] =  (m21 * d24 + m22 * d41 + m24 * d12);
    tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

    // Compute determinant as early as possible using these cofactors.
    float det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

    // Run singularity test.
    if( det == 0.0 )
    {
        cout << "Warning: Call to invertMatrix produced a Singular matrix." << endl;

        float identity[16] = 
        {
           1.0, 0.0, 0.0, 0.0,
           0.0, 1.0, 0.0, 0.0,
           0.0, 0.0, 1.0, 0.0,
           0.0, 0.0, 0.0, 1.0
        };

       memcpy( result.m_matrix, identity, 16*sizeof(float) );
    }
    else
    {
       float invDet = 1.0f / det;
       
       // Compute rest of inverse.
       tmp[0] *= invDet;
       tmp[1] *= invDet;
       tmp[2] *= invDet;
       tmp[3] *= invDet;

       tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
       tmp[5] =  (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
       tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
       tmp[7] =  (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

       // Pre-compute 2x2 dets for first two rows when computing cofactors 
       // of last two rows.
       d12 = m11 * m22 - m21 * m12;
       d13 = m11 * m23 - m21 * m13;
       d23 = m12 * m23 - m22 * m13;
       d24 = m12 * m24 - m22 * m14;
       d34 = m13 * m24 - m23 * m14;
       d41 = m14 * m21 - m24 * m11;

       tmp[8]  =  (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
       tmp[9]  = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
       tmp[10] =  (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
       tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
       tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
       tmp[13] =  (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
       tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
       tmp[15] =  (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

       memcpy( result.m_matrix, tmp, 16*sizeof(float) );
    }

    #undef m11
    #undef m12
    #undef m13
    #undef m14
    #undef m21
    #undef m22
    #undef m23
    #undef m24
    #undef m31
    #undef m32
    #undef m33
    #undef m34
    #undef m41
    #undef m42
    #undef m43
    #undef m44
    #undef MAT
    
    return result;
}
