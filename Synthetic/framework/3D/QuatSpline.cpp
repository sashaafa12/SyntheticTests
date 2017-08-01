// 
// Simple class for quaternions interpolation
// Based on algorithm from CAAT book.
//

#include	"QuatSpline.h"

QuatSpline :: QuatSpline  ( Quaternion * qs, int count )
{
	int	i;

	n = count;
	a = new Quaternion [n];
	b = new Quaternion [n];
	
	for ( i = 0; i < n; i++ )
		q [i] = qs [i];
		
	a [0] = slerp ( q [0], q [1] - q [0], 0.5 );
	b [0] = slerp ( q [0], q [0] - a [0], 0.5 );
	
	for ( i = 1; i < n - 1; i++ )
	{
		a [i] = slerp ( q [i] + (q [i] - q [i-1]), q [i+1], 0.5 );
		b [i] = slerp ( q [i], q [i] - a [i], 0.5 );
	}
}

QuatSpline :: ~QuatSpline ()
{
	delete q;
	delete a;
	delete b;
}
	
Quaternion	QuatSpline :: valueAt ( float t ) const
{
	if ( t < 0 )
		return q [0];
		
	if ( t >= n - 1 )
		return q [n-1];
		
	int		i = (int)floor ( t );			// got the segment
	float	u = t - (float) i;
	
										// do De Castello interpolation 
										// for q [i], a [i], b [i], q [i+1]
	Quaternion	q1 = slerp ( q [i], a [i],   u );
	Quaternion	q2 = slerp ( a [i], b [i],   u );
	Quaternion	q3 = slerp ( b [i], q [i+1], u );
	
	Quaternion	q12 = slerp ( q1, q2, u );
	Quaternion	q23 = slerp ( q2, q3, u );
	
	return slerp ( q12, q23, u );									
}

