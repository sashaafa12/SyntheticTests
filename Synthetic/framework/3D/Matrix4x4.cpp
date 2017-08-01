//
// Basic class for 4x4 matrices
// Supports all basic matrix operations
//
// Author: Alex V. Boreskoff
//

#include	<math.h>
#include	<memory.h>
#include	"Matrix4x4.h"

Matrix4x4 :: Matrix4x4 ( float a )
{
	m [0][1] = m [0][2] = m [0][3] =
	m [1][0] = m [1][2] = m [1][3] =
	m [2][0] = m [2][1] = m [2][3] =
	m [3][0] = m [3][1] = m [3][2] = 0;
	m [0][0] = m [1][1] = m [2][2] = a;
	m [3][3]                       = 1;
}

Matrix4x4 :: Matrix4x4 ( const Matrix4x4& a )
{
	memcpy ( m, &a.m, sizeof ( m ) );
}

Matrix4x4& Matrix4x4 :: operator = ( const Matrix4x4& a )
{
	memcpy ( m, &a.m, sizeof ( m ) );

	return *this;
}

Matrix4x4& Matrix4x4 :: operator = ( float a )
{
	m [0][1] = m [0][2] = m [0][3] =
	m [1][0] = m [1][2] = m [1][3] =
	m [2][0] = m [2][1] = m [2][3] = 0;
	m [0][0] = m [1][1] = m [2][2] = a;
	m [3][3]                       = 1;

	return *this;
}

Matrix4x4& Matrix4x4 :: operator += ( const Matrix4x4& a )
{
	for ( register int i = 0; i < 4; i++ )
		for ( register int j = 0; j < 4; j++ )
			m [i][j] += a.m [i][j];

	return *this;
}

Matrix4x4& Matrix4x4 :: operator -= ( const Matrix4x4& a )
{
	for ( register int i = 0; i < 4; i++ )
		for ( register int j = 0; j < 4; j++ )
			m [i][j] -= a.m [i][j];

	return *this;
}

Matrix4x4& Matrix4x4 :: operator *= ( const Matrix4x4& a )
{
	float	t [4][4];

	for ( register int i = 0; i < 4; i++ )
		for ( register int j = 0; j < 4; j++ )
			t [i][j] = m [i][0]*a.m [0][j] + m [i][1]*a.m[1][j] + m [i][2]*a.m[2][j] + m [i][3]*a.m[3][j];

	memcpy ( m, t, sizeof ( m ) );

	return *this;
}

Matrix4x4& Matrix4x4 :: operator *= ( float a )
{
	for ( register int i = 0; i < 4; i++ )
		for ( register int j = 0; j < 4; j++ )
			m [i][j] *= a;

	return *this;
}

Matrix4x4& Matrix4x4 :: operator /= ( float a )
{
	for ( register int i = 0; i < 4; i++ )
		for ( register int j = 0; j < 4; j++ )
			m [i][j] /= a;

	return *this;
};

					// build a homogenous matrix 
void Matrix4x4 :: getHomMatrix ( float * matrix ) const
{
	if ( matrix == (float *) 0l )
		return;

		// 1st row
	matrix [ 0] = m [0][0];
	matrix [ 1] = m [0][1];
	matrix [ 2] = m [0][2];
	matrix [ 3] = m [0][3];

		// 2nd row
	matrix [ 4] = m [1][0];
	matrix [ 5] = m [1][1];
	matrix [ 6] = m [1][2];
	matrix [ 7] = m [1][3];

		// 3rd row
	matrix [ 8] = m [2][0];
	matrix [ 9] = m [2][1];
	matrix [10] = m [2][2];
	matrix [11] = m [2][3];

		// 4th row
	matrix [12] = m [3][0];
	matrix [13] = m [3][1];
	matrix [14] = m [3][2];
	matrix [15] = m [3][3];
}

Matrix4x4&	Matrix4x4 :: transpose ()
{
	Matrix4x4 a;

	for ( register int i = 0; i < 4; i++ )
		for ( register int j = 0; j < 4; j++ )
			a.m [i][j] = m [j][i];

	return (*this = a);
}

