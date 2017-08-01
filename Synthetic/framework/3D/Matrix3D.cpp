//
// Basic class for 3d matrices
// Supports all basic matrix operations
//
// Author: Alex V. Boreskoff
//

#include <math.h>
#include "Matrix3D.h"

//
// Create a*E matrix
//

Matrix3D :: Matrix3D ( float a )
{
	x [0][1] = x [0][2] = x [1][0] =
	x [1][2] = x [2][0] = x [2][1] = 0;
	x [0][0] = x [1][1] = x [2][2] = a;
}

//
// copy c-tor
//

Matrix3D :: Matrix3D ( const Matrix3D& a )
{
	x [0][0] = a.x [0][0];
	x [0][1] = a.x [0][1];
	x [0][2] = a.x [0][2];
	x [1][0] = a.x [1][0];
	x [1][1] = a.x [1][1];
	x [1][2] = a.x [1][2];
	x [2][0] = a.x [2][0];
	x [2][1] = a.x [2][1];
	x [2][2] = a.x [2][2];
}

//
// Create matrix on column vectors
//

Matrix3D :: Matrix3D ( const Vector3D& c1, const Vector3D& c2, const Vector3D& c3 )
{
	x [0][0] = c1.x;
	x [0][1] = c2.x;
	x [0][2] = c3.x;

	x [1][0] = c1.y;
	x [1][1] = c2.y;
	x [1][2] = c3.y;

	x [2][0] = c1.z;
	x [2][1] = c2.z;
	x [2][2] = c3.z;
}

Matrix3D& Matrix3D :: operator = ( const Matrix3D& a )
{
	x [0][0] = a.x [0][0];
	x [0][1] = a.x [0][1];
	x [0][2] = a.x [0][2];
	x [1][0] = a.x [1][0];
	x [1][1] = a.x [1][1];
	x [1][2] = a.x [1][2];
	x [2][0] = a.x [2][0];
	x [2][1] = a.x [2][1];
	x [2][2] = a.x [2][2];

	return *this;
}

Matrix3D& Matrix3D :: operator = ( float a )
{
	x [0][1] = x [0][2] = x [1][0] =
	x [1][2] = x [2][0] = x [2][1] = 0.0;
	x [0][0] = x [1][1] = x [2][2] = a;

	return *this;
}

Matrix3D& Matrix3D :: operator += ( const Matrix3D& a )
{
	x [0][0] += a.x [0][0];
	x [0][1] += a.x [0][1];
	x [0][2] += a.x [0][2];
	x [1][0] += a.x [1][0];
	x [1][1] += a.x [1][1];
	x [1][2] += a.x [1][2];
	x [2][0] += a.x [2][0];
	x [2][1] += a.x [2][1];
	x [2][2] += a.x [2][2];

	return *this;
}

Matrix3D& Matrix3D :: operator -= ( const Matrix3D& a )
{
	x [0][0] -=a.x [0][0];
	x [0][1] -=a.x [0][1];
	x [0][2] -=a.x [0][2];
	x [1][0] -=a.x [1][0];
	x [1][1] -=a.x [1][1];
	x [1][2] -=a.x [1][2];
	x [2][0] -=a.x [2][0];
	x [2][1] -=a.x [2][1];
	x [2][2] -=a.x [2][2];

	return *this;
}

Matrix3D& Matrix3D :: operator *= ( const Matrix3D& a )
{
	Matrix3D c ( *this );

	x[0][0]=c.x[0][0]*a.x[0][0]+c.x[0][1]*a.x[1][0]+c.x[0][2]*a.x[2][0];
	x[0][1]=c.x[0][0]*a.x[0][1]+c.x[0][1]*a.x[1][1]+c.x[0][2]*a.x[2][1];
	x[0][2]=c.x[0][0]*a.x[0][2]+c.x[0][1]*a.x[1][2]+c.x[0][2]*a.x[2][2];
	x[1][0]=c.x[1][0]*a.x[0][0]+c.x[1][1]*a.x[1][0]+c.x[1][2]*a.x[2][0];
	x[1][1]=c.x[1][0]*a.x[0][1]+c.x[1][1]*a.x[1][1]+c.x[1][2]*a.x[2][1];
	x[1][2]=c.x[1][0]*a.x[0][2]+c.x[1][1]*a.x[1][2]+c.x[1][2]*a.x[2][2];
	x[2][0]=c.x[2][0]*a.x[0][0]+c.x[2][1]*a.x[1][0]+c.x[2][2]*a.x[2][0];
	x[2][1]=c.x[2][0]*a.x[0][1]+c.x[2][1]*a.x[1][1]+c.x[2][2]*a.x[2][1];
	x[2][2]=c.x[2][0]*a.x[0][2]+c.x[2][1]*a.x[1][2]+c.x[2][2]*a.x[2][2];

	return *this;
}

