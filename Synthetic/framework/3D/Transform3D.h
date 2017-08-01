//
// Basic 3D affine transform
// in the form:
//      p' = M*p + v
//
// Author: Alex V. Boreskoff
//
// Last changed: 4/11/2002
//

#ifndef __TRANSFORM__
#define __TRANSFORM__

#include "Matrix3D.h"
#include "Vector3D.h"

class	Plane;

class    Transform3D
{
public:
    Matrix3D m;         // linear transform matrix
    Vector3D v;         // translation vector

    Transform3D () {}
    Transform3D ( const Matrix3D matrix, const Vector3D& vector ) : m ( matrix ), v ( vector )
    {

    }

    Transform3D ( const Transform3D& tr ) : m ( tr.m ), v ( tr.v ) {}

	Transform3D ( const Matrix3D& matrix ) : m ( matrix )
	{
		v.x = 0;
		v.y = 0;
		v.z = 0;
	}

    Transform3D& invert ()
    {
        m.invert ();

        v = -(m * v);

        return *this;
    }

	Transform3D	getInverse () const
	{
		return Transform3D ( *this ).invert ();
	}
										// transform a point in space
    Vector3D  transformPoint ( const Vector3D& p ) const
    {
        return Vector3D ( v.x + m.x [0][0]*p.x + m.x [0][1]*p.y + m.x [0][2]*p.z,
                          v.y + m.x [1][0]*p.x + m.x [1][1]*p.y + m.x [1][2]*p.z,
                          v.z + m.x [2][0]*p.x + m.x [2][1]*p.y + m.x [2][2]*p.z );
    }

										// transform a direction
										// can change length
	Vector3D	transformDir ( const Vector3D& p ) const
	{
        return Vector3D ( m.x [0][0]*p.x + m.x [0][1]*p.y + m.x [0][2]*p.z,
                          m.x [1][0]*p.x + m.x [1][1]*p.y + m.x [1][2]*p.z,
                          m.x [2][0]*p.x + m.x [2][1]*p.y + m.x [2][2]*p.z );
    }
		
	void	buildHomogeneousMatrix ( float matrix [16] ) const;

	const Vector3D& getTranslation () const
	{
		return v;
	}

	const Matrix3D& getLinearPart () const
	{
		return m;
	}

	static	Transform3D getIdentity ()
	{
		return Transform3D ( Matrix3D :: identity () );
	}

	static	Transform3D	getTranslate ( const Vector3D& );
	static	Transform3D	getScale     ( const Vector3D& );
	static	Transform3D	getScale     ( float factor    );
	static	Transform3D	getRotateX   ( float angle     );
	static	Transform3D	getRotateY   ( float angle     );
	static	Transform3D	getRotateZ   ( float angle     );
	static	Transform3D getRotate    ( const Vector3D& axis, float angle );
	static	Transform3D	getMirror    ( const Plane&    );
};

inline Transform3D operator * ( const Transform3D& t1, const Transform3D& t2 )
{
    return Transform3D ( t1.m * t2.m, t1.m * t2.v + t1.v );
}

#endif
