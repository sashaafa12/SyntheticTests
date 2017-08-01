//
// Some simple math routines
//
// Author: Alex V. Boreskoff
//

#ifndef __MATH_3D__
#define __MATH_3D__

#include    "Vector3D.h"

Vector3D    closestPointToSegment ( const Vector3D& p, const Vector3D& a, const Vector3D& b );
int			computeNearPointMask  ( const Vector3D& n );

inline float area ( const Vector3D& a, const Vector3D& b, const Vector3D& c )
{
    Vector3D v1 = b - a;
    Vector3D v2 = c - a;
    Vector3D v  = v1 ^ v2;

    return 0.5f * v.length ();
}

inline float intersectSphereByRay ( const Vector3D& center, float radius, const Vector3D& org, const Vector3D& dir )
{
    Vector3D q = center - org;
    float    c = q & q;             // squared length
    float    v = q & dir;
    float    d = radius * radius - (c * c - v* v);

    if ( d < 0.0f )
       return -1.0f;

    return (float)(v - sqrt ( d ));
}

#endif