Matrix3D& Matrix3D :: operator *= ( float a )
{
	x [0][0] *= a;
	x [0][1] *= a;
	x [0][2] *= a;
	x [1][0] *= a;
	x [1][1] *= a;
	x [1][2] *= a;
	x [2][0] *= a;
	x [2][1] *= a;
	x [2][2] *= a;

	return *this;
}

Matrix3D& Matrix3D :: operator /= ( float a )
{
	x [0][0] /= a;
	x [0][1] /= a;
	x [0][2] /= a;
	x [1][0] /= a;
	x [1][1] /= a;
	x [1][2] /= a;
	x [2][0] /= a;
	x [2][1] /= a;
	x [2][2] /= a;

	return *this;
};

float	Matrix3D :: det () const
{
	return x [0][0]*(x [1][1]*x [2][2]-x [1][2]*x [2][1]) -
	       x [0][1]*(x [1][0]*x [2][2]-x [1][2]*x [2][0]) +
	       x [0][2]*(x [1][0]*x [2][1]-x [1][1]*x [2][0]);
}

Matrix3D&	Matrix3D :: invert ()
{

	float	 det;
	Matrix3D a;
					// compute a determinant
	det = x [0][0]*(x [1][1]*x [2][2]-x [1][2]*x [2][1]) -
	      x [0][1]*(x [1][0]*x [2][2]-x [1][2]*x [2][0]) +
	      x [0][2]*(x [1][0]*x [2][1]-x [1][1]*x [2][0]);

	a.x [0][0] = (x [1][1]*x [2][2]-x [1][2]*x [2][1]) / det;
	a.x [0][1] = (x [0][2]*x [2][1]-x [0][1]*x [2][2]) / det;
	a.x [0][2] = (x [0][1]*x [1][2]-x [0][2]*x [1][1]) / det;
	a.x [1][0] = (x [1][2]*x [2][0]-x [1][0]*x [2][2]) / det;
	a.x [1][1] = (x [0][0]*x [2][2]-x [0][2]*x [2][0]) / det;
	a.x [1][2] = (x [0][2]*x [1][0]-x [0][0]*x [1][2]) / det;
	a.x [2][0] = (x [1][0]*x [2][1]-x [1][1]*x [2][0]) / det;
	a.x [2][1] = (x [0][1]*x [2][0]-x [0][0]*x [2][1]) / det;
	a.x [2][2] = (x [0][0]*x [1][1]-x [0][1]*x [1][0]) / det;

	return *this = a;
/*
	float	t   [3][6];
	float * ptr [3];
	float * temp;
	register int i;
	register int j;
	register int k;

											// copy matrix to temp
	for ( i = 0; i < 3; i++ )
	{
		ptr [i]    = &x [i][0];
		t   [i][0] = x  [i][0];
		t   [i][1] = x  [i][1];
		t   [i][2] = x  [i][2];
		t   [i][3] = 0;
		t   [i][4] = 0;
		t   [i][5] = 0;
	}

	t [0][3] = 1;
	t [1][4] = 1;
	t [2][5] = 1;

	for ( i = 0; i < 2; i++ )				// do one row
	{
											// 1. choose pivot point
		k = i;

											// find max. candidate
		for ( j = i + 1; j < 3; j++ )
			if ( fabs ( ptr [j][i] ) > fabs ( ptr [k][i] ) )
				k = j;

											// swap if required
		if ( k != i )
		{
			temp    = ptr [i];
			ptr [i] = ptr [k];
			ptr [k] = temp;
		}

											// rescale whole row
		float	inv = 1 / fabs ( ptr [k][i] );

		for ( j = 0; j < 6; j++ )
			ptr [i][j] *= inv;

											// now ptr [i][i] == 1

											// eleminate rows below
		for ( k = i + 1; k < 3; k++ )
			for ( j = i; j < 6; j++ )
				ptr [k][j] -= ptr [i][j] * ptr [k][j];
	}

											// now copy matrix back
	for ( i = 0; i < 3; i++ )
		for ( j = 0; j < 3; j++ )
			x [i][j] = ptr [i][j + 3];

	return *this;
*/
}