Matrix4x4 operator + ( const Matrix4x4& a, const Matrix4x4& b )
{
	Matrix4x4 c;

	for ( register int i = 0; i < 4; i++ )
		for ( register int j = 0; j < 4; j++ )
			c.m [i][j] = a.m [i][j] + b.m [i][j];

	return c;
}

Matrix4x4 operator - ( const Matrix4x4& a, const Matrix4x4& b )
{
	Matrix4x4 c;

	for ( register int i = 0; i < 4; i++ )
		for ( register int j = 0; j < 4; j++ )
			c.m [i][j] = a.m [i][j] - b.m [i][j];

	return c;
}

Matrix4x4 operator * ( const Matrix4x4& a, const Matrix4x4& b )
{
	Matrix4x4 c;

	for ( register int i = 0; i < 4; i++ )
		for ( register int j = 0; j < 4; j++ )
			c.m [i][j] = a.m [i][0]*b.m [0][j]+a.m [i][1]*b.m [1][j]+a.m [i][2]*b.m [2][j]+a.m [i][3]*b.m [3][j];

	return c;
}

Matrix4x4 operator * ( const Matrix4x4& a, float b )
{
	Matrix4x4 c ( a );

	return (c *= b);
}

Matrix4x4 operator * ( float b, const Matrix4x4& a )
{
	Matrix4x4 c ( a );

	return (c *= b);
}

Matrix4x4	Matrix4x4 :: identity ()
{
	return Matrix4x4 ( 1 );
}

Matrix4x4 Matrix4x4 :: scale ( const Vector3D& s, float d )
{
	Matrix4x4 a ( 1 );

	a.m [0][0] = s.x;
	a.m [1][1] = s.y;
	a.m [2][2] = s.z;
	a.m [3][3] = d;

	return a;
}

Matrix4x4 Matrix4x4 :: rotateX ( float angle )
{
	Matrix4x4 a ( 1 );
	float	 cosine = (float)cos ( angle );
	float	 sine   = (float)sin ( angle );

	a.m [1][1] = cosine;
	a.m [1][2] = sine;
	a.m [2][1] = -sine;
	a.m [2][2] = cosine;

	return a;
}

Matrix4x4 Matrix4x4 :: rotateY ( float angle )
{
	Matrix4x4	a ( 1 );
	float		cosine = (float)cos ( angle );
	float		sine   = (float)sin ( angle );

	a.m [0][0] = cosine;
	a.m [0][2] = -sine;
	a.m [2][0] = sine;
	a.m [2][2] = cosine;

	return a;
}

Matrix4x4 Matrix4x4 :: rotateZ ( float angle )
{
	Matrix4x4	a ( 1 );
	float		cosine = (float)cos ( angle );
	float		sine   = (float)sin ( angle );

	a.m [0][0] = cosine;
	a.m [0][1] = sine;
	a.m [1][0] = -sine;
	a.m [1][1] = cosine;

	return a;
}

Matrix4x4 Matrix4x4 :: rotate ( const Vector3D& v, float angle )
{
	Matrix4x4	a;
	float		cosine = (float)cos ( angle );
	float		sine   = (float)sin ( angle );

	a.m [0][0] = v.x *v.x + (1-v.x*v.x) * cosine;
	a.m [0][1] = v.x *v.y * (1-cosine) + v.z * sine;
	a.m [0][2] = v.x *v.z * (1-cosine) - v.y * sine;
	a.m [0][3] = 0;
	a.m [1][0] = v.x *v.y * (1-cosine) - v.z * sine;
	a.m [1][1] = v.y *v.y + (1-v.y*v.y) * cosine;
	a.m [1][2] = v.y *v.z * (1-cosine) + v.x * sine;
	a.m [1][3] = 0;
	a.m [2][0] = v.x *v.z * (1-cosine) + v.y * sine;
	a.m [2][1] = v.y *v.z * (1-cosine) - v.x * sine;
	a.m [2][2] = v.z *v.z + (1-v.z*v.z) * cosine;
	a.m [2][3] = 0;
	a.m [3][0] =
	a.m [3][1] =
	a.m [3][2] = 0;
	a.m [3][3] = 1;

	return a;
}

