//
// Quaternion class
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef	__QUATERNION__
#define	__QUATERNION__

#include	"Vector3D.h"
#include	"Matrix3D.h"

class	Quaternion
{
public:							// make all members public
	float	x, y, z, w;

	Quaternion () {}
	Quaternion ( float theX, float theY = 0, float theZ = 0, float theW = 0 ) : x ( theX ), y ( theY ), z ( theZ ) , w ( theW ) {}
	Quaternion ( const Quaternion& q ) : x ( q.x ), y ( q.y ), z ( q.z ), w ( q.w ) {}
	Quaternion ( const Vector3D& v ) : x ( v.x ), y ( v.y ), z ( v.z ), w ( 0 ) {}
	
	Quaternion ( float angle, const Vector3D& axis );
	Quaternion ( const Matrix3D& mat );
	Quaternion ( float mat [3][3]    );

	Quaternion operator + () const
	{
		return *this;
	}

	Quaternion operator - () const
	{
		return Quaternion ( -x, -y, -z, -w );
	}

	Quaternion& conj ()
	{
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}

	Quaternion& operator += ( const Quaternion& q )
	{
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}

	Quaternion& operator -= ( const Quaternion& q )
	{
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
	}

	Quaternion& operator *= ( const Quaternion& q )
	{
		*this = Quaternion ( y * q.z - z * q.y + w * q.x + x * q.w,
							 z * q.x - x * q.z + w * q.y + y * q.w,
							 x * q.y - y * q.x + w * q.z + z * q.w,
							 w * q.w - x * q.x - y * q.y - z * q.z );
		return *this;
	}

	Vector3D	rotate         ( const Vector3D& v ) const;
	Quaternion&	normalize      ();
	Quaternion&	initWithAngles ( float yaw, float pitch, float roll );
	Matrix3D	getMatrix      () const;
	void		getHomMatrix   ( float * m ) const;		// homogeneous matrix in OpenGL style

};

inline	Quaternion operator + ( const Quaternion& q1, const Quaternion& q2 )
{
	return Quaternion ( q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w );
}

inline	Quaternion operator - ( const Quaternion& q1, const Quaternion& q2 )
{
	return Quaternion ( q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w );
}

inline	Quaternion operator * ( const Quaternion& q1, const Quaternion& q2 )
{
	return Quaternion ( q1.y * q2.z - q1.z * q2.y + q1.w * q2.x + q1.x * q2.w,
		                q1.z * q2.x - q1.x * q2.z + q1.w * q2.y + q1.y * q2.w,
						q1.x * q2.y - q1.y * q2.x + q1.w * q2.z + q1.z * q2.w,
						q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z );
}

Quaternion	slerp ( const Quaternion& q1, const Quaternion& q2, float t );

#endif