Matrix3D&	Matrix3D :: transpose ()
{
	Matrix3D a;

	a.x [0][0] = x [0][0];
	a.x [0][1] = x [1][0];
	a.x [0][2] = x [2][0];
	a.x [1][0] = x [0][1];
	a.x [1][1] = x [1][1];
	a.x [1][2] = x [2][1];
	a.x [2][0] = x [0][2];
	a.x [2][1] = x [1][2];
	a.x [2][2] = x [2][2];

	return *this = a;
}

Matrix3D operator + ( const Matrix3D& a, const Matrix3D& b )
{
	Matrix3D c;

	c.x [0][0] = a.x [0][0] + b.x [0][0];
	c.x [0][1] = a.x [0][1] + b.x [0][1];
	c.x [0][2] = a.x [0][2] + b.x [0][2];
	c.x [1][0] = a.x [1][0] + b.x [1][0];
	c.x [1][1] = a.x [1][1] + b.x [1][1];
	c.x [1][2] = a.x [1][2] + b.x [1][2];
	c.x [2][0] = a.x [2][0] + b.x [2][0];
	c.x [2][1] = a.x [2][1] + b.x [2][1];
	c.x [2][2] = a.x [2][2] + b.x [2][2];

	return c;
}

Matrix3D operator - ( const Matrix3D& a, const Matrix3D& b )
{
	Matrix3D c;

	c.x [0][0] = a.x [0][0] - b.x [0][0];
	c.x [0][1] = a.x [0][1] - b.x [0][1];
	c.x [0][2] = a.x [0][2] - b.x [0][2];
	c.x [1][0] = a.x [1][0] - b.x [1][0];
	c.x [1][1] = a.x [1][1] - b.x [1][1];
	c.x [1][2] = a.x [1][2] - b.x [1][2];
	c.x [2][0] = a.x [2][0] - b.x [2][0];
	c.x [2][1] = a.x [2][1] - b.x [2][1];
	c.x [2][2] = a.x [2][2] - b.x [2][2];

	return c;
}

Matrix3D operator * ( const Matrix3D& a, const Matrix3D& b )
{
	Matrix3D c ( a );

	c.x[0][0]=a.x[0][0]*b.x[0][0]+a.x[0][1]*b.x[1][0]+a.x[0][2]*b.x[2][0];
	c.x[0][1]=a.x[0][0]*b.x[0][1]+a.x[0][1]*b.x[1][1]+a.x[0][2]*b.x[2][1];
	c.x[0][2]=a.x[0][0]*b.x[0][2]+a.x[0][1]*b.x[1][2]+a.x[0][2]*b.x[2][2];
	c.x[1][0]=a.x[1][0]*b.x[0][0]+a.x[1][1]*b.x[1][0]+a.x[1][2]*b.x[2][0];
	c.x[1][1]=a.x[1][0]*b.x[0][1]+a.x[1][1]*b.x[1][1]+a.x[1][2]*b.x[2][1];
	c.x[1][2]=a.x[1][0]*b.x[0][2]+a.x[1][1]*b.x[1][2]+a.x[1][2]*b.x[2][2];
	c.x[2][0]=a.x[2][0]*b.x[0][0]+a.x[2][1]*b.x[1][0]+a.x[2][2]*b.x[2][0];
	c.x[2][1]=a.x[2][0]*b.x[0][1]+a.x[2][1]*b.x[1][1]+a.x[2][2]*b.x[2][1];
	c.x[2][2]=a.x[2][0]*b.x[0][2]+a.x[2][1]*b.x[1][2]+a.x[2][2]*b.x[2][2];

	return c;
}

Matrix3D operator * ( const Matrix3D& a, float b )
{
	Matrix3D c ( a );

	return (c *= b);
}

Matrix3D operator * ( float b, const Matrix3D& a )
{
	Matrix3D c ( a );

	return (c *= b);
}

Matrix3D	Matrix3D :: identity ()
{
	return Matrix3D ( 1 );
}

Matrix3D Matrix3D :: scale ( const Vector3D& v )
{
	Matrix3D a ( 1 );

	a.x [0][0] = v.x;
	a.x [1][1] = v.y;
	a.x [2][2] = v.z;

	return a;
}

