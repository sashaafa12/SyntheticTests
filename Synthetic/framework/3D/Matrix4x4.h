//
// Basic class for 4x4 matrices
// Supports all basic matrix operations
//
// Author: Alex V. Boreskoff
//

#ifndef __MATRIX_4X4__
#define __MATRIX_4X4__

#include	"Vector3D.h"

class Matrix4x4
{
public:
	float m [4][4];

	Matrix4x4 () {}
	Matrix4x4 ( float val );
	Matrix4x4 ( float v1, float v2, float v3, float v4 = 1 );
	Matrix4x4 ( const Matrix4x4& );

	Matrix4x4& operator =  ( const Matrix4x4& );
	Matrix4x4& operator =  ( float );
	Matrix4x4& operator += ( const Matrix4x4& );
	Matrix4x4& operator -= ( const Matrix4x4& );
	Matrix4x4& operator *= ( const Matrix4x4& );
	Matrix4x4& operator *= ( float );
	Matrix4x4& operator /= ( float );

	const float * operator [] ( int i ) const
	{
		return &m[i][0];
	}

	float * operator [] ( int i )
	{
		return &m[i][0];
	}

	bool		invert    ();
	Matrix4x4& 	transpose ();
	float		det       () const;

	Matrix4x4	inverse () const
	{
		Matrix4x4 m ( *this );

		m.invert ();

		return m;
	}

	Vector3D transformPoint ( const Vector3D& b ) const
	{
		return Vector3D ( m [0][0]*b.x + m [0][1]*b.y + m [0][2]*b.z + m [0][3],
			              m [1][0]*b.x + m [1][1]*b.y + m [1][2]*b.z + m [1][3],
		                  m [2][0]*b.x + m [2][1]*b.y + m [2][2]*b.z + m [2][3] );
	}

	Vector3D transformDirection ( const Vector3D& b ) const
	{
		return Vector3D ( m [0][0]*b.x + m [0][1]*b.y + m [0][2]*b.z,
			              m [1][0]*b.x + m [1][1]*b.y + m [1][2]*b.z,
		                  m [2][0]*b.x + m [2][1]*b.y + m [2][2]*b.z );
	}

	void getHomMatrix ( float * matrix ) const;

	static	Matrix4x4	identity ();
												// scale
	static	Matrix4x4	scale    ( const Vector3D& s, float d = 1 );

												// rotations
	static	Matrix4x4	rotateX  ( float );		// rotate counter-clockwiose, angle in radians
	static	Matrix4x4	rotateY  ( float );		// rotate counter-clockwiose, angle in radians
	static	Matrix4x4	rotateZ  ( float );		// rotate counter-clockwiose, angle in radians
	static	Matrix4x4	rotate   ( const Vector3D& v, float angle );
	static	Matrix4x4	rotate   ( float yaw, float pitch, float roll );

												// reflections
	static	Matrix4x4	mirrorX  ();
	static	Matrix4x4	mirrorY  ();
	static	Matrix4x4	mirrorZ  ();

	friend Matrix4x4 operator + ( const Matrix4x4&, const Matrix4x4& );
	friend Matrix4x4 operator - ( const Matrix4x4&, const Matrix4x4& );
	friend Matrix4x4 operator * ( const Matrix4x4&, const Matrix4x4& );
	friend Matrix4x4 operator * ( const Matrix4x4&, float );
	friend Matrix4x4 operator * ( float, const Matrix4x4& );
	friend Vector3D  operator * ( const Matrix4x4&, const Vector3D& );
};

#endif
