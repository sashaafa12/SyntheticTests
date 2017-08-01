//
// Quaternion class
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#include	"Quaternion.h"

					// build a quternion for rotation around axis on angle
Quaternion :: Quaternion ( float angle, const Vector3D& axis )
{
	float	sine   = (float)sin ( angle * 0.5f );
	float	cosine = (float)cos ( angle * 0.5f );

	x = axis.x * sine;
	y = axis.y * sine;
	z = axis.z * sine;
	w = cosine;
}

Quaternion :: Quaternion ( const Matrix3D& mat )
{
	float	s, q [4];
	int		i, j, k;
	int		nxt [3] = { 1, 2, 0 };

	float	trace = mat [0][0] + mat [1][1] + mat [2][2];

	if ( trace > 0.0f )
	{
		s = (float) sqrt ( trace + 1.0f );
		w = s / 2.0f;
		s = 0.5f / s;
		x = ( mat [2][1] - mat [1][2] ) * s;
		y = ( mat [0][2] - mat [2][0] ) * s;
		z = ( mat [1][0] - mat [0][1] ) * s;
	}
	else
	{
		i = 0;

		if ( mat [1][1] > mat [0][0] )
			i = 1;

		if ( mat [2][2] > mat [i][i] )
			i = 2;

		j = nxt [i];
		k = nxt [j];

		s = (float) sqrt ( ( mat [i][i] - ( mat [j][j] + mat [k][k] )) + 1.0f );

		q [i] = s * 0.5f;

		if ( s != 0.0f )
			s = 0.5f / s;

		q [3] = ( mat [k][j] - mat [j][k] ) * s;
		q [j] = ( mat [j][i] + mat [i][j] ) * s;
		q [k] = ( mat [k][i] + mat [i][k] ) * s;

		x = q [0];
		y = q [1];
		z = q [2];
		w = q [3];
	}
}

Quaternion :: Quaternion ( float mat [3][3] )
{
	float	s, q [4];
	int		i, j, k;
	int		nxt [3] = { 1, 2, 0 };

	float	trace = mat [0][0] + mat [1][1] + mat [2][2];

	if ( trace > 0.0f )
	{
		s = (float) sqrt ( trace + 1.0f );
		w = s / 2.0f;
		s = 0.5f / s;
		x = ( mat [2][1] - mat [1][2] ) * s;
		y = ( mat [0][2] - mat [2][0] ) * s;
		z = ( mat [1][0] - mat [0][1] ) * s;
	}
	else
	{
		i = 0;

		if ( mat [1][1] > mat [0][0] )
			i = 1;

		if ( mat [2][2] > mat [i][i] )
			i = 2;

		j = nxt [i];
		k = nxt [j];

		s = (float) sqrt ( ( mat [i][i] - ( mat [j][j] + mat [k][k] )) + 1.0f );

		q [i] = s * 0.5f;

		if ( s != 0.0f )
			s = 0.5f / s;

		q [3] = ( mat [k][j] - mat [j][k] ) * s;
		q [j] = ( mat [j][i] + mat [i][j] ) * s;
		q [k] = ( mat [k][i] + mat [i][k] ) * s;

		x = q [0];
		y = q [1];
		z = q [2];
		w = q [3];
	}
}

Vector3D	Quaternion :: rotate ( const Vector3D& v ) const
{
	Quaternion	p     ( v );
	Quaternion	qConj ( -x, -y, -z, w );

	p  = *this * p * qConj;

	return Vector3D ( p.x, p.y, p.z );
}

Quaternion&	Quaternion :: normalize ()
{
	float	lenSq = x * x + y * y + z * z;

	if ( lenSq > 1.0f - EPS )
	{
		float	invLen = 1.0f / lenSq;

		x *= invLen;
		y *= invLen;
		z *= invLen;
		w = 0.0f;
	}
	else
		w = (float) sqrt ( 1.0f - lenSq );

	return *this;
}

