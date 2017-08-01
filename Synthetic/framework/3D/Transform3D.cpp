//
// Basic 3D affine transform
// in the form:
//      p' = M*p + v
//
// Author: Alex V. Boreskoff
//
// Last changed: 14/11/2002
//

#include	"Transform3D.h"
#include	"Plane.h"

void	Transform3D :: buildHomogeneousMatrix ( float matrix [16] ) const
{
	matrix [3]  = 0.0f;
	matrix [7]  = 0.0f;
	matrix [11] = 0.0f;
	matrix [15] = 1.0f;

	matrix [12] = v.x;
	matrix [13] = v.y;
	matrix [14] = v.z;

	matrix [0]  = m [0][0];
	matrix [1]  = m [1][0];
	matrix [2]  = m [2][0];

	matrix [4]  = m [0][1];
	matrix [5]  = m [1][1];
	matrix [6]  = m [2][1];

	matrix [8]  = m [0][2];
	matrix [9]  = m [1][2];
	matrix [10] = m [2][2];
}

Transform3D	Transform3D :: getTranslate ( const Vector3D& v )
{
	return Transform3D ( Matrix3D :: identity (), v );
}

Transform3D	Transform3D :: getScale ( const Vector3D& v )
{
	return Transform3D ( Matrix3D :: scale ( v ), Vector3D ( 0.0f, 0.0f, 0.0f ) );
}

Transform3D	Transform3D :: getScale ( float factor )
{
	return Transform3D ( Matrix3D :: scale ( Vector3D ( factor, factor, factor ) ), Vector3D ( 0.0f, 0.0f, 0.0f ) );
}

Transform3D	Transform3D :: getRotateX ( float angle )
{
	return Transform3D ( Matrix3D :: rotateX ( angle ), Vector3D ( 0, 0, 0 ) );
}

Transform3D	Transform3D :: getRotateY ( float angle )
{
	return Transform3D ( Matrix3D :: rotateY ( angle ), Vector3D ( 0, 0, 0 ) );
}

Transform3D	Transform3D :: getRotateZ ( float angle )
{
	return Transform3D ( Matrix3D :: rotateZ ( angle ), Vector3D ( 0, 0, 0 ) );
}

Transform3D Transform3D :: getRotate ( const Vector3D& axis, float angle )
{
	return Transform3D ( Matrix3D :: rotate ( axis, angle ), Vector3D ( 0, 0, 0 ) );
}

Transform3D	Transform3D :: getMirror ( const Plane& plane )
{
	Matrix3D	m;
	Vector3D	n ( plane.n );
	Vector3D	v ( -2.0f * plane.dist * n );

	m [0][0] = 1.0f - 2.0f * n.x * n.x;
	m [0][1] = -2.0f * n.x * n.y;
	m [0][2] = -2.0f * n.x * n.z;
	m [1][0] = m [0][1];
	m [1][1] = 1.0f - 2.0f * n.y * n.y;
	m [1][2] = -2.0f * n.y * n.z;
	m [2][0] = m [0][2];
	m [2][1] = m [1][2];
	m [2][2] = 1.0f - 2.0f * n.z * n.z;

	return Transform3D ( m, v );
}
