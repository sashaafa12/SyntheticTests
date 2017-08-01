// 
// Simple class for quaternions interpolation
// Based on algorithm from CAAT book.
//
// Class comppute two sets iof control points, so that curve is a set of Bezier patches
// (q [i], a [i], b [i], q [i+1])

#ifndef	__QUAT_SPLINE__
#define	__QUAT_SPLINE__

#include	"Quaternion.h"

class	QuatSpline
{
protected:
	Quaternion * q;				// base points
	Quaternion * a;				// control points
	Quaternion * b;
	int			 n;				// number of base points
	
public:
	QuatSpline  ( Quaternion * qs, int count );
	~QuatSpline ();
	
	Quaternion	valueAt ( float t ) const;
};

#endif