Matrix4x4 Matrix4x4 :: rotate ( float yaw, float pitch, float roll )
{
    return rotateY ( yaw ) * rotateZ ( roll ) * rotateX ( pitch );
/*
	Matrix4x4 a;
	float    cy = cos ( yaw );
	float    sy = sin ( yaw );
    float    cp = cos ( pitch );
    float    sp = sin ( pitch );
    float    cr = cos ( roll );
    float    sr = sin ( roll );

	a.x [0][0] = cy*cr + sy*sp*sr;
	a.x [1][0] = -cy*sr + sy*sp*cr;
	a.x [2][0] = sy*cp;
	a.x [0][1] = sr * cp;
	a.x [1][1] = cr*cp;
	a.x [2][1] = -sp;
	a.x [0][2] = -sy*cr - cy*sp*sr;
	a.x [1][2] = sr*sy + cy*sp*cr;
	a.x [2][2] = cy*cp;

	return a;
*/
}

Matrix4x4 Matrix4x4 :: mirrorX ()
{
	Matrix4x4 a ( 1 );

	a.m [0][0] = -1;

	return a;
}

Matrix4x4 Matrix4x4 :: mirrorY ()
{
	Matrix4x4 a ( 1 );

	a.m [1][1] = -1;

	return a;
}

Matrix4x4 Matrix4x4 :: mirrorZ ()
{
	Matrix4x4 a ( 1 );

	a.m [2][2] = -1;

	return a;
}

/*
 * Compute inverse of 4x4 transformation SINGLE-PRECISION matrix.
 * Code contributed by Jacques Leroy <jle@star.be>
 * Code lifted from Brian Paul's Mesa freeware OpenGL implementation.
 * Return GL_TRUE for success, GL_FALSE for failure (singular matrix)
 */

