//
// Basic class for 3d matrices
// Supports all basic matrix operations
//
// Author: Alex V. Boreskoff
//

#ifndef __MATRIX3D__
#define __MATRIX3D__

#include	"Vector3D.h"

class Matrix3D
{
public:
	float x [3][3];

	Matrix3D () {}
	Matrix3D ( float );
	Matrix3D ( const Matrix3D& );
	Matrix3D ( float d1, float d2, float d3 );
	Matrix3D ( const Vector3D& c1, const Vector3D& c2, const Vector3D& c3 );

	Matrix3D& operator =  ( const Matrix3D& );
	Matrix3D& operator =  ( float );
	Matrix3D& operator += ( const Matrix3D& );
	Matrix3D& operator -= ( const Matrix3D& );
	Matrix3D& operator *= ( const Matrix3D& );
	Matrix3D& operator *= ( float );
	Matrix3D& operator /= ( float );

	const float * operator [] ( int i ) const
	{
		return & x[i][0];
	}

	float * operator [] ( int i )
	{
		return & x[i][0];
	}

	Matrix3D&	invert    ();
	Matrix3D& 	transpose ();
	float		det       () const;

	Matrix3D	inverse () const
	{
		return Matrix3D ( *this ).invert ();
	}

	void getHomMatrix ( float * matrix ) const;
	void getHomMatrix ( float * matrix, const Vector3D& offs ) const;

	static	Matrix3D	identity ();
	static	Matrix3D	scale    ( const Vector3D& );
	static	Matrix3D	rotateX  ( float );
	static	Matrix3D	rotateY  ( float );
	static	Matrix3D	rotateZ  ( float );
	static	Matrix3D	rotate   ( const Vector3D&, float );
	static	Matrix3D	rotate   ( float yaw, float pitch, float roll );
	static	Matrix3D	mirrorX  ();
	static	Matrix3D	mirrorY  ();
	static	Matrix3D	mirrorZ  ();

	friend Matrix3D operator + ( const Matrix3D&, const Matrix3D& );
	friend Matrix3D operator - ( const Matrix3D&, const Matrix3D& );
	friend Matrix3D operator * ( const Matrix3D&, const Matrix3D& );
	friend Matrix3D operator * ( const Matrix3D&, float );
	friend Matrix3D operator * ( float, const Matrix3D& );
	friend Vector3D operator * ( const Matrix3D&, const Vector3D& );
};

inline Vector3D operator * ( const Matrix3D& a, const Vector3D& b )
{
	return Vector3D ( a.x [0][0]*b.x + a.x [0][1]*b.y + a.x [0][2]*b.z,
	                  a.x [1][0]*b.x + a.x [1][1]*b.y + a.x [1][2]*b.z,
	                  a.x [2][0]*b.x + a.x [2][1]*b.y + a.x [2][2]*b.z );
}

#endif
