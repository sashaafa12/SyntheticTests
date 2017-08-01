//
// Some simple math routines
//
// Author: Alex V. Boreskoff
//

#include    "Math3D.h"

Vector3D    closestPointToSegment ( const Vector3D& p, const Vector3D& a, const Vector3D& b )
{
    Vector3D c = p - a;
    Vector3D v = b - a;
    float    d = v.length ();

    v /= d;                 // normalize v

    float   t = v & c;

    if ( t < 0.0f )
        return a;
    else
    if ( t > d )
        return b;

    return a + t*v;
}

int	computeNearPointMask ( const Vector3D& n )
{
    if ( n.x > 0.0f )
        if ( n.y > 0.0f )
            if ( n.z > 0.0f )
                return 0;
            else
                return 4;
        else
            if ( n.z > 0.0f )
                return 2;
            else
                return 6;
    else
        if ( n.y > 0.0f )
            if ( n.z > 0.0f )
                return 1;
            else
                return 5;
        else
            if ( n.z > 0.0f )
                return 3;
            else
                return 7;
}