Matrix3D Matrix3D :: rotateX ( float angle )
{
	Matrix3D a ( 1 );
	float	 cosine = (float)cos ( angle );
	float	 sine   = (float)sin ( angle );

	a.x [1][1] = cosine;
	a.x [1][2] = sine;
	a.x [2][1] = -sine;
	a.x [2][2] = cosine;

	return a;
}

Matrix3D Matrix3D :: rotateY ( float angle )
{
	Matrix3D a ( 1 );
	float	 cosine = (float)cos ( angle );
	float	 sine   = (float)sin ( angle );

	a.x [0][0] = cosine;
	a.x [0][2] = -sine;
	a.x [2][0] = sine;
	a.x [2][2] = cosine;

	return a;
}

Matrix3D Matrix3D :: rotateZ ( float angle )
{
	Matrix3D a ( 1 );
	float	 cosine = (float)cos ( angle );
	float	 sine   = (float)sin ( angle );

	a.x [0][0] = cosine;
	a.x [0][1] = sine;
	a.x [1][0] = -sine;
	a.x [1][1] = cosine;

	return a;
}

Matrix3D Matrix3D :: rotate ( const Vector3D& v, float angle )
{
	Matrix3D a;
	float	 cosine = (float)cos ( angle );
	float	 sine   = (float)sin ( angle );

	a.x [0][0] = v.x *v.x + (1-v.x*v.x) * cosine;
	a.x [0][1] = v.x *v.y * (1-cosine) + v.z * sine;
	a.x [0][2] = v.x *v.z * (1-cosine) - v.y * sine;
	a.x [1][0] = v.x *v.y * (1-cosine) - v.z * sine;
	a.x [1][1] = v.y *v.y + (1-v.y*v.y) * cosine;
	a.x [1][2] = v.y *v.z * (1-cosine) + v.x * sine;
	a.x [2][0] = v.x *v.z * (1-cosine) + v.y * sine;
	a.x [2][1] = v.y *v.z * (1-cosine) - v.x * sine;
	a.x [2][2] = v.z *v.z + (1-v.z*v.z) * cosine;

	return a;
}

Matrix3D Matrix3D :: rotate ( float yaw, float pitch, float roll )
{
    return  rotateZ ( roll ) * rotateY ( yaw ) * rotateX ( pitch );
}

					// build a homogenous matrix 
void Matrix3D :: getHomMatrix ( float * matrix ) const
{
	if ( matrix == (float *) 0l )
		return;

		// 1st row
	matrix [ 0] = x [0][0];
	matrix [ 1] = x [0][1];
	matrix [ 2] = x [0][2];
	matrix [ 3] = 0;

		// 2nd row
	matrix [ 4] = x [1][0];
	matrix [ 5] = x [1][1];
	matrix [ 6] = x [1][2];
	matrix [ 7] = 0;

		// 3rd row
	matrix [ 8] = x [2][0];
	matrix [ 9] = x [2][1];
	matrix [10] = x [2][2];
	matrix [11] = 0;

		// 4th row
	matrix [12] = 0;
	matrix [13] = 0;
	matrix [14] = 0;
	matrix [15] = 1;
}

void Matrix3D :: getHomMatrix ( float * matrix, const Vector3D& offs ) const
{
	if ( matrix == (float *) 0l )
		return;

		// 1st row
	matrix [ 0] = x [0][0];
	matrix [ 1] = x [0][1];
	matrix [ 2] = x [0][2];
	matrix [ 3] = 0;

		// 2nd row
	matrix [ 4] = x [1][0];
	matrix [ 5] = x [1][1];
	matrix [ 6] = x [1][2];
	matrix [ 7] = 0;

		// 3rd row
	matrix [ 8] = x [2][0];
	matrix [ 9] = x [2][1];
	matrix [10] = x [2][2];
	matrix [11] = 0;

		// 4th row
	matrix [12] = offs.x;
	matrix [13] = offs.y;
	matrix [14] = offs.z;
	matrix [15] = 1;
}

Matrix3D Matrix3D :: mirrorX ()
{
	Matrix3D a ( 1 );

	a.x [0][0] = -1;

	return a;
}

Matrix3D Matrix3D :: mirrorY ()
{
	Matrix3D a ( 1 );

	a.x [1][1] = -1;

	return a;
}

Matrix3D Matrix3D :: mirrorZ ()
{
	Matrix3D a ( 1 );

	a.x [2][2] = -1;

	return a;
}