Quaternion&	Quaternion :: initWithAngles ( float yaw, float pitch, float roll )
{
	yaw   *= 0.5f;
	pitch *= 0.5f;
	roll  *= 0.5f;

	float	cx = (float)cos ( yaw );
	float	cy = (float)cos ( pitch );
	float	cz = (float)cos ( roll );
	float	sx = (float)sin ( yaw );
	float	sy = (float)sin ( pitch );
	float	sz = (float)sin ( roll );

	float	cc = cx * cz;
	float	cs = cx * sz;
	float	sc = sx * cz;
	float	ss = sx * sz;

	x = cy * sc - sy * cs;
	y = cy * ss + sy * cc;
	z = cy * cs - sy * sc;
	w = cy * cc + sy * ss;

	return *this;
}

					// build a homogenous matrix from quaternion
					// in OpenGL style
void Quaternion :: getHomMatrix ( float * matrix ) const
{
	if ( matrix == (float *) 0l )
		return;

		// 1st col ( 0, 4, 8, 12)
	matrix [ 0] = 1.0f - 2.0f * ( y * y + z * z );  
	matrix [ 4] = 2.0f * ( x * y - w * z );  
	matrix [ 8] = 2.0f * ( x * z + w * y );  
	matrix [12] = 0.0f;  

		// 2nd col ( 1, 5, 9, 13)
	matrix [ 1] = 2.0f * ( x * y + w * z );  
	matrix [ 5] = 1.0f - 2.0f * ( x * x + z * z );  
	matrix [ 9] = 2.0f * ( y * z - w * x );  
	matrix [13] = 0.0f;  

		// 3rd col ( 2, 6, 10, 14)
	matrix [ 2] = 2.0f * ( x * z - w * y );  
	matrix [ 6] = 2.0f * ( y * z + w * x );  
	matrix [10] = 1.0f - 2.0f * ( x * x + y * y );  
	matrix [14] = 0.0f;  

		// 4th col ( 3, 7, 11, 15)
	matrix [ 3] = 0;  
	matrix [ 7] = 0;  
	matrix [11] = 0;  
	matrix [15] = 1.0f;
}

Matrix3D	Quaternion :: getMatrix () const
{
	Matrix3D	m;

		// 1st row
	m [0][0] = 1.0f - 2.0f * ( y * y + z * z );
	m [0][1] = 2.0f * ( x * y - w * z );
	m [0][2] = 2.0f * ( x * z + w * y );

		// 2nd row
	m [1][0] = 2.0f * ( x * y + w * z );
	m [1][1] = 1.0f - 2.0f * ( x * x + z * z );
	m [1][2] = 2.0f * ( y * z - w * x );

		// 3rd row
	m [2][0] = 2.0f * ( x * z - w * y );
	m [2][1] = 2.0f * ( y * z + w * x );
	m [2][2] = 1.0f - 2.0f * ( x * x + y * y );

	return m;
}


Quaternion	slerp ( const Quaternion& q1, const Quaternion& q2, float t )
{
	float	dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

					// check whether q1 and q2 are opposite
	if ( 1 + dot> EPS )
	{
		if ( 1 - dot > EPS )
		{
			float	omega   = (float) acos ( dot );
			float	invSine = 1.0f / (float) sin ( omega );
			float	scale1  = invSine * (float) sin ( ( 1.0f - t ) * omega );
			float	scale2  = invSine * (float) sin ( t * omega );

			return Quaternion ( scale1 * q1.x + scale2 * q2.x,
							    scale1 * q1.y + scale2 * q2.y,
								scale1 * q1.z + scale2 * q2.z,
								scale1 * q1.w + scale2 * q2.w );
		}
		else
		{
			float	scale1 = 1.0f - t;
			float	scale2 = t;

			return Quaternion ( scale1 * q1.x + scale2 * q2.x,
							    scale1 * q1.y + scale2 * q2.y,
								scale1 * q1.z + scale2 * q2.z,
								scale1 * q1.w + scale2 * q2.w );
		}
	}


				// quaternions are nearly opposite, create a perpendicual quaternion and s;erp it
	float	scale1 = (float) sin ( ( 1.0f - t ) * M_PI * 0.5f );
	float	scale2 = (float) sin ( t * M_PI * 0.5f );

	return Quaternion ( scale1 * q1.x + scale2 * ( -q2.y ),
					    scale1 * q1.y + scale2 * q2.x,
						scale1 * q1.z + scale2 * ( -q2.w ),
						scale1 * q1.w + scale2 * q2.w );
}