bool	Matrix4x4 :: invert ()
{
#define SWAP_ROWS(a, b) {  float * _tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c)     m [r][c]

	float	wtmp [4][8];
	float	m0, m1, m2, m3, s;
	float	* r0, * r1, * r2, * r3;

	r0 = wtmp [0];
	r1 = wtmp [1];
	r2 = wtmp [2];
	r3 = wtmp [3];

	r0 [0] = MAT(m,0,0);
	r0 [1] = MAT(m,0,1);
	r0 [2] = MAT(m,0,2);
	r0 [3] = MAT(m,0,3);
	r0 [4] = 1;
	r0 [5] =
	r0 [6] =
	r0 [7] = 0;

	r1 [0] = MAT(m,1,0);
	r1 [1] = MAT(m,1,1);
	r1 [2] = MAT(m,1,2);
	r1 [3] = MAT(m,1,3);
	r1 [5] = 1;
	r1 [4] =
	r1 [6] =
	r1 [7] = 0,

	r2 [0] = MAT(m,2,0);
	r2 [1] = MAT(m,2,1);
	r2 [2] = MAT(m,2,2);
	r2 [3] = MAT(m,2,3);
	r2 [6] = 1;
	r2 [4] =
	r2 [5] =
	r2 [7] = 0;

	r3 [0] = MAT(m,3,0);
	r3 [1] = MAT(m,3,1);
	r3 [2] = MAT(m,3,2);
	r3 [3] = MAT(m,3,3);
	r3 [7] = 1;
	r3 [4] =
	r3 [5] =
	r3 [6] = 0;

											// choose pivot - or die
	if ( fabs (r3 [0] )> fabs ( r2 [0] ) )
		SWAP_ROWS ( r3, r2 );

	if ( fabs ( r2 [0] ) > fabs ( r1 [0] ) )
		SWAP_ROWS ( r2, r1 );

	if ( fabs ( r1 [0] ) > fabs ( r0 [0 ] ) )
		SWAP_ROWS ( r1, r0 );

	if ( r0 [0] == 0 )
		return false;

											// eliminate first variable
	m1 = r1[0]/r0[0];
	m2 = r2[0]/r0[0];
	m3 = r3[0]/r0[0];

	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;

	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;

	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;

	s = r0[4];

	if ( s != 0 )
	{
		r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s;
	}

	s = r0[5];

	if ( s != 0.0 )
	{
		r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s;
	}

	s = r0[6];

	if ( s != 0 )
	{
		r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s;
	}

	s = r0[7];

	if ( s != 0 )
	{
		r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s;
	}

											// choose pivot - or die
	if ( fabs (r3 [1] ) > fabs ( r2 [1] ) )
		SWAP_ROWS ( r3, r2 );

	if ( fabs ( r2 [1] ) > fabs ( r1 [1] ) )
		SWAP_ROWS ( r2, r1 );

	if ( r1 [1] == 0 )
		return false;


											// eliminate second variable
	m2     = r2[1]/r1[1]; m3     = r3[1]/r1[1];
	r2[2] -= m2 * r1[2];  r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];  r3[3] -= m3 * r1[3];

	s = r1[4];

	if ( 0 != s )
	{
		r2[4] -= m2 * s; r3[4] -= m3 * s;
	}

	s = r1[5];

	if ( 0 != s )
	{
		r2[5] -= m2 * s; r3[5] -= m3 * s;
	}

	s = r1[6];

	if ( 0 != s )
	{
		r2[6] -= m2 * s; r3[6] -= m3 * s;
	}

	s = r1[7];

	if ( 0 != s )
	{
		r2[7] -= m2 * s; r3[7] -= m3 * s;
	}

											// choose pivot - or die
	if ( fabs ( r3 [2] ) > fabs ( r2 [2] ) )
		SWAP_ROWS ( r3, r2 );

	if ( r2 [2] == 0)
	    return false;

											// eliminate third variable
	m3     = r3[2]/r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
	r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
	r3[7] -= m3 * r2[7];

											// last check
	if ( r3 [3] == 0 )
		return false;


											// now back substitute row 3
	s      = 1/r3[3];
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

											// now back substitute row 2
	m2    = r2[3];
	s     = 1/r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
	r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);

	m1     = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
	r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;

	m0     = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
	r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

											// now back substitute row 1
	m1    = r1[2];
	s     = 1/r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
	r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);

	m0     = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
	r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

											// now back substitute row 0
	m0    = r0[1];
	s     = 1/r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
	r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

	MAT(m,0,0) = r0[4]; MAT(m,0,1) = r0[5],
	MAT(m,0,2) = r0[6]; MAT(m,0,3) = r0[7],
	MAT(m,1,0) = r1[4]; MAT(m,1,1) = r1[5],
	MAT(m,1,2) = r1[6]; MAT(m,1,3) = r1[7],
	MAT(m,2,0) = r2[4]; MAT(m,2,1) = r2[5],
	MAT(m,2,2) = r2[6]; MAT(m,2,3) = r2[7],
	MAT(m,3,0) = r3[4]; MAT(m,3,1) = r3[5],
	MAT(m,3,2) = r3[6]; MAT(m,3,3) = r3[7];

#undef MAT
#undef SWAP_ROWS

	return true;
}

float	Matrix4x4 :: det () const
{
	return 
	(m [0][0] * m [1][1] - m [0][1] * m [1][0]) * (m [2][2] * m [3][3] - m [2][3] * m [3][2]) -
	(m [0][0] * m [1][2] - m [0][2] * m [1][0]) * (m [2][1] * m [3][3] - m [2][3] * m [3][1]) +
	(m [0][0] * m [1][3] - m [0][3] * m [1][0]) * (m [2][1] * m [3][2] - m [2][2] * m [3][1]) +
	(m [0][1] * m [1][2] - m [0][2] * m [1][1]) * (m [2][0] * m [3][3] - m [2][3] * m [3][0]) -
	(m [0][1] * m [1][3] - m [0][3] * m [1][1]) * (m [2][0] * m [3][2] - m [2][2] * m [3][0]) +
	(m [0][2] * m [1][3] - m [0][3] * m [1][2]) * (m [2][0] * m [3][1] - m [2][1] * m [3][0]);

}
